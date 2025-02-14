/** \file dab_flock_env_behavior.h
 *
 *  Created by Daniel Bisig on 2/22/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/08/17.
 */

#ifndef _dab_flock_env_behavior_h_
#define _dab_flock_env_behavior_h_

#include "dab_flock_behavior.h"
#include "dab_flock_env_parameter.h"

namespace dab
{

namespace flock
{

class Env;

class EnvBehavior : public Behavior
{
public:
    /**
     \brief create environment behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output parameter string (paramaters are space separated)
     */
    EnvBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create environment behavior
     \param pEnv reference to environment to which this behavior is added
     \param pBehaviorName behavior name
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output parameter string (paramaters are space separated)
     */
    EnvBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief destructor
     */
    virtual ~EnvBehavior();
    
    /**
     \brief perform behavior
     */
    virtual void act() = 0;
    
    /**
     \brief print behavior information
     */
    virtual operator std::string() const;
    
    /**
     \brief print behavior information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual behavior info
     \param pOstream output text stream
     \param pBehavior behavior
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const EnvBehavior& pBehavior )
    {
        pOstream << pBehavior.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    EnvBehavior();
    
    /**
     \brief create input behavior parameters
     \exception Exception failed to create input parameters
     */
    virtual void createInputParameters() throw (Exception);
    
    /**
     \brief create output behavior parameters
     \exception Exception failed to create output parameters
     */
    virtual void createOutputParameters() throw (Exception);
    
    /**
     \brief create internal behavior parameter
     \param pParameterName name of parameter
     \param pValues values of parameter
     \return parameter
     
     creates an internal behavior parameter which is visible at the level of the agent (i.e. other behaviors can access it).\n
     the name of the parameter is constructed as follows: behaviorName_parameterName.\n
     */
    // Parameter* createInternalParameter(const base::String& pParameterName, const math::Vector<real>& pValues);
    
    Env* mEnv; /// \brief environment this behavior belongs to
    
    std::map< std::string, std::vector<std::string> >	mInputAgentParameterNames;
    std::map< std::string, std::vector<std::string> >	mOutputAgentParameterNames;
};

};

};

#endif