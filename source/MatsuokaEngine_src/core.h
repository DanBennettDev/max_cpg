#pragma once



/*! \class core
*  \brief definitions, functions etc. to be used across the whole project
*/


#define WAVETABLE_LENGTH 1024
// no of samples in the wavetable when oscillator is driven by phasor

#define TWOPI 6.2831853

// default sample rate at which calculations are to be performed. 
// 1000 gives 1ms resolution - plenty for rhythm generation purposes
// audio rate applications will obviously need to be higher
#define DEFAULTSAMPLERATE 1000 

// default cycles per second for node
#define DEFAULTNODEFREQ 1

// Maximum lengths of node output delay lines in samples. Related to 
// DEFAULTSAMPLERATE & values should be changed in tandem.
// High values will have memory implications since one delayline occurs at 
// each node. Delay lines are used for pseudo-phase offset, so this value 
// should be set high enough to offset by one waveform cycle at the lowest 
// expected operating frequency.
// For rhythmic uses I have accounted for 40 second cycles to allow longform, 
// sound organisation. Since samplerate can be kept low this is not a problem: 
// If 32 nodes are active, and all running at slowest frequency (improbable case) 
// then 20000 sample lines results in c 3mb total allocated to delay lines.
#define MAX_DELAYLINE_LENGTH 20000

// This sets the length at initialisation - see above for discussion, for 
// present I have opted to set to max from start to avoid any memory 
// allocation during performance 
#define INIT_DELAY_LENGTH 20000

// never set lower than 2
#define MIN_DELAY_LENGTH 2

// Delay line for a node is increased in size when requirement reaches 
// DELAYLINE_RESIZE_THRESHOLD * delayLineLength. When resized delayline
// size is multiplied by DELAYLINE_RESIZE_AMOUNT.
// Delayline requirement = node cyclelength
#define DELAYLINE_RESIZE_THRESHOLD 0.5
#define DELAYLINE_RESIZE_AMOUNT 3.0



// constant used in calculating frequency of node based on parameters, and vice
// versa. Should be set in tandem with the default parameters 
// TODO: move default parameters into this header.
// see K.Matsuoka, "Analysis of a neural oscillator," Biological 
// cybernetics, vol. 104, no. 4 - 5, pp. 297{304, 2011.
#define DEFAULTFREQCOMPENSAITON 0.9732
#define MINFREQCOMPENSAITON 0.0000001


// maximum number of nodes handled by system. In several places
// arrays large enough for all nodes are allocated on initialisation, 
// ahead of time to simplify implementation and allow optimisations.
#define MAX_NODES 16

// pitch that objects will be initialised to
#define DEFAULT_NOTE_FREQ 200

// folder for config files
#define CONFIG_ADDRESS "config"

// file that defines the weight scaling curve
#define SCALING_CURVE_SOURCE  CONFIG_ADDRESS "/scalingCurve.txt"

/// min stable freq currently seems to be c 0.2
/// This is ok for present uses, but in future it might be worth investigating
#define MIN_NODE_FREQ 0.2

// DEBUGGING

//#define _DEBUG_LOG 

#ifdef _DEBUG_LOG 

#include <fstream>
#include <string>
#include <iostream>
#define DTB_LOG(A)  dtb_text_to_log(A)
void dtb_text_to_log(const std::string &text);

#else
#define DTB_LOG_VAR(A, B) 
#define DTB_LOG(A) 

#endif



