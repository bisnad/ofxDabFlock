/** \file dab_flock_alignment_behavior.cpp
 */

#include "dab_flock_alignment_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::flock;

AlignmentBehavior::AlignmentBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "AlignmentBehavior";
}

AlignmentBehavior::AlignmentBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "AlignmentBehavior";
	
	if( mInputParameters.size() < 2 && ( mInputParameters.size() < 1 && mNeighborInputParameterNames.size() > 0 ) ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters.size() > 1 && mInputParameters[0]->dim() != mInputParameters[1]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPositionPar = mInputParameters[0];
	
	if( mNeighborInputParameterNames.size() > 0 ) // new parameter version: position@positionspace:velocity
	{
		mVelocityPar = mAgent->parameter( mNeighborInputParameterNames[0] );
	}
	else // old parameter version: position:positionspace velocity
	{
		mVelocityPar = mInputParameters[1];
	}
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f } );
	mMaxDistPar = createInternalParameter("maxDist", { 0.5f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
	
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
	
	// remaining stuff
	unsigned int dim = mForcePar->dim();
	mAvgVelocity.resize(dim, 1);
	mTmpForce.resize(dim, 1);
}

AlignmentBehavior::~AlignmentBehavior()
{}

Behavior*
AlignmentBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new AlignmentBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new AlignmentBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
AlignmentBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new AlignmentBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
AlignmentBehavior::act()
{
    //std::cout << "AlignmentBehavior::act()\n";
    
	//std::cout << "AlignmentBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
	
	Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& velocity = mVelocityPar->values();
	const std::string& velocityName = mVelocityPar->name();
	
	
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
	Eigen::VectorXf& avgVelocity = mAvgVelocity;
	Eigen::VectorXf& tmpForce = mTmpForce;
	
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	unsigned int neighborCount = 0;
	
	float distance;
	
	avgVelocity.setConstant(0.0);
    
    //std::cout << "totalNeighborCount " << totalNeighborCount << "\n";
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		distance = positionNeighbors.distance(i);
		
		if(minDist > 0.0 && distance < minDist) continue;
		if(maxDist > 0.0 && distance > maxDist) continue;
		
        Eigen::VectorXf& neighborVelocity = static_cast<Parameter*>(positionNeighbors.neighbor(i))->agent()->parameter( velocityName )->values();
		
		//std::cout << "agent " << mAgent->name().toStdString() << " pos " << position << " neighbor " << ( static_cast<Parameter*>(positionNeighbors.neighbor(i))->agent()->name().toStdString() )  << " npos " << ( static_cast<Parameter*>(positionNeighbors.neighbor(i))->agent()->parameter( mPositionPar->name() ).values() ) << " distance " << distance << "\n";
		
		avgVelocity += neighborVelocity;
		neighborCount++;
	}
	
	if(neighborCount == 0) return;
	
	avgVelocity /= static_cast<float>(neighborCount);
	
	tmpForce = avgVelocity - velocity;
	tmpForce *= amount;
	
	force += tmpForce;
}