# Overview
MAX MSP objects for creative use of Central Pattern Generator networks (CPGs) (specifically Matsuoka's Neural Oscillator (MNO)), as used in my "neurythmic" system. [(see paper here)](https://www.researchgate.net/publication/324360165_Neurythmic_A_Rhythm_Creation_Tool_Based_on_Central_Pattern_Generators)

If you do anything useful with this system please let me know - this is an ongoing piece of research. I'm interested in linking to your work as part of a showcase of creativity using CPGs. I'm also interested in collaboration if you have any ideas you'd like help realising, or would like to work together on a paper in this area. [db15237@bristol.ac.uk](db15237@bristol.ac.uk)

# Description
CPGs are small networks of neurons which exhibit oscillation behaviour. They appear in animals controlling adaptive rhythmic systems like heartbeat, lung function, gait, and it is proposed many other complex motor control behaviours. It is also suggested that they may be central to human rhythm perception. For all these reasons and more I think they have huge potential for rhythmic creativity.

The goal of these externals is to support hands-on interaction, exploration and creativity with these networks. CPG networks tend to be much smaller than the kinds of networks used in machine learning, and these externals provide various interface helpers that make it possible (and I think we've shown rewarding) to interact with these networks more-or-less directly: e.g. to tune them by hand and use them to explore and control rhythms. [My Neurythmic drum sequencer took just this approach](https://www.researchgate.net/publication/324360165_Neurythmic_A_Rhythm_Creation_Tool_Based_on_Central_Pattern_Generators). 

At a basic level, these are non-linear dynamic systems, which exhibit entrainment behaviour (a kind of neural synchronisation). This means that they will adapt to synchronise to incoming signals. Depending on the strength, shape and relative frequency of the incoming signal.

They are currently used almost exclusively in robotic motor control. I think they show huge potential for music, in a great number of areas including e.g.:

- Engine for a semi-generative drum machine/ sequencer (like Neurythmic)
- Pattern engine for audio granulation - using CPG network to generate control pulses offers a means of intuitively moving between microrhythmic patterns  
- Direct audio synthesis (the oscillator can be tuned to a range of waveshapes ranging from near-sine to pulse-like, and the entrainment behaviour of the network can create complex timbral, micro-rhythmic and harmonic effects and transitions)
- Interfaces for musical control - use CPG networks alongside force-feedback input devices or motion capture systems so that computer rhythm generators can entrain to human motion

# The externals
Three externals are provided

1. A friendly full-network object, with quantiser, allowing easy creation and manipulation of CPG networks built up from individual MNO nodes 
2. A single MNO-node object with a direct-frequency control interface, and message-based control of equation parameters - the frequency control system is recalibrated when you change system parameters
3. A single barebones MNO-node with signal control over equation parameters

In all cases the MNO equation is solved using 4th order Runge Kutta methods, and a stepsize of 8

# Optimisation, Gradient Descent, etc.
These are not the kind of neural networks used in machine learning. They are quite different both in behaviour and in usage. Gradient descent or other "training" techniques are less widely used and far less well documented with these networks than in e.g. Deep Learning networks. Of course, however, as a deterministic, network-based system  it is possible to tune these networks towards specific outcomes using gradient descent (or use genetic algorithms, etc. to explore the parameter space in a less teleological manner). 

These externals are not geared to such usage with optimisation algorithms. Though it may be possible to use them in this way, MAX is arguably not the best environment for this. The sourcecode for the underlying network engine is located in ./source/MatsuokaEngine_src, and it may be easier to build optimisation processes around this. The top level class is MatsuokaEngine.h


# Coming soon

- Max Help Patches
- "neurythmic" drum machine / sequencer patch and control interface
- macOS version
- M4L patch
