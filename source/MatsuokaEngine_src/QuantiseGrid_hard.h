#pragma once

#include <cstdint>

/// legacy - ignore
class QuantiseGrid_hard
{

public:

    QuantiseGrid_hard(unsigned id, unsigned divisions, unsigned sampleRate, 
                        float tempo);

    uint64_t tick();

    unsigned getID();

    uint64_t getMarker();

    uint64_t getBarStart();

    uint64_t getPlaceWithinBar();

    uint64_t jumpToNextBar();

    uint64_t jumpToNextDivision();

    void setTempo(float tempo);

    void setSampleRate(unsigned sampleRate);

    float getTempo() const;

    unsigned getSampleRate() const;

    bool   getIsNoteDue(uint64_t noteMarker);

    uint64_t   getNoteCoordinate();

    // overload to handle coarser grids and offsets, for more traditional sequencing
    // returns the next qualifying coordinate
    uint64_t  getNoteCoordinate(unsigned gridMultiple, unsigned offset);


private:

    void _setPhaseDelta();
    void _incrementDivision();

    unsigned _id;
    double _phase;
    unsigned _sampleRate;
    float _divisions;
    float _tempo;
    double _phaseDelta;
    uint64_t _gridMarker;
    uint64_t _nextBar;


};