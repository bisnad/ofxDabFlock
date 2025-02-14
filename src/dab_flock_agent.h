/**	\file dab_flock_agent.h
 *	\class dab::flock::Agent agent class
 *	\brief agent class
 *
 *
 *  Created by Daniel Bisig on 3/23/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 3/08/17.
 */

#ifndef _dab_flock_agent_h_
#define _dab_flock_agent_h_

#include <Eigen/Dense>
#include "dab_exception.h"
#include "dab_flock_parameter_list.h"
#include "dab_flock_behavior_list.h"

namespace dab
{

namespace flock
{

class NeighborListAlg;

class Agent
{
public:
    /**
     \brief default name
     */
    static const std::string sClassName;
    
    /**
     \brief number of agent instances
     */
    static unsigned int sInstanceCount;
    
    /**
     \brief default constructor
     */
    Agent();
    
    /**
     \brief create agent
     \param pName name of agent
     */
    Agent(const std::string& pName);
    
    /**
     \brief copy constructor
     \param pAgent agent to copy properties and behaviors from
     */
    Agent(const Agent& pAgent);
    
    /**
     \brief copy constructor
     \param pName name of agent
     \param pAgent agent to copy properties and behaviors from
     */
    Agent(const std::string& pName, const Agent& pAgent);
    
    /**
     \brief destructor
     */
    virtual ~Agent();
    
    /**
     \brief return agent index
     \return agent index
     */
    unsigned int index() const;
    
    /**
     \brief return agent name
     \returns agent name
     */
    const std::string& name() const;
    
    /**
     \brief return number of parameters the agent contains (first level parameters only
     \return number of parameters the agent contains
     */
    unsigned int parameterCount() const;
    
    /**
     \brief check if agent has parameter
     \param pParameterName parameter name
     \return true if agent possesses a particular parameter
     */
    bool checkParameter(const std::string& pParameterName) const;
    
    /**
     \brief return parameter index
     \param pParameterName parameter name
     \return parameter index
     \exception Exception parameter does not exist
     */
    unsigned int parameterIndex(const std::string& pParameterName) const  throw (Exception);
    
    /**
     \brief get parameter
     \param pParameterIndex parameter index
     \return parameter
     \exception Exception parameter does not exist
     */
    Parameter* parameter(unsigned int pParameterIndex) throw (Exception);
    
    /**
     \brief get parameter
     \param pParameterIndex parameter index
     \return parameter
     \exception Exception parameter does not exist
     */
    const Parameter* parameter(unsigned int pParameterIndex) const throw (Exception);
    
    /**
     \brief get parameter
     \param pParameterName parameter name
     \return parameter
     \exception Exception parameter does not exist
     */
    Parameter* parameter(const std::string& pParameterName) throw (Exception);
    
    /**
     \brief get parameter
     \param pParameterName parameter name
     \return parameter
     \exception Exception parameter does not exist
     */
    const Parameter* parameter(const std::string& pParameterName) const throw (Exception);
    
    /**
     \brief add parameter
     \param pParameter parameter
     \exception Exception parameter name already exists
     */
    virtual void addParameter(Parameter* pParameter) throw (Exception);
    
    /**
     \brief create parameter
     \param pName parameter name
     \param pDim dimension of parameter (> 0)
     \exception Exception parameter name already exists
     */
    virtual void addParameter(const std::string& pName, unsigned int pDim) throw (Exception);
    
    /**
     \brief create parameter
     \param pName parameter name
     \param pDim dimension of parameter (> 0)
     \param pValue uniform parameter value
     \exception Exception parameter name already exists
     */
    virtual void addParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception);
    
    /**
     \brief create parameter
     \param pName parameter name
     \param pValues parameter values
     \exception Exception parameter name already exists
     */
    virtual void addParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief create parameter
     \param pName parameter name
     \param pValues parameter values
     \exception Exception parameter name already exists
     */
    virtual void addParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief create parameter
     \param pParameterName parameter name
     \exception Exception failed to remove parameter
     */
    virtual void removeParameter(const std::string& pParameterName) throw (Exception);
    
    /**
     \brief set parameter value
     \param pParameterName parameter name
     \param pParameterValue parameter value
     \exception Exception parameter name not found
     */
    void setParameterValue(const std::string& pParameterName, float pParameterValue) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    void setParameterValues(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    void setParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValue parameter value
     \exception Exception parameter name is not found
     */
    virtual void set(const std::string& pParameterName, float pParameterValue) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    virtual void set(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    virtual void set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pParameterName parameter name
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue maximum parameter value
     \exception Exception parameter name not found
     */
    virtual void randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception parameter name not found or parameter values has wrong dimension
     */
    virtual void randomize(const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception parameter name not found or parameter values has wrong dimension
     */
    virtual void randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    /**
     \brief assign neighbors for a parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     */
    virtual void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception);
    
    /**
     \brief assign neighbors for a parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that passes NULL for NeighborGroupAlg
     */
    virtual void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception);
    
    /**
     \brief assign neighbors for a parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborRadius radius within which neighbors are searched
     \param pMaxNeighborCount maximum number of neighbors in neighbor list if >= 0 (-1: no limit)
     \param pReplaceNeighborMode replace more distant neighbors with closer neighbors if true
     \exception FlockException parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that automaticall creates a NeighborGroupAlg.
     */
    virtual void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode );
    
    /**
     \brief return number of behaviors
     \return number of behaviors
     */
    unsigned int behaviorCount() const;
    
    /**
     \brief check if agent has behavior
     \param pBehaviorName behavior name
     \return true if agent possesses a particular behavior
     */
    bool checkBehavior(const std::string& pBehaviorName) const;
    
    /**
     \brief get behavior
     \param pBehaviorIndex behavior index
     \return behavior
     \exception Exception behavior does not exist
     */
    Behavior* behavior(unsigned int pBehaviorIndex) throw (Exception);
    
    /**
     \brief get behavior
     \param pBehaviorIndex behavior index
     \return behavior
     \exception Exception behavior does not exist
     */
    const Behavior* behavior(unsigned int pBehaviorIndex) const throw (Exception);
    
    /**
     \brief get behavior
     \param pBehaviorName behavior name
     \return behavior
     \exception Exception behavior not found
     */
    Behavior* behavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief get behavior
     \param pBehaviorName behavior name
     \return behavior
     \exception Exception behavior not found
     */
    const Behavior* behavior(const std::string& pBehaviorName) const throw (Exception);
    
    /**
     \brief add behavior
     \param pBehavior behavior
     \exception Exception behavior already exists
     */	
    virtual void addBehavior(Behavior* pBehavior) throw (Exception);
    
    /**
     \brief add behavior
     \param pBehaviorName behavior name
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    virtual void addBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    virtual void addBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    virtual void addBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief move behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \exception Exception behavior not found
     */	
    virtual void moveBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception);
    
    /**
     \brief move behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \exception Exception behavior not found
     */	
    virtual void moveBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception);
    
    /**
     \brief remove behavior
     \param pBehaviorName behavior name
     \exception Exception failed to remove behavior
     */		
    virtual void removeBehavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief perform behaviors
     */
    virtual void act();
    
    /**
     \brief update parameters
     */
    virtual void flush();
    
    /**
     \brief print agent information
     */
    virtual operator std::string() const;
    
    /**
     \brief print agent information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief print agent information
     */	
    friend std::ostream& operator<< (std::ostream& pOstream, const Agent& pAgent)
    {
        pOstream << pAgent.info();
        
        return pOstream;
    };
    
protected:
    std::string mName; /// \brief agent name
    unsigned int mIndex; /// \brief agent index
    ParameterList mParameterList; /// \brief list of parameters
    BehaviorList mBehaviorList;	/// \brief list of behaviors
};

};

};

#endif