/** \file dab_flock_orbit_behavior.cpp
 */

#include "dab_flock_orbit_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"
#include "ofVectorMath.h"
#include "ofMath.h"

using namespace dab;
using namespace dab::flock;

OrbitBehavior::OrbitBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "OrbitBehavior";
}

OrbitBehavior::OrbitBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "OrbitBehavior";
	
	if( mInputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mInputParameters[1]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != 3 ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
    if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPositionPar = mInputParameters[0];
	mVelocityPar = mInputParameters[1];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mFrequencyPar = createInternalParameter("frequency", { 2.0f });
	mPhasePar = createInternalParameter("phase", { 0.0f });
	mMinDistPar = createInternalParameter("minDist", { 0.0f });
	mMaxDistPar = createInternalParameter("maxDist", { 0.5f });
	mRadAmountPar = createInternalParameter("radAmount", { 3.0f });
	mTanAmountPar = createInternalParameter("tanAmount", { 6.0f });
	mAmountPar = createInternalParameter("amount", { 0.1f });
	
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
    
	// remaining stuff
	unsigned int dim = mForcePar->dim();
	mAvgDirection.resize(dim, 1);
	mTmpForce.resize(dim, 1);
}

OrbitBehavior::~OrbitBehavior()
{}

Behavior*
OrbitBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new OrbitBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new OrbitBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
OrbitBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new OrbitBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
OrbitBehavior::act()
{
	//std::cout << "OrbitBehavior::act() begin\n";
    
	//std::cout << "OrbitBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
    Eigen::VectorXf& position = mPositionPar->values();
	Eigen::VectorXf& velocity = mVelocityPar->values();
	
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& frequency = mFrequencyPar->value();
	float& phase = mPhasePar->value();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& radAmount = mRadAmountPar->value();
	float& tanAmount = mTanAmountPar->value();
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
		
		//std::cout << "dist " << distance << " sin " <<  10.0 * sin( distance * 10.0 * math::Math::PI ) << "\n";
		
		mNormDirection = direction;
		mNormDirection.normalize();
		mNormVelocity = velocity;
		mNormVelocity.normalize();
		
		mCrossVec = mNormDirection.cross( mNormVelocity );
		mCrossVec.normalize();
		
		if( mCrossVec[0] < 0 ) mCrossVec2 = mNormDirection.cross( mCrossVec );
		else mCrossVec2 = mCrossVec.cross( mNormDirection );
		mCrossVec2.normalize();
		
		avgDirection += mNormDirection * radAmount * -cos( phase + distance * frequency * glm::pi<float>() );
		avgDirection -= mCrossVec2 * tanAmount;
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
	
	//std::cout << "OrbitBehavior::act() end\n";
}