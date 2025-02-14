/** \file dab_flock_reset_behavior.h
 *  \class dab::flock::ResetBehavior reset parameter value
 *	\brief reset parameter value
 *
 *  The Behavior randomizes parameter value\n
 *  \n
 *  Input Parameter:\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_resetValue dim: nD defaultValue: 0.0\n
 *  name: xxx_resetAmount dim: nD defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 4/16/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017.
 */

#ifndef _dab_flock_reset_behavior_h_
#define _dab_flock_reset_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class ResetBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ResetBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ResetBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ResetBehavior();
    
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
    Parameter* mResetPar; /// \brief reset parameter (output)
    Parameter* mResetValuePar; /// \brief reset value parameter (internal)
    Parameter* mResetAmountPar; /// \brief reset amount parameters (internal)
    
    Eigen::VectorXf mUnitVector;
};

};

};

#endif