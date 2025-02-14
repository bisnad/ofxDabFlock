/** \file dab_flock_env_gray_scott_behavior.h
 */

#ifndef _dab_flock_env_gray_scott_behavior_h_
#define _dab_flock_env_gray_scott_behavior_h_

#include "dab_flock_env_behavior.h"

namespace dab
{

namespace flock
{

class EnvGrayScottBehavior : public EnvBehavior
{
public:
    EnvGrayScottBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    EnvGrayScottBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    ~EnvGrayScottBehavior();
    
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
    EnvParameter* mInputChem1Par; // input environment parameter
    EnvParameter* mInputChem2Par; // input environment parameter
    Parameter* mFPar; // internal parameter
    Parameter* mKPar; // internal parameter
    EnvParameter* mOutputChem1Par; // output environment parameter
    EnvParameter* mOutputChem2Par; // output environment parameter
};

};

};

#endif