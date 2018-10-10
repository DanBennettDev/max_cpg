
#include "CPG.h"


using input = CPG::input;


CPG::CPG(unsigned sampleRate)
    :_nodes(MAX_NODES)
    , _sampleRate(sampleRate)
{
    _nodes.reserve(MAX_NODES);
    create(T1_INIT, T2_INIT, C_INIT, C_INIT, B_INIT, G_INIT);
};

CPG::CPG(double t1, double t2, double c1, double c2, double b, double g)
{
    create(t1,t2,c1,c2,b,g);
    
};

CPG::CPG(double t1, double t2, double c, double b, double g)
{
    create(t1,t2,c,c,b,g);
};




/// Returns a complete vector of unique identifiers of the nodes in the CPG, sorted ascending
const std::vector<unsigned>& CPG::getNodeList() const
{
    return _activeNodes;
}

/// Calculates the next state and output values for the Matsuoka oscillators at each node. 
void CPG::step()    // calculates all nodes in class
{
    for (auto id : _activeNodes) {
        _nodes[id].doCalcStep();
    }
}

void CPG::step(unsigned nodeID)    // calculates all nodes in class
{
	_nodes[nodeID].doCalcStep();
}



/// Resets the internal state and output value for all _nodes to (stable) initialisation values
/*! Useful should one or more nodes stop responding.
*/
void CPG::reset(unsigned nodeID)
{
    _nodes[nodeID].reset();
}


void CPG::reset(unsigned nodeID, double x1, double x2, double v1, double v2)
{
    _nodes[nodeID].reset(x1, x2, v1, v2);;
}




void CPG::reset()
{
    for (auto id : _activeNodes) {
        _nodes[id].reset();
    }
}



/// Resets the internal state and output value for the node to the provided values
/*! Useful should one or more _nodes stop responding, or to restart the whole network
*  from a given point
*/
void CPG::reset(double x1, double x2, double v1, double v2)
{
    for (auto id : _activeNodes) {
        _nodes[id].reset(x1, x2, v1, v2);
    }
}

void CPG::zeroSync(unsigned nodeID) {
	_nodes[nodeID].reset(_zeroState.x1, _zeroState.x2, _zeroState.v1, _zeroState.v2);;
}


/// Adds, or changes weight of, a one-way connection between two nodes
/*! If the connection specified exists, then its weight is set to that specified.
*  Otherwise a new input is added to the node whose ID is specified by nodeTo,
*  from the node whose ID is specified by nodeFrom, with weight specified.
*  If either node specified does not exist, an runtime_error is thrown.
*/
void CPG::setConnection(unsigned nodeFrom, unsigned nodeTo, double weight)
{
    connect(nodeFrom, nodeTo, weight, 0u, false);
}



void CPG::setConnectionPhaseOffset(unsigned nodeFrom, unsigned nodeTo, 
                                double phaseOffset)
{
    if (phaseOffset < 0.0) { phaseOffset = 0.0; }
    if (phaseOffset > 1.0) { phaseOffset = 1.0; }
    double freq = _nodes[nodeFrom].getFrequency(_sampleRate);
    _nodes[nodeTo].setInputDelay(_nodes[nodeFrom], 
                                    (unsigned)((phaseOffset * _sampleRate)/ freq));
}




/// Adds, or changes weight of, a reciprocal connection between two nodes
/*! If a connection specified exists, then its weight is set to that specified.
*  Otherwise a new input is added: to the node specified by nodeTo,
*  from the node specified by nodeFrom, with weight specified.
*  If either node specified does not exist, an runtime_error is thrown.
*/
void CPG::setConnection(unsigned NodeA, unsigned NodeB, double weightAtoB,
    double weightBtoA)
{
    connect(NodeA, NodeB, weightAtoB, weightBtoA, true);
}


void CPG::removeConnection(unsigned nodeFrom, unsigned nodeTo)
{
    disconnect(nodeFrom, nodeTo);
}




/// Returns a sorted vector of all [inputs](@ref input) for a given node
std::vector<input> CPG::getInputs(unsigned nodeID) const
{
    std::vector<input> inputList;

    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }

    std::vector<unsigned> inputs = _nodes[nodeID].getInputIds();

    for(auto sourceID : inputs){
        input in;
        in.sourceID = sourceID;
        in.weight = _nodes[nodeID].getInputWeight(sourceID);
        in.phase = _nodes[nodeID].getInputDelay(sourceID) * 
                                    _nodes[nodeID].getFrequency(sourceID);
        inputList.push_back(in);
    }

    return inputList;
}


/// returns const reference to the node whose nodeID is provided.
/*! all read access to nodes is via const functions on node.
*  This allows easy extending of the node class' read methods,
*  while CPG class retains ownership of write methods.
*/
const MatsuNode& CPG::getNode(unsigned nodeID) const
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }
    return _nodes[nodeID];
}


double CPG::getNodeSelfNoise(unsigned nodeID)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }
    return _nodes[nodeID].getSelfNoiseAmount();
}


bool CPG::exists(unsigned nodeID) const
{
    if (nodeID >= MAX_NODES) {
        return false;
    }
    return _nodes[nodeID].isActive();
}


void CPG::addChild(unsigned parentID, unsigned newID)
{
    if (exists(newID)) {
        throw std::runtime_error("new node ID already exists");
    }
    if (!exists(parentID)) {
        throw std::runtime_error("parent ID of node is invalid");
    }
    _activeNodes.push_back(newID);
    std::sort(_activeNodes.begin(), _activeNodes.end());

    // reset node
    _nodes[newID].setIsActive(true);
    _nodes[newID].clone(_nodes[parentID]);
    _nodes[newID].addInput(_nodes[parentID]);
    _nodes[newID].setParent(parentID);
    //_nodes[newID].setFreqCompensation(_nodes[parentID].getFreqCompensation());
    _nodes[newID].setDelayLineLength(calculateDelayLineLength(newID)*2);
    _nodes[newID].setSynchMode(MatsuNode::synchMode::synchOnce);
    _nodes[parentID].addChild(_nodes[newID]);

}


void CPG::moveNode(unsigned nodeID, unsigned newParentID,
    bool breakCurrParentChildConn, bool breakCurrChildParentConn)
{
    if (nodeID == 0) { throw std::runtime_error("Cannot move the root node"); }
    if (!exists(nodeID)) { throw std::runtime_error("node to be moved does not exist"); }
    if (!exists(newParentID)) { throw std::runtime_error("new parent node does not exist"); }
   
    unsigned oldParentID = _nodes[nodeID].getParentID();

    // error has occured if a child has no parent, but this could be recoverable
    // simply avoid parent actions, rather than throw
    if (oldParentID != -1) {
        _nodes[oldParentID].removeChild(nodeID);
        if (breakCurrChildParentConn && _nodes[oldParentID].isInput(nodeID)) {
            _nodes[oldParentID].removeInput(nodeID);
        }
    }
    if (breakCurrParentChildConn) {
        _nodes[nodeID].removeInput(oldParentID);
    }
    if (!_nodes[nodeID].isInput(newParentID)) {
        _nodes[nodeID].addInput(_nodes[newParentID]);
    }
    _nodes[nodeID].setParent(newParentID);
    _nodes[newParentID].addChild(_nodes[nodeID]);

}


// moves children of deleted node up one in heirarchy. New connections from parent->child have weight 0.0
void CPG::deleteNode(unsigned nodeID)
{
    if (nodeID == 0) { throw std::runtime_error("Cannot delete the root node"); }
    if (!exists(nodeID)) { throw std::runtime_error("Node does not exist"); }

    unsigned parentID = _nodes[nodeID].getParentID();
    if (parentID == -1) {
        throw std::runtime_error("non-root node without a parent");
    }
    if (!exists(parentID)) {
        throw std::runtime_error("parent ID is not an active node");
    }

    if (_nodes[nodeID].getChildCount()>0) {
        // move any child nodes beneath deleted node's parent
        std::vector<unsigned> child_vector = _nodes[nodeID].getChildIDs();
        std::vector<unsigned>::const_iterator iter = child_vector.begin();
        while (iter != child_vector.end()) {
            moveNode(*iter, parentID, true, true);
            iter++;
        }
    }
    // detatch the node to be removed
    _nodes[parentID].removeChild(nodeID);
    removeNode(nodeID);

}


void CPG::clear()
{
    create(T1_INIT, T2_INIT, C_INIT, C_INIT, B_INIT, G_INIT);
}


void CPG::setSampleRate(unsigned sampleRate)
{
    _sampleRate = sampleRate;
}


void CPG::setUnityConnectionWeight(float unity)
{
    scaler.setInputMax(unity);
}

void CPG::setConnectionWeightScaling(bool on)
{
    if (on) {
        scaler.enableScaling();
    } else {
        scaler.disableScaling();
    }
}



bool CPG::loadWeightScalingCurve(std::string curveSource)
{
    return scaler.loadCurve(curveSource);
}

bool CPG::loadWeightScalingCurve(std::vector<float> x, std::vector<float> y)
{
	return scaler.loadCurve(x,y);
}


// every node in the network
void CPG::setFreqCompensation(double compensation)
{
    for (int i = 0; i < MAX_NODES; i++) {
        _nodes[i].setFreqCompensation(compensation);
        if (_nodes[i].isActive()) {
            setNodeDelayLine(i, _nodes[i].getFrequency(_sampleRate));
        }
    }
}

void CPG::setParam_t2Overt1(double val)
{
    for (int i = 0; i < MAX_NODES; i++) {
		_nodes[i].set_t2_over_t1(val);
    }
}

void CPG::setParam_c(double val)
{
	for (int i = 0; i < MAX_NODES; i++) {
		_nodes[i].set_c1(val);
		_nodes[i].set_c2(val);
	}
}

void CPG::setParam_b(double val)
{
	for (int i = 0; i < MAX_NODES; i++) {
		_nodes[i].set_b(val);
	}
}

void CPG::setParam_g(double val)
{
	for (int i = 0; i < MAX_NODES; i++) {
		_nodes[i].set_g(val);
	}
}

void CPG::setNodeFrequency(unsigned nodeID, double freq, bool inherit)
{
    if (!exists(nodeID)) { 
        throw std::runtime_error("tried to set freq on inactive node");
        return; 
    }
    float oldFreq = (float)_nodes[nodeID].getFrequency(_sampleRate);

    if (inherit) {
        setNodeFrequencyInherit(nodeID, freq);
    } else {
        _nodes[nodeID].setFrequency(freq, _sampleRate);
        updateConnectionBasedOnFreq(nodeID, oldFreq);
    }

    // update delayline sizes (used for phase offset so should accomodate cycle length)
    if(nodeID==0){ setNodeDelayLine(nodeID, freq); } 
    for(auto child :  getNode(nodeID).getChildIDs()){
        setNodeDelayLine(child, getNode(nodeID).getFrequency(_sampleRate)); 
    } 
}


void CPG::setNodeFrequencyMultiple(unsigned nodeID, double multipleOfParent,
    bool inherit)
{
    if (inherit) {
        setNodeFrequencyMultipleInherit(nodeID, multipleOfParent);
        return;
    }

    unsigned parentID = _nodes[nodeID].getParentID();
    if (parentID == -1) {
        throw std::runtime_error("no parent, cannot set frequency to multiple of parent frequency");
    }
    double newFreq = _nodes[parentID].getFrequency(_sampleRate)
        * multipleOfParent;

    float oldFreq = (float)_nodes[nodeID].getFrequency(_sampleRate);
    _nodes[nodeID].setFrequency(newFreq, _sampleRate);
    updateConnectionBasedOnFreq(nodeID, oldFreq);
    setNodeDelayLine(nodeID, newFreq);
}

// assumes that delayline lengths are maintained to suitable lengths elsewhere
void CPG::setNodePhaseOffset(unsigned nodeID, double offset)
{
    if (offset < 0.0) { offset = 0.0; }
    else if (offset > 1.0) { offset = 1.0; }

    // phase is relative to parent freq, except in case of root
    unsigned relativeTo = nodeID == 0 ? nodeID : getNode(nodeID).getParentID();
    double freq = _nodes[relativeTo].getFrequency(_sampleRate);
	unsigned offSamp = (unsigned)((offset * _sampleRate) / freq);
    _nodes[nodeID].setNodeOutputDelay(offSamp);

}



void CPG::setNodeSelfNoise(unsigned nodeID, double amount)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }
    _nodes[nodeID].setSelfNoiseAmount(amount);
}


void CPG::resetNodeChangeFlag_Params(unsigned nodeID)
{
    _nodes[nodeID].resetChangeFlag_Params();
}


void CPG::resetNodeChangeFlag_Inputs(unsigned nodeID)
{
    _nodes[nodeID].resetChangeFlag_Inputs();
}



unsigned CPG::getNextNodeID()
{
    int prev = -1;

    for (auto id : _activeNodes) {
        if ((int)id != prev + 1) {
            if ((int)id < prev + 1 || prev == -1) {
                throw std::runtime_error("CPG's node vector is out of order or " \
                    " missing root node");
            }
            return prev + 1;
        }
        prev = (int)id;
    }
    return prev + 1;
}


void CPG::setNodeSynchMode(unsigned nodeID, MatsuNode::synchMode mode)
{ 
    _nodes[nodeID].setSynchMode(mode);
}



MatsuNode::synchMode CPG::getNodeSynchMode(unsigned nodeID)
{ 
    return _nodes[nodeID].getSynchMode();
}



// PRIVATE ///////////////////////////////////////////////////////////////////

// TODO - remove
//void CPG::addNode(unsigned newID)
//{
//    double t1, t2, c1, c2, b, g;
//    getRootParams(t1, t2, c1, c2, b, g);
//    return addNode(newID, t1, t2, c1, c2, b, g);
//}
//
//void CPG::addNode(unsigned newID, double t1, double t2, double c1,
//    double c2, double b, double g)
//{
//    if (exists(newID)) {
//        throw std::invalid_argument("node id already exists");
//    }
//    _nodes[newID] = MatsuNode(newID, t1, t2, c1, c2, b, g);
//    _activeNodes.push_back(newID);
//    std::sort(_activeNodes.begin(), _activeNodes.end());
//}


bool CPG::removeNode(unsigned nodeID)
{
    if (nodeID == 0) {
        throw std::invalid_argument("may not remove root");
    }
    // remove node from node vector, 
    bool found = false;
    auto n = _activeNodes.begin();
    while (n < _activeNodes.end()) {
        if (*n == nodeID) {
            found = true;
            _nodes[*n].setIsActive(0);
            n = _activeNodes.erase(n);
        } else {
            _nodes[*n].removeInput(nodeID);
            n++;
        }
    }

    return found;
}

void CPG::setExternalInput(unsigned nodeID, double input, double weight)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }
    _nodes[nodeID].setExternalInput(input * weight);
}


void CPG::setDrivingInput(double input)
{
	if (input < 0) { input = 0; }
	if (input > 1) { input = 1; }
	_nodes[0].driveOutput(wavetableLookup(input));
}

void CPG::setDriven(bool driven) {
	_driven = driven;
	_nodes[0].setDrivenMode(driven);
}

void CPG::createExternalSyncResources()
{
	_wavetable.clear();

	// move oscillator to positive zero crossing
	while (_nodes[0].getOutput() > 0) {
		_nodes[0].doCalcStep(true, false);
	}
	while (_nodes[0].getOutput() < 0) {
		_nodes[0].doCalcStep(true, false);
	}
	// record state prior to zero crossing
	_zeroState.x1 = _nodes[0].getInternal(MatsuNode::matsuInternal::X1);
	_zeroState.x2 = _nodes[0].getInternal(MatsuNode::matsuInternal::X2);
	_zeroState.v1 = _nodes[0].getInternal(MatsuNode::matsuInternal::V1);
	_zeroState.v2 = _nodes[0].getInternal(MatsuNode::matsuInternal::V2);

	// record wavetable
	for (int i = 0; i < _sampleRate; i++) {
		_wavetable.push_back((float)_nodes[0].getOutput(true));
		_nodes[0].doCalcStep(true, false);
	}
}






// TODO - split into components.
void CPG::setParam(unsigned nodeID, MatsuNode::matsuParam param, double val)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }
    _nodes[nodeID].setParam(param, val);
}



void CPG::setParams(unsigned nodeID, double t1, double t2, double c1,
    double c2, double b, double g)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID");}
    _nodes[nodeID].setParams(t1, t2, c1, c2, b, g);
}


void CPG::disconnect(unsigned nodeFrom, unsigned nodeTo)
{
    if (!exists(nodeFrom)) { throw std::invalid_argument("invalid node ID"); }
    if (!exists(nodeTo)) { throw std::invalid_argument("invalid node ID"); }
    if (nodeFrom == _nodes[nodeTo].getParentID()) {
        throw std::invalid_argument("cannot remove connection to parent");
    }
    _nodes[nodeTo].removeInput(nodeFrom);
}


void CPG::create(double t1, double t2, double c1, double c2, double b, double g)
{
    for (int i=0; i < MAX_NODES; i++) {
        _nodes[i] = MatsuNode(i, t1, t2, c1, c2, b, g);
        _nodes[i].setIsActive(i==0);
    }
    _activeNodes.clear();
    _activeNodes.push_back(0);
	_zeroState.x1 = X1_INIT;
	_zeroState.x2 = X2_INIT;
	_zeroState.v1 = V1_INIT;
	_zeroState.v2 = V2_INIT;

}


void CPG::uncreate() { /* no actions required at present */ };


void CPG::getRootParams(double &t1, double &t2, double &c1,
    double &c2, double &b, double &g)
{
    if (_nodes[0].getIdentifier() != 0) {
        throw std::runtime_error("CPG's node vector is out of order");
    }
    t1 = _nodes[0].getParam(MatsuNode::matsuParam::T1);
    t2 = _nodes[0].getParam(MatsuNode::matsuParam::T2);
    c1 = _nodes[0].getParam(MatsuNode::matsuParam::C1);
    c2 = _nodes[0].getParam(MatsuNode::matsuParam::C2);
    b = _nodes[0].getParam(MatsuNode::matsuParam::B);
    g = _nodes[0].getParam(MatsuNode::matsuParam::G);
}





void CPG::connect(unsigned NodeID_A, unsigned NodeID_B, double weightAtoB,
    double weightBtoA, bool bothWays)
{
    if (!exists(NodeID_A)) { throw std::invalid_argument("invalid node ID A"); }
    if (!exists(NodeID_B)) { throw std::invalid_argument("invalid node ID B"); }

    // scale weights based on frequency ratios;
    float freqRatioAB = (float)(_nodes[NodeID_B].getFrequency(_sampleRate) 
                            / _nodes[NodeID_A].getFrequency(_sampleRate));
    float freqRatioBA = 1.0f / freqRatioAB;
    weightAtoB = scaler.getValue(freqRatioAB, (float)weightAtoB);
    

    _nodes[NodeID_B].setInputWeight(_nodes[NodeID_A], weightAtoB);
    if (bothWays) {
        weightBtoA = scaler.getValue(freqRatioBA, (float)weightBtoA);
        _nodes[NodeID_A].setInputWeight(_nodes[NodeID_B], weightBtoA);
    }
}


// added from AV_CPG
 

void CPG::setNodeFrequencyInherit(unsigned nodeID, double newFreq)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }

    float oldFreq = (float)_nodes[nodeID].getFrequency(_sampleRate);
    double childFreq;

    // update frequencies of child nodes (recursive)
    std::vector<unsigned> child_vector = _nodes[nodeID].getChildIDs();

    for(auto child : child_vector) {
        childFreq = (_nodes[child].getFrequency(_sampleRate) / oldFreq) * newFreq;
        setNodeFrequencyInherit(child, childFreq);
    }

    _nodes[nodeID].setFrequency(newFreq, _sampleRate);
    updateConnectionBasedOnFreq(nodeID, oldFreq);
}


void CPG::setNodeFrequencyMultipleInherit(unsigned nodeID, 
                                            double multipleOfParent)
{
    if (!exists(nodeID)) { throw std::invalid_argument("invalid node ID"); }

    unsigned parentID = _nodes[nodeID].getParentID();
    if (parentID == -1) {
        throw std::runtime_error("no parent, cannot set frequency to multiple of parent frequency");
    }
    double newFreq = _nodes[parentID].getFrequency(_sampleRate)
        * multipleOfParent;

    setNodeFrequencyInherit(nodeID, newFreq);
}

// todo: split into components
void CPG::setParam(MatsuNode::matsuParam param, double val)
{
    for (int i=0; i < MAX_NODES; i++) { _nodes[i].setParam(param, val); }
}

unsigned CPG::calculateDelayLineLength(unsigned nodeID)
{
    // delay line should accomodate 1 cycle of parent node for events driven by own signal
    // and 1 cycle of parent node, to allow offsetting of its signal for input child nodes
    // in case of root, always 
    double slowestFreq;
    double thisNodeFreq = _nodes[nodeID].getFrequency(_sampleRate);
    if(nodeID==0){
        slowestFreq = thisNodeFreq;
    } else {
        slowestFreq = _nodes[getNode(nodeID).getParentID()].getFrequency(_sampleRate);
        slowestFreq = slowestFreq < thisNodeFreq ? slowestFreq : thisNodeFreq;
    }
    unsigned len = (unsigned) ((double)_sampleRate / slowestFreq);

    if (len > MAX_DELAYLINE_LENGTH) { len = MAX_DELAYLINE_LENGTH; }
    return len;
}


void CPG::setNodeDelayLine(unsigned nodeID, double freq)
{
    unsigned newLength = calculateDelayLineLength(nodeID);
    if (newLength > (double)(_nodes[nodeID].getDelayLineLength())
        * DELAYLINE_RESIZE_THRESHOLD) {
        _nodes[nodeID].setDelayLineLength(unsigned(newLength * DELAYLINE_RESIZE_AMOUNT));
    }
}



void CPG::updateConnectionBasedOnFreq(unsigned nodeID, float oldFreq)
{
    //if we're using frequency-based scaling of connection weight, update weights
    if (scaler.isScalingOn()) {
        // handle all nodes which take input from this node
        for (auto destinationID : _activeNodes) {
            auto inputList = getNode(destinationID).getInputIds();
            for (auto inputID : inputList) {
                if (inputID == nodeID) {
                    float destFreq = (float)_nodes[destinationID].getFrequency(_sampleRate);
                    float oldFreqRatio = destFreq / oldFreq;
                    float oldWeight = (float)_nodes[destinationID].getInputWeight(nodeID);
                    setConnection(nodeID, destinationID, scaler.getInputValue(oldFreqRatio, oldWeight));
                }
            }
        }
        // handle all nodes which send input to this node
        for (auto inputID : _nodes[nodeID].getInputIds()) {
            float inputFreq = (float)_nodes[inputID].getFrequency(_sampleRate);
            float oldFreqRatio = oldFreq / inputFreq;
            float oldWeight = (float)_nodes[nodeID].getInputWeight(inputID);
            setConnection(inputID, nodeID, scaler.getInputValue(oldFreqRatio, oldWeight));
        }
    }
}


float  CPG::wavetableLookup(float i) {

	// scale up to wavetable size
	i *= _sampleRate-1;
	int iFloor = (int)i;
	float delta = i - iFloor;
	if (delta > 0) {
		float a = _wavetable[iFloor];
		float b = _wavetable[iFloor + 1];
		return ((b - a) * delta) + a;
	}
	else {
		return _wavetable[iFloor];
	}
}
