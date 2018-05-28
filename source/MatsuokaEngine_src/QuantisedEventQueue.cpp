#include "QuantisedEventQueue.h"



QuantisedEventQueue::QuantisedEventQueue(unsigned sampleRate, float tempo)
    :   _grid24(0, 24, sampleRate, tempo), 
        _grid32(0,32,sampleRate,tempo),
    _mark24(0, 0),
    _mark32(0, 0)
{
    _tempo = tempo;
    _sampleRate = sampleRate;
    setNoteDelays();
    _quantiseAmount = 1;

    setNodeGrid(0, gridType::unQuantised);
    for (int i = 1; i < MAX_NODES; i++) {
        setNodeGrid(i, gridType::_32nd);
        setNodeGridMultiple(i, 8);
    }
}

QuantisedEventQueue::QuantisedEventQueue(const QuantisedEventQueue &rhs)
    : _grid24(0, 24, rhs.getSampleRate(), rhs.getTempo()),
    _grid32(0, 32, rhs.getSampleRate(), rhs.getTempo()),
    _mark24(0, 0),
    _mark32(0, 0)
{
    _tempo = rhs.getTempo();
    _sampleRate = rhs.getSampleRate();
    setNoteDelays();
}




void QuantisedEventQueue::addEvent(outputEvent e)
{
    if (nodes[e.nodeID].grid!= gridType::unQuantised) {
        unsigned mult = nodes[e.nodeID].multiple;
        unsigned off = nodes[e.nodeID].offset;
        std::list<queuedEvent>::iterator eventIt;
        queuedEvent qe ;
        qe.event = e;
        // add to correct place in relevant queue
        switch (nodes[e.nodeID].grid) {
            case gridType::_32nd:
            {
                std::lock_guard<std::mutex> lock(_queue_mutex);
				_grid32.setQuantiseAmount(nodes[e.nodeID].amount * _quantiseAmount);
                qe.queueMarker = _grid32.getNoteCoordinate(mult, off);
                eventIt = _eventQueue32.begin();
                if (_eventQueue32.size() > 0) {
                    while (eventIt != _eventQueue32.end() &&
                        eventIt->queueMarker < qe.queueMarker ) {
                        eventIt++;
                    }
                }
                _eventQueue32.insert(eventIt, qe);
                return;
            }
            case gridType::_24th:
            {
                std::lock_guard<std::mutex> lock(_queue_mutex);
				_grid24.setQuantiseAmount(nodes[e.nodeID].amount * _quantiseAmount);
                qe.queueMarker = _grid24.getNoteCoordinate(mult, off);
                eventIt = _eventQueue24.begin();
                if (_eventQueue24.size() > 0) {
                    while (eventIt != _eventQueue24.end() &&
                            eventIt->queueMarker < qe.queueMarker) {
                        eventIt++;
                    }
                }
                _eventQueue24.insert(eventIt, qe);

                return;
            }
        }

    } else {
        queuedEvent_free qe;
        qe.event = e;
        qe.countDown = (float)_delayNotes;
        _eventQueueFree.push_back(qe);
    }
}



void QuantisedEventQueue::tick()
{
    for (auto &e : _eventQueueFree) {
        e.countDown -= e.delta;
    }
    _mark32 = _grid32.tick();
    _mark24 = _grid24.tick();
}


std::vector<QuantisedEventQueue::outputEvent> QuantisedEventQueue::getNotes()
{
    std::vector<outputEvent> output = std::vector<outputEvent>();

    std::lock_guard<std::mutex> lock(_queue_mutex);
    if (!_eventQueue24.empty() && _eventQueue24.begin()->queueMarker <= _mark24) {
        auto eventIt = _eventQueue24.begin();
        while (eventIt != _eventQueue24.end() && eventIt->queueMarker <= _mark24) {
				output.push_back(eventIt->event);
            eventIt = _eventQueue24.erase(eventIt);
        }
    }
    if (!_eventQueue32.empty() && _eventQueue32.begin()->queueMarker <= _mark32) {
        auto eventIt = _eventQueue32.begin();
        while (eventIt!= _eventQueue32.end() && eventIt->queueMarker <= _mark32) {
				output.push_back(eventIt->event);
            eventIt = _eventQueue32.erase(eventIt);
        }
    }
    while (!_eventQueueFree.empty() && _eventQueueFree.begin()->countDown<=0) {
        output.push_back(_eventQueueFree.front().event);
        // note 0 is the root and sets the tempo
        if (_eventQueueFree.front().event.nodeID == 0) {
            syncToBarStart();
        }
        _eventQueueFree.pop_front();
    }

    return output;
}

void QuantisedEventQueue::setSampleRate(unsigned sampleRate)
{
    _sampleRate = sampleRate;
    _grid24.setSampleRate(sampleRate);
    _grid32.setSampleRate(sampleRate);
    setNoteDelays();

}

void QuantisedEventQueue::setTempo(float tempo)
{
    float changeTempo = tempo/_tempo;
    _tempo = tempo;
    _grid24.setTempo(tempo);
    _grid32.setTempo(tempo);
    setNoteDelays();

    for (auto e : _eventQueueFree) {
        e.delta *= changeTempo;
    }
}

void QuantisedEventQueue::setQuantiseAmount(float amount)
{
    if (amount < 0) {
        amount = 0;
    } else if (amount > 1) {
        amount = 1;
    }
    _quantiseAmount = amount;
    _grid24.setQuantiseAmount(amount);
    _grid32.setQuantiseAmount(amount);
}


void QuantisedEventQueue::setQuantiseAmount(unsigned node, float amount)
{
	if (amount < 0) {
		amount = 0;
	}
	else if (amount > 1) {
		amount = 1;
	}
	nodes[node].amount = amount;
}



float QuantisedEventQueue::getQuantiseAmount()
{
    return _quantiseAmount;
}

float QuantisedEventQueue::getQuantiseAmount(unsigned node)
{
	return nodes[node].amount;
}



unsigned QuantisedEventQueue::getSampleRate() const
{
    return _sampleRate;
}

float QuantisedEventQueue::getTempo() const
{
    return _tempo;
}

void QuantisedEventQueue::setNodeGrid(unsigned nodeID, gridType grid)
{
    if (nodeID == 0) {
        // node 0 must remain free running - it syncs the rest
        return;
    }
    nodes[nodeID].grid = grid;
}


void QuantisedEventQueue::setNodeGridMultiple(unsigned nodeID, unsigned mult)
{
    nodes[nodeID].multiple = mult;
}

void QuantisedEventQueue::setNodeGridOffset(unsigned nodeID, unsigned off)
{
    nodes[nodeID].offset = off;
}


QuantisedEventQueue::gridType QuantisedEventQueue::getNodeGrid(unsigned nodeID)
{
    return nodes[nodeID].grid;
}

unsigned QuantisedEventQueue::getNodeGridMultiple(unsigned nodeID)
{
    return nodes[nodeID].multiple;
}

unsigned QuantisedEventQueue::getNodeGridOffset(unsigned nodeID)
{
    return nodes[nodeID].offset;

}

unsigned QuantisedEventQueue::getNodeBarDivision(unsigned nodeID)
{
    float grid;
    if (nodes[nodeID].grid == gridType::_24th) {
        grid = 24;
    } else if (nodes[nodeID].grid == gridType::_32nd) {
        grid = 32;
    } else {
        return 0;
    }

    return (unsigned)(grid / nodes[nodeID].multiple);
}



void QuantisedEventQueue::syncToBarStart()
{
        _grid24.resetToBarStart();
        _grid32.resetToBarStart();
}


void QuantisedEventQueue::setNoteDelays()
{
    // all notes are delayed a 1/4 note - this allows free and 
    // quantised notes to be played alongside each other
    _delayNotes =  (unsigned)((_sampleRate * 60 ) / (_tempo * 4));
    _grid24.setDelay(_delayNotes);
    _grid32.setDelay(_delayNotes);
}

unsigned QuantisedEventQueue::getNoteDelay() const
{
	return _delayNotes;
}