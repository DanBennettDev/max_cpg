#include "MatsuNode.h"

using std::vector;

MatsuNode::MatsuNode()
	:MatsuNode(0)
{
	return;
}

MatsuNode::MatsuNode(unsigned id)
{
	create(id, T1_INIT, T2_INIT, C_INIT, C_INIT, B_INIT, G_INIT);
}


MatsuNode::MatsuNode(unsigned id, double t1, double t2, double c,
	double b, double g)
{
	create(id, t1, t2, c, c, b, g);
}


MatsuNode::MatsuNode(unsigned id, double t1, double t2, double c1,
	double c2, double b, double g)
{
	create(id, t1, t2, c1, c2, b, g);
}

unsigned MatsuNode::getIdentifier() const
{
	return _identifier;
}


void MatsuNode::setIdentifier(unsigned id)
{
	if (id >= MAX_NODES) {
		throw std::invalid_argument("ID > MAX no of permitted nodes");
	}
	_identifier = id;
}


void MatsuNode::doCalcStep(bool basicsOnly, bool withInput)
{
#ifdef _DEBUG_LOG
	prevState = matsuParams.state;
#endif
	double input = withInput ? getInput() : 0;
	double calcVal = matsuoka_calc_nextVal_RK(input,
		matsuParams.t1, matsuParams.t2,
		matsuParams.c1, matsuParams.c2,
		matsuParams.b, matsuParams.g,
		&matsuParams.state);

	if(_driven){
		matsuParams.out.pushSample(_drivenValue);
	} else {
		matsuParams.out.pushSample(calcVal);
	}

	if (!basicsOnly) {
		updateSignalState();
		doAuxiliaryStepActions();
	}
}


void MatsuNode::doAuxiliaryStepActions()
{
	;// to be overriden by classes extending MatsuNode
}

double MatsuNode::getInput() const
{
	double sum = 0.0, in;

	for (auto &inpt : _inputs) {
		in = inpt.node->getOutput(inpt.delay);
		sum += in * inpt.weight;
	}

	sum += _ext_input;

	if (_selfNoiseAmount > 0.0) {
		sum += XORRand::nextSigVal() * _selfNoiseAmount;
	}

	return sum;
}


double MatsuNode::getOutput(bool forceStateVal) const
{
	if (forceStateVal) {
		return matsuParams.state.out;
	}
	return matsuParams.out.getDelayed(_nodeOutputDelay);
}


double MatsuNode::getOutput(unsigned samplesDelay, bool startAtPhaseOffset) const
{
	samplesDelay += startAtPhaseOffset ? _nodeOutputDelay : 0;
	if (samplesDelay > matsuParams.out.size()) {
		samplesDelay = (unsigned)matsuParams.out.size();
	}
	return matsuParams.out.getDelayed(samplesDelay);
}





double MatsuNode::getLastMaxima() const
{
	return _lastMaxima;
}

double MatsuNode::getLastMinima() const
{
	return _lastMinima;
}




MatsuNode::signalState MatsuNode::getSignalState() const
{
	return _signalState;
}


double MatsuNode::getInternal(matsuInternal param) const
{
	switch (param) {
	case matsuInternal::X1:
		return matsuParams.state.x1;
		break;
	case matsuInternal::X2:
		return matsuParams.state.x2;
		break;
	case matsuInternal::V1:
		return matsuParams.state.v1;
		break;
	case matsuInternal::V2:
		return matsuParams.state.v2;
		break;
	default:
		throw std::invalid_argument("invalid parameter");
		break;
	}
}

void MatsuNode::setFreqCompensation(double comp)
{
	_freqCompensation = comp > MINFREQCOMPENSAITON ? comp : DEFAULTFREQCOMPENSAITON;
}

double MatsuNode::getFreqCompensation() const
{
	return _freqCompensation;
}



void MatsuNode::resetFreqCompensation()
{
	_freqCompensation = DEFAULTFREQCOMPENSAITON;
}

void MatsuNode::reset()
{
	reset(X1_INIT, X2_INIT, V1_INIT, V2_INIT);
}

void MatsuNode::reset(double x1, double x2, double v1, double v2)
{
	matsuParams.state.x1 = x1;
	matsuParams.state.x2 = x2;
	matsuParams.state.v1 = v1;
	matsuParams.state.v2 = v2;
	matsuParams.out.pushSample(
		POSPART(matsuParams.state.x1) - POSPART(matsuParams.state.x2));
}


void MatsuNode::resetChangeFlag_Params() { _haveParamsChanged = false; }

void MatsuNode::resetChangeFlag_Inputs() { _haveInputsChanged = false; }

bool MatsuNode::hasChanged_Params() const { return _haveParamsChanged; }

bool MatsuNode::hasChanged_Inputs() const { return _haveInputsChanged; }



// see K. Matsuoka, "Analysis of a neural oscillator," Biological cybernetics, 
// vol. 104, no. 4-5,pp. 297{304, 2011.
// sets frequency of oscillator based on current internal parameters
// changes t1 and t2 but not value of t2/t1
// invalidated by direct change of parameters
// Only accurate within limited range of values of B, G
// compensation must be recalculated if b,g change
void MatsuNode::setFrequency(double freq, unsigned sampleRate)
{
	if (freq < MIN_NODE_FREQ) {
		freq = MIN_NODE_FREQ;
	};

	double mult = matsuParams.t2 / matsuParams.t1;

	double t1 = (_freqCompensation*sampleRate / (freq * TWOPI)) *
		sqrt((matsuParams.b + (matsuParams.b * mult) - (matsuParams.g * mult)) /
		(matsuParams.g * mult));

	if (validateT(t1)) {
		double t2 = t1 * mult;
		if (validateT(t2)) {
			matsuParams.t1 = t1;
			matsuParams.t2 = t2;
			_haveParamsChanged = true;
		}
	}
}

void MatsuNode::setDrivenMode(bool driven)
{
	_driven = driven;
}



void MatsuNode::driveOutput(float val)
{
	_drivenValue = val;
}


double MatsuNode::getFrequency(unsigned sampleRate) const
{
	double comp = _freqCompensation * sampleRate;
	double mult = matsuParams.t2 / matsuParams.t1;

	//return (comp / matsuParams.t2)*
	//            sqrt(((matsuParams.t1 + matsuParams.t2) * matsuParams.b
	//                - matsuParams.t1*matsuParams.g) /
	//                (matsuParams.t1 *matsuParams.g));


	return (_freqCompensation*sampleRate / (matsuParams.t1 * TWOPI)) *
		sqrt((matsuParams.b + (matsuParams.b * mult) - (matsuParams.g * mult)) /
		(matsuParams.g * mult));
}


// returns true if input is added, false if already exists
bool MatsuNode::setInputWeight(MatsuNode const &node, double weight)
{
	// check if node already exists as input
	for (auto &inpt : _inputs) {
		if (inpt.node == &node) {
			inpt.weight = weight;
			_haveInputsChanged = true;
			return false;
		}
		if (inpt.node->getIdentifier() == node.getIdentifier()) {
			throw std::invalid_argument("same node id already exists for different input");
		}
	}
	// if not, add input
	input in;
	in.node = &node;
	in.weight = weight;
	in.delay = 0;
	_inputs.push_back(in);
	_haveInputsChanged = true;
	return true;
}

void MatsuNode::setInputDelay(MatsuNode const &node, unsigned delay)
{
	if (delay > node.getDelayLineLength()) { delay = node.getDelayLineLength(); }
	for (auto &inpt : _inputs) {
		if (node.getIdentifier() == inpt.node->getIdentifier()) {
			inpt.delay = delay;
		}
	}
}




void MatsuNode::setSelfNoiseAmount(double amount)
{
	if (amount < 0) { amount = 0; }
	_selfNoiseAmount = amount;
}



bool MatsuNode::removeInput(unsigned identifier)
{

	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		if (iter->node->_identifier == identifier) {
			_inputs.erase(iter);
			_haveInputsChanged = true;
			return true;    // input removed
		}
	}
	return false;   // node was not an input, no change
}

bool MatsuNode::isInput(unsigned identifier) const
{
	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		if (iter->node->getIdentifier() == identifier) {
			return true;
		}
	}
	return false;
}


vector<unsigned> MatsuNode::getInputIds() const
{
	vector<unsigned> out;
	out.reserve(_inputs.size());
	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		out.push_back(iter->node->_identifier);
	}
	std::sort(out.begin(), out.end());
	return out;
}



// ToDo - should I handle not found differently?
double MatsuNode::getInputWeight(unsigned identifier) const
{
	// check if node exists as input
	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		if (iter->node->_identifier == identifier) {
			return iter->weight;
		}
	}
	return 0.0;
}

double MatsuNode::getInputDelay(unsigned identifier) const
{
	// check if node exists as input
	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		if (iter->node->_identifier == identifier) {
			return iter->delay;
		}
	}
	return 0.0;
}




const MatsuNode* MatsuNode::getInputNode(unsigned identifier) const
{
	// check if node already exists as input
	for (auto iter = _inputs.begin(); iter != _inputs.end(); iter++) {
		if (iter->node->_identifier == identifier) {
			return iter->node;
		}
	}
	throw std::invalid_argument("invalid parameter");
}

double MatsuNode::getSelfNoiseAmount() const
{
	return _selfNoiseAmount;
}

unsigned MatsuNode::getOutputDelay() const
{
	return _nodeOutputDelay;
}

unsigned MatsuNode::getDelayLineLength() const
{
	return (unsigned)matsuParams.out.size();
}





void MatsuNode::clearInputs()
{
	_inputs.clear();
	_ext_input = 0.0;
	_haveInputsChanged = true;
}

void MatsuNode::setExternalInput(double const input)
{
	_ext_input = input;
	_haveInputsChanged = true;
}



double MatsuNode::calcFreqCompensation(int cycleCount, int sampleRate)
{
	// simulate the node for cycleCount cycles and measure freq
	if (cycleCount <= 0) {
		cycleCount = 1;
	}
	int cyclesCompleted = 0;
	_signalState = signalState::nonSignificant;
	bool isCounting = false;
	int counter = 0;
	int bail_out = CALIBRATION_STOP_LIMIT * cycleCount * sampleRate;

	while (cyclesCompleted < cycleCount && bail_out > 0) {
		doCalcStep(false, false);
		if (getSignalState() == signalState::zeroXup) {
			if (isCounting) {
				cyclesCompleted++;
			} else {
				isCounting = true;
			}
		}
		counter = isCounting ? counter + 1 : counter;
		bail_out--;
	}

	// node is in hung state - frequency = 0;
	if (bail_out <= 0) {
		return _freqCompensation;
	}

	double desired = getFrequency(sampleRate);
	double actual = (double)sampleRate / (double)(counter / cycleCount);
	double error = actual / desired;
	double _freqCompensation = getFreqCompensation() * error;
	return _freqCompensation > MINFREQCOMPENSAITON ? _freqCompensation : DEFAULTFREQCOMPENSAITON;
}


// throws exception if any parameters invalid
void MatsuNode::setParam(matsuParam param, double val)
{
	double t2;
	_haveParamsChanged = true;

	switch (param) {
	case matsuParam::T:
		if (validateT(val)) {
			matsuParams.t1 = val;
			matsuParams.t2 = val;
		}
		break;
	case matsuParam::T1:
		if (validateT(val)) {
			matsuParams.t1 = val;
		}
		break;
	case matsuParam::T2:
		if (validateT(val)) {
			matsuParams.t2 = val;
		}
		break;
	case matsuParam::T2OverT1:
		t2 = matsuParams.t1 * val;
		if (validateT(t2)) {
			matsuParams.t2 = t2;
		}
		break;
	case matsuParam::C:
		if (validateC(val)) {
			matsuParams.c1 = val;
			matsuParams.c2 = val;
		}
		break;
	case matsuParam::C1:
		if (validateC(val)) {
			matsuParams.c1 = val;
		}
		break;
	case matsuParam::C2:
		if (validateC(val)) {
			matsuParams.c2 = val;
		}
		break;
	case matsuParam::B:
		if (validateB(val)) {
			matsuParams.b = val;
		}
		break;
	case matsuParam::G:
		if (validateG(val)) {
			matsuParams.g = val;
		}
		break;
	default:
		throw std::invalid_argument("invalid parameter");
		break;
	}
}


void    MatsuNode::set_t(double val)
{
	if (val<T_MIN) {
		val = T_MIN;
	} else if (val > T_MAX) {
		val = T_MAX;
	}
	matsuParams.t1 = val;
	matsuParams.t2 = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_t1(double val)
{
	if (val<T_MIN) {
		val = T_MIN;
	} else if (val > T_MAX) {
		val = T_MAX;
	}
	matsuParams.t1 = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_t2(double val)
{
	if (val<T_MIN) {
		val = T_MIN;
	} else if (val > T_MAX) {
		val = T_MAX;
	}
	matsuParams.t2 = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_b(double val)
{
	if (val<B_MIN) {
		val = B_MIN;
	} else if (val > B_MAX) {
		val = B_MAX;
	}
	if (val < matsuParams.g) {
		val = matsuParams.g;
	}
	matsuParams.b = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_g(double val)
{
	if (val<G_MIN) {
		val = G_MIN;
	} else if (val > G_MAX) {
		val = G_MAX;
	}
	if (matsuParams.b < val) {
		matsuParams.b = val;
	}
	matsuParams.g = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_c1(double val)
{
	if (val<C_MIN) {
		val = C_MIN;
	} else if (val > C_MAX) {
		val = C_MAX;
	}
	matsuParams.c1 = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_c2(double val)
{
	if (val<C_MIN) {
		val = C_MIN;
	} else if (val > C_MAX) {
		val = C_MAX;
	}
	matsuParams.c2 = val;
	_haveParamsChanged = true;
}

void    MatsuNode::set_c(double val)
{
	if (val<C_MIN) {
		val = C_MIN;
	} else if (val > C_MAX) {
		val = C_MAX;
	}
	matsuParams.c1 = val;
	matsuParams.c2 = val;
	_haveParamsChanged = true;
}



void    MatsuNode::set_t2_over_t1(double val)
{
	double t2;
	_haveParamsChanged = true;
	t2 = matsuParams.t1 * val;
	if (t2<T_MIN) {
		t2 = T_MIN;
	} else if (t2 > T_MAX) {
		t2 = T_MAX;
	}
	matsuParams.t2 = t2;
}



void MatsuNode::setParams(double t1, double t2, double c1, double c2,
	double b, double g)
{
	if (validateParams(t1, t2, c1, c2, b, g)) {
		_haveInputsChanged = true;
		matsuParams.t1 = t1;    matsuParams.t2 = t2;
		matsuParams.c1 = c1;    matsuParams.c2 = c2;
		matsuParams.b = b;      matsuParams.g = g;
	}
}

void MatsuNode::setParams(double t1, double t2, double c,
	double b, double g)
{
	setParams(t1, t2, c, c, b, g);
}

double MatsuNode::getParam(matsuParam param) const
{
	switch (param) {
	case matsuParam::T:
	case matsuParam::T1:
		return matsuParams.t1;
		break;
	case matsuParam::T2:
		return matsuParams.t2;
		break;
	case matsuParam::T2OverT1:
		return matsuParams.t2 / matsuParams.t1;
	case matsuParam::C:
	case matsuParam::C1:
		return matsuParams.c1;
		break;
	case matsuParam::C2:
		return matsuParams.c2;
		break;
	case matsuParam::B:
		return matsuParams.b;
		break;
	case matsuParam::G:
		return matsuParams.g;
		break;
	default:
		throw std::invalid_argument("invalid parameter");
		break;
	}
}


void MatsuNode::setDelayLineLength(unsigned length)
{
	if (length < INIT_DELAY_LENGTH) { length = INIT_DELAY_LENGTH; }

	if (_nodeOutputDelay > length) {
		_nodeOutputDelay = length;
	}

	matsuParams.out.resize(length);
}


void MatsuNode::setNodeOutputDelay(unsigned samples)
{
	if (samples < MIN_DELAY_LENGTH) { samples = MIN_DELAY_LENGTH; }
	if (samples >= matsuParams.out.size()) {
		samples = (unsigned)matsuParams.out.size() - 1;
	}

	_nodeOutputDelay = samples;
}





// Hierarchy stuff



void MatsuNode::setParent(unsigned nodeID)
{
	if (isInput(nodeID)) {
		_haveInputsChanged = true;
		_parentNodeID = nodeID;
		return;
	}
	throw std::invalid_argument("node to be added as parent is not an input");
}


void MatsuNode::removeParent()
{
	_haveInputsChanged = true;
	_parentNodeID = -1;
}


bool MatsuNode::addChild(MatsuNode &node)
{
	if (&node == this) {
		throw std::invalid_argument("node cannot be its own child");

	}
	if (isChild(node)) { return false; }
	_children.push_back(&node);
	std::sort(_children.begin(), _children.end());
	_haveInputsChanged = true;
	return true;
}


bool MatsuNode::removeChild(unsigned nodeID)
{
	for (auto iter = _children.begin(); iter != _children.end(); iter++) {
		if ((*iter)->getIdentifier() == nodeID) {
			_children.erase(iter);
			_haveInputsChanged = true;
			return true;
		}
	}
	return false;
}


bool MatsuNode::removeChild(MatsuNode &node)
{
	for (auto iter = _children.begin(); iter != _children.end(); iter++) {
		if (*iter == &node) {
			_children.erase(iter);
			_haveInputsChanged = true;
			return true;
		}
	}
	return false;
}


int MatsuNode::getParentID() const { return _parentNodeID; }


bool MatsuNode::isChild(MatsuNode & node) const
{
	for (auto iter = _children.begin(); iter != _children.end(); iter++) {
		if (&node == *iter) { return true; }
	}
	return false;
}

bool MatsuNode::isChild(unsigned nodeID) const
{
	for (auto iter = _children.begin(); iter != _children.end(); iter++) {
		if ((*iter)->getIdentifier() == nodeID) { return true; }
	}
	return false;
}


std::vector<unsigned> MatsuNode::getChildIDs() const
{
	std::vector<unsigned> childIDs;
	childIDs.reserve(_children.size());
	for (auto iter = _children.begin(); iter != _children.end(); iter++) {
		childIDs.push_back((*iter)->getIdentifier());
	}
	return childIDs;
}


unsigned MatsuNode::getChildCount() const
{
	return (int)_children.size();
}




void MatsuNode::clone(MatsuNode &from)
{
	matsuParams.t1 = from.getParam(matsuParam::T1);
	matsuParams.t2 = from.getParam(matsuParam::T2);
	matsuParams.c1 = from.getParam(matsuParam::C1);
	matsuParams.c2 = from.getParam(matsuParam::C2);
	matsuParams.b = from.getParam(matsuParam::B);
	matsuParams.g = from.getParam(matsuParam::G);
	_freqCompensation = from.getFreqCompensation();
	_hasCrossedZero = 0;
	_signalState = signalState::nonSignificant;
	_lastMaxima = _lastMinima = 0;
	_haveParamsChanged = _haveInputsChanged = false;
}


void MatsuNode::setSynchMode(synchMode doSync) { _synchMode = doSync; }

MatsuNode::synchMode MatsuNode::getSynchMode() const { return _synchMode; }





///////////////////////////////////////////////////////////////////////////////
// PRIVATE
///////////////////////////////////////////////////////////////////////////////

void MatsuNode::create(unsigned id, double t1, double t2, double c1,
	double c2, double b, double g)
{
	_inputs.reserve(MAX_NODES);
	_children.reserve(MAX_NODES);

	if (validateParams(t1, t2, c1, c2, b, g)) {
		_active = true;
		_identifier = id;
		matsuParams.t1 = t1;
		matsuParams.t2 = t2;
		matsuParams.c1 = c1;
		matsuParams.c2 = c2;
		matsuParams.b = b;
		matsuParams.g = g;
		matsuParams.state.x1 = X1_INIT;
		matsuParams.state.x2 = X2_INIT;
		matsuParams.state.v1 = V1_INIT;
		matsuParams.state.v2 = V2_INIT;
		matsuParams.out.pushSample(
			POSPART(matsuParams.state.x1) - POSPART(matsuParams.state.x2));
		_selfNoiseAmount = 0.0;

		_nodeOutputDelay = 0;
		_freqCompensation = DEFAULTFREQCOMPENSAITON;
		_ext_input = 0.0;
		_parentNodeID = -1;
		_hasCrossedZero = 0;
		_signalState = signalState::nonSignificant;
		_lastMaxima = _lastMinima = 0;
		_haveParamsChanged = _haveInputsChanged = false;
		_synchMode = synchMode::free;
	}
}

void MatsuNode::uncreate()
{
	// nothing to do yuet
}


// throws exception if any parameters invalid
bool MatsuNode::validateParams(double t1, double t2, double c1, double c2,
	double b, double g)
{
	return validateT(t1) || validateT(t2) || validateC(c1)
			|| validateC(c2) || validateB(b) || validateG(g);
}

bool MatsuNode::validateT(double t)
{
	if (t != t || t<T_MIN || t>T_MAX) {
		return false;
	}
	return true;
}

bool MatsuNode::validateC(double c)
{
	if (c!=c || c<C_MIN || c>C_MAX) {
		return false;
	}
	return true;
}

bool MatsuNode::validateB(double b)
{
	if (b!=b || b<B_MIN || b>B_MAX) {
		return false;
	}
	return true;
}

bool MatsuNode::validateG(double g)
{
	if (g!=g || g<G_MIN || g>G_MAX) {
		return false;
	}
	return true;
}


void MatsuNode::updateSignalState()
{

	if (matsuParams.out.getDelayed(_nodeOutputDelay) > 0
		&& matsuParams.out.getDelayed(_nodeOutputDelay + 1) <= 0) {
		_signalState = signalState::zeroXup;
		_hasCrossedZero = 1;
		return;
	}
	if (matsuParams.out.getDelayed(_nodeOutputDelay) < 0
		&& matsuParams.out.getDelayed(_nodeOutputDelay + 1) >= 0) {
		_signalState = signalState::zeroXdown;
		_hasCrossedZero = -1;
		return;
	}
	if (_hasCrossedZero == 1
		&& matsuParams.out.getDelayed(_nodeOutputDelay)
		< matsuParams.out.getDelayed(_nodeOutputDelay + 1)) {
		_signalState = signalState::firstPeak;
		_lastMaxima = matsuParams.out.getDelayed(_nodeOutputDelay + 1);
		_hasCrossedZero = 0;

#ifdef _DEBUG_LOG
		// dump last 2 states (_DEBUG_LOG only)
		if (_identifier != 0 && prevState.x1 > 0.475) {
			std::string out;
			out = std::to_string(matsuParams.t1) + ", ";
			out += std::to_string(matsuParams.t2) + ", ";
			out += std::to_string(matsuParams.c1) + ", ";
			out += std::to_string(matsuParams.c2) + ", ";
			out += std::to_string(matsuParams.b) + ", ";
			out += std::to_string(matsuParams.g) + ", , ";
			out += std::to_string(prevState.v1) + ", ";
			out += std::to_string(prevState.v2) + ", ";
			out += std::to_string(prevState.x1) + ", ";
			out += std::to_string(prevState.x2) + ", , ";
			out += std::to_string(_freqCompensation) + ", , ";

			dtb_text_to_log(out);
		}


#endif
		return;
	}
	if (_hasCrossedZero == -1
		&& matsuParams.out.getDelayed(_nodeOutputDelay)
	> matsuParams.out.getDelayed(_nodeOutputDelay + 1)) {
		_signalState = signalState::firstTrough;
		_lastMinima = matsuParams.out.getDelayed(_nodeOutputDelay + 1);
		_hasCrossedZero = 0;
		return;
	}
	_signalState = signalState::nonSignificant;
}