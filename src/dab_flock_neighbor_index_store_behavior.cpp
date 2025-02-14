/** \file dab_flock_neighbor_index_store_behavior.cpp
 */

#include "dab_flock_neighbor_index_store_behavior.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbors.h"
#include "dab_space_neighbor_relation.h"
#include <cfloat>

using namespace dab;
using namespace dab::flock;

NeighborIndexStoreBehavior::NeighborIndexStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborIndexStoreBehavior";
}

NeighborIndexStoreBehavior::NeighborIndexStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborIndexStoreBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mParameter = mInputParameters[0];
    
	// neighbor parameter
	mNeighborStorePar = mOutputParameters[0];
	
	// input neighbor groups
	mNeighbors = mInputNeighborGroups[0];
}

NeighborIndexStoreBehavior::~NeighborIndexStoreBehavior()
{}

Behavior*
NeighborIndexStoreBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new NeighborIndexStoreBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new NeighborIndexStoreBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
NeighborIndexStoreBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new NeighborIndexStoreBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
NeighborIndexStoreBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
    
	space::NeighborGroup& neighbors = *mNeighbors;
	unsigned int neighborCount = neighbors.neighborCount();
    Eigen::VectorXf& neighborStoreValues = mNeighborStorePar->backupValues();
	unsigned int neighborStoreParDim = mNeighborStorePar->dim();
	
	int storeIndex = 0;
	neighborStoreValues[storeIndex++] = static_cast<float>( mAgent->index() );
	
	for(unsigned int neighborNr=0; neighborNr<neighborCount && storeIndex<neighborStoreParDim; ++neighborNr)
	{
		neighborStoreValues[storeIndex++] = static_cast<float>( ( static_cast<Parameter*>( neighbors[neighborNr]->neighbor() ) )->agent()->index() );
	}
	for(; storeIndex<neighborStoreParDim; )
	{
		neighborStoreValues[storeIndex++] = FLT_MIN;
	}
	
	//std::cout << "agent " << mAgent->name().toStdString() << " neighborStore " << *mNeighborStorePar << "\n";
}