#pragma once
#include <cstdint>
#include <list>
#include <vector>
#include <mutex>
#include "QuantiseGrid_soft.h"
#include "core.h"


/*! \class QuantisedEventQueue
*  \brief Engine: Takes events as inputs and outputs them at the correct quantised interval
*   correct quantised interval depends on settings, quantiser grid size, etc.
*   introduces a delay. Push pull quantiser - moves incoming events to nearest beat
*/

class QuantisedEventQueue
{

public:
    using noteCoordinate = QuantiseGrid_soft::noteCoordinate;

    QuantisedEventQueue(unsigned sampleRate, float tempo);
    QuantisedEventQueue(const QuantisedEventQueue &rhs);

    /// struct specifying for the events the queue quantises
    struct outputEvent
    {
        unsigned nodeID;
        double velocity;

        bool operator < (const outputEvent &rhs) { return nodeID < rhs.nodeID; }
    };

    /// the types of grids available to be quantised to 
    enum class gridType
    {
        unQuantised, _24th, _32nd
    };


    // Thoughts - handling of multiple notes for same coord - take most recent
    //  build QuantisedNoteQueue to handle the note level side of this
    // allows setting of quantisation (or lack of) per node, plus divisions, courseness, offset

    /// move the system forwards 1 sample
    void tick();

    /// add an event to the system
    void addEvent(outputEvent e);

    /// set the sample rate of the system
    void setSampleRate(unsigned sampleRate);

    /// set the tempo in beats per minute
    void setTempo(float tempo);

    /// set the amount of quantisation (0=none, 1=strict)
    void setQuantiseAmount(float amount);
    

	void setQuantiseAmount(unsigned node, float amount);


	/// get the amount of quantisation (0=none, 1=strict)
    float getQuantiseAmount();
	float getQuantiseAmount(unsigned node);

    /// set the quantise grid type for the node
    void setNodeGrid(unsigned nodeID, gridType grid);
    /// sets the grid multiplier for the node (space between grid lines 
    /// that are valid for beat placement)
    void setNodeGridMultiple(unsigned nodeID, unsigned mult);

    /// sets the grid offset 
    void setNodeGridOffset(unsigned nodeID, unsigned off);


	unsigned getNoteDelay() const;

    unsigned getSampleRate() const;
    float getTempo() const;
    gridType getNodeGrid(unsigned nodeID);
    unsigned getNodeGridMultiple(unsigned nodeID);
    unsigned getNodeGridOffset(unsigned nodeID);
    unsigned getNodeBarDivision(unsigned nodeID);

    /// moves to the start of the next bar
    void syncToBarStart();

    /// returns any notes which are ready to be played (call every tick())
    std::vector<outputEvent> getNotes();



private:

    struct queuedEvent
    {
        outputEvent event;
        noteCoordinate queueMarker;

        queuedEvent():
            event(),
            queueMarker(0, 0)
        {}
    } ;

    struct queuedEvent_free
    {
        outputEvent event;
        float countDown;
        float delta;

        queuedEvent_free() :
            event()
        {
            countDown = 1;
            delta = 1;
        }
    };


    struct nodeDetails
    {
        gridType grid;
        unsigned multiple;
        unsigned offset;
		float amount;


        nodeDetails()
        {
            grid = gridType::unQuantised;
            multiple = 1;
            offset = 0;
			amount = 1;
        }
    };

    void setNoteDelays();
    unsigned _delayNotes;
    float _tempo;
    float _quantiseAmount;
    unsigned _sampleRate;
    nodeDetails nodes[MAX_NODES];
    std::list<queuedEvent> _eventQueue24;
    std::list<queuedEvent> _eventQueue32;
    std::list<queuedEvent_free> _eventQueueFree;

    QuantiseGrid_soft _grid24;
    QuantiseGrid_soft _grid32;
    noteCoordinate _mark24;
    noteCoordinate _mark32;
    std::mutex _queue_mutex;


};