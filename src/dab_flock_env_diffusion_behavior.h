/** \file dab_flock_env_diffusion_behavior.h
 */

#ifndef _dab_flock_env_diffusion_behavior_h_
#define _dab_flock_env_diffusion_behavior_h_

#include "dab_flock_env_behavior.h"

namespace dab
{

namespace flock
{

class EnvDiffusionBehavior : public EnvBehavior
{
public:
    EnvDiffusionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    EnvDiffusionBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    ~EnvDiffusionBehavior();
    
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
    Parameter* mDiffusionPar; // internal parameter
    EnvParameter* mOutputEnvPar; // output environment parameter
    
};

};

};

#endif