#pragma once

#include <cstdint>
#include <math.h>

/*! \class QuantiseGrid_soft
*   \brief Engine: mechanics for the variable quantiser, used in QuantisedEventQueue - see that
*    class for more detail
*/
class QuantiseGrid_soft
{

public:

    struct noteCoordinate
    {
        uint64_t gridline;
        float phase;

        noteCoordinate(uint64_t g, float p);
        bool operator< (const noteCoordinate& other);
        bool operator<= (const noteCoordinate& other);
    };

    QuantiseGrid_soft(unsigned id, unsigned divisions, unsigned sampleRate,
        float tempo);

    noteCoordinate tick();

    unsigned getID();

    noteCoordinate getMarker();

    noteCoordinate getBarStart();

    noteCoordinate getPlaceWithinBar();

    noteCoordinate jumpToNextBar();

    void resetToBarStart();

    noteCoordinate jumpToNextDivision();

    void setTempo(float tempo);

    void setSampleRate(unsigned sampleRate);

    void setDelay(unsigned samples);

    float getTempo() const;

    unsigned getSampleRate() const;

    bool   getIsNoteDue(noteCoordinate noteMarker);

    noteCoordinate  getNoteCoordinate(unsigned gridMultiple, unsigned offset);


    float getQuantiseAmount();
    void setQuantiseAmount(float amount);
    uint64_t getPrevValidGridline(uint64_t position, unsigned mult, unsigned offset);


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
    float _quantiseAmount;
    float _quantiseAmount_display;

    // should be 1/4 bar I think
    int _delayGrid;
    float   _delayPhase;

};