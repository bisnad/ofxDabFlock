/** \file dab_flock_neighbor_parameter_store_behavior.cpp
 */

#include "dab_flock_neighbor_parameter_store_behavior.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbors.h"
#include "dab_space_neighbor_relation.h"
#include <cfloat>

using namespace dab;
using namespace dab::flock;

NeighborParameterStoreBehavior::NeighborParameterStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborParameterStoreBehavior";
}

NeighborParameterStoreBehavior::NeighborParameterStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "NeighborParameterStoreBehavior";
	
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mNeighborInputParameterNames.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters names supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mNeighborParameterName = mNeighborInputParameterNames[0];
	
	// neighbor parameter
	mNeighborStorePar = mOutputParameters[0];
	
	// input neighbor groups
	mNeighbors = mInputNeighborGroups[0];
}

NeighborParameterStoreBehavior::~NeighborParameterStoreBehavior()
{}

Behavior*
NeighborParameterStoreBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new NeighborParameterStoreBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new NeighborParameterStoreBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
NeighborParameterStoreBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new NeighborParameterStoreBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
NeighborParameterStoreBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
	
	try
	{
		space::NeighborGroup& neighbors = *mNeighbors;
        Eigen::VectorXf& neighborStoreValues = mNeighborStorePar->backupValues();
		unsigned int neighborStoreParDim = mNeighborStorePar->dim();
		unsigned int neighborParDim;
		
		space::SpaceNeighborRelation* neighborRelation;
		unsigned int neighborCount = neighbors.neighborCount();
		
		for(unsigned int d=0; d<neighborStoreParDim; ++d) neighborStoreValues[d] = FLT_MIN;
		
		int storeIndex = 0;
		for(unsigned int neighborNr=0; neighborNr<neighborCount && storeIndex<neighborStoreParDim; ++neighborNr)
		{
			Eigen::VectorXf& neighborParValues = static_cast<Parameter*>(neighbors.neighbor(neighborNr))->agent()->parameter( mNeighborParameterName )->values();
			
			neighborParDim = neighborParValues.rows();
			
			for(unsigned int d=0; d<neighborParDim && storeIndex<neighborStoreParDim; ++d)
			{
				neighborStoreValues[storeIndex++] = neighborParValues[d];
			}
		}
	}
	catch(Exception& e)
	{
		std::cout << e << "\n";
	}
	
	//std::cout << "agent " << mAgent->name().toStdString() << " neighborStore " << *mNeighborStorePar << "\n";
}