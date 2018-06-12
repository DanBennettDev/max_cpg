# Overview
MAX MSP objects for creative use of Central Pattern Generators (CPG) (specifically Matsuoka's Neural Oscillator (MNO)), as used in my "neurythmic" system. [(see paper here)](https://www.researchgate.net/publication/324360165_Neurythmic_A_Rhythm_Creation_Tool_Based_on_Central_Pattern_Generators)

If you do anything useful with this system please let me know - this is an ongoing piece of research. I'm interested in linking to your work as part of a showcase of creativity using CPGs. I'm also interested in collaboration if you have any ideas you'd like help realising, or would like to work together on a paper in this area. [db15237@bristol.ac.uk](db15237@bristol.ac.uk)

# Description
These are non-linear dynamic systems, which exhibit entrainment behaviour (a kind of neural synchronisation). I think they show potential in a great number of areas including:

- Engine for a semi-generative drum machine/ sequencer (like Neurythmic)
- Pattern engine for audio granulation - using CPG network to generate control pulses offers a means of intuitively moving between microrhythmic patterns  
- Direct audio synthesis
- Interfaces for musical control - use CPG networks alongside force-feedback input devices or motion capture systems so that computer rhythm generators can entrain to human motion


Three externals 

1. A friendly full-network object, with quantiser, allowing easy creation and manipulation of CPG networks built up from individual MNO nodes 
2. A single MNO-node object with a direct-frequency control interface, and message-based control of equation parameters - the frequency control system is recalibrated when you change system parameters
3. A single barebones MNO-node with signal control over equation parameters

In all cases the MNO equation is solved using 4th order Runge Kutta methods, and a stepsize of 8


Coming soon:

- Max Help Patches
- "neurythmic" drum machine / sequencer patch and control interface
- macOS version
- M4L patch
