#include "QuantiseGrid_soft.h"



QuantiseGrid_soft::noteCoordinate::noteCoordinate(uint64_t g, float p)
{
    gridline = g;
    phase = p;
}


bool QuantiseGrid_soft::noteCoordinate::operator< (const noteCoordinate& other)
{
    if (gridline < other.gridline) {
        return true;
    }
    if (gridline == other.gridline) {
        return phase < other.phase;
    }
    return false;
}

bool QuantiseGrid_soft::noteCoordinate::operator<= (const noteCoordinate& other)
{
    if (gridline < other.gridline) {
        return true;
    }
    if (gridline == other.gridline) {
        return phase <= other.phase;
    }
    return false;
}



QuantiseGrid_soft::QuantiseGrid_soft(unsigned id, unsigned divisions, unsigned sampleRate, float tempo)
{
    _id = id;
    _divisions = (float)divisions;
    _sampleRate = (float)sampleRate;
    _tempo = tempo;
    _gridMarker = 0;
    _nextBar = (float)_divisions;
    _phase = 0.0f;
    _setPhaseDelta();
    _quantiseAmount = 1;
    _delayGrid = 0;
    _delayPhase = 0;
}


QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::tick()
{
    _phase += _phaseDelta;
    if (_phase >= 1.0) {
        _phase -= 1.0;
        _incrementDivision();
    }

    return noteCoordinate(_gridMarker, _phase)
        ;
}

unsigned QuantiseGrid_soft::getID()
{
    return _id;
}


QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::getMarker()
{
    return noteCoordinate(_gridMarker, _phase);
}



QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::getBarStart()
{
    return noteCoordinate(_gridMarker - (_gridMarker % (int)_divisions), _phase);
}

QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::getPlaceWithinBar()
{
    return noteCoordinate(_gridMarker - getBarStart().gridline, _phase);
}


QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::jumpToNextBar()
{
    _phase = 0;
    _gridMarker = _nextBar;
    _nextBar = _gridMarker + _divisions;
    return noteCoordinate(_gridMarker, _phase);
}


void QuantiseGrid_soft::resetToBarStart()
{
    if (getPlaceWithinBar().gridline == 0) {
        _phase = 0;
        return;
    }
    _phase = 0;
    _gridMarker = _nextBar;
    _nextBar = _gridMarker + _divisions;
}

QuantiseGrid_soft::noteCoordinate QuantiseGrid_soft::jumpToNextDivision()
{
    _phase = 0;
    _incrementDivision();
    return noteCoordinate(_gridMarker, _phase);
}




void QuantiseGrid_soft::setTempo(float tempo)
{
    _tempo = tempo;
    _setPhaseDelta();
}


void QuantiseGrid_soft::setSampleRate(unsigned sampleRate)
{
    _sampleRate = (float)sampleRate;
    _setPhaseDelta();
}

void QuantiseGrid_soft::setDelay(unsigned samples)
{
    float minDelay = samples * _phaseDelta;
    _delayGrid = floor(minDelay);
    _delayPhase = minDelay - _delayGrid;
}


float QuantiseGrid_soft::getTempo() const
{
    return _tempo;
}

unsigned QuantiseGrid_soft::getSampleRate() const
{
    return (unsigned)_sampleRate;
}



bool QuantiseGrid_soft::getIsNoteDue(QuantiseGrid_soft::noteCoordinate noteMarker)
{
    if (noteMarker.gridline < _gridMarker) { return true; }
    if (noteMarker.gridline == _gridMarker) {
        return noteMarker.phase < _phase;
    }
    return false;
}

void QuantiseGrid_soft::setQuantiseAmount(float amount)
{
    _quantiseAmount_display = amount;
    amount = amount > 1 ? 1 : amount;
    amount = amount < 0 ? 0 : amount;
    amount = 1 - amount;
    _quantiseAmount = 1 - (amount * amount * amount);
}

float QuantiseGrid_soft::getQuantiseAmount()
{
    return _quantiseAmount_display;
}


QuantiseGrid_soft::noteCoordinate  QuantiseGrid_soft::getNoteCoordinate(unsigned gridMultiple, unsigned offset)
{
    // project forward by min delay - all notes are delayed this much
    uint64_t rawGrid = _gridMarker + _delayGrid;
    float rawPhase = _phase + _delayPhase;

    uint64_t prevValidGridline = getPrevValidGridline(rawGrid, gridMultiple, offset);
    uint64_t nextValidGrid = prevValidGridline + gridMultiple;
    uint64_t targetGrid;

    if (prevValidGridline < _gridMarker) {
        // if lastValidGrid is in the past, then nextValidGrid is target by default
        targetGrid = nextValidGrid;
    } else {
        // find the closest gridMarker
		// llabs not available on some platforms
        //if (llabs(prevValidGridline - rawGrid) < llabs(nextValidGrid - rawGrid)) {
		if ((rawGrid - prevValidGridline) < (nextValidGrid - rawGrid)) {
            targetGrid = prevValidGridline;
        } else {
            targetGrid = nextValidGrid;
        }
    }
    // variable quantisation: work out how far to move towards the target
    int gridMove = (targetGrid - rawGrid);
    float move = ((float(gridMove) - _phase) * _quantiseAmount);
    uint64_t finalGrid = rawGrid + floor(move);
    // handle numeric wraparound (should only ever happen in first bar, and once per several hours)
    if (finalGrid > (rawGrid + _divisions * 100)) {
        // 100 bars diffference is an arbitrary large number that shouldn't occur
        // and should reliably indicate that we've moved backwards to before zero
        finalGrid += (floor(move) * 2);
    }
    float finalPhase = _phase + move - floor(move);
    if (finalGrid >= 1.0) {
        finalGrid -= 1.0;
        finalGrid += 1;
    }
    return noteCoordinate(finalGrid, finalGrid);
}


void QuantiseGrid_soft::_setPhaseDelta()
{
    _phaseDelta = (_tempo * _divisions) / (_sampleRate * 60);
}

void QuantiseGrid_soft::_incrementDivision()
{
    if (_gridMarker++ == _nextBar) {
        _nextBar += (uint64_t)_divisions;
    }
}



uint64_t QuantiseGrid_soft::getPrevValidGridline(uint64_t position, unsigned mult, unsigned offset)
{
    uint64_t prevValidGridline = position + (offset % mult) - (position % mult);
    
    if (prevValidGridline > position) {
        prevValidGridline -= mult;
    }

    return prevValidGridline;
}