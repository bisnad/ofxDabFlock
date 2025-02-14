/** \file dab_flock_target_parameter_behavior.h
 *  \class dab::flock::TargetParameterBehavior shift the values of an input parameter towards those of a target parameter
 *  \brief shift the values of an input parameter towards those of a target parameter
 *
 *  This behavior shifts the values of an input parameter towards those of a target parameter\n
 *  The internal adapt parameter controls for each dimension how far the values are shifted: 0: no shift at all, 1: full shift\n
 *  The internal amount parameter controls for all dimensions how far the values are shifted: 0: no shift at all, 1: full shift\n
 *  The effect of the amount and adapt parameters is combined\n
 *  The internal absolute parameter controls if the shifting happens towards absolute or non absolute values\n
 * \n
 *  Input Parameter:\n
 *  type: InputParameter dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: OutputParameter dim: nD neighbors: ignore\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_target dim: nD defaultValue: 0.0\n
 *  name: xxx_adapt dim: nD defaultValue: 1.0\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  name: xxx_absolute dim: 1D defaultValue: 0.0\n
 *  \n
 *  Created by Daniel Bisig on 4/21/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_target_parameter_behavior_h_
#define _dab_flock_target_parameter_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class TargetParameterBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    TargetParameterBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    TargetParameterBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~TargetParameterBehavior();
    
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
    Parameter* mInputPar; ///\brief input parameter
    Parameter* mOutputPar; ///\brief output parameter
    Parameter* mTargetPar; ///\brief target parameter
    Parameter* mAdaptPar; ///\brief adapt parameter
    Parameter* mAmountPar; ///\brief behavior amount parameter
    Parameter* mAbsPar; ///\brief treat differences as absolute values
    
    
    Eigen::VectorXf mDiffValues; /// \brief difference values
};

};

};

#endif