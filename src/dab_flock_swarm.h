/** \file dab_flock_swarm.h
 *
 *  Created by Daniel Bisig on 10/9/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/8/17.
 */

#ifndef _dab_flock_swarm_h_
#define _dab_flock_swarm_h_

#include "dab_listener.h"
#include "dab_flock_agent.h"
#include "dab_flock_parameter.h"
#include "dab_flock_behavior.h"
#include "dab_space_neighbor_group_alg.h"
#include <map>

namespace dab
{

namespace flock
{

class NeighborAssignInfo
{
public:
    
    std::string mParameterName;
    std::string mSpaceName;
    bool mVisible;
    space::NeighborGroupAlg* mNeighborGroupAlg;
    
    NeighborAssignInfo( std::string pParameterName, std::string pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg )
    : mParameterName( pParameterName )
    , mSpaceName( pSpaceName )
    , mVisible( pVisible )
    , mNeighborGroupAlg( pNeighborGroupAlg )
    {};
    
    ~NeighborAssignInfo()
    {
        if(mNeighborGroupAlg != nullptr) delete mNeighborGroupAlg;
    };
};

class Swarm : public Agent, public UpdateListener
{
public:
    /**
     \brief create swarm
     \param pName swarm name
     */
    Swarm(const std::string& pName);
    
    /**
     \brief copy constructor
     \param pName swarm name
     \param pSwarm swarm to copy from
     */
    Swarm(const std::string& pName, const Swarm& pSwarm);
    
    /**
     \brief destructor
     */
    ~Swarm();
    
    // TODO: figure out to properly deal with the combination of smart and simple pointers
    
    /**
     \brief clear swarm
     \remark triggers destructor
     */
    void clear();
    
    /**
     \brief return number of agents
     \return number of agents
     */
    unsigned int agentCount() const;

	/**
	 \brief return agent
	 \param pAgentCount number of agents to add
	\return agent
	\exception Exception failed to return agent
	*/
	Agent* agent(unsigned int pAgentIndex) throw (Exception);
    
    /**
     \brief return agents
     \return agents
     */
    std::vector<Agent*>& agents();
    
    /**
     \brief return agents
     \return agents
     */
    const std::vector<Agent*>& agents() const;
    
    /**
     \brief add single agent to swarm
     \exception Exception failed to add agent
     */
    void addAgent() throw (Exception);
    
    /**
     \brief add a number of agents to swarm
     \param pAgentCount number of agents to add
     \exception Exception failed to add agents
     */
    void addAgents(unsigned int pAgentCount) throw (Exception);
    
    /**
     \brief remove last agent from swarm
     \exception Exception failed to remove agent
     */
    void removeAgent() throw (Exception);
    
    /**
     \brief remove particular agent from swarm
     \exception Exception failed to remove agent
     */
    void removeAgent(unsigned int pAgentIndex) throw (Exception);
    
    /**
     \brief remove the last n agents from swarm
     \param pAgentCount number of agents to remove
     \exception Exception failed to remove agents
     */
    void removeAgents(unsigned int pAgentCount) throw (Exception);
    
    /**
     \brief remove agents starting from an agent index
     \brief pAgentIndex start agent from where agents will be removed
     \param pAgentCount number of agents to remove
     \exception Exception failed to remove agents
     */
    void removeAgents(unsigned int pAgentIndex, unsigned int pAgentCount) throw (Exception);
    
    /**
     \brief remove all agents from swarm
     \exception Exception failed to remove agents
     */
    void removeAgents() throw (Exception);
    
    /**
     \brief return number of swarm parameters the swarm contains
     \return number of swarm parameters the swarm contains
     */
    unsigned int swarmParameterCount() const;
    
    /**
     \brief check if swarm has swarm parameter
     \param pSwarmParameterName swarm parameter name
     \return true if swarm possesses a particular swarm parameter
     */
    bool checkSwarmParameter(const std::string& pSwarmParameterName) const;
    
    /**
     \brief get swarm parameter
     \param pParameterName swarm parameter name
     \return parameter
     \exception Exception swarm parameter does not exist
     */
    Parameter* swarmParameter(const std::string& pParameterName) throw (Exception);

    /**
     \brief get swarm parameter
     \param pParameterName swarm parameter name
     \return parameter
     \exception Exception swarm parameter does not exist
     */
    const Parameter* swarmParameter(const std::string& pParameterName) const throw (Exception);
    
    /**
     \brief get swarm parameter
     \param pParameterIndex swarm parameter index
     \return parameter
     \exception Exception swarm parameter does not exist
     */
    Parameter* swarmParameter(unsigned int pParameterIndex) throw (Exception);
    
    /**
     \brief get swarm parameter
     \param pParameterIndex swarm parameter index
     \return parameter
     \exception Exception swarm parameter does not exist
     */
    const Parameter* swarmParameter(unsigned int pParameterIndex) const throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterName agent parameter name
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    Parameter* parameter(unsigned int pAgentIndex, const std::string& pParameterName) throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterName agent parameter name
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    const Parameter* parameter(unsigned int pAgentIndex, const std::string& pParameterName) const throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterIndex agent parameter index
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    Parameter* parameter(unsigned int pAgentIndex, unsigned int pParameterIndex) throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterIndex agent parameter index
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    const Parameter* parameter(unsigned int pAgentIndex, unsigned int pParameterIndex) const throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterName agent parameter name
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    Parameter* agentParameter(unsigned int pAgentIndex, const std::string& pParameterName) throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterName agent parameter name
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    const Parameter* agentParameter(unsigned int pAgentIndex, const std::string& pParameterName) const throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterIndex agent parameter index
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    Parameter* agentParameter(unsigned int pAgentIndex, unsigned int pParameterIndex) throw (Exception);
    
    /**
     \brief get agent parameter
     \param pAgentIndex agent index
     \param pParameterIndex agent parameter index
     \return parameter
     \exception Exception agent does not exist or agent parameter does not exist
     */
    const Parameter* agentParameter(unsigned int pAgentIndex, unsigned int pParameterIndex) const throw (Exception);
    
    /**
     \brief add agent parameter
     \param pParameter agent parameter
     \exception Exception agent parameter name already exists
     */
    void addParameter(Parameter* pParameter) throw (Exception);
    
    /**
     \brief add agent parameter
     \param pParameter agent parameter
     \exception Exception agent parameter name already exists
     */
    void addAgentParameter(Parameter* pParameter) throw (Exception);
    
    /**
     \brief add swarm parameter
     \param pParameter swarm parameter
     \exception Exception swarm parameter name already exists
     */
    void addSwarmParameter(Parameter* pParameter) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pDim dimension of agent parameter (> 0)
     \exception Exception agent parameter name already exists
     */
    void addParameter(const std::string& pName, unsigned int pDim) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pDim dimension of agent parameter (> 0)
     \exception Exception agent parameter name already exists
     */
    void addAgentParameter(const std::string& pName, unsigned int pDim) throw (Exception);
    
    /**
     \brief create swarm parameter
     \param pName swarm parameter name
     \param pDim dimension of swarm parameter (> 0)
     \exception Exception swarm parameter name already exists
     */
    void addSwarmParameter(const std::string& pName, unsigned int pDim) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pDim dimension of agent parameter (> 0)
     \param pValue uniform agent parameter value
     \exception Exception agent parameter name already exists
     */
    void addParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pDim dimension of agent parameter (> 0)
     \param pValue uniform agent parameter value
     \exception Exception agent parameter name already exists
     */
    void addAgentParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception);
    
    /**
     \brief create swarm parameter
     \param pName swarm parameter name
     \param pDim dimension of swarm parameter (> 0)
     \param pValue uniform swarm parameter value
     \exception Exception swarm parameter name already exists
     */
    void addSwarmParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pValues agent parameter values
     \exception Exception agent parameter name already exists
     */
    void addParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pValues agent parameter values
     \exception Exception agent parameter name already exists
     */
    void addParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pValues agent parameter values
     \exception Exception agent parameter name already exists
     */
    void addAgentParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief create agent parameter
     \param pName agent parameter name
     \param pValues agent parameter values
     \exception Exception agent parameter name already exists
     */
    void addAgentParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief create swarm parameter
     \param pName swarm parameter name
     \param pValues swarm parameter values
     \exception Exception swarm parameter name already exists
     */
    void addSwarmParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief create swarm parameter
     \param pName swarm parameter name
     \param pValues swarm parameter values
     \exception Exception swarm parameter name already exists
     */
    void addSwarmParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief remove agent parameter
     \param pName agent parameter
     \exception Exception failed to remove agent parameter
     */
    void removeParameter( const std::string pName ) throw (Exception);
    
    /**
     \brief remove agent parameter
     \param pName agent parameter
     \exception Exception failed to remove agent parameter
     */
    void removeAgentParameter( const std::string pName ) throw (Exception);
    
    /**
     \brief remove agent parameter
     \param pName agent parameter
     \exception Exception failed to remove agent parameter
     */
    void removeSwarmParameter( const std::string pName ) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValue parameter value
     \exception Exception parameter name is not found
     */
    void set(const std::string& pParameterName, float pParameterValue) throw (Exception);
    
    /**
     \brief set parameter values
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pParameterValue parameter value
     \exception Exception agent index out of bounds or parameter name is not found
     */
    void set(unsigned int pAgentIndex, const std::string& pParameterName, float pParameterValue) throw (Exception);

    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    void set(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    void set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception agent index out of bounds or parameter name is not found
     */
    void set(unsigned int pAgentIndex, const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception agent index out of bounds or parameter name is not found
     */
    void set(unsigned int pAgentIndex, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pParameterName parameter name
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue maximum parameter value
     \exception Exception parameter name not found
     */
    void randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue maximum parameter value
     \exception Exception agent index out of bounds or parameter name is not found
     */
    void randomize(unsigned int pAgentIndex, const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception parameter name not found or parameter values has wrong dimension
     */
    void randomize(const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception parameter name not found or parameter values has wrong dimension
     */
    void randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception agent index out of bounds, parameter name not found or parameter values has wrong dimension
     */
    void randomize(unsigned int pAgentIndex, const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pAgentIndex agent index
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception agent index out of bounds, parameter name not found or parameter values has wrong dimension
     */
    void randomize(unsigned int pAgentIndex, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    std::vector< NeighborAssignInfo* > agentNeighborInfo( const std::string& pParameterName );
    std::vector< NeighborAssignInfo* > swarmNeighborInfo( const std::string& pParameterName );
    
    /**
     \brief assign neighbors for an agent and/or swarm parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     */
    void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception);
    
    /**
     \brief assign neighbors for a agent parameter
     \param pParameterName agent parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     */
    void assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception);
    
    /**
     \brief assign neighbors for a swarm parameter
     \param pParameterName swarm parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborGroupAlg neighbor group algorithm
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     */
    void assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception);
    
    /**
     \brief assign neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that passes nullptr for NeighborGroupAlg
     */
    void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception);
    
    /**
     \brief assign neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that passes nullptr for NeighborGroupAlg
     */
    void assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception);
    
    /**
     \brief assign neighbors for a swarm parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that passes nullptr for NeighborGroupAlg
     */
    void assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception);
    
    /**
     \brief assign neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborRadius radius within which neighbors are searched
     \param pMaxNeighborCount maximum number of neighbors in neighbor list if >= 0 (-1: no limit)
     \param pReplaceNeighborMode replace more distant neighbors with closer neighbors if true
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that automaticall creates a NeighborGroupAlg.
     */
    void assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception);
    
    /**
     \brief assign neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborRadius radius within which neighbors are searched
     \param pMaxNeighborCount maximum number of neighbors in neighbor list if >= 0 (-1: no limit)
     \param pReplaceNeighborMode replace more distant neighbors with closer neighbors if true
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that automaticall creates a NeighborGroupAlg.
     */
    void assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception);
    
    /**
     \brief assign neighbors for a swarm parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \param pVisible whether parameter is visible
     \param pNeighborRadius radius within which neighbors are searched
     \param pMaxNeighborCount maximum number of neighbors in neighbor list if >= 0 (-1: no limit)
     \param pReplaceNeighborMode replace more distant neighbors with closer neighbors if true
     \exception Exception parameter not found, space not found, parameter and space dimension mismatch
     
     simpified assignNeighbors method that automaticall creates a NeighborGroupAlg.
     */
    void assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception);
    
    /**
     \brief remove neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \exception Exception parameter not found, space not found
     */
    void removeNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception);
    
    /**
     \brief remove neighbors for an agent parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \exception Exception parameter not found, space not found
     */
    void removeAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception);
    
    /**
     \brief remove neighbors for a swarm parameter
     \param pParameterName parameter name
     \param pSpaceName name of neighbor space
     \exception Exception parameter not found, space not found
     */
    void removeSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception);
    
    /**
     \brief return number of swarm behaviors the swarm contains
     \return number of swarm behaviors the swarm contains
     */
    unsigned int swarmBehaviorCount() const;
    
    /**
     \brief check if swarm has swarm behavior
     \param pSwarmBehaviorName swarm behavior name
     \return true if swarm possesses a particular swarm behavior
     */
    bool checkSwarmBehavior(const std::string& pSwarmBehaviorName) const;
    
    /**
     \brief get swarm behavior
     \param pBehaviorName swarm behavior name
     \return swarm behavior
     \exception FlockException swarm behavior does not exist
     */
    Behavior* swarmBehavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief get swarm behavior
     \param pBehaviorName swarm behavior name
     \return swarm behavior
     \exception FlockException swarm behavior does not exist
     */
    const Behavior* swarmBehavior(const std::string& pBehaviorName) const throw (Exception);
    
    /**
     \brief get swarm behavior
     \param pBehaviorIndex swarm behavior index
     \return swarm behavior
     \exception FlockException swarm behavior does not exist
     */
    Behavior* swarmBehavior(unsigned int pBehaviorIndex) throw (Exception);
    
    /**
     \brief get swarm behavior
     \param pBehaviorIndex swarm behavior index
     \return swarm behavior
     \exception FlockException swarm behavior does not exist
     */
    const Behavior* swarmBehavior(unsigned int pBehaviorIndex) const throw (Exception);
    
    /**
     \brief add swarm behavior
     \param pBehavior swarm behavior
     \exception Exception swarm behavior already exists
     */
    void addSwarmBehavior(Behavior* pBehavior) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName behavior name
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */
    void addBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */
    void addBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */
    void addBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief move agent behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \exception FlockException behavior not found
     */	
    void moveBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception);
    
    /**
     \brief move agent behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \exception Exception behavior not found
     */	
    void moveBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName agent behavior name
     \param pBehavior agent behavior
     \exception Exception agent behavior already exists or Illegal Behavior Parameters
     */	
    void addAgentBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    void addAgentBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add agent behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    void addAgentBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief move agent behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \exception Exception behavior not found
     */	
    void moveAgentBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception);
    
    /**
     \brief move agent behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \exception Exception behavior not found
     */	
    void moveAgentBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception);
    
    /**
     \brief add swarm behavior
     \param pBehaviorName swarm behavior name
     \param pBehavior swarm behavior
     \exception Exception swarm behavior already exists or Illegal Behavior Parameters
     */	
    void addSwarmBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add swarm behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    void addSwarmBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief add swarm behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \param pBehavior behavior
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */	
    void addSwarmBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception);
    
    /**
     \brief move swarm behavior
     \param pBehaviorName behavior name
     \param pBehaviorPosition behavior position
     \exception Exception behavior not found
     */	
    void moveSwarmBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception);
    
    /**
     \brief move swarm behavior
     \param pBehaviorName behavior name
     \param pSuccessorBehaviorName name of behavior in front of which the new behavior is inserted
     \exception Exception behavior not found
     */	
    void moveSwarmBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception);
    
    /**
     \brief remove agent behavior
     \param pBehaviorName agent behavior name
     \exception Exception failed to remove agent behavior
     */
    void removeBehavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief remove agent behavior
     \param pBehaviorName agent behavior name
     \exception Exception failed to remove agent behavior
     */
    void removeAgentBehavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief remove swarm behavior
     \param pBehaviorName swarm behavior name
     \exception Exception failed to remove swarm behavior
     */
    void removeSwarmBehavior(const std::string& pBehaviorName) throw (Exception);
    
    /**
     \brief perform behaviors
     */
    virtual void act();
    
    /**
     \brief update parameters
     */
    virtual void flush();
    
    /**
     \brief notify swarm whenever the simulation progresses a step
     */
    void notify();
    
    /**
     \brief print swarm information
     */
    virtual operator std::string() const;
    
    /**
     \brief print swarm information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief print swarm information
     */	
    friend std::ostream& operator<< (std::ostream& pOstream, const Swarm& pSwarm)
    {
        pOstream << pSwarm.info();
        
        return pOstream;
    };
    
    static unsigned int sInstanceCount; /// \brief total number of swarm instances ever created	
    
protected:
    /**
     \brief default constructor
     */
    Swarm();
    
    std::shared_ptr<Swarm> mSelf;
    
    ParameterList& mAgentParameterList; /// \brief list of agent parameters
    BehaviorList& mAgentBehaviorList;	/// \brief list of agent behaviors
    ParameterList mSwarmParameterList; /// \brief swarm exclusive list of parameters
    BehaviorList mSwarmBehaviorList;	/// \brief swarm exclusive list of behaviors
    std::vector<Agent*> mAgents; /// \brief swarm agents
    
    unsigned int mAgentCreationCount; /// \brief numbers of agents ever created for this swarm
    
    std::map< std::string, std::vector<NeighborAssignInfo*> > mAgentNeighborAssignRegistry; /// \brief registry for all agent parameters that are assigned to some neighbor space
    std::map< std::string, std::vector<NeighborAssignInfo*> > mSwarmNeighborAssignRegistry; /// \brief registry for all swarm parameters that are assigned to some neighbor space
};

};

};

#endif