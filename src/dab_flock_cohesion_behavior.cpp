/** \file dab_flock_cohesion_behavior.cpp
 */

#include "dab_flock_cohesion_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::flock;

CohesionBehavior::CohesionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "CohesionBehavior";
}

CohesionBehavior::CohesionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "CohesionBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );

	
	// input parameter
	mPositionPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f } );
	mMaxDistPar = createInternalParameter("maxDist", { 0.5f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
	
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
	
	//std::cout << "cohesion neighbors: " << mPositionNeighbors->name().toStdString() << "\n";
    
	// remaining stuff
	unsigned int dim = mForcePar->dim();
    mAvgDirection.resize(dim, 1);
	mTmpForce.resize(dim, 1);
}

CohesionBehavior::~CohesionBehavior()
{}

Behavior*
CohesionBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new CohesionBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new CohesionBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
CohesionBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new CohesionBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
CohesionBehavior::act()
{
	//std::cout << "CohesionBehavior::act() begin\n";
    
	//std::cout << "CohesionBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
    
	Eigen::VectorXf& avgDirection = mAvgDirection;
	Eigen::VectorXf& tmpForce = mTmpForce;
    
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	
	unsigned int neighborCount = 0;
    
	//std::cout << "cohesion agent " << mAgent->name().toStdString() << " pos " << position << " neighborCount " << totalNeighborCount << "\n";
	
	if(totalNeighborCount == 0) return;
	
	float distance;
	float avgDistance;
	float scale;
	
	avgDirection.setConstant(0.0);
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		const Eigen::VectorXf& direction = positionNeighbors.direction(i);
		distance = positionNeighbors.distance(i);
		
		//std::cout << "agent " << mAgent->name().toStdString() << " pos " << position << " neighbor pos " << positionNeighbors.neighbor(i)->position() << " distance " << positionNeighbors.distance(i) << " minDist " << minDist << " maxDist " << maxDist << "\n";
		
		//std::cout << " neighbor direction " << direction << "\n";
		
		if(minDist > 0.0 && distance < minDist) continue;
		if(maxDist > 0.0 && distance > maxDist) continue;
		
		avgDirection += direction;
		neighborCount++;
        
		//std::cout << "agent " << mAgent->name().toStdString() << " light " << ( static_cast<Parameter*>( positionNeighbors.neighbor(i) ) )->agent()->name().toStdString() << " " << positionNeighbors.neighbor(i)->position()   << "\n";
	}
	
	if(neighborCount == 0) return;
	
	avgDirection /= static_cast<float>(neighborCount);
	
	avgDistance = avgDirection.norm();
	if(maxDist > 0.0 && minDist > 0.0) scale = (avgDistance - minDist) / (maxDist - minDist);
	else scale = 1.0;
	
	tmpForce = avgDirection;
	
	tmpForce.normalize();
	tmpForce *= scale;
	tmpForce *= amount;

	//std::cout << "force " << tmpForce << "\n";
	
	force += tmpForce;
	
	//std::cout << "CohesionBehavior::act() end\n";
}