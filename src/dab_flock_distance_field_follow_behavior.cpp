/** \file dab_flock_distance_field_follow_behavior.cpp
 */

#include "dab_flock_distance_field_follow_behavior.h"
#include <float.h>

using namespace dab;
using namespace dab::flock;

DistanceFieldFollowBehavior::DistanceFieldFollowBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "DistanceFieldFollowBehavior";
}

DistanceFieldFollowBehavior::DistanceFieldFollowBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "DistanceFieldFollowBehavior";
	
	if(mInputParameters.size() < 2) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mInputParameters[0]->dim() != mInputParameters[1]->dim() || mInputParameters[0]->dim() != mOutputParameters[0]->dim()) throw Exception( "FLOCK ERROR: dimension of input and output parameter not equal", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameters
	mPositionPar = mInputParameters[0];
	mVelocityPar = mInputParameters[1];
	
	// output parameters
	mForcePar = mOutputParameters[0];
	
	// internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f } );
	mMaxDistPar = createInternalParameter("maxDist", { 1.0f } );
	mOrtAmountPar = createInternalParameter("ortAmount", { 1.0f } );
	mTanAmountPar = createInternalParameter("tanAmount", { 1.0f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
    
	// input neighbor groups
	mGridValues = mInputNeighborGroups[0];
    
    // other stuff
	unsigned int dim = mPositionPar->dim();
	mOrtDirection.resize(dim,1);
	mTangDirection.resize(dim,1);
	mDistanceVector.resize(dim,1);
	mNormVelocity.resize(dim,1);
}

DistanceFieldFollowBehavior::~DistanceFieldFollowBehavior()
{}

Behavior*
DistanceFieldFollowBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new DistanceFieldFollowBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new DistanceFieldFollowBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
DistanceFieldFollowBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new DistanceFieldFollowBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
DistanceFieldFollowBehavior::act()
{
	//std::cout << "DistanceFieldFollowBehavior::act() begin\n";
    
	if(mActivePar->value() <= 0.0) return;
	
	// input parameters
    Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mGridValues;
    Eigen::VectorXf& velocity = mVelocityPar->values();
    
	// output parameter
	Eigen::VectorXf& force = mForcePar->backupValues();
	
	// internal parameters
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& ortAmount = mOrtAmountPar->value();
	float& tanAmount = mTanAmountPar->value();
	float& amount = mAmountPar->value();
	
	float distanceVectorLength;
	float minDistanceVectorLength = FLT_MAX;
    
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	unsigned int neighborCount = 0;
	
	if(totalNeighborCount == 0) return;
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		//mDistanceVector = positionNeighbors.neighbor(i)->position();
		mDistanceVector = positionNeighbors.value(i);
		distanceVectorLength = mDistanceVector.norm();
		
		//std::cout << "dist vec " << mDistanceVector << " length " << distanceVectorLength << "\n";
        
		if(minDist > 0.0 && distanceVectorLength < minDist) continue;
		if(maxDist > 0.0 && distanceVectorLength > maxDist) continue;
		
		if(minDistanceVectorLength > distanceVectorLength)
		{
			minDistanceVectorLength = distanceVectorLength;
			mOrtDirection = mDistanceVector;
		}
		
		neighborCount++;
	}
	
	if(neighborCount == 0) return;
	
	float ort2tanRatio = (minDistanceVectorLength - minDist) / (maxDist - minDist);
	
	mOrtDirection.normalize();
	mNormVelocity = velocity;
	mNormVelocity.normalize();
	
	mTangDirection = mNormVelocity - mOrtDirection * mOrtDirection.dot(mNormVelocity);
	
	ort2tanRatio = sqrt(ort2tanRatio);
    
	force += ( mOrtDirection * ortAmount * ort2tanRatio + mTangDirection * tanAmount * (1.0 - ort2tanRatio) ) * amount;
	
	//std::cout << "force " << force << "\n";
	
	//std::cout << "DistanceFieldFollowBehavior::act() end\n";
}