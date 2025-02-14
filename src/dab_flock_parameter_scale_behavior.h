/** \file dab_flock_parameter_scale_behavior.h
 *  \class dab::flock::ParameterScaleBehavior scale parameter values
 *	\brief scale parameter values
 *
 *  The Behavior scales parameter values\n
 *  It copies the scaled values into an output parameter\n
 *  \n
 *  Input Parameter:\n
 *  type: input dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_minInput dim: nD defaultValue: 0.0\n
 *  name: xxx_maxInput dim: nD defaultValue: 1.0\n
 *  name: xxx_minOutput dim: nD defaultValue: 0.0\n
 *  name: xxx_maxOutput dim: nD defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 4/5/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_parameter_scale_behavior_h_
#define _dab_flock_parameter_scale_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class ParameterScaleBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ParameterScaleBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ParameterScaleBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ParameterScaleBehavior();
    
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
    Parameter* mMinInPar; /// \brief minimum input value parameter (internal)
    Parameter* mMaxInPar; /// \brief maximum input value parameter (internal)
    Parameter* mMinOutPar; /// \brief minimum output value parameter (internal)
    Parameter* mMaxOutPar; /// \brief maximum output value parameter (internal)
};

};

};

#endif