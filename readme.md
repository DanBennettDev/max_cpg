# Central Pattern Generator Objects for MAX/MSP

## Public version 0.1

MAX MSP objects for creative use of Central Pattern Generator networks (CPGs) (specifically Matsuoka's Neural Oscillator (MNO)), as used in my "neurythmic" system. [(see paper here)](https://www.researchgate.net/publication/324360165_Neurythmic_A_Rhythm_Creation_Tool_Based_on_Central_Pattern_Generators). This repository contains all the externals, helpfiles etc., and an implementation of my Neurythmic software for MAX/MSP. The externals should work on macOS or Windows, but the GUI for neurythmic will only run on Windows at present.

Download or clone this repo, to your max packages folder. 

See [this page](https://danbennettdev.github.io/projects/cpg_max.html) for a guide to the neurythmic GUI. and [this page](https://danbennettdev.github.io/projects/cpg_creativity.html) for a description of why you might be interested.

## What are CPGs?
CPGs are small networks of neurons which exhibit oscillation behaviour. They are found in animals, where they control adaptive rhythmic systems like heartbeat, lung function, gait, and (it is suggested) underly more complex motor control behaviours. It is also suggested that they may be central to human rhythm perception. For all these reasons and more I think they have huge potential for use in creative rhythm generation and interaction.

The goal of these externals is to support hands-on interaction, exploration and creativity with these networks. CPG networks tend to be much smaller than the kinds of networks used in machine learning, and these externals provide various interface helpers that make it possible (and I think we've shown rewarding) to interact with these networks more-or-less directly: e.g. to tune them by hand and use them to explore and control rhythms. [My Neurythmic drum sequencer took just this approach](https://www.researchgate.net/publication/324360165_Neurythmic_A_Rhythm_Creation_Tool_Based_on_Central_Pattern_Generators). 

At a basic level, these are non-linear dynamic systems, which exhibit entrainment behaviour (a kind of neural synchronisation). This means that they will adapt to synchronise to incoming signals. Depending on the strength, shape and relative frequency of the incoming signal.

They are currently most commonly used in robotic motor control. I think they show huge potential for music, in a great number of areas including e.g.:

- Engine for a semi-generative drum machine/ sequencer (like Neurythmic)
- Pattern engine for audio granulation - using CPG network to generate control pulses offers a means of intuitively moving between microrhythmic patterns  
- Direct audio synthesis (the oscillator can be tuned to a range of waveshapes ranging from near-sine to pulse-like, and the entrainment behaviour of the network can create complex timbral, micro-rhythmic and harmonic effects and transitions)
- Interfaces for musical control - use CPG networks alongside force-feedback input devices or motion capture systems so that computer rhythm generators can entrain to human motion

## The externals
Three externals are provided

1. cpg.network~ A full featured, full-network object, with quantiser, allowing easy creation and manipulation of CPG networks built up from individual MNO nodes 
2. cpg.node_freq~ A single MNO-node object with a direct-frequency control interface, and message-based control of equation parameters - the frequency control system is recalibrated when you change system parameters
3. cpg.node_basic~ A single barebones MNO-node with signal control over equation parameters

In all cases the MNO equation is solved using 4th order Runge Kutta methods, and a stepsize of 8

## Optimisation, Gradient Descent, etc.
These are not the kind of neural networks used in machine learning. They are quite different both in behaviour and in usage. The use of gradient descent or other "training" techniques is far less well documented for these networks than in e.g. Deep Learning networks. Of course you can probably adapt these kind of approaches or use e.g. genetic algorithms to explore the parameter space in a less teleological manner. 

These externals are not, however, developed with such usage in mind. MAX MSP is arguably not the best environment for this kind of approach, and as noted above, these externals focus on hands-on usage. If you are interested, though, the sourcecode for the underlying network engine is located in ./source/MatsuokaEngine_src. This was pulled more-or-less wholesale from the non-max, standalone version of my neurythmic system, and it works as a standalone library. It has a lot of features that the max externals don't exploit. If you are interested in using CPGs with optimisation methods, then this "library" is probably the best starting point. If you take this approach, all the methods you want should be in the MatsuokaEngine class (MatsuokaEngine.h) 
