#include "MatsuokaEngine.h"


using input = CPG::input;


MatsuokaEngine::MatsuokaEngine() : MatsuokaEngine(DEFAULTSAMPLERATE) 
{
}


MatsuokaEngine::MatsuokaEngine(unsigned sampleRate, 
                                bool eventOnRise,
                                bool eventOnFall,
                                bool fireOnPeak)
    : _cpg(sampleRate),
      _quantiser(sampleRate, DEFAULTNODEFREQ)
{
    _outputs.reserve(MAX_NODES);
    _fcc_node0Inputs.reserve(MAX_NODES);

    _sampleRate =  sampleRate;
    _eventOnRise = eventOnRise;
    _eventOnFall = eventOnFall;
    _fireOnPeak = fireOnPeak;
    _compCalcState = CompensationCalcState::IDLE;
    _fcc_sampleCount = 0u;
    _fcc_cycleCount = 0u;
    _shutdown = false;
    _paused = false;
    _idle = true;
    _callbackSet = false;
    _quantiser.setTempo((float)_cpg.getNode(0).getFrequency(sampleRate));
    _stepCounter = 0;

}


MatsuokaEngine::MatsuokaEngine(const MatsuokaEngine& rhs)
    : _cpg(rhs._sampleRate),
      _quantiser(rhs._quantiser)
{
    _sampleRate =  rhs._sampleRate;
    _eventOnRise = rhs._eventOnRise;
    _eventOnFall = rhs._eventOnFall;
    _fireOnPeak = rhs._fireOnPeak;
    _compCalcState = CompensationCalcState::IDLE;
    _fcc_sampleCount = 0u;
    _fcc_cycleCount = 0u;
    _shutdown = false;
    _paused = false;
    _idle = true;
    _quantiser.setTempo(60.0f / (float)_cpg.getNode(0).getFrequency(_sampleRate));

}



// IMMEDIATELY ACCTIONED FUNCTIONS
std::vector<unsigned> MatsuokaEngine::getNodeList() const
{ 
    return _cpg.getNodeList(); 
}

std::vector<input> MatsuokaEngine::getInputs(unsigned nodeID) const
{
    return _cpg.getInputs(nodeID);
}

bool MatsuokaEngine::nodeExists(unsigned nodeID) const 
{ 
    return _cpg.exists(nodeID); 
}

const MatsuNode& MatsuokaEngine::getNode(unsigned nodeID) const
{
    return _cpg.getNode(nodeID);
}


unsigned MatsuokaEngine::getSampleRate() const
{
    return (unsigned)_sampleRate;
}

double MatsuokaEngine::getNodeFrequency(unsigned nodeID) const
{
    return getNode(nodeID).getFrequency(_sampleRate);
}


unsigned MatsuokaEngine::getNodeParent(unsigned nodeID) const
{
    return getNode(nodeID).getParentID();
}

std::vector<unsigned> MatsuokaEngine::getNodeChildren(unsigned nodeID) const
{
    return getNode(nodeID).getChildIDs();

}

double MatsuokaEngine::getNodeOutput(unsigned nodeID, unsigned sampleOffset, bool matchQuantiser) const
{
    return !matchQuantiser ? 
				getNode(nodeID).getOutput(sampleOffset, true)
				: getNode(nodeID).getOutput(_quantiser.getNoteDelay() + sampleOffset, true);
}

double MatsuokaEngine::getNodePhaseOffset(unsigned nodeID) const
{
    auto &node = _cpg.getNode(nodeID);
    double samples = node.getOutputDelay();
    unsigned relativeTo = nodeID == 0 ? nodeID : node.getParentID();
    double freq = _cpg.getNode(relativeTo).getFrequency(_sampleRate);
    return (samples * freq) / _sampleRate;
}

double MatsuokaEngine::getNodeSelfNoise(unsigned nodeID) const
{
    return _cpg.getNode(nodeID).getSelfNoiseAmount();
}

double MatsuokaEngine::getNodeInputPhase(unsigned nodeID, unsigned inputID) const
{
    auto &node = _cpg.getNode(nodeID);
    double samples = node.getInputDelay(inputID);
    unsigned relativeTo = nodeID == 0 ? nodeID : node.getParentID();
    double freq = _cpg.getNode(relativeTo).getFrequency(_sampleRate);
    return (samples * freq) / _sampleRate;
}


// TODO: tuning for audiorate running: 
// do this using a ring buffer to avoid allocation of vector each cpl of samples
// if run every sample this accounts for about 8% of total CPU usage
const std::vector<MatsuokaEngine::outputEvent> MatsuokaEngine::getEvents()
{
    std::lock_guard<std::mutex> lock(_output_mutex);

    const std::vector<outputEvent> ret = _outputs;
    _outputs.clear();

    return ret;
}


void MatsuokaEngine::step()
{
    if (!_shutdown && !_paused) {
        _idle = false;
        // prevent actions being applied until config is complete
        if (_compCalcState == CompensationCalcState::IDLE) { doQueuedActions(); }
            // calculate next cpg step
        _cpg.step();
        _quantiser.tick();
        // compile output events
        fillOutputs();
        // check if in config state, and progress if so
        updateFreqCompensationState();
        _stepCounter++;
        _idle = true;
    }
}

void MatsuokaEngine::stepBareBones()
{
	if (!_shutdown && !_paused) {
		_idle = false;
		// calculate next cpg step
		_cpg.step();
		_stepCounter++;
		_idle = true;
	}
}



void MatsuokaEngine::step(unsigned nodeID)
{
    if (!_shutdown && !_paused) {
        _idle = false;
        // prevent actions being applied until config is complete
        if (_compCalcState == CompensationCalcState::IDLE) { doQueuedActions(); }
        // calculate next cpg step
        _cpg.step(nodeID);
        _quantiser.tick();

        // compile output events
        fillOutputs();
        // check if in config state, and progress if so
        updateFreqCompensationState();
        _idle = true;
    }
}


void MatsuokaEngine::calibrate()
{
    doQueuedActions();
    int stabilise = _sampleRate * 4;
    while (stabilise-- > 0) {
        _cpg.step();
    }
    calcFrequencyCompensation();
    stabilise = _sampleRate * (_fcc_cyclesPerCalc * 4);
    while (stabilise-- > 0) {
        _cpg.step();
        updateFreqCompensationState();
    }
    _outputs.clear();

}




void MatsuokaEngine::calcFrequencyCompensation()
{
    // record inputs and detatch
    _fcc_node0Inputs = _cpg.getInputs(0);
    std::vector<CPG::input>::const_iterator iter 
                                        = _fcc_node0Inputs.begin();
    while (iter != _fcc_node0Inputs.end()) {
        _cpg.setConnection(iter->sourceID, 0, 0.0);
        iter++;
    }
    // trigger calculations
    _compCalcState = CompensationCalcState::WAITING;
}


void MatsuokaEngine::quitFrequencyCompensationCalcs()
{
    // reset inputs to node 0 to saved state
    std::vector<CPG::input>::const_iterator iter
                                    = _fcc_node0Inputs.begin();
    while (iter != _fcc_node0Inputs.end()) {
        _cpg.setConnection(iter->sourceID, 0, iter->weight);
        iter++;
    }
    // enter idle state
    _compCalcState = CompensationCalcState::IDLE;
}


double MatsuokaEngine::getFrequencyCompensation() const
{
    return _cpg.getNode(0).getFreqCompensation();
}


// QUEUEING FUNCTIONS

void  MatsuokaEngine::clear() { 
    
    void (CPG::*fptr)(void) = &CPG::clear;

    std::function<void()> fn = std::bind(fptr, &_cpg);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}



/// Queue an addChild instruction
unsigned MatsuokaEngine::addChild(unsigned parentID, unsigned newID)
{
    void (CPG::*fptr)(unsigned, unsigned) = &CPG::addChild;

    std::function<void()> fn = std::bind(fptr, &_cpg, parentID, newID);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);

    return 0;
}

void MatsuokaEngine::reset() 
{
    void (CPG::*fptr)() = &CPG::reset;
    std::function<void()> fn = std::bind(fptr, &_cpg);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::reset(unsigned nodeID, double x1, double x2, double v1, double v2)
{

    void (CPG::*fptr)(unsigned, double, double, double, double) = &CPG::reset;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, x1, x2, v1, v2);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}



void MatsuokaEngine::setConnection(unsigned NodeFrom, unsigned nodeTo,
    double weight)
{
    void (CPG::*fptr)(unsigned, unsigned, double) 
                                = &CPG::setConnection;
    std::function<void()> fn = std::bind(fptr, &_cpg, NodeFrom, nodeTo, weight);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setConnection(unsigned NodeA, unsigned NodeB, 
    double weightAtoB, double weightBtoA)
{
    void (CPG::*fptr)(unsigned, unsigned, double, double)
        = &CPG::setConnection;
    std::function<void()> fn = std::bind(fptr, &_cpg,
                                    NodeA, NodeB, weightAtoB, weightBtoA);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}


void MatsuokaEngine::setConnectionPhaseOffset(unsigned NodeFrom, unsigned nodeTo,
    double phase)
{
    void (CPG::*fptr)(unsigned, unsigned, double)
        = &CPG::setConnectionPhaseOffset;
    std::function<void()> fn = std::bind(fptr, &_cpg, NodeFrom, nodeTo, phase);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}







void MatsuokaEngine::removeConnection(unsigned nodeFrom, unsigned nodeTo)
{
    if (nodeFrom == _cpg.getNode(nodeTo).getParentID()) {
        throw std::invalid_argument("cannot remove connection to parent");
    }

    void (CPG::*fptr)(unsigned, unsigned) = &CPG::removeConnection;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeFrom, nodeTo);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}


void MatsuokaEngine::moveNode(unsigned nodeID, unsigned newParentID,
    bool breakCurrParentChildConn, bool breakCurrChildParentConn)
{
    void (CPG::*fptr)(unsigned, unsigned, bool, bool)
        = &CPG::moveNode;
    std::function<void()> fn = std::bind(fptr, &_cpg,
                                            nodeID, newParentID, 
                                            breakCurrParentChildConn, 
                                            breakCurrChildParentConn);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::deleteNode(unsigned nodeID)
{
    void (CPG::*fptr)(unsigned)
        = &CPG::deleteNode;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setFreqCompensation(double compensation)
{
    void (CPG::*fptr)(double)
        = &CPG::setFreqCompensation;
    std::function<void()> fn = std::bind(fptr, &_cpg, compensation);
    // then update quantiser tempo
    std::function<void(void)> fn2
        = std::bind(&MatsuokaEngine::updateQuantiserTempo, this);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
    _actions.push(fn2);

}

void MatsuokaEngine::setParam_t2Overt1(double val)
{
    void (CPG::*fptr)(double)
        = &CPG::setParam_t2Overt1;
    std::function<void()> fn = std::bind(fptr, &_cpg, val);
    // then update quantiser tempo
    std::function<void(void)> fn2
        = std::bind(&MatsuokaEngine::updateQuantiserTempo, this);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
    _actions.push(fn2);

}

void MatsuokaEngine::setParam_c(double val)
{
    void (CPG::*fptr)(double)
        = &CPG::setParam_c;
    std::function<void()> fn = std::bind(fptr, &_cpg, val);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setParam_b(double val)
{
    void (CPG::*fptr)(double)
        = &CPG::setParam_b;
    std::function<void()> fn = std::bind(fptr, &_cpg, val);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setParam_g(double val)
{
    void (CPG::*fptr)(double)
        = &CPG::setParam_g;
    std::function<void()> fn = std::bind(fptr, &_cpg, val);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}


void MatsuokaEngine::setNodeFrequency(unsigned nodeID, double freq, 
    bool inherit)
{
    void (CPG::*fptr)(unsigned, double, bool)
        = &CPG::setNodeFrequency;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, freq, inherit);
    // then update quantiser tempo
    std::function<void(void)> fn2
        = std::bind(&MatsuokaEngine::updateQuantiserTempo, this);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
    _actions.push(fn2);

}


void MatsuokaEngine::setNodeFrequencyMultiple(unsigned nodeID, 
    double multipleOfParent,  bool inherit)
{
    void (CPG::*fptr)(unsigned, double, bool)
        = &CPG::setNodeFrequencyMultiple;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, multipleOfParent,
                                             inherit);
    // then update quantiser tempo
    std::function<void(void)> fn2
        = std::bind(&MatsuokaEngine::updateQuantiserTempo, this);


    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
    _actions.push(fn2);

}

void MatsuokaEngine::setNodePhaseOffset(unsigned nodeID, double phase)
{
    void (CPG::*fptr)(unsigned, double)
        = &CPG::setNodePhaseOffset;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, phase);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setNodeSelfNoise(unsigned nodeID, double amount)
{
    void (CPG::*fptr)(unsigned, double)
        = &CPG::setNodeSelfNoise;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, amount);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::setNodeSynchMode(unsigned nodeID, synchMode mode)
{
    void (CPG::*fptr)(unsigned, synchMode)
        = &CPG::setNodeSynchMode;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, mode);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}



void MatsuokaEngine::setNodeExternalInput(unsigned nodeID, double weight, double input)
{
    void (CPG::*fptr)(unsigned, double, double)
        = &CPG::setExternalInput;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID, input, weight);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}



void MatsuokaEngine::resetNodeChangeFlag_Params(unsigned nodeID)
{
    void (CPG::*fptr)(unsigned)
        = &CPG::resetNodeChangeFlag_Params;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

void MatsuokaEngine::resetNodeChangeFlag_Inputs(unsigned nodeID)
{
    void (CPG::*fptr)(unsigned)
        = &CPG::resetNodeChangeFlag_Inputs;
    std::function<void()> fn = std::bind(fptr, &_cpg, nodeID);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}



void MatsuokaEngine::setSampleRate(unsigned sampleRate)
{
    void (MatsuokaEngine::*fptr)(unsigned) = &MatsuokaEngine::_setSampleRate;
    std::function<void()> fn = std::bind(fptr, this, sampleRate);

    std::lock_guard<std::mutex> lock(_actions_mutex);
    _actions.push(fn);
}

bool MatsuokaEngine::loadConnectionWeightCurve(std::string source)
{
    return _cpg.loadWeightScalingCurve(source);
}
 
bool MatsuokaEngine::loadConnectionWeightCurve(std::vector<float> x, std::vector<float> y)
{
	return _cpg.loadWeightScalingCurve(x,y);
}

void MatsuokaEngine::setUnityConnectionWeight(float unity)
{
    _cpg.setUnityConnectionWeight(unity);
}

void MatsuokaEngine::setConnectionWeightScaling(bool on)
{
    _cpg.setConnectionWeightScaling(on);
}


void MatsuokaEngine::setNodeQuantiser_Grid(unsigned nodeID, gridType grid)
{
    _quantiser.setNodeGrid(nodeID, grid);
}


void MatsuokaEngine::setNodeQuantiser_Multiple(unsigned nodeID, float mult)
{
    _quantiser.setNodeGridMultiple(nodeID, (unsigned)mult);
}


void    MatsuokaEngine::setNodeQuantiser_Offset(unsigned nodeID, float off)
{
    _quantiser.setNodeGridOffset(nodeID, (unsigned)off);

}




MatsuokaEngine::gridType MatsuokaEngine::getNodeQuantiser_Grid(unsigned nodeID)
{
    return _quantiser.getNodeGrid(nodeID);
}


float MatsuokaEngine::getNodeQuantiser_Multiple(unsigned nodeID)
{
    return (float)_quantiser.getNodeGridMultiple(nodeID);
}


float    MatsuokaEngine::getNodeQuantiser_Offset(unsigned nodeID)
{
    return (float)_quantiser.getNodeGridOffset(nodeID);

}


unsigned MatsuokaEngine::getNodeQuantiser_BarDivision(unsigned nodeID)
{
    return _quantiser.getNodeBarDivision(nodeID);

}

void     MatsuokaEngine::setQuantiseAmount(float amount)
{
    _quantiser.setQuantiseAmount(amount);
}

void     MatsuokaEngine::setQuantiseAmount(unsigned node, float amount)
{
	_quantiser.setQuantiseAmount(node, amount);
}


float    MatsuokaEngine::getQuantiseAmount()
{
    return _quantiser.getQuantiseAmount();
}

float    MatsuokaEngine::getQuantiseAmount(unsigned node)
{
	return _quantiser.getQuantiseAmount(node);
}

void MatsuokaEngine::doQueuedActions()
{
    if (_actions.size() == 0) { return; }

    std::lock_guard<std::mutex> lock(_actions_mutex);
    while (!_actions.empty()) {
        std::function<void()> fn = _actions.front();
        _actions.pop();
        try {
            fn();
        } // TODO:  placeholder, needs to do more than this!
        catch (const std::runtime_error& e) {
            throw e;    
        }
    }
}

void MatsuokaEngine::setEventCallback(Callback cb)
{
    _eventCallback = cb;
    _callbackSet = true;
}



void MatsuokaEngine::setShutdown(bool shutdown) { _shutdown = shutdown; }
void MatsuokaEngine::setPause(bool paused) { _paused = paused; }

bool MatsuokaEngine::isShuttingDown() { return _shutdown; }
bool MatsuokaEngine::isIdle() { return _idle; }
uint64_t MatsuokaEngine::getEngineStepCounter(){ return _stepCounter; }


void MatsuokaEngine::setDriven(externalSync driven)
{
	if (driven != externalSync::none) {
		_cpg.createExternalSyncResources();
	}
	if (driven == externalSync::driving) {
		_cpg.setDriven(driven);
	}

}


void MatsuokaEngine::setDrivingInput(float val)
{
	_cpg.setDrivingInput(val);
}


void MatsuokaEngine::zeroSync(unsigned nodeID)
{
	_cpg.zeroSync(nodeID);
}


// PRIVATE ///////////////////////////////////////////////////////////////


void MatsuokaEngine::_setSampleRate(unsigned sampleRate)
{
    _sampleRate = sampleRate;
    _cpg.setSampleRate(sampleRate);
    _quantiser.setSampleRate(sampleRate);
}


// On optimised build this is the biggest bottleneck outside
// the RK calcs
void MatsuokaEngine::fillOutputs()
{
    using signalState = MatsuNode::signalState;

    std::lock_guard<std::mutex> lock(_output_mutex);

    const std::vector<unsigned> &IDs = _cpg.getNodeList();
    std::vector<unsigned>::const_iterator iter = IDs.begin();

    while (iter != IDs.end()) {
        MatsuNode const &thisNode = _cpg.getNode(*iter);


        if (_fireOnPeak &&
            (_eventOnRise &&thisNode.getSignalState() == signalState::firstPeak)) {
            outputEvent e = { thisNode.getIdentifier(), abs(thisNode.getLastMaxima()) };
            _quantiser.addEvent(e);
            synchroniseChildren(thisNode.getChildIDs());
      
        } else if (_fireOnPeak &&
            (_eventOnFall && thisNode.getSignalState() == signalState::firstTrough)) {
            outputEvent e = { thisNode.getIdentifier(), thisNode.getLastMinima() };
            _quantiser.addEvent(e);
            // TODO - not synchronising on fall,  - revisit
      
        } else if (!_fireOnPeak &&
            (_eventOnRise &&thisNode.getSignalState() == signalState::zeroXup) ) {
            outputEvent e = { thisNode.getIdentifier(), 1.0 };
            _quantiser.addEvent(e);
            synchroniseChildren(thisNode.getChildIDs());
		}
		else if (!_fireOnPeak &&
			(_eventOnFall && thisNode.getSignalState() == signalState::zeroXdown)) {
			outputEvent e = { thisNode.getIdentifier(), -1.0 };
			_quantiser.addEvent(e);
			synchroniseChildren(thisNode.getChildIDs());
		}
        iter++;
    }


    for (auto ev : _quantiser.getNotes()) {
        _outputs.push_back(ev);
        if (_eventCallback) {
            _eventCallback(ev.nodeID, (float)ev.velocity);
        }
    }
}


void MatsuokaEngine::updateFreqCompensationState()
{
    using signalState =  MatsuNode::signalState;

    switch (_compCalcState) {
        case CompensationCalcState::IDLE:
            break;

        case  CompensationCalcState::WAITING:
            if (_cpg.getNode(0).getSignalState() == signalState::zeroXup) {
                _compCalcState = CompensationCalcState::COUNTING;
                _fcc_sampleCount = 0;
                _fcc_cycleCount = 0;
            }
            break;
    
        case CompensationCalcState::COUNTING:
            _fcc_sampleCount++;
            if (_cpg.getNode(0).getSignalState() == signalState::zeroXup) {
                if (++_fcc_cycleCount == _fcc_cyclesPerCalc) {
                    doFreqCompensationCalcs();
                }
            }

    }
}

// TODO: Move into the node, keep an extra disconnected dummy node and
// just call calcs on that node. Lose all the messy structure at this level
void MatsuokaEngine::doFreqCompensationCalcs()
{
    double desired = _cpg.getNode(0).getFrequency(_sampleRate);
    double actual = (double)_sampleRate / (double)(_fcc_sampleCount / _fcc_cyclesPerCalc);
    double error = actual / desired;
    double newCompensation = _cpg.getNode(0).getFreqCompensation() * error;

    _actions_mutex.lock();
    _cpg.setFreqCompensation(newCompensation);
    // re-set the original frequencies based on new compensation value;
    _cpg.setNodeFrequency(0, desired, true);
    _actions_mutex.unlock();


    _fcc_sampleCount = 0;
    _fcc_cycleCount = 0;
    quitFrequencyCompensationCalcs();
}


void MatsuokaEngine::synchroniseChildren(std::vector<unsigned> childIDs)
{
    for (auto id : childIDs) {
        if (_cpg.getNodeSynchMode(id) == synchMode::synchLock) {
            _cpg.reset(id);
            return;
        }
        if (_cpg.getNodeSynchMode(id) == synchMode::synchOnce) {
            _cpg.reset(id);
            _cpg.setNodeSynchMode(id, synchMode::free);
        }
    }
}


void MatsuokaEngine::updateQuantiserTempo()
{
    float freq = (float)_cpg.getNode(0).getFrequency(_sampleRate);
    _quantiser.setTempo(60.0f * freq);
}


