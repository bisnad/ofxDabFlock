/** \file dab_flock_neighbor_direction_store_behavior.cpp
 */

#include "dab_flock_neighbor_direction_store_behavior.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbors.h"
#include "dab_space_neighbor_relation.h"
#include <cfloat>

using namespace dab;
using namespace dab::flock;

NeighborDirectionStoreBehavior::NeighborDirectionStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborDirectionStoreBehavior";
}

NeighborDirectionStoreBehavior::NeighborDirectionStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborDirectionStoreBehavior";
	
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

NeighborDirectionStoreBehavior::~NeighborDirectionStoreBehavior()
{}

Behavior*
NeighborDirectionStoreBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new NeighborDirectionStoreBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new NeighborDirectionStoreBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
NeighborDirectionStoreBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new NeighborDirectionStoreBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
NeighborDirectionStoreBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
    
	space::NeighborGroup& neighbors = *mNeighbors;
    Eigen::VectorXf& neighborStoreValues = mNeighborStorePar->backupValues();
	unsigned int neighborStoreParDim = mNeighborStorePar->dim();
	unsigned int parameterDim = mParameter->dim();
	
	if(neighborStoreParDim < parameterDim) return;
	
	space::SpaceNeighborRelation* neighborRelation;
	unsigned int neighborCount = neighbors.neighborCount();
	
	int storeIndex = 0;
	for(unsigned int d=0; d<parameterDim; ++d) neighborStoreValues[storeIndex++] = 0.0;
	
	for(unsigned int neighborNr=0; neighborNr<neighborCount && storeIndex<neighborStoreParDim; ++neighborNr)
	{
		neighborRelation = neighbors[neighborNr];
        
		const Eigen::VectorXf& direction = neighborRelation->direction();
		
		for(unsigned int d=0; d<parameterDim; ++d)
		{
			neighborStoreValues[storeIndex++] = direction[d];
		}
	}
	for(; storeIndex<neighborStoreParDim;)
	{
		for(unsigned int d=0; d<parameterDim; ++d)
		{
			neighborStoreValues[storeIndex++] = FLT_MIN;
		}
	}
	
	//std::cout << "agent " << mAgent->name().toStdString() << " neighborStore " << *mNeighborStorePar << "\n";
}