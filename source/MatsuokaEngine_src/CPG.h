#pragma once

/*! \class CPG
 *  \brief Engine: Matsuoka Oscillator Central Pattern Generator Template Class
 *
 *  Instantiates a central pattern generator network with node type 
 *  depending on the type passed to it.
 * 
 *  The type passed to the template must have the same interface as my 
 *  MatsuNode class. And my design intention is that type passed will 
 *  inherit from MatsuNode.
 *
 *  For this reason getter methods are sparse, beyond a const child node
 *  getter, via which all getter methods of child class can be accessed.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "matsuNode.h"
#include "ScalingCurve.h"

class CPG
{


public:

    /// Data specifying a single input to a node from another node 
    struct input
    {
        unsigned sourceID;  /*!< Unique identifier of the node which is the source of the input. */ 
        double weight;      /*!< Scaling multiplier for the input signal. Typically 0.0 to c5.0 */ 
        double phase;       /*!< Phase offset for the input signal. 0.0 to 1.0 */ 
    };


    CPG(unsigned sampleRate);

    CPG(double t1, double t2, double c1, double c2, double b, double g);
    CPG(double t1, double t2, double c, double b, double g);
    ~CPG() { uncreate(); };

    // on current implementation CPG not suitable to be copied
    // would need to move pointers out of matsunode or develop complex copy constructor
    // CPG is naturally a singleton object for most uses so no big issue at present
    CPG(const CPG& that) = delete;

    /// Returns a complete vector of unique identifiers of the nodes in the CPG, sorted ascending
    const std::vector<unsigned>& getNodeList() const;

    /// Calculates the next state and output values for the Matsuoka oscillators at each node. 
    void step();

    /// Calculates the next state and output values for the indicated node. 
    void step(unsigned nodeID);

    /// Resets the internal state and output value for all _nodes to (stable) initialisation values
    /*! Useful should one or more nodes stop responding.
     */
    void reset();

    /// Resets the internal state and output value for the node to the provided values
    /*! Useful should one or more _nodes stop responding, or to restart the whole network
     *  from a given point
     */
    void reset(double x1, double x2, double v1, double v2);

    /// Resets the internal state and output value for single nodes to (stable) initialisation values
    /*! Useful should one or more nodes stop responding.
    */
    void reset(unsigned nodeID);


    /// Resets the internal state and output value for single nodes to (stable) initialisation values
    /*! Useful should one or more nodes stop responding.
    */
    void reset(unsigned nodeID, double x1, double x2, double v1, double v2);


	/// if createExternalSyncResources has been run, this function resets the node to its state just prior to positive zero crossing 
	/// otherwise behaviour is as reset(nodeID)
	void zeroSync(unsigned nodeID);


    /// Adds, or changes weight of, a one-way connection between two nodes
    /*! If the connection specified exists, then its weight is set to that specified.
     *  Otherwise a new input is added to the node whose ID is specified by nodeTo, 
     *  from the node whose ID is specified by nodeFrom, with weight specified.
     *  If either node specified does not exist, an exception is thrown.
     */
    void setConnection(unsigned nodeFrom, unsigned nodeTo, double weight);

    /// Adds, or changes weight of, a reciprocal connection between two nodes
    /*! If a connection specified exists, then its weight is set to that specified.
     *  Otherwise a new input is added: to the node specified by nodeTo, 
     *  from the node specified by nodeFrom, with weight specified.
     *  If either node specified does not exist, an exception is thrown.
     */
    void setConnection(unsigned nodeA, unsigned nodeB, double weightAtoB,
        double weightBtoA);


    /// Changes phase offset of signal on input from nodeFrom to nodeTo
    /*! Phase between 0 and 1.
     */
    void setConnectionPhaseOffset(unsigned nodeFrom, unsigned nodeTo, 
                                                    double phaseOffset);


    void removeConnection(unsigned nodeFrom, unsigned nodeTo);


    /// Returns a sorted vector of all [inputs](@ref input) for a given node
    std::vector<input> getInputs(unsigned nodeID) const;


    /// returns const reference to the node whose nodeID is provided.
    /*! all read access to nodes is via const functions on node.
     *  This allows easy extending of the node class' read methods,
     *  while CPG class retains ownership of write methods.
     */
    const MatsuNode& getNode(unsigned nodeID) const;

    double getNodeSelfNoise(unsigned nodeID);

    bool exists(unsigned nodeID) const;



    void addChild(unsigned parentID, unsigned newID);

    void moveNode(unsigned nodeID, unsigned newParentID,
        bool breakCurrParentChildConn, bool breakCurrChildParentConn);


    // moves children of deleted node up one in heirarchy. New connections from parent->child have weight 0.0
    void deleteNode(unsigned nodeID);

    /// remove all nodes but root from CPG;
    void clear();

    // every node in the network
    void setFreqCompensation(double compensation);

    void setParam_t2Overt1(double val);

    void setParam_c(double val);

    void setParam_b(double val);

    void setParam_g(double val);

    void setSampleRate(unsigned sampleRate);

    bool loadWeightScalingCurve(std::string curveSource);

	bool loadWeightScalingCurve(std::vector<float> x, std::vector<float> y);
    
    void setUnityConnectionWeight(float unity);
    void setConnectionWeightScaling(bool on);


    void setNodeFrequency(unsigned nodeID, double freq, bool inherit);

    void setNodeFrequencyMultiple(unsigned nodeID, double multipleOfParent,
         bool inherit);

    void setNodePhaseOffset(unsigned nodeID, double offset);

    void setNodeSelfNoise(unsigned nodeID, double amount);

    void setNodeDelayLineLength(unsigned nodeID, unsigned length);

    void resetNodeChangeFlag_Params(unsigned nodeID);
    void resetNodeChangeFlag_Inputs(unsigned nodeID);

    void setNodeSynchMode(unsigned nodeID, MatsuNode::synchMode mode);
    MatsuNode::synchMode getNodeSynchMode(unsigned nodeID);

    unsigned getNextNodeID();

    // protected - may be useful to limit which nodes can take external input 
    void setExternalInput(unsigned nodeID, double input, double weight = 1.0);

	// Are we driving the root node from an external input? 
	void setDriven(bool driven);
	// if we are driving from an external input, set that value 
	void setDrivingInput(double input);
	// create the lookup wavetable and zero-state in case we are driving via an external phasor
	void createExternalSyncResources();


///////////////////////////////////////////////////////////////////////////////

protected:



///////////////////////////////////////////////////////////////////////////////

private:
	using matsuInternal = MatsuNode::matsuInternal;
    std::vector<MatsuNode> _nodes;
    std::vector<unsigned> _activeNodes;
    unsigned _sampleRate;
    ScalingCurve scaler;
	float _drivingInput;
	std::vector<float> _wavetable;
	bool _driven{ false };
	matsuoka_internals _zeroState;

    /// Removes the specified node from the CPG.
    /*! All inputs from the removed node to other nodes in the network
    *  are also removed, leaving no hanging references.
    *  Will not permit the root node (nodeID 0) to be removed
    */
    // protected - will want to add elements to removal process if e.g. parent
    //  and child relationships are maintained.
    bool removeNode(unsigned nodeID);

    //  useful separate freq and waveshape parameters for some uses
    // and limit access depending on position in network
	// TODO - split into components.
    void setParam(unsigned nodeID, MatsuNode::matsuParam param, double val);

    void setParams(unsigned nodeID, double t1, double t2, double c1,
        double c2, double b, double g);

    /// Removes a one-way connection between two _nodes
    /*! If a reciprocal connection exists between the nodes specified, only
    *  the connection running in the direction specified will be removed.
    *  If either node specified does not exist, an exception is thrown.
    */
    void disconnect(unsigned nodeFrom, unsigned nodeTo);


    // UTILITY

    void create(double t1, double t2, double c1, double c2, double b, double g);

    void uncreate();


    void getRootParams(double &t1, double &t2, double &c1,
        double &c2, double &b, double &g);

    void connect(unsigned nodeID_A, unsigned nodeID_B, double weightAtoB,
        double weightBtoA, bool bothWays);  

    // added from AV_CPG

    void setNodeFrequencyInherit(unsigned nodeID, double newFreq);


    void setNodeFrequencyMultipleInherit(unsigned nodeID,
        double multipleOfParent);


    void setParam(MatsuNode::matsuParam param, double val);
    
    unsigned calculateDelayLineLength(unsigned nodeID);

    void setNodeDelayLine(unsigned nodeID, double freq);

    void updateConnectionBasedOnFreq(unsigned nodeID, float oldFreq);

	float wavetableLookup(float i);

};



