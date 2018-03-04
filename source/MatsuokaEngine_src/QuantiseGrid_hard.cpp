#include "QuantiseGrid_hard.h"

QuantiseGrid_hard::QuantiseGrid_hard(unsigned id, unsigned divisions, unsigned sampleRate, float tempo)
{
    _id = id;
    _divisions = (float)divisions;
    _sampleRate = (float)sampleRate;
    _tempo = tempo;
    _gridMarker = 0;
    _nextBar = (float)_divisions;
    _phase = 0.0f;
    _setPhaseDelta();
}


uint64_t QuantiseGrid_hard::tick()
{
    _phase += _phaseDelta;
    if (_phase >= 1.0) {
        _phase -= 1.0;
        _incrementDivision();
    }

    return _gridMarker;
}

unsigned QuantiseGrid_hard::getID()
{
    return _id;
}

uint64_t QuantiseGrid_hard::getMarker()
{
    return _gridMarker;
}

uint64_t QuantiseGrid_hard::getBarStart()
{
    return _gridMarker - (_gridMarker % (int)_divisions);
}

uint64_t QuantiseGrid_hard::getPlaceWithinBar()
{
    return _gridMarker - getBarStart();
}

uint64_t QuantiseGrid_hard::jumpToNextBar()
{
    _phase = 0;
    _gridMarker = _nextBar;
    _nextBar = _gridMarker + _divisions;
    return _gridMarker;
}

uint64_t QuantiseGrid_hard::jumpToNextDivision()
{
    _phase = 0;
    _incrementDivision();
    return _gridMarker;
}

void QuantiseGrid_hard::setTempo(float tempo)
{
    _tempo = tempo;
    _setPhaseDelta();
}


void QuantiseGrid_hard::setSampleRate(unsigned sampleRate)
{
    _sampleRate = (float)sampleRate;
    _setPhaseDelta();
}


float QuantiseGrid_hard::getTempo() const
{
    return _tempo;
}

unsigned QuantiseGrid_hard::getSampleRate() const
{
    return (unsigned)_sampleRate;
}



bool QuantiseGrid_hard::getIsNoteDue(uint64_t noteMarker)
{
    return noteMarker <= _gridMarker;
}

uint64_t   QuantiseGrid_hard::getNoteCoordinate()
{
    return _phase < 0.5 ? _gridMarker + 1 : _gridMarker + 2;
}

// overload to handle coarser grids and offsets, for more traditional sequencing
// returns the next qualifying coordinate
uint64_t  QuantiseGrid_hard::getNoteCoordinate(unsigned gridMultiple, unsigned offset)
{
    uint64_t base = getNoteCoordinate() - 1;
    return base + gridMultiple + offset - (base % gridMultiple);
}




void QuantiseGrid_hard::_setPhaseDelta()
{
    _phaseDelta =  (_tempo * _divisions)/ (_sampleRate * 60.0);
}

void QuantiseGrid_hard::_incrementDivision()
{
    if (_gridMarker++ == _nextBar) {
        _nextBar += (uint64_t)_divisions;
    }
}