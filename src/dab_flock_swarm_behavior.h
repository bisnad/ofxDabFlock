/** \file dab_flock_swarm_behavior.h
 *  \class dab::flock::SwarmBehavior swarm behavior
 *  \brief swarm behavior
 *
 *  Created by Daniel Bisig on 8/1/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 12/10/08.
 *
 */

#ifndef _dab_flock_swarm_behavior_h_
#define _dab_flock_swarm_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class Swarm;

class SwarmBehavior : public Behavior
{
public:
    /**
     \brief create swarm behavior
     \param pInputSwarmParameterString input swarm parameter string (parameters are space separated)
     \param pInputAgentParameterString input agent parameter string (parameters are space separated)
     \param pOutputSwarmParameterString output swarm parameter string (paramaters are space separated)
     \param pOutputAgentParameterString output agent parameter string (paramaters are space separated)
     */
    SwarmBehavior(const std::string& pInputSwarmParameterString, const std::string& pInputAgentParameterString, const std::string& pOutputSwarmParameterString, const std::string& pOutputAgentParameterString);
    
    /**
     \brief create swarm behavior
     \param pSwarm reference to swarm to which this behavior is added
     \param pSwarmBehaviorName swarm behavior name
     \param pInputSwarmParameterString input swarm parameter string (parameters are space separated)
     \param pInputAgentParameterString input agent parameter string (parameters are space separated)
     \param pOutputSwarmParameterString output swarm parameter string (parameters are space separated)
     \param pOutputAgentParameterString output agent parameter string (parameters are space separated)
     */
    SwarmBehavior(Swarm* pSwarm, const std::string& pSwarmBehaviorName, const std::string& pInputSwarmParameterString, const std::string& pInputAgentParameterString, const std::string& pOutputSwarmParameterString, const std::string& pOutputAgentParameterString);
    
    /**
     \brief destructor
     */
    virtual ~SwarmBehavior();
    
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
    virtual Behavior* create(const std::string& pInputSwarmParameterString, const std::string& pInputAgentParameterString, const std::string& pOutputSwarmParameterString, const std::string& pOutputAgentParameterString) const = 0;
    
    /**
     \brief create agent parameter indices
     */
    void createAgentParameterIndices() throw (Exception);
    
    /**
     \brief return input swarm parameter string
     \return input swarm parameter string
     */
    const std::string& inputSwarmParameterString() const;
    
    /**
     \brief return output swarm parameter string
     \return output swarm parameter string
     */
    const std::string& outputSwarmParameterString() const;
    
    /**
     \brief return input agent parameter string
     \return input agent parameter string
     */
    const std::string& inputAgentParameterString() const;
    
    /**
     \brief return output agent parameter string
     \return output agent parameter string
     */
    const std::string& outputAgentParameterString() const;
    
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
    friend std::ostream& operator << ( std::ostream& pOstream, const SwarmBehavior& pBehavior )
    {
        pOstream << pBehavior.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    SwarmBehavior();
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    /**
     \brief create swarm input behavior parameters
     \exception Exception failed to create swarm input parameters
     */
    void createInputSwarmParameters() throw (Exception);
    
    /**
     \brief create swarm output behavior parameters
     \exception Exception failed to create swarm output parameters
     */
    void createOutputSwarmParameters() throw (Exception);
    
    /**
     \brief create internal swarm behavior parameter
     \param pSwarmParameterName name of swarm parameter
     \param pValues values of swarm parameter
     \exception Exception failed to create swarm internal parameters
     \return swarm parameter
     
     creates an internal swarm behavior parameter which is visible at the level of the swarm (i.e. other swarm behaviors can access it).\n
     the name of the swarm parameter is constructed as follows: behaviorName_parameterName.\n
     */
    Parameter* createInternalSwarmParameter(const std::string& pSwarmParameterName, const std::initializer_list<float>& pValues) throw (Exception);
    
    Swarm* mSwarm; /// \brief swarm this behavior belongs to
    std::string mInputSwarmParameterString; /// \brief unparsed string containing all swarm input parameter names
    std::string mOutputSwarmParameterString; /// \brief unparsed string containing all swarm output parameter names
    std::vector<Parameter*> mInputSwarmParameters; /// \brief input swarm parameters (behavior reads from them)
    std::vector<space::NeighborGroup*> mInputSwarmNeighborGroups; /// \brief input swarm neighbor groups (behavior reads from them)
    std::vector<Parameter*> mOutputSwarmParameters; /// \brief output swarm parameters (behavior writes to them)
    std::vector<Parameter*> mInternalSwarmParameters; /// \brief internal swarm parameters (including scales)
    
    std::vector<Parameter*>& mInputAgentParameters; /// \brief input agent parameters (behavior reads from them)
    std::vector<space::NeighborGroup*>& mInputAgentNeighborGroups; /// \brief input agent neighbor groups (behavior reads from them)
    std::vector<Parameter*>& mOutputAgentParameters; /// \brief output agent parameters (behavior writes to them)
    std::vector<space::NeighborGroup*>& mOutputAgentNeighborGroups; /// \brief output agent neighbor groups (behavior writes to them)
    std::vector<Parameter*>& mInternalAgentParameters; /// \brief internal agent parameters (including scales)
    
    std::vector<unsigned int> mInputAgentParameterIndices; /// \brief input agent parameter indices
    std::vector<unsigned int> mInputAgentNeighborGroupIndices; /// \brief input agent neighbor group indices
    std::vector<unsigned int> mOutputAgentParameterIndices; /// \brief output agent parameter indices
    std::vector<unsigned int> mOutputAgentNeighborGroupIndices; /// \brief output agent neighbor group indices
    std::vector<unsigned int> mInternalAgentParameterIndices; /// \brief internal agent parameter indices	
};

};

};

#endif
