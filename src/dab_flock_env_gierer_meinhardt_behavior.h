/** \file dab_flock_env_gierer_meinhardt_behavior.h
 */

#ifndef _dab_flock_env_gierer_meinhardt_behavior_h_
#define _dab_flock_env_gierer_meinhardt_behavior_h_

#include "dab_flock_env_behavior.h"

namespace dab
{

namespace flock
{

class EnvGiererMeinhardtBehavior : public EnvBehavior
{
public:
    EnvGiererMeinhardtBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    EnvGiererMeinhardtBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    ~EnvGiererMeinhardtBehavior();
    
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
    Parameter* mChem1ProdPar; // internal parameter
    Parameter* mChem2ProdPar; // internal parameter
    Parameter* mChem1DecayPar; // internal parameter
    Parameter* mChem2DecayPar; // internal parameter
    Parameter* mReactRatePar; // internal parameter
    EnvParameter* mOutputChem1Par; // output environment parameter
    EnvParameter* mOutputChem2Par; // output environment parameter
};

};

};

#endif