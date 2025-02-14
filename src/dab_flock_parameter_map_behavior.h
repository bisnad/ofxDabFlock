/** \file dab_flock_parameter_map_behavior.h
 *  \class dab::flock::ParameterMapBehavior map input parameter into output parameter
 *	\brief map input parameter into output parameter
 *
 *  The Behavior maps input parameter into output parameter\n
 *  Any combination of input parameter components can be combined, scaled and copied into any combination of output parameter components\n
 *  This behavior can for instance be used to copy parameter values into a parameter of different dimension\n
 *  Or it can be used to scale the range of some parameter components\n
 *  The mapping relies on the values stored in the internal map parameter\n
 *  This parameter represents the mapping as follows:\n
 *  in0->out0, in0->out1,... in0->outY, in1->out0, in1->out1,... in1->outX,... inX->out0, inX->out1,... inX->outY\n
 *  For example to map a 3D parameter to a 2D parameter by combining the second and third component of the input into the second component of the output:
 *  1, 0, 0, 0, 0.5, 0.5
 *  \n
 *  Input Parameter:\n
 *  type: input dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: mD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_map dim: m*nD defaultValue: 1.0 for all components that map input component into the same output component, 0.0 for all the others\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 3/5/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_parameter_map_behavior_h_
#define _dab_flock_parameter_map_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class ParameterMapBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ParameterMapBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ParameterMapBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ParameterMapBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception Exception wrong number of type of parameters
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pAgent) const  throw (Exception);
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    /**
     \brief perform behavior
     */
    void act();
    
protected:
    Parameter* mInputPar; /// \brief input parameter
    Parameter* mOutputPar; /// \brief output parameter
    Parameter* mMapPar; /// \brief map parameter (internal)
};

};

};

#endif