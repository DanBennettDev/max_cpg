#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>
#include "matsu_signal_calcs.h"
#include "core.h"
#include "delayLine.h"
#include "XORRand.h"

#define MIN_MATSUOKA_FREQ 0.001
#define CALIBRATION_STOP_LIMIT 2





/*! \class MatsuNode
*  \brief Engine: Matsuoka Oscillator Node Class
*
*  Matsuoka Oscillator Node class -  a single half-centre matsuoka oscillator,
*  designed for use in an audiovisual installation, but built to be (hopefully)
*  useful in other contexts too.
*
*  Designed to be inherited from, so that use-case specific calculations and elements
*  can be added. In the context of the installation engine, additional node-level
*  characteristics are found in wrapper classes which inherit from this
*/




class MatsuNode
{
public:

	///  Describes firing states of the node. 
	/*! Firing is defined by the node's output signal having crossed zero
	*  this calculation step.
	*/
	enum class signalState : unsigned
	{
		nonSignificant,   /*!< Node signal has not crossed zero this step. */
		zeroXup,         /*!< Node signal crossed zero, rising this step. */
		zeroXdown,       /*!< Node signal crossed zero, falling this step. */
		firstPeak,      /*!< Node signal reached its first peak post positive zero crossing, prev step. */
		firstTrough   /*!< Node signal reached its first trough post negative zero crossing prev step. */
	};


	/// Equation Parameter names for Matsuoka Oscillator. {#matsuParam}  
	enum class matsuParam : unsigned
	{
		T1,         /*!< Time constant, reaction time of neuron 1. Determines frequency of oscillator. */
		T2,         /*!< Time constant, reaction time of neuron 2. See T1. */
		T,          /*!< Value for both T1 and T2. Allows setting of both to same val. For setter methods both T1 and T2 are affected. For access methods T1 is returned   */
		T2OverT1,   /*!< Ratio of T2:T1 - Affects waveshape */
		C1,         /*!< Tonic excitatory input for neuron 1, determines amplitude and in relation to C2, the +/- skew of waveform*/
		C2,         /*!< Tonic excitatory input for neuron 2, see C1 */
		C,          /*!< In many uses C1==C2 - this allows setting of both. For setter methods both C1 and C2 are affected. For access methods C1 is returned  */
		B,          /*!< Constant determining adaptation time of both neurons */
		G           /*!< Constant determining degree of adaptation of each neuron to the other */
	};

	/// Equation internal variable names for Matsuoka Oscillator. {#matsuInternal} 
	enum class matsuInternal : unsigned
	{
		X1,         /*!< Membrane potential of Neuron 1 */
		X2,         /*!< Membrane potential of Neuron 2 */
		V1,         /*!< State of adaptation of Neuron 1 */
		V2          /*!< State of adaptation of Neuron 2 */
	};

	// determines whether and how the node should be synchronised to parent node's cycle
	// like hard-sync in analog synthesis - this node's cycle is reset by the initialisation of the parent
	// node's cycle. Logic is handled at CPG level since connection is a 
	// network-level property
	enum class synchMode : unsigned
	{
		free,
		synchOnce,
		synchLock
	};


	/// Initialises a node with default parameters and id of 0. 
	MatsuNode();

	/// Initialises a node with default parameters and the provided node identifier. 
	/*! Identifier is used to uniquely identify the node, eg. in identifying inputs
	*  Uniqueness of id should be enforced within a single network.
	*/
	MatsuNode(unsigned id);
	/*! Initialises a node with provided parameters [see matsuParam](@ref matsuParam) */
	MatsuNode(unsigned id, double t1, double t2, double c1, double c2, double b,
		double g);
	/*! Initialises a node with provided parameters [see matsuParam](@ref matsuParam) */
	MatsuNode(unsigned id, double t1, double t2, double c, double b, double g);

	/// Calculates the next state and output value for the Matsuoka oscillator at this node. 
	/*! Uses 4th order Runge Kutta method. */
	void    doCalcStep(bool basicsOnly = false, bool withInput = true);

	/// Empty method, be overriden by classes extending MatsuNode.
	/*! allows additional actions to be added after the
	*  calculation step. When adding actions here,
	*/
	void    doAuxiliaryStepActions();






	/// Returns the identifier number of the node. 
	unsigned getIdentifier() const;

	/// returns sum of weighted input signals to the node 
	double getInput() const;


	/// returns output signal value for the node   
	double  getOutput(bool forceStateVal =false) const;


	/// returns output signal value for the node at indicated delay   
	double  getOutput(unsigned samplesDelay, bool startAtPhaseOffset = false) const;

	/// returns output signal value for the node at its most recent first local maximum after positive zero crossing
	double getLastMaxima() const;

	/// returns output signal value for the node at its most recent first local minimum after negative zero crossing
	double getLastMinima() const;

	/// returns firing state of the node, this step. [See signalState](@ref signalState)       
	signalState getSignalState() const;

	/// Returns current value of named internal 
	double  getInternal(matsuInternal param) const;

	/// Returns value of named Matsuoka Oscilator parameter. [See matsuParam](@ref matsuParam)   
	double  getParam(matsuParam param) const;

	double getFrequency(unsigned sampleRate) const;

	/// Returns a vector of the identifiers of the nodes with input connections to this node.
	std::vector<unsigned> getInputIds() const;

	/// Returns the weight of the input to this node from the node whose identifier is provided.  
	double getInputWeight(unsigned identifier) const;

	/// Returns the delay of the input to this node from the node whose identifier is provided.  
	double getInputDelay(unsigned identifier) const;

	/// Returns read only pointer to the node whose identifier is provided.
	const MatsuNode* getInputNode(unsigned identifier) const;

	double getSelfNoiseAmount() const;

	unsigned getOutputDelay() const;

	unsigned getDelayLineLength() const;

	//todo - deprecate ?
	double getFreqCompensation() const;

	bool isInput(unsigned identifier) const;


	/// sets the identifier number of the node. 
	void setIdentifier(unsigned id);

	/// Sets named Matsuoka Oscilator parameter to value provided. [See matsuParam](@ref matsuParam) 
	// TODO: DEPRECATE
	void    setParam(matsuParam param, double val);

	void    set_t(double val);
	void    set_t1(double val);
	void    set_t2(double val);
	void    set_t2_over_t1(double val);
	void    set_b(double val);
	void    set_g(double val);
	void    set_c(double val);
	void    set_c1(double val);
	void    set_c2(double val);

	/// Sets named Matsuoka Oscilator parameters to values provided. [See matsuParam](@ref matsuParam)  
	void    setParams(double t1, double t2, double c1, double c2,
		double b, double g);
	void    setParams(double t1, double t2, double c,
		double b, double g);

	/// Sets frequency of Matsuoka Oscillator directly.
	/*! This depends on correct setting of frequency compensation value.
	*  Frequency is set by altering equation parameters.
	*  Subsequently changing equation parameters except C (excluding separate setting
	*  of C1 and C2) will alter the frequency (and may necessitate re-setting of FreqCompensation)
	*  see K. Matsuoka, "Analysis of a neural oscillator," Biological cybernetics, vol. 104, no. 4-5,pp. 297{304, 2011.
	*  TODO - provide more details here
	*/
	void    setFrequency(double freq, unsigned sampleRate);

	void	setDrivenMode(bool driven);
	void	driveOutput(float val);

	/// Returns frequency of Matsuoka Oscillator. 
	/*! This depends on correct setting of frequency compensation value.
	*  If the matsuoka oscillator's parameters have been set manually
	*  since either set setFrequencyCompensation() was called, the value
	*  cannot be trusted.
	*/
	/// Sets weight of input into this node, from the node referenced. 
	/*! Adds input to input vector if not already present
	*  TODO: what does return indicate, throws?
	*/
	bool setInputWeight(MatsuNode const &node, double weight = 0);

	void setInputDelay(MatsuNode const &node, unsigned delay);


	void setSelfNoiseAmount(double weight);


	bool addInput(MatsuNode const &node)
	{
		return setInputWeight(node);
	};
	/// Removes the input to this node from the node whose identifier is provided. 
	/*! If there was no input from indicated node, nothing is done, returns false.
	*  If input is successfully removed, returns true.
	*/
	bool removeInput(unsigned identifier);


	/// Removes all inputs to the node, if any exist.
	void clearInputs();


	/// Sets pointer to external signal input
	void setExternalInput(double const input);

	/// runs node for required No of Cycles and returns the freq compensation amount
	double calcFreqCompensation(int cycleCount, int sampleRate);

	/// Sets value for node frequency calculation compensation. [See setFrequency()](@ref setFrequency)  
	void setFreqCompensation(double comp);

	/// Resets value for node frequency calculation compensation to default value. [See setFrequency()](@ref setFrequency) 
	void resetFreqCompensation();

	/// Resets node internal state and so output value to default initialisation values
	void reset();

	/// Resets node internal state and so output value based on provided values 
	void reset(double x1, double x2, double v1, double v2);


	void resetChangeFlag_Params();
	void resetChangeFlag_Inputs();
	bool hasChanged_Params() const;
	bool hasChanged_Inputs() const;

	/// sets the length of the delay line which allows both global delay of the 
	/// node and random access to node output history - for use in phase offsets
	/// between nodes
	void setDelayLineLength(unsigned length);

	/// sets the Delay, in samples, between last generated sample and output value.
	/// _signalState, last maxima and last minima are generated based on the 
	/// output at this delay. Value clamped between MIN_DELAY_LENGTH (2) and 
	/// the length of the delay line - for use in phase offsets between nodes
	void setNodeOutputDelay(unsigned samples);



	// HIERARCHY HANDLING

	void setParent(unsigned nodeID);
	void removeParent();

	bool addChild(MatsuNode &node);

	bool removeChild(unsigned nodeID);

	bool removeChild(MatsuNode &node);

	int getParentID() const;

	bool isChild(MatsuNode & node) const;

	bool isChild(unsigned nodeID) const;

	std::vector<unsigned> getChildIDs() const;

	unsigned getChildCount() const;


	bool isActive() const { return _active; }
	void setIsActive(bool active) { _active = active; }

	void setSynchMode(synchMode doSync);
	synchMode getSynchMode() const;


	void clone(MatsuNode &from);


private:

	/// equation parameters and variables for the matsuoka oscillator. See [MatsuParam](@ref matsuParam) and [MatsuInternal](@ref matsuInternal)  
	struct params
	{
		matsuoka_internals state;
		DelayLine<double> out;
		double t1;
		double t2;
		double c1;
		double c2;
		double b;
		double g;

		params()
			:out(INIT_DELAY_LENGTH)
		{}
	} matsuParams;

#ifdef _DEBUG_LOG
	matsuoka_internals prevState;
#endif

	/// Specifies the input from a single node to this node - source and weight.   
	struct input
	{
		MatsuNode const *node;
		double weight;
		unsigned delay;
	};

	/// Delay, in samples, between last generated sample and output
	/// _signalState, last maxima and last minima are generated based on the 
	/// output at this delay
	unsigned _nodeOutputDelay;

	/// indicates whether or not the node is active
	bool _active;

	/// indicates whether or not the node is driven by external value rather than calculated
	bool _driven{ false };
	float _drivenValue{ 0 };

	/// Numeric identifier for this node. Should be unique within a network
	unsigned _identifier;

	/*! Compensation value used in calculating equation parameters for a desired frequency */
	double _freqCompensation;
	/*! List of inputs to this node */
	std::vector<input> _inputs;

	// TODO - don't like pointer  here - should rewrite to store & use node ids, but that
	//          will probably mean substantial work
	std::vector<MatsuNode const *> _children;

	double _ext_input;
	double _selfNoiseAmount;

	int _parentNodeID;

	int _hasCrossedZero;
	signalState _signalState;
	double _lastMaxima, _lastMinima;
	bool _haveParamsChanged, _haveInputsChanged;
	synchMode _synchMode;

	void create(unsigned id, double t1, double t2, double c1, double c2, double b,
		double g);

	void uncreate();

	static bool validateParams(double t1 = T1_INIT,
		double t2 = T2_INIT,
		double c1 = C_INIT,
		double c2 = C_INIT,
		double b = B_INIT,
		double g = G_INIT);

	static bool validateT(double t);
	static bool validateC(double c);
	static bool validateB(double b);
	static bool validateG(double g);
	void updateSignalState();

};


