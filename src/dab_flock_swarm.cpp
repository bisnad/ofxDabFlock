/** \file dab_flock_swarm.cpp
 */

#include "dab_math.h"
#include "dab_flock_swarm.h"
#include "dab_flock_agent.h"
#include "dab_flock_swarm_behavior.h"
#include "dab_flock_simulation.h"
#include "dab_flock_stats.h"

using namespace dab;
using namespace dab::flock;

unsigned int Swarm::sInstanceCount = 0;

Swarm::Swarm()
: Agent()
, mAgentParameterList(mParameterList)
, mAgentBehaviorList(mBehaviorList)
, mSelf(this)
{
	assert("illegal constructor");
}

Swarm::Swarm(const std::string& pName)
: Agent(pName)
, mAgentParameterList(mParameterList)
, mAgentBehaviorList(mBehaviorList)
, mAgentCreationCount(0)
, mSelf(this)
{
	mIndex = sInstanceCount++;
    
	//std::cout << "swarm " << mName.toStdString() << " index " << mIndex << "\n";
	
	addSwarmParameter( new Parameter( this, "active", {1.0} ) );
    
	Simulation::get().addAgent(this);
	Simulation::get().addSwarm(this);
	Simulation::get().addListener(mSelf);
}

Swarm::Swarm(const std::string& pName, const Swarm& pSwarm)
: Agent(pName, pSwarm)
, mAgentParameterList(mParameterList)
, mAgentBehaviorList(mBehaviorList)
, mAgentCreationCount(0)
, mSelf(this)
{
	mIndex = sInstanceCount++;
    
	// copy swarm parameters
	unsigned int parCount = pSwarm.swarmParameterCount();
	for(unsigned int i=0; i<parCount; ++i)
	{
		//std::cout << "copy swarm par: " << pSwarm.swarmParameter(i) << "\n";
        
		addSwarmParameter( new Parameter(this, *pSwarm.swarmParameter(i) ) );
	}
    
	// copy swarm behaviors
	unsigned int behCount = pSwarm.behaviorCount();
	for(unsigned int i=0; i<behCount; ++i)
	{
		const Behavior* beh = pSwarm.behavior(i);
		addSwarmBehavior( beh->create( beh->name(), this ) );
	}
    
	Simulation::get().addAgent(this);
	Simulation::get().addSwarm(this);
	Simulation::get().addListener(mSelf);
    
	// add same number of agents
	addAgents( pSwarm.mAgents.size() );
}

Swarm::~Swarm()
{
    std::cout << "destructor " << mName << "\n";
    
//    Simulation::get().removeListener(mSelf);
//    
//    if(mSelf == nullptr)
//    {
//        std::cout << "mSelf null\n";
//        
//        return;
//    }
//    
//    std::cout << "destructor2 " << mName << "\n";
//    
//    mSelf = std::shared_ptr<Swarm>(nullptr);
	
	removeAgents();
    
	Simulation::get().removeAgent(this);
	Simulation::get().removeSwarm(this);
    
    for(auto regIter = mAgentNeighborAssignRegistry.begin(); regIter != mAgentNeighborAssignRegistry.end(); ++regIter)
    {
        std::vector<NeighborAssignInfo*>& infos = regIter->second;
        int infoCount = infos.size();
        
        for(int i=0; i<infoCount; ++i)
        {
            delete infos[i];
        }
        
        infos.clear();
    }
    mAgentNeighborAssignRegistry.clear();
    
    for(auto regIter = mSwarmNeighborAssignRegistry.begin(); regIter != mSwarmNeighborAssignRegistry.end(); ++regIter)
    {
        std::vector<NeighborAssignInfo*>& infos = regIter->second;
        int infoCount = infos.size();
        
        for(int i=0; i<infoCount; ++i)
        {
            delete infos[i];
        }
        
        infos.clear();
    }
    mSwarmNeighborAssignRegistry.clear();

    std::cout << "destructor3 " << mName << "\n";
}

void
Swarm::clear()
{
    Simulation::get().removeListener(mSelf);
    mSelf.reset();
}

unsigned int
Swarm::agentCount() const
{
	return mAgents.size();
}

Agent* 
Swarm::agent(unsigned int pAgentIndex) throw (Exception)
{
	if (pAgentIndex >= mAgents.size()) throw Exception("FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " exceeds numbers of agents " + std::to_string(mAgents.size()), __FILE__, __FUNCTION__, __LINE__);

	return mAgents[pAgentIndex];
}

std::vector<Agent*>&
Swarm::agents()
{
	return mAgents;
}

const std::vector<Agent*>&
Swarm::agents() const
{
	return mAgents;
}

void
Swarm::addAgent() throw (Exception)
{
    //	std::cout << "Swarm::addAgent() begin\n";
    
	try
	{
		Agent* agent = new Agent( std::string( mName + "_" + std::to_string(mAgents.size()) ), *this );
		
		// debug
		//std::cout << "swarm " << mName<< " add agent " << agent->index() << "\n";
        
        //		std::cout << "create agent " << agent->name() << "\n";
		
		//////////////////////////////////////////////
		// assign parameters to neighborhood spaces //
		//////////////////////////////////////////////
		
		NeighborAssignInfo* neighborAssignInfo;
		
        for(auto iter = mAgentNeighborAssignRegistry.begin(); iter != mAgentNeighborAssignRegistry.end(); ++iter)
        {
            const std::string& parameterName = iter->first;
			Parameter* agentParameter = agent->parameter(parameterName);

            std::vector< NeighborAssignInfo* >& neighborAssigns = mAgentNeighborAssignRegistry[parameterName];
			unsigned int neighborAssignCount = neighborAssigns.size();
			
			//std::cout << "parameterName " << parameterName << " neighborAssignCount " << neighborAssignCount << "\n";
			
			for(unsigned int neighborAssignIndex=0; neighborAssignIndex<neighborAssignCount; ++neighborAssignIndex)
			{
				neighborAssignInfo = neighborAssigns[neighborAssignIndex];
				
				//std::cout << "pName " << neighborAssignInfo->mParameterName << " sName " << neighborAssignInfo->mSpaceName << "\n";
				
				space::NeighborGroup* agentParameterNeighborGroup = agentParameter->neighborGroup(neighborAssignInfo->mSpaceName);
				agentParameterNeighborGroup->setVisible( neighborAssignInfo->mVisible );
				if(neighborAssignInfo->mNeighborGroupAlg != nullptr) agentParameterNeighborGroup->setNeighborGroupAlg( new space::NeighborGroupAlg( *( neighborAssignInfo->mNeighborGroupAlg ) ) );
			}

        }
        
		/////////////////////////////
		// add agent to simulation //
		/////////////////////////////
		
		mAgents.push_back(agent);
		Simulation::get().addAgent(agent);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add agent to swarm", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
    //	std::cout << "Swarm::addAgent() done\n";
}

void
Swarm::addAgents(unsigned int pAgentCount) throw (Exception)
{
	try
	{
		for(unsigned int i=0; i<pAgentCount; ++i) addAgent();
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgent() throw (Exception)
{
	try
	{
		removeAgent( mAgents.size() - 1 );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgent(unsigned int pAgentIndex) throw (Exception)
{
	if( pAgentIndex > mAgents.size() ) throw Exception( "FLOCK ERROR: Agent Nr " + std::to_string(pAgentIndex) + " Does Not Exist", __FILE__, __FUNCTION__, __LINE__ );
	
    Agent* agent = mAgents[pAgentIndex];
    
	try
	{
        mAgents.erase(mAgents.begin() + pAgentIndex);
		
		Simulation& simulation = Simulation::get();
		simulation.removeAgent(agent);
		
		delete agent;
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to remove agent " + agent->name() + " from swarm", __FILE__, __FUNCTION__, __LINE__);
		throw;
	}
}

void
Swarm::removeAgents(unsigned int pAgentCount) throw (Exception)
{
	try
	{
		if( mAgents.size() == 0 ) return;
		if(pAgentCount > mAgents.size() ) pAgentCount = mAgents.size();
        
		for(unsigned int i=0; i<pAgentCount; ++i) removeAgent();
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgents(unsigned int pAgentIndex, unsigned int pAgentCount) throw (Exception)
{
	//std::cout << "swarm " << mName.toStdString() << " removeAgents index " << pAgentIndex << " agentCount " << pAgentCount << "\n";
	
	try
	{
		if( mAgents.size() == 0 ) return;
		if( pAgentCount > mAgents.size() - pAgentIndex ) pAgentCount = mAgents.size() - pAgentIndex;
		
		//std::cout << "remove agent count " << pAgentCount << " agent size " << mAgents.size() << "\n";
		
		for(unsigned int i=0; i<pAgentCount; ++i )
		{
			//std::cout << "i " << i << " agentIndex " << pAgentIndex << "\n";
			
			removeAgent( pAgentIndex );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgents() throw (Exception)
{
	int agentCount = mAgents.size();
    
	try
	{
		for(unsigned int i=0; i<agentCount; ++i) removeAgent();
	}
	catch(Exception& e)
	{
		throw;
	}
}

unsigned int
Swarm::swarmParameterCount() const
{
	return mSwarmParameterList.parameterCount();
}

bool
Swarm::checkSwarmParameter(const std::string& pSwarmParameterName) const
{
	return mSwarmParameterList.contains(pSwarmParameterName);
}

Parameter*
Swarm::swarmParameter(const std::string& pParameterName) throw (Exception)
{
	try
	{
		return mSwarmParameterList.parameter(pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::swarmParameter(const std::string& pParameterName) const throw (Exception)
{
	try
	{
		return mSwarmParameterList.parameter(pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Parameter*
Swarm::swarmParameter(unsigned int pParameterIndex) throw (Exception)
{
	try
	{
		return mSwarmParameterList.parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::swarmParameter(unsigned int pParameterIndex) const throw (Exception)
{
	try
	{
		return mSwarmParameterList.parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Parameter*
Swarm::parameter(unsigned int pAgentIndex, const std::string& pParameterName) throw (Exception)
{
	try
	{
		return agentParameter(pAgentIndex, pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::parameter(unsigned int pAgentIndex, const std::string& pParameterName) const throw (Exception)
{
	try
	{
		return agentParameter(pAgentIndex, pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Parameter*
Swarm::parameter(unsigned int pAgentIndex, unsigned int pParameterIndex) throw (Exception)
{
	try
	{
		return agentParameter(pAgentIndex, pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::parameter(unsigned int pAgentIndex, unsigned int pParameterIndex) const throw (Exception)
{
	try
	{
		return agentParameter(pAgentIndex, pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Parameter*
Swarm::agentParameter(unsigned int pAgentIndex, const std::string& pParameterName) throw (Exception)
{
	if(pAgentIndex >= mAgents.size()) throw Exception( "FLOCK ERROR: Agent Nr " + std::to_string(pAgentIndex) + " Does Not Exist", __FILE__, __FUNCTION__, __LINE__ );
	try
	{
		return mAgents[pAgentIndex]->parameter(pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::agentParameter(unsigned int pAgentIndex, const std::string& pParameterName) const throw (Exception)
{
	if(pAgentIndex >= mAgents.size()) throw Exception( "FLOCK ERROR: Agent Nr " + std::to_string(pAgentIndex) + " Does Not Exist", __FILE__, __FUNCTION__, __LINE__ );
	try
	{
		return mAgents[pAgentIndex]->parameter(pParameterName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Parameter*
Swarm::agentParameter(unsigned int pAgentIndex, unsigned int pParameterIndex) throw (Exception)
{
	if(pAgentIndex >= mAgents.size()) throw Exception( "FLOCK ERROR: Agent Nr " + std::to_string(pAgentIndex) + " Does Not Exist", __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		return mAgents[pAgentIndex]->parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Parameter*
Swarm::agentParameter(unsigned int pAgentIndex, unsigned int pParameterIndex) const throw (Exception)
{
	if(pAgentIndex >= mAgents.size()) throw Exception( "FLOCK ERROR: Agent Nr " + std::to_string(pAgentIndex) + " Does Not Exist", __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		return mAgents[pAgentIndex]->parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addParameter(Parameter* pParameter) throw (Exception)
{
	try
	{
		addAgentParameter(pParameter);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentParameter(Parameter* pParameter) throw (Exception)
{
	try
	{
		Agent::addParameter(pParameter);
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->addParameter( new Parameter( mAgents[i], *pParameter ) );
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add agent patameter " + pParameter->name(), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Swarm::addSwarmParameter(Parameter* pParameter) throw (Exception)
{
	//std::cout << "Swarm " << mName << " addParameter(Parameter* " << pParameter->name() << " ) begin\n";
    
	try
	{
		mSwarmParameterList.addParameter(pParameter);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add swarm patameter " + pParameter->name(), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
	
	//std::cout << "Swarm " << mName << " addParameter(Parameter* " << pParameter->name() << " ) end\n";
}

void
Swarm::addParameter(const std::string& pName, unsigned int pDim) throw (Exception)
{
	try
	{
		addAgentParameter( pName, pDim );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentParameter(const std::string& pName, unsigned int pDim) throw (Exception)
{
	try
	{
		Agent::addParameter( pName, pDim );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->addParameter( pName, pDim );
		}
 	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmParameter(const std::string& pName, unsigned int pDim) throw (Exception)
{
	try
	{
		addSwarmParameter( new Parameter(this, pName, pDim) );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception)
{
	try
	{
		addAgentParameter(pName, { pValue } );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception)
{
	try
	{
        Eigen::VectorXf parValue(pDim);
        parValue.setConstant(pValue);
        
		addAgentParameter(pName, parValue );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception)
{
	try
	{
        Eigen::VectorXf parValue(pDim);
        parValue.setConstant(pValue);
		addSwarmParameter(pName, parValue );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception)
{
	try
	{
		addAgentParameter( pName, pValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception)
{
	try
	{
		addAgentParameter( pName, pValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception)
{
	try
	{
		Agent::addParameter( pName, pValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception)
{
	try
	{
		Agent::addParameter( pName, pValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception)
{
	try
	{
		addSwarmParameter( new Parameter(this, pName, pValues) );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception)
{
	try
	{
		addSwarmParameter( new Parameter(this, pName, pValues) );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeParameter( const std::string pName ) throw (Exception)
{
	try
	{
		removeAgentParameter( pName );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgentParameter( const std::string pName ) throw (Exception)
{
	try
	{
		unsigned int agentCount = mAgents.size();
		for(unsigned int agentNr=0; agentNr < agentCount; ++agentNr) mAgents[agentNr]->removeParameter( pName );
		Agent::removeParameter( pName );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeSwarmParameter( const std::string pName ) throw (Exception)
{
	try
	{
		mSwarmParameterList.removeParameter(pName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(const std::string& pParameterName, float pParameterValue) throw (Exception)
{
	try
	{
		bool parameterFound = false;
        
		// swarm parameter
		if(mSwarmParameterList.contains(pParameterName))
		{
			mSwarmParameterList.setValue(pParameterName, pParameterValue);
			parameterFound = true;
		}
        
		// agent parameters
		if(mAgentParameterList.contains(pParameterName))
		{
			unsigned int parIndex = parameterIndex(pParameterName);
			Agent::parameter(parIndex)->setValue(pParameterValue);
            
			unsigned int agentCount = mAgents.size();
            
			for(unsigned int i=0; i<agentCount; ++i)
			{
				mAgents[i]->parameter(parIndex)->setValue(pParameterValue);
			}
			
			parameterFound = true;
		}
		
		if(parameterFound == false)  throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(unsigned int pAgentIndex, const std::string& pParameterName, float pParameterValue) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex)  + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		mAgents[pAgentIndex]->parameter( pParameterName )->setValue( pParameterValue );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception)
{
	//std::cout << "swarm set " << pParameterName << " : " << pParameterValues << "\n";
    
	try
	{
        int parDim = pParameterValues.size();
        Eigen::VectorXf parValues(parDim);
        
        auto iter = pParameterValues.begin();
        
        for( int d=0; d < parDim; ++d, ++iter)
        {
            parValues[d] = *iter;
        }
        
        set(pParameterName, parValues);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception)
{
	//std::cout << "swarm set " << pParameterName << " : " << pParameterValues << "\n";
    
	try
	{
		bool parameterFound = false;
        
		// swarm parameter
		if(mSwarmParameterList.contains(pParameterName))
		{
			mSwarmParameterList.setValues(pParameterName, pParameterValues);
			parameterFound = true;
		}
        
		// agent parameters
		if(mAgentParameterList.contains(pParameterName))
		{
			unsigned int parIndex = parameterIndex(pParameterName);
			Agent::parameter(parIndex)->setValues(pParameterValues);
            
			unsigned int agentCount = mAgents.size();
            
			for(unsigned int i=0; i<agentCount; ++i)
			{
				mAgents[i]->parameter( parIndex )->setValues( pParameterValues );
			}
			
			parameterFound = true;
		}
		
		if(parameterFound == false)  throw Exception("FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(unsigned int pAgentIndex, const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		mAgents[pAgentIndex]->parameter( pParameterName )->setValues( pParameterValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::set(unsigned int pAgentIndex, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		mAgents[pAgentIndex]->parameter( pParameterName )->setValues( pParameterValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception)
{
	math::Math<>& math = math::Math<>::get();
    
	try
	{
		bool parameterFound = false;
        
		// swarm parameter
		if(mSwarmParameterList.contains(pParameterName))
		{
			mSwarmParameterList.setValue(pParameterName, math.random( pMinParameterValue, pMaxParameterValue ));
			parameterFound = true;
		}
        
		// agent parameters
		if(mAgentParameterList.contains(pParameterName))
		{
			//Agent::set( pParameterName, math.random( pMinParameterValue, pMaxParameterValue ) );
			
			unsigned int parIndex = parameterIndex(pParameterName);
			Agent::parameter(parIndex)->setValue( math.random( pMinParameterValue, pMaxParameterValue ) );
            
			unsigned int agentCount = mAgents.size();
            
			for(unsigned int i=0; i<agentCount; ++i)
			{
				mAgents[i]->parameter(parIndex)->setValue(  math.random( pMinParameterValue, pMaxParameterValue ) );
			}
			
			parameterFound = true;
		}
		
		if(parameterFound == false)  throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::randomize(unsigned int pAgentIndex, const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
    
    math::Math<>& math = math::Math<>::get();
    
	try
	{
		mAgents[pAgentIndex]->parameter( pParameterName )->setValue( math.random( pMinParameterValue, pMaxParameterValue ) );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::randomize(const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception)
{
    if(pMinParameterValues.size() != pMaxParameterValues.size() ) throw Exception("FLOCK ERROR: minim parameter dimension " + std::to_string(pMinParameterValues.size()) + " doesn't match maximum parameter size " + std::to_string(pMaxParameterValues.size()), __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        int parDim = pMinParameterValues.size();
        
        Eigen::VectorXf minParValues(parDim);
        
        auto minIter = pMinParameterValues.begin();
        
        for( int d=0; d < parDim; ++d, ++minIter)
        {
            minParValues[d] = *minIter;
        }
        
        Eigen::VectorXf maxParValues(parDim);
        
        auto maxIter = pMaxParameterValues.begin();
        
        for( int d=0; d < parDim; ++d, ++maxIter)
        {
            maxParValues[d] = *maxIter;
        }
        
        randomize(pParameterName, minParValues, maxParValues);
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
Swarm::randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
	if(pMinParameterValues.rows() != pMaxParameterValues.rows() ) throw Exception( "FLOCK ERROR: minimum parameter dim " + std::to_string(pMinParameterValues.rows()) + " doesn't match maximum parameter dim " + std::to_string(pMinParameterValues.rows()), __FILE__, __FUNCTION__, __LINE__ );
    
	math::Math<>& math = math::Math<>::get();
	unsigned int parameterDim = pMinParameterValues.rows();
	Eigen::VectorXf randValue( parameterDim );
    
	try
	{
		bool parameterFound = false;
        
		// swarm parameter
		if(mSwarmParameterList.contains(pParameterName))
		{
			for( unsigned int i=0; i<parameterDim; ++i ) randValue[i] = math.random( pMinParameterValues[i], pMaxParameterValues[i] );
			mSwarmParameterList.setValues(pParameterName, randValue );
			parameterFound = true;
		}
        
		// agent parameters
		if(mAgentParameterList.contains(pParameterName))
		{
			unsigned int parIndex = parameterIndex(pParameterName);
			
			for( unsigned int i=0; i<parameterDim; ++i ) randValue[i] = math.random( pMinParameterValues[i], pMaxParameterValues[i] );
			Agent::parameter(parIndex)->setValues( randValue);
            
			//Agent::set( pParameterName, randValue );
            
			unsigned int agentCount = mAgents.size();
            
			for(unsigned int i=0; i<agentCount; ++i)
			{
				for( unsigned int j=0; j<parameterDim; ++j ) randValue[j] = math.random( pMinParameterValues[j], pMaxParameterValues[j] );
				mAgents[i]->parameter( pParameterName )->setValues( randValue );
			}
			
			parameterFound = true;
		}
		
		if(parameterFound == false)  throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::randomize(unsigned int pAgentIndex, const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	if(pMinParameterValues.size() != pMaxParameterValues.size() ) throw Exception( "FLOCK ERROR: minimum parameter dim " + std::to_string(pMinParameterValues.size()) + " doesn't match maximum parameter dim " + std::to_string(pMaxParameterValues.size()), __FILE__, __FUNCTION__, __LINE__ );
    
    try
    {
        int parDim = pMinParameterValues.size();
        
        Eigen::VectorXf minParValues(parDim);
        
        auto minIter = pMinParameterValues.begin();
        
        for( int d=0; d < parDim; ++d, ++minIter)
        {
            minParValues[d] = *minIter;
        }
        
        Eigen::VectorXf maxParValues(parDim);
        
        auto maxIter = pMaxParameterValues.begin();
        
        for( int d=0; d < parDim; ++d, ++maxIter)
        {
            maxParValues[d] = *maxIter;
        }
        
        randomize(pAgentIndex, pParameterName, minParValues, maxParValues);
    }
    catch(Exception& e)
    {
        throw;
    }
}


void
Swarm::randomize(unsigned int pAgentIndex, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
	if( pAgentIndex >= mAgents.size() ) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pAgentIndex) + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	if(pMinParameterValues.rows() != pMaxParameterValues.rows() ) throw Exception( "FLOCK ERROR: minimum parameter dim " + std::to_string(pMinParameterValues.rows()) + " doesn't match maximum parameter dim " + std::to_string(pMaxParameterValues.rows()), __FILE__, __FUNCTION__, __LINE__ );
    
	math::Math<>& math = math::Math<>::get();
	unsigned int parameterDim = pMinParameterValues.rows();
	Eigen::VectorXf randValue( parameterDim );
	for( unsigned int i=0; i<parameterDim; ++i ) randValue[i] = math.random( pMinParameterValues[i], pMaxParameterValues[i] );
    
	try
	{
		mAgents[pAgentIndex]->parameter(pParameterName)->setValues( randValue);
	}
	catch(Exception& e)
	{
		throw;
	}
}

std::vector< NeighborAssignInfo* >
Swarm::agentNeighborInfo( const std::string& pParameterName )
{
	if( mAgentNeighborAssignRegistry.find( pParameterName ) != mAgentNeighborAssignRegistry.end() ) return mAgentNeighborAssignRegistry.at( pParameterName );
	else return std::vector< NeighborAssignInfo* >();
}

std::vector< NeighborAssignInfo* >
Swarm::swarmNeighborInfo( const std::string& pParameterName )
{
	if( mSwarmNeighborAssignRegistry.find( pParameterName ) != mSwarmNeighborAssignRegistry.end() ) return mSwarmNeighborAssignRegistry.at( pParameterName );
	else return std::vector< NeighborAssignInfo* >();
}

void
Swarm::assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception)
{
	try
	{
		bool parameterFound = false;
        
		if(mSwarmParameterList.contains(pParameterName))
		{
			parameterFound = true;
			assignSwarmNeighbors(pParameterName, pSpaceName, pVisible, pNeighborGroupAlg);
		}
		if(mAgentParameterList.contains(pParameterName))
		{
			parameterFound = true;
			assignAgentNeighbors(pParameterName, pSpaceName, pVisible, pNeighborGroupAlg);
		}
		
		if(parameterFound == false)  throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception)
{
	try
	{
		if(mAgentParameterList.contains(pParameterName))
		{
			NeighborAssignInfo* neighborAssignInfo;
			
			// check if there is already a registry available for this neighbor assignment
			bool spaceFound = false;
            
			if( mAgentNeighborAssignRegistry.find( pParameterName ) != mAgentNeighborAssignRegistry.end() )
			{
                std::vector<NeighborAssignInfo*>& neighborAssignInfos = mAgentNeighborAssignRegistry.at( pParameterName );
				int neighborAssignInfoCount = neighborAssignInfos.size();
				for( int nI=0; nI<neighborAssignInfoCount; ++nI )
				{
					neighborAssignInfo = neighborAssignInfos[nI];
					if( neighborAssignInfo->mSpaceName == pSpaceName )
					{
						spaceFound = true;
						neighborAssignInfo->mVisible = pVisible;
						if( neighborAssignInfo->mNeighborGroupAlg != nullptr ) delete neighborAssignInfo->mNeighborGroupAlg;
						neighborAssignInfo->mNeighborGroupAlg = pNeighborGroupAlg;
					}
				}
			}
			
			if( spaceFound == false ) // create new neighbor assign info
			{
                neighborAssignInfo = new NeighborAssignInfo(pParameterName, pSpaceName, pVisible, pNeighborGroupAlg );
                mAgentNeighborAssignRegistry[pParameterName].push_back(neighborAssignInfo);
			}
            
			if(pNeighborGroupAlg != nullptr) Agent::assignNeighbors( pParameterName, pSpaceName, false, new space::NeighborGroupAlg( pNeighborGroupAlg->neighborRadius(), 0, pNeighborGroupAlg->replaceNeighborMode() ) );
			else Agent::assignNeighbors( pParameterName, pSpaceName, false, nullptr );
            
			unsigned int agentCount = mAgents.size();
			
			for(unsigned int i=0; i<agentCount; ++i)
			{
				if(pNeighborGroupAlg != nullptr) mAgents[i]->assignNeighbors( pParameterName, pSpaceName, pVisible, new space::NeighborGroupAlg( *pNeighborGroupAlg ) );
				else  mAgents[i]->assignNeighbors( pParameterName, pSpaceName, pVisible, nullptr );
			}
		}
		else throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception)
{
	try
	{
		if(mSwarmParameterList.contains(pParameterName))
		{
			NeighborAssignInfo* neighborAssignInfo;
			
			// check if there is already a registry available for this neighbor assignment
			bool spaceFound = false;
            
			if( mSwarmNeighborAssignRegistry.find( pParameterName ) != mSwarmNeighborAssignRegistry.end() )
			{
                std::vector<NeighborAssignInfo*> neighborAssignInfos = mSwarmNeighborAssignRegistry.at( pParameterName );
				int neighborAssignInfoCount = neighborAssignInfos.size();
				for( int nI=0; nI<neighborAssignInfoCount; ++nI )
				{
					neighborAssignInfo = neighborAssignInfos[nI];
					if( neighborAssignInfo->mSpaceName == pSpaceName )
					{
						spaceFound = true;
						neighborAssignInfo->mVisible = pVisible;
						if( neighborAssignInfo->mNeighborGroupAlg != NULL ) delete neighborAssignInfo->mNeighborGroupAlg;
						neighborAssignInfo->mNeighborGroupAlg = pNeighborGroupAlg;
					}
				}
			}
			
			if( spaceFound == false ) // create new neighbor assign info
			{
				neighborAssignInfo = new NeighborAssignInfo(pParameterName, pSpaceName, pVisible, pNeighborGroupAlg );
                mSwarmNeighborAssignRegistry[pParameterName].push_back(neighborAssignInfo);
			}
            
            std::shared_ptr<space::Space> space = space::SpaceManager::get().space(pSpaceName);
			Parameter* parameter = mSwarmParameterList.parameter(pParameterName);
			
			if( space->checkObject( parameter ) == true ) space->setObject( parameter, pVisible, pNeighborGroupAlg  );
			else space->addObject( parameter, pVisible, pNeighborGroupAlg);
		}
		else throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception)
{
	try
	{
		assignNeighbors( pParameterName, pSpaceName, pVisible, nullptr );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception)
{
	try
	{
		assignAgentNeighbors( pParameterName, pSpaceName, pVisible, nullptr );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible ) throw (Exception)
{
	try
	{
		assignSwarmNeighbors( pParameterName, pSpaceName, pVisible, nullptr );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception)
{
	try
	{
		bool parameterFound = false;
        
		if(mSwarmParameterList.contains(pParameterName))
		{
			parameterFound = true;
			assignSwarmNeighbors(pParameterName, pSpaceName, pVisible, pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode);
		}
		if(mAgentParameterList.contains(pParameterName))
		{
			parameterFound = true;
			assignAgentNeighbors(pParameterName, pSpaceName, pVisible, pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode);
		}
		
		if(parameterFound == false)  throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::assignAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception)
{
	try
	{
		NeighborAssignInfo* neighborAssignInfo;
		
		// check if there is already a registry available for this neighbor assignment
		bool spaceFound = false;
		if( mAgentNeighborAssignRegistry.find( pParameterName ) != mAgentNeighborAssignRegistry.end())
		{
            std::vector<NeighborAssignInfo*> neighborAssignInfos = mAgentNeighborAssignRegistry.at( pParameterName );
			int neighborAssignInfoCount = neighborAssignInfos.size();
			for( int nI=0; nI<neighborAssignInfoCount; ++nI )
			{
				neighborAssignInfo = neighborAssignInfos[nI];
				if( neighborAssignInfo->mSpaceName == pSpaceName )
				{
					spaceFound = true;
					neighborAssignInfo->mVisible = pVisible;
					if( neighborAssignInfo->mNeighborGroupAlg != nullptr ) delete neighborAssignInfo->mNeighborGroupAlg;
					neighborAssignInfo->mNeighborGroupAlg = new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode );
				}
			}
		}
		
		if( spaceFound == false ) // create new neighbor assign info
		{
			neighborAssignInfo = new NeighborAssignInfo(pParameterName, pSpaceName, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
			mAgentNeighborAssignRegistry[pParameterName].push_back(neighborAssignInfo);
		}
		
		Agent::assignNeighbors( pParameterName, pSpaceName, false, new space::NeighborGroupAlg( pNeighborRadius, 0, pReplaceNeighborMode ) );
		
		if(mAgentParameterList.contains(pParameterName))
		{
			unsigned int agentCount = mAgents.size();
			
			for(unsigned int i=0; i<agentCount; ++i)
			{
				mAgents[i]->assignNeighbors( pParameterName, pSpaceName, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
			}
		}
		else throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}


void
Swarm::assignSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode ) throw (Exception)
{
	try
	{
		NeighborAssignInfo* neighborAssignInfo;
		
		// check if there is already a registry available for this neighbor assignment
		bool spaceFound = false;
		if( mSwarmNeighborAssignRegistry.find( pParameterName ) != mSwarmNeighborAssignRegistry.end() )
		{
            std::vector<NeighborAssignInfo*> neighborAssignInfos = mSwarmNeighborAssignRegistry.at( pParameterName );
			int neighborAssignInfoCount = neighborAssignInfos.size();
			for( int nI=0; nI<neighborAssignInfoCount; ++nI )
			{
				neighborAssignInfo = neighborAssignInfos[nI];
				if( neighborAssignInfo->mSpaceName == pSpaceName )
				{
					spaceFound = true;
					neighborAssignInfo->mVisible = pVisible;
					if( neighborAssignInfo->mNeighborGroupAlg != NULL ) delete neighborAssignInfo->mNeighborGroupAlg;
					neighborAssignInfo->mNeighborGroupAlg = new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode );
				}
			}
		}
		
		if( spaceFound == false ) // create new neighbor assign info
		{
			neighborAssignInfo = new NeighborAssignInfo(pParameterName, pSpaceName, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
			mSwarmNeighborAssignRegistry[pParameterName].push_back(neighborAssignInfo);
		}
		
		if(mSwarmParameterList.contains(pParameterName))
		{
            std::shared_ptr<space::Space> space = space::SpaceManager::get().space(pSpaceName);
			Parameter* parameter = mSwarmParameterList.parameter(pParameterName);
			
			if( space->checkObject( parameter ) == true ) space->setObject( parameter, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode )  );
			else space->addObject( parameter, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
		}
		else throw Exception( "FLOCK ERROR: parameter " + pParameterName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception)
{
	try
	{
		if( Simulation::get().space().checkSpace(pSpaceName) == false ) throw Exception( "FLOCK ERROR: space " + pSpaceName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
        
		bool neighborsFound = false;
        
		if( mSwarmNeighborAssignRegistry.find( pParameterName ) != mSwarmNeighborAssignRegistry.end())
		{
			neighborsFound = true;
			removeSwarmNeighbors( pParameterName, pSpaceName );
		}
		if( mAgentNeighborAssignRegistry.find( pParameterName ) != mAgentNeighborAssignRegistry.end() )
		{
			neighborsFound = true;
			removeAgentNeighbors( pParameterName, pSpaceName );
		}
		
		if(neighborsFound == false) throw Exception( "FLOCK ERROR: no neighbors assigned to parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeSwarmNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception)
{
	try
	{
		if( mSwarmNeighborAssignRegistry.find( pParameterName ) == mSwarmNeighborAssignRegistry.end() ) throw Exception( "FLOCK ERROR: no neighbors assigned to parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		if( Simulation::get().space().checkSpace(pSpaceName) == false ) throw Exception( "FLOCK ERROR: space " + pSpaceName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
        
        std::vector< NeighborAssignInfo* > neighborAssigns = mSwarmNeighborAssignRegistry.at( pParameterName );
		int neighborAssignCount = neighborAssigns.size();
		
		for(int nI=neighborAssignCount - 1; nI>=0; --nI)
		{
			NeighborAssignInfo* neighborAssign = neighborAssigns[nI];
			
			if(neighborAssign->mSpaceName == pSpaceName)
			{
                std::shared_ptr<space::Space> space = Simulation::get().space().space( pSpaceName );
				
				if(mSwarmParameterList.contains(pParameterName))
				{
					Parameter* parameter = mSwarmParameterList.parameter(pParameterName);
					space->removeObject( parameter );
				}
                
				neighborAssigns.pop_back();
				delete neighborAssign;
			}
		}
		
		if(neighborAssigns.size() == 0)
		{
			mSwarmNeighborAssignRegistry.erase( pParameterName );
		}
		
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeAgentNeighbors( const std::string& pParameterName, const std::string& pSpaceName ) throw (Exception)
{
	try
	{
		if( mAgentNeighborAssignRegistry.find( pParameterName ) == mAgentNeighborAssignRegistry.end() ) throw Exception( "FLOCK ERROR: no neighbors assigned to parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		if( Simulation::get().space().checkSpace(pSpaceName) == false ) throw Exception( "FLOCK ERROR: space " + pSpaceName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
        
        std::vector< NeighborAssignInfo* > neighborAssigns = mAgentNeighborAssignRegistry.at( pParameterName );
		int neighborAssignCount = neighborAssigns.size();
		
		for(int nI=neighborAssignCount - 1; nI>=0; --nI)
		{
			NeighborAssignInfo* neighborAssign = neighborAssigns[nI];
			
			if(neighborAssign->mSpaceName == pSpaceName)
			{
                std::shared_ptr<space::Space> space = Simulation::get().space().space( pSpaceName );
				
				if(mAgentParameterList.contains(pParameterName))
				{
					// agent parameter (of swarm)
					Parameter* parameter = mAgentParameterList.parameter(pParameterName);
					space->removeObject( parameter );
                    
					// agent parameters (of agents)
					unsigned int agentCount = mAgents.size();
					
					for(unsigned int aI=0; aI<agentCount; ++aI)
					{
						Parameter* parameter = mAgents[aI]->parameter(pParameterName);
						space->removeObject( parameter );
					}
				}
                
				neighborAssigns.pop_back();
				delete neighborAssign;
			}
		}
		
		if(neighborAssigns.size() == 0)
		{
			mAgentNeighborAssignRegistry.erase( pParameterName );
		}
		
	}
	catch(Exception& e)
	{
		throw;
	}
}


unsigned int
Swarm::swarmBehaviorCount() const
{
	return mSwarmBehaviorList.behaviorCount();
}

bool
Swarm::checkSwarmBehavior(const std::string& pSwarmBehaviorName) const
{
	return mSwarmBehaviorList.contains(pSwarmBehaviorName);
}

Behavior*
Swarm::swarmBehavior(const std::string& pBehaviorName) throw (Exception)
{
	try
	{
		return mSwarmBehaviorList.behavior(pBehaviorName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Behavior*
Swarm::swarmBehavior(const std::string& pBehaviorName) const throw (Exception)
{
	try
	{
		return mSwarmBehaviorList.behavior(pBehaviorName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
Swarm::swarmBehavior(unsigned int pBehaviorIndex) throw (Exception)
{
	try
	{
		return mSwarmBehaviorList.behavior(pBehaviorIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

const Behavior*
Swarm::swarmBehavior(unsigned int pBehaviorIndex) const throw (Exception)
{
	try
	{
		return mSwarmBehaviorList.behavior(pBehaviorIndex);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmBehavior(Behavior* pBehavior) throw (Exception)
{
	try
	{
		mSwarmBehaviorList.addBehavior(pBehavior);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	try
	{
		addAgentBehavior(pBehaviorName, pBehavior);
	}
	catch(Exception& e)
	{
		throw;
	}
}


void
Swarm::addBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception)
{
	//std::cout << "Swarm::addBehavior( pBehaviorName " << pBehaviorName << ", pBehaviorPosition " << pBehaviorPosition << ", pBehavior " << pBehavior.className() << " ) begin\n";
	
	try
	{
		addAgentBehavior(pBehaviorName, pBehaviorPosition, pBehavior);
	}
	catch(Exception& e)
	{
		throw;
	}
	
	//std::cout << "Swarm::addBehavior( pBehaviorName " << pBehaviorName << ", pBehaviorPosition " << pBehaviorPosition << ", pBehavior " << pBehavior.className() << " ) end\n";
}

void
Swarm::addBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	//std::cout << "Swarm::addBehavior( pBehaviorName " << pBehaviorName << ", pSuccessorBehaviorName " << pSuccessorBehaviorName << ", pBehavior " << pBehavior.className() << " ) begin\n";
	
	try
	{
		addAgentBehavior(pBehaviorName, pSuccessorBehaviorName, pBehavior);
	}
	catch(Exception& e)
	{
		throw;
	}
	
	//std::cout << "Swarm::addBehavior( pBehaviorName " << pBehaviorName.toStdString() << ", pSuccessorBehaviorName " << pSuccessorBehaviorName.toStdString() << ", pBehavior " << pBehavior.className().toStdString() << " ) end\n";
	
}

void
Swarm::moveBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception)
{
	try
	{
		moveAgentBehavior(pBehaviorName, pBehaviorPosition);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::moveBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception)
{
	try
	{
		moveAgentBehavior(pBehaviorName, pSuccessorBehaviorName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	try
	{
		Agent::addBehavior( pBehaviorName, pBehavior );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->addBehavior( pBehaviorName, pBehavior );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception)
{
	try
	{
		Agent::addBehavior( pBehaviorName, pBehaviorPosition, pBehavior );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->addBehavior( pBehaviorName, pBehaviorPosition, pBehavior );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addAgentBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	// std::cout << "Swarm::addAgentBehavior( pBehaviorName " << pBehaviorName << ", pSuccessorBehaviorName " << pSuccessorBehaviorName << ", pBehavior " << pBehavior.className()<< " ) begin\n";
	
	try
	{
		//std::cout << "swarm agent\n";
		
		Agent::addBehavior( pBehaviorName, pSuccessorBehaviorName, pBehavior );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			//std::cout << "agent " << i << "\n";
			
			mAgents[i]->addBehavior( pBehaviorName, pSuccessorBehaviorName, pBehavior );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
	
	// std::cout << "Swarm::addAgentBehavior( pBehaviorName " << pBehaviorName.toStdString() << ", pSuccessorBehaviorName " << pSuccessorBehaviorName.toStdString() << ", pBehavior " << pBehavior.className().toStdString() << " ) end\n";
}

void
Swarm::moveAgentBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception)
{
	try
	{
		Agent::moveBehavior( pBehaviorName, pBehaviorPosition );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->moveBehavior( pBehaviorName, pBehaviorPosition );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::moveAgentBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception)
{
	try
	{
		Agent::moveBehavior( pBehaviorName, pSuccessorBehaviorName );
		
		unsigned int agentCount = mAgents.size();
		
		for(unsigned int i=0; i<agentCount; ++i)
		{
			mAgents[i]->moveBehavior( pBehaviorName, pSuccessorBehaviorName );
		}
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::addSwarmBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	//std::cout << "Swarm::addSwarmBehavior(const base::String& pBehaviorName, const Behavior& pBehavior) begin\n";

	if( mSwarmBehaviorList.contains(pBehaviorName) == true ) throw Exception( "FLOCK ERROR: swarm behavior name " + pBehaviorName + " already exists", __FILE__, __FUNCTION__, __LINE__ );

	
	Behavior* behavior;
	
	try
	{
		behavior = pBehavior.create(pBehaviorName, this);
		mSwarmBehaviorList.addBehavior(behavior);
	}
	catch(Exception& e)
	{
		std::cout << e << "\n";
        
		delete behavior;
		throw;
	}
	
	//std::cout << "Swarm::addSwarmBehavior(const base::String& pBehaviorName, const Behavior& pBehavior) end\n";
}

void
Swarm::addSwarmBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception)
{
	if( mSwarmBehaviorList.contains(pBehaviorName) == true ) throw Exception( "FLOCK ERROR: swarm behavior name " + pBehaviorName + " already exists", __FILE__, __FUNCTION__, __LINE__ );
	
	Behavior* behavior;
	
	try
	{
		behavior = pBehavior.create(pBehaviorName, this);
		mSwarmBehaviorList.addBehavior(behavior, pBehaviorPosition);
	}
	catch(Exception& e)
	{
		if(behavior != nullptr) delete behavior;
		
		throw;
	}
}

void
Swarm::addSwarmBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	try
	{
		int pSuccessorBehaviorPosition = mSwarmBehaviorList.behaviorIndex(pSuccessorBehaviorName);
		addSwarmBehavior(pBehaviorName, pSuccessorBehaviorPosition, pBehavior);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void
Swarm::moveSwarmBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception)
{
	try
	{
		int oldBehaviorPosition = mSwarmBehaviorList.behaviorIndex(pBehaviorName);
		mSwarmBehaviorList.moveBehavior(oldBehaviorPosition, pBehaviorPosition);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::moveSwarmBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception)
{
	try
	{
		int oldBehaviorPosition = mSwarmBehaviorList.behaviorIndex(pBehaviorName);
		int newBehaviorPosition = mSwarmBehaviorList.behaviorIndex(pBehaviorName);
		
		if( oldBehaviorPosition == newBehaviorPosition ) return;
		else if( oldBehaviorPosition < newBehaviorPosition ) mSwarmBehaviorList.moveBehavior(oldBehaviorPosition, newBehaviorPosition - 1 );
		else mSwarmBehaviorList.moveBehavior(oldBehaviorPosition, newBehaviorPosition );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::removeBehavior(const std::string& pBehaviorName) throw (Exception)
{
	try
	{
		removeAgentBehavior(pBehaviorName);
	}
	catch(Exception& e)
	{
		throw;
	}
}


void
Swarm::removeAgentBehavior(const std::string& pBehaviorName) throw (Exception)
{
	//std::cout << "remove behavior " << pBehaviorName.toStdString() << " begin\n";
    
	try
	{
		// remove behaviors
		unsigned int agentCount = mAgents.size();
		for(unsigned int agentNr=0; agentNr < agentCount; ++agentNr) mAgents[agentNr]->removeBehavior( pBehaviorName );
		Agent::removeBehavior( pBehaviorName );
	}
	catch(Exception& e)
	{
		throw;
	}
	
	//std::cout << "remove behavior " << pBehaviorName.toStdString() << " end\n";
}

void
Swarm::removeSwarmBehavior(const std::string& pBehaviorName) throw (Exception)
{
	try
	{
		mBehaviorList.removeBehavior(pBehaviorName);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Swarm::act()
{
	// perform agent behaviors
	Agent::act();
	
	//std::cout << "agent " << mName.toStdString() << " act\n";
	
	if( mSwarmParameterList.parameter(0)->value() == 0.0 ) return;
    
	// update agent parameter indices of the behaviors before performing swarm behaviors
	SwarmBehavior* swarmBehavior;
	unsigned int bC = mSwarmBehaviorList.behaviorCount();
	for(unsigned bI=0; bI < bC; ++bI)
	{
		swarmBehavior = dynamic_cast<SwarmBehavior*>( mSwarmBehaviorList.behavior(bI) );
		if(swarmBehavior != nullptr) swarmBehavior->createAgentParameterIndices();
	}
    
	// perform swarm behaviors
	mSwarmBehaviorList.act();
}

void 
Swarm::flush()
{
	mSwarmParameterList.flush();
	
	unsigned int aC = mAgents.size();
	for(unsigned int aI=0; aI<aC; ++aI) mAgents[aI]->flush();
}

void 
Swarm::notify()
{}

Swarm::operator std::string() const
{
    return info();
}

std::string
Swarm::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << "\n";
    ss << " agentCount: " << mAgents.size() << "\n";

	if(pPropagationLevel != 0) 
	{
        ss << Agent::info(pPropagationLevel - 1);
	}
	
	if(pPropagationLevel < 0 || pPropagationLevel >= 2) 
	{
		for( unsigned int i=0; i<mAgents.size(); ++i ) ss << mAgents[i]->info(pPropagationLevel - 2);
	}
    
	return ss.str();
}

