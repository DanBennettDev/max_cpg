#pragma once


/*! \class MatsuokaEngine
*  \brief Engine: Main interface for controlling and calculating the CPG network
*
*  Threadsafe - step(), getEvents() etc. can be run in one thread, while controlling the system
*  using the other functions from another thread
* 
*   Some functions are immediately actioned, some are queued and actioned 
*   on calling doQueuedActions(). If queued this is commented
*/


#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <string>
#include "CPG.h"
#include "QuantisedEventQueue.h"



class MatsuokaEngine
{

typedef  std::function<void(int, float)> Callback;

public:
    // simplify usage in classes interacting with the engine
    using input = CPG::input;    
    using synchMode = MatsuNode::synchMode;
    using outputEvent = QuantisedEventQueue::outputEvent;
    using gridType = QuantisedEventQueue::gridType;

	enum externalSync {
		none, driving, reseting
	};

    MatsuokaEngine();
    MatsuokaEngine(unsigned sampleRate,
                    bool eventOnRise = true,
                    bool eventOnFall = false,
                    bool fireOnPeak = false);

    MatsuokaEngine(const MatsuokaEngine& rhs);


    // METHODS

    /// calculate CPG and populate event queue,
    void step();

    /// calculate indicated node, 
    void step(unsigned nodeID);

	/// no syncing, no quantiser, no freq Compensation Calcs etc.,
	void stepBareBones();


    /// list active nodes in the network 
    std::vector<unsigned> getNodeList() const;
    
    /// return inputs for the requested node 
    std::vector<CPG::input> getInputs(unsigned nodeID) const;

    /// returns true if the nodeID exists in the network
    bool nodeExists(unsigned nodeID) const;

    /// returns const reference to the requested node
    const MatsuNode& getNode(unsigned nodeID) const;

    /// returns sample rate of the system
    unsigned getSampleRate() const;


    /// turns on or off the contextual scaling of connection weights based on 
    /// the ratio of frequencies of the nodes connected (the entrainment 
    /// threshold of Matsuoka's oscillator depends on this ratio, turning this
    /// on and setting an appropriate curve for your equation tuning, using
    /// loadConnectionWeightCurve ensures that when two different 
    /// are set to the same value they will have the same effect, regardless
    /// of node frequencies involved
    void setConnectionWeightScaling(bool on);


    /// pass a table of values to be used as source for scaling 
    /// connection weights based on frequency ratios of the nodes connected. 
    /// Each line should hold two floats, one for frequency ratio, the other
    /// for the weight multiplier. See ScalingCurve.h
    bool loadConnectionWeightCurve(std::string source);

	bool loadConnectionWeightCurve(std::vector<float> x, std::vector<float> y);

    /// sets the input value which should map to weight of 1 when setting connections
    void setUnityConnectionWeight(float unity);


    //// Lists all node ids which are in use in the engine
    //std::vector<unsigned> listActiveNodeIDs() const;

    /// returns the frequency of the requested node
    double getNodeFrequency(unsigned nodeID) const;

    /// returns the parent id of the requested node
    unsigned getNodeParent(unsigned nodeID) const;

    /// returns list of IDs of children of the requested node
    std::vector<unsigned> getNodeChildren(unsigned nodeID) const;
    
    /// returns the current oscillator output value of the requested node
	double getNodeOutput(unsigned nodeID, unsigned sampleOffset = 0 , bool matchQuantiser = false) const;

    /// returns the frequency error correction value currently set for the network
    double getFrequencyCompensation() const;

    /// returns the phase offset for the output of the requested node (0-1)
    double getNodePhaseOffset(unsigned nodeID) const;

    /// returns the amplitude of the noise input to the requested node
    double getNodeSelfNoise(unsigned nodeID) const;

    /// returns the phase offset of the connection to node nodeID, from node inputID (0-1)
    double getNodeInputPhase(unsigned nodeID, unsigned inputID) const;


    /// Returns vector of queued events. If sort ==true (default), sorted by parentID.
    const std::vector<outputEvent> getEvents();

    /// runs a calibration routine which sets the frequency error correction value for
    /// the set of equation values set. Should be run while engine is paused / not step()ing
    void calibrate();

    /// starts frequency compensation calculations to set the error correction 
    /// value for the network - use calibrate instead unless you have a very good reason
    void calcFrequencyCompensation();

    /// quits frequency compensation calculations. If called during calibrate()
    /// will ruin the results
    void quitFrequencyCompensationCalcs();


    // Not sure these will actually be neccessary. Suspend judgement
    //const vector<MatsuNode&> getNetworkDescription() const;
    //// virtual function on receiving class to handle this regardless of its event specialisation
    //MatsuNode getEventDetails(unsigned nodeID) const;


    // QUEUEING METHODS

    /// QUEUED ACTION - adds a child node to the indicated node, with the requested id
    unsigned addChild(unsigned parentID, unsigned newID);
   
    /// QUEUED ACTION - clears the entire network back to a single root node
    void reset();

    /// QUEUED ACTION - resets the internal state of the indicated node to the requested values
    void reset(unsigned nodeID, double x1, double x2, double v1, double v2);
    
    /// QUEUED ACTION - sets the connection weight between indicated nodes. 
    /// Adds connection if one does not already exist
    void setConnection(unsigned NodeFrom, unsigned NodeTo, double weight);
    
    /// QUEUED ACTION - sets connection weight both ways between indicated nodes
    /// adds connections if they do not exist
    void setConnection(unsigned NodeA, unsigned NodeB, double weightAtoB,
        double weightBtoA);

    /// QUEUED ACTION - sets the phase offset of the connection (phase 0-1)
    void setConnectionPhaseOffset(unsigned NodeFrom, unsigned NodeTo, double phase);

    /// QUEUED ACTION - removes connection between indicated nodes if one exists
    void removeConnection(unsigned nodeFrom, unsigned NodeTo);
    
    /// QUEUED ACTION - moves the node in the network topology to the indicated parent
    /// optional parameters to break current parent and child connections as well as 
    /// create new parent connection
    void moveNode(unsigned nodeID, unsigned newParentID,
        bool breakCurrParentChildConn, bool breakCurrChildParentConn);
    
    /// QUEUED ACTION -deletes the node, removing any connections to/from
    void deleteNode(unsigned nodeID);

    /// QUEUED ACTION -directly sets the frequency compensation for the control of node frequency
    /// in most cases, just use calibrate() instead.
    void setFreqCompensation(double compensation);

    /// QUEUED ACTION -sets the ratio between the matsuoka's oscillator parameters t2 and t1
    void setParam_t2Overt1(double val);

    /// QUEUED ACTION - sets the matsuoka's oscillator parameter C
    void setParam_c(double val);

    /// QUEUED ACTION - sets the matsuoka's oscillator parameter b
    void setParam_b(double val);

    /// QUEUED ACTION - sets the matsuoka's oscillator parameter g
    void setParam_g(double val);

    /// QUEUED ACTION - sets frequency of the node (assumes compensation has been
    /// set or calibrate() run since system start, or since setParam_ methods
    /// have been called. If inherit=true it changes child nodes frequencies 
    /// proportionally
    void setNodeFrequency(unsigned nodeID, double freq, bool inherit);

    /// QUEUED ACTION - sets frequency of the node  as multiple of its parent
    /// node's frequency (assumes compensation has been
    /// set or calibrate() run since system start, or since setParam_ methods
    /// have been called. If inherit=true it changes child nodes frequencies 
    /// proportionally
    void setNodeFrequencyMultiple(unsigned nodeID, double multipleOfParent,
         bool inherit);

    /// QUEUED ACTION - sets the phase offset of the node's output (0-1)
    /// handled via delay
    void setNodePhaseOffset(unsigned nodeID, double phase);

    /// QUEUED ACTION -sets the amplitude of white noise to the node input
    void setNodeSelfNoise(unsigned nodeID, double amount);

    /// QUEUED ACTION -sets synchronisation mode for the node - free, synchronise once (on next
    /// parent pos zero crossing, node is reset to zero crossing state), or synchLock 
    /// (on every pos zero crossing of parent, node is reset to  zero crossing state)
    void setNodeSynchMode(unsigned nodeID, synchMode mode);

    /// QUEUED ACTION -allows input of external signals to nodes in the network. Sets connection.
    /// weight and pointer to the input value. Update the input value with signal
    void setNodeExternalInput(unsigned nodeID, double weight, double input);

    /// resets the flag that indicates that node parameters have been changed
    void resetNodeChangeFlag_Params(unsigned nodeID);
    /// resets the flag that indicates that node inputs have been changed
    void resetNodeChangeFlag_Inputs(unsigned nodeID);

    /// sets the grid type for the quantiser for the node
    void setNodeQuantiser_Grid(unsigned nodeID, gridType grid);

    /// sets the multiplier for grid coarseness (defines no of valid positions on grid and gaps between them)
    void setNodeQuantiser_Multiple(unsigned nodeID, float mult);

    /// sets the offset from zero of the valid positions on the grid for the quantiser for the node
    void setNodeQuantiser_Offset(unsigned nodeID, float off);
    
    /// returns the grid type of the quantiser for the node
    gridType getNodeQuantiser_Grid(unsigned nodeID);

    /// returns the multiplier for grid coarseness (defines no of valid positions on grid and gaps between them)
    float getNodeQuantiser_Multiple(unsigned nodeID);

    /// returns the offset from zero of the valid positions on the grid for the quantiser for the node
    float    getNodeQuantiser_Offset(unsigned nodeID);

    /// returns the no of valid positions on the grid for the quantiser for the node
    /// set by setNodeQuantiser_Multiple
    unsigned getNodeQuantiser_BarDivision(unsigned nodeID);

    /// sets the strength of quantisation effect (0=no quantisation 1= strict quantisation)
    void     setQuantiseAmount(float amount);
	void     setQuantiseAmount(unsigned node, float amount);

    /// gets the strength of quantisation effect (0=no quantisation 1= strict quantisation)
    float    getQuantiseAmount();
	float    getQuantiseAmount(unsigned node);

    /// sets the number of calculation steps per second - used in internal calculations
    /// (this value must match the number of times per second you call step() )
    void setSampleRate(unsigned sampleRate);

    /// clears the network back to single root node, resets all values to defaults
    void clear();

    /// actions results of any methods called which are commented  QUEUED ACTION
    void doQueuedActions(); 

    /// sets a callback to be actioned upon a network output event 
    /// (e.g. on first peak after pos zero crossing for a node)
    /// calback should take an int (nodeID) and a float (amplitude peak at event)
    void setEventCallback(Callback cb);

    /// tells the engine to shutdown
    void setShutdown(bool shutdown);

    /// (un)pauses the engine (step() will have no effect while paused
    void setPause(bool pause);

    /// reports true if the system is in the process of shutting down
    bool isShuttingDown();

    /// reports true if not currently calculating a step
    bool isIdle();

    /// Step counter begins at zero on initialisation and increments by one each step. 
    /// this reports the current value of the step counter
    uint64_t getEngineStepCounter();

	/// Set system to be driven by external phasor (drives node0)
	void setDriven(externalSync driven);

	/// Set value of external driving phasor (drives node0)
	void setDrivingInput(float val);

	/// if SetDriven has been run, with value other than "none", this 
	/// resets the node to its state prior to zero-crossing 
	/// otherwise behaviour is same as reset(nodeID)
	void zeroSync(unsigned nodeID);

private:
    std::mutex _actions_mutex;
    std::mutex _output_mutex;
    uint64_t _stepCounter;

    CPG _cpg;
    std::queue<std::function<void()>> _actions;
    std::vector<outputEvent> _outputs;

    Callback _eventCallback;
    bool _callbackSet;


    enum class CompensationCalcState : char
    {
        WAITING, COUNTING, IDLE
    } _compCalcState;

    std::vector<CPG::input> _fcc_node0Inputs;
    unsigned _fcc_sampleCount;
    unsigned _fcc_cycleCount;
    static constexpr unsigned _fcc_cyclesPerCalc = 4;


    QuantisedEventQueue _quantiser;
    unsigned _sampleRate;
    bool _eventOnRise, _eventOnFall, _fireOnPeak;
    bool _shutdown, _idle, _paused;

    void fillOutputs(); // called at end of step


    void _setSampleRate(unsigned sampleRate);

    void updateFreqCompensationState();
    void doFreqCompensationCalcs();
    void synchroniseChildren(std::vector<unsigned> childIDs);
    void updateQuantiserTempo();

};

