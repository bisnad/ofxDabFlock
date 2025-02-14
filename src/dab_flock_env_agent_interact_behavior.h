/** \file dab_flock_env_agent_interact_behavior.h
 */

#ifndef _dab_flock_env_agent_interact_behavior_h_
#define _dab_flock_env_agent_interact_behavior_h_

#include "dab_flock_env_behavior.h"

namespace dab
{

namespace flock
{

class EnvAgentInteractBehavior : public EnvBehavior
{
public:
    EnvAgentInteractBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    EnvAgentInteractBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    ~EnvAgentInteractBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception FlockException wrong number of type of parameters
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pAgent) const;
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    void act();
    
protected:
    EnvParameter* mEnvPar; // input par
    Parameter* mAmountPar; // interal par
    std::string mAgentParName; // agent input par
    
    
};

};

};

#endif