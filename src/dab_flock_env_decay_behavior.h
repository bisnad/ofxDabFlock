/** \file dab_flock_env_decay_behavior.h
 */

#ifndef _dab_flock_env_decay_behavior_h_
#define _dab_flock_env_decay_behavior_h_

#include "dab_flock_env_behavior.h"

namespace dab
{

namespace flock
{

class EnvDecayBehavior : public EnvBehavior
{
public:
    EnvDecayBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    EnvDecayBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    ~EnvDecayBehavior();
    
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
    EnvParameter* mInputEnvPar; // input environment parameter
    Parameter* mDecayPar; // internal parameter
    EnvParameter* mOutputEnvPar; // output environment parameter
    
};

};

};

#endif