/**	\file dab_flock_behavior.h
 *	\class dab::flock::Behavior behavior class
 *	\brief behavior class
 *
 *  Abstract base class for agent behaviors.\n
 *  Any derived behavior class needs to implement an act methods, that is called once during each simulation step.\n
 *  \n
 *  Input Parameter:\n
 *  \n
 *  Output Parameter:\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx:active dim: 1D\n
 *  \n
 *  Created by Daniel Bisig on 3/23/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 3/08/17.
 */

#ifndef _dab_flock_behavior_h_
#define _dab_flock_behavior_h_

#include "dab_flock_parameter.h"
#include "dab_space_neighbor_group.h"
#include <Eigen/Dense>
#include <vector>

namespace dab
{

namespace flock
{

class Agent;
class Parameter;
class Behavior
{
    
friend class Swarm;
    
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    Behavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent reference to agent to which this behavior is added
     \param pBehaviorName behavior name
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    Behavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief destructor
     */
    virtual ~Behavior();
    
    /**
     \brief create copy of behavior
     \param pSwarm swarm this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pSwarm) const = 0;
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const = 0;
    
    /**
     \brief return behavior name
     \return behavior name
     */
    const std::string& name() const;
    
    std::string className() const;
    
    /**
     \brief return input parameter string
     \return input parameter string
     */
    const std::string& inputParameterString() const;
    
    /**
     \brief return output parameter string
     \return output parameter string
     */
    const std::string& outputParameterString() const;
    
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
    friend std::ostream& operator << ( std::ostream& pOstream, const Behavior& pBehavior )
    {
        pOstream << pBehavior.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    Behavior();
    
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
    Parameter* createInternalParameter(const std::string& pParameterName, const std::vector<float>& pValues );
    
    /**
     \brief create internal behavior parameter
     \param pParameterName name of parameter
     \param pDim dimension of parameter
     \param pValues value of parameter
     \return parameter
     
     creates an internal behavior parameter which is visible at the level of the agent (i.e. other behaviors can access it).\n
     the name of the parameter is constructed as follows: behaviorName_parameterName.\n
     */
    Parameter* createInternalParameter(const std::string& pParameterName, int pDim, float pValue );
    
    /**
     \brief create internal behavior parameter
     \param pParameterName name of parameter
     \param pValues values of parameter
     \return parameter
     
     creates an internal behavior parameter which is visible at the level of the agent (i.e. other behaviors can access it).\n
     the name of the parameter is constructed as follows: behaviorName_parameterName.\n
     */
    Parameter* createInternalParameter(const std::string& pParameterName, const std::initializer_list<float>& pValues);
    
    /**
     \brief return internal behavior parameters
     \return internal behavior parameters
     */
    std::vector< Parameter* >& internalParameters();
    
    std::string mName; /// \brief behavior name
    std::string mClassName; /// \brief behavior class name
    Agent* mAgent; /// \brief agent this behavior belongs to	
    std::string mInputParameterString; /// \brief unparsed string containing all input parameter names
    std::string mOutputParameterString; /// \brief unparsed string containing all output parameter names
    std::vector<Parameter*> mInputParameters; /// \brief input parameters (behavior reads from them)
    std::vector<space::NeighborGroup*> mInputNeighborGroups; /// \brief input neighbor groups (behavior reads from them)
    std::vector<std::string> mNeighborInputParameterNames; /// \brief input parameter names for parameters that are retrieved via neighbor groups
    std::vector<Parameter*> mOutputParameters; /// \brief output parameters (behavior writes to them)
    std::vector<space::NeighborGroup*> mOutputNeighborGroups; /// \brief output neighbor groups (behavior writes to them)
    std::vector<std::string> mNeighborOutputParameterNames; /// \brief output parameter names for parameters that are retrieved via neighbor groups
    std::vector<Parameter*> mInternalParameters; /// \brief internal parameters (including scales)
    Parameter* mActivePar; /// \brief active parameter (internal)
};

};

};

#endif