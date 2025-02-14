/** \file dab_flock_neighbor_store_behavior.cpp
 */

#include "dab_flock_neighbor_store_behavior.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::flock;

NeighborStoreBehavior::NeighborStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborStoreBehavior";
}

NeighborStoreBehavior::NeighborStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborStoreBehavior";
	
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

NeighborStoreBehavior::~NeighborStoreBehavior()
{}

Behavior*
NeighborStoreBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != nullptr) return new NeighborStoreBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new NeighborStoreBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
NeighborStoreBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new NeighborStoreBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
NeighborStoreBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
    
	space::NeighborGroup& neighbors = *mNeighbors;
    Eigen::VectorXf& neighborStoreValues = mNeighborStorePar->backupValues();
	unsigned int neighborStoreParDim = mNeighborStorePar->dim();
	
	space::SpaceNeighborRelation* neighborRelation;
	unsigned int neighborCount = neighbors.neighborCount();
	
	// reset neighbor store values
    neighborStoreValues.setConstant(-1.0);
	
	unsigned int storeIndex = 0;
	neighborStoreValues[storeIndex++] = mAgent->index();
	
	for(unsigned int neighborNr=0; neighborNr<neighborCount && storeIndex<neighborStoreParDim; ++neighborNr)
	{
		neighborRelation = neighbors[neighborNr];
        
		neighborStoreValues[storeIndex++] = ( static_cast<Parameter*>( neighborRelation->neighbor() ) )->agent()->index();
		neighborStoreValues[storeIndex++] = neighborRelation->distance();
	}
	
	//std::cout << "agent " << mAgent->name().toStdString() << " neighborStore " << *mNeighborStorePar << "\n";
}