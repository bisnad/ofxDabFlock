## ofxDabFlock

**Author**:  Daniel Bisig - Coventry University, UK - [ad5041@coventry.ac.uk](ad5041@coventry.ac.uk) - Zurich University of the Arts, CH - [daniel.bisig@zhdk.ch](daniel.bisig@zhdk.ch)

**Dependencies**: [ofxDabBase](https://bitbucket.org/dbisig/ofxdabbase_011/src/master/), [ofxDabEvent](https://bitbucket.org/dbisig/ofxdabevent_011/src/master/), [ofxDabMath](https://bitbucket.org/dbisig/ofxdabmath_011/src/master/), [ofxDabGeom](https://bitbucket.org/dbisig/ofxdabgeom_011/src/master/), [ofxDabOsc](https://bitbucket.org/dbisig/ofxdabosc_011/src/master/), [ofxDabSpace](https://bitbucket.org/dbisig/ofxdabspace_011/src/master/), [ofxAssimpModelLoader](https://openframeworks.cc/documentation/ofxAssimpModelLoader/ofxAssimpModelLoader/), [ofxJSON](https://github.com/jeffcrouse/ofxJSON)

---

## Summary

ofxDabFlock is a framework that supports the creation of flocking simulations. ofxDabFlock is highly generic in it that it doesn't impose any restrictions on the number, type and dimensionality of parameters that agents can possess. ofxDabFlock provides functions to calculate spatial relationships among parameters as well as between parameters and other spatial structures such as splines or vector-fields. Agent behaviours establish functional relationships among these parameters. ofxDabFlock can communicate via OSC messages with other applications.  OSC messages can be used by other applications to receive the values of simulation parameters. In addition, OSC messages can also be employed as commands to remotely configure and control the simulation. Finally, ofxDabFlock provides simple rendering functionality to visualise the simulation. The code is compatible with OpenFrameworks 0.11 and has been tested on Windows and MacOS. 

### Entities

**Agent**: an agent is a single autonomous entity that is typically part of a larger group. And agent possesses properties and behaviours. 

**Swarm**: a swarm is a collection of agents that typically identical the same properties and behaviours. Agents can be removed or added from a swarm. The swarm can also possess its own properties and behaviours that are independent of those of the agents.

**Env**: an environment is a space that is discretised as a grid with each grid. An environment can possess its own properties and behaviours. Its properties can possess different values at each grid location. 

### Properties

**Parameter**: represents a named property of an agent or swarm and stores an array of floating point values. Neighbourhood relationships are based on the Euclidian distance between parameters.

**EnvParameter**: represents a named property of an environment. Stores the values of the parameter at each cell position for a spatial grid. 

**ParameterList**: a collection of all parameters of an agent, swarm, or environment.

### Behaviours

**Behavior**: represents a named behaviour of an agent or swarm. Behaviours operate on input and output parameters, and overwrites the values of the latter based on the values of the former. Depending on the behaviour, neighbourhood relationships between parameters are taken into account or not. 

**EnvBehavior**: represents a named behaviour of an environment. 

**BehaviorList**: a collection of all behaviours of an agent, swarm, or environment.

### Communication

**FlockCom**: manages all communication with a flocking simulation. Allows to register parameters whose values are then sent via OSC to a listening port. 

**OscControl**: handles the remote control of a flocking simulation via OSC messages. 

### Serialisation

**SerializeTools**: The configuration and state of a flocking simulation can be saved as JSON files and later on restored from these files. 

### Visualisation

**FlockVisuals**: provides simple functionality for visualisation the flocking simulation. 

**VisAgentShape**: draws agents as geometrical object

**VisAgentTrail**: draws the trajectory of an agent parameter as polyline

**VisNeighborSpace**: draws neigbourhood relationships between parameters as lines

**VisGridSpace**: draws the values at the grid positions of an environment grid as lines

