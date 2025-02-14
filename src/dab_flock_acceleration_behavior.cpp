/** \file dab_flock_acceleration_behavior.cpp
 */

#include "dab_flock_acceleration_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

AccelerationBehavior::AccelerationBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "AccelerationBehavior";
}

AccelerationBehavior::AccelerationBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "AccelerationBehavior";
	
	if( mInputParameters.size() < 3 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size() ) + " input parameters supplied, " + std::to_string(3) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__);
	if( mInputParameters[1]->dim() != mOutputParameters[0]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()) + " must match output parameter " + mOutputParameters[1]->name() + " dim " + std::to_string(mOutputParameters[1]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[2]->dim() != mOutputParameters[0]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[2]->name() + " dim " + std::to_string(mInputParameters[2]->dim()) + " must match output parameter " + mOutputParameters[1]->name() + " dim " + std::to_string(mOutputParameters[1]->dim()), __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameters
	mMassPar = mInputParameters[0];
	mVelocityPar = mInputParameters[1];
	mForcePar = mInputParameters[2];
    
	// output parameters
	mAcceleration = mOutputParameters[0];
	
	// create internal parameters
    mMaxLinearAccelerationPar = createInternalParameter("maxLinearAcceleration", mOutputParameters[0]->dim(), 0.5);
    mMaxAngularAccelerationPar = createInternalParameter("maxAngularAcceleration", mOutputParameters[0]->dim(), 0.5);
    
	// remaining stuff
	unsigned int dim = mForcePar->dim();
    mPrefAccel.resize(dim, 1);
    mLinearAccel.resize(dim, 1);
    mAngularAccel.resize(dim, 1);
}

AccelerationBehavior::~AccelerationBehavior()
{}

Behavior*
AccelerationBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new AccelerationBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new AccelerationBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
AccelerationBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new AccelerationBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
AccelerationBehavior::act()
{
	//std::cout << "AccelerationBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	
	if(mActivePar->value() <= 0.0) return;
    
	float& mass = mMassPar->value();
	Eigen::VectorXf& velocity = mVelocityPar->values();
	Eigen::VectorXf& force = mForcePar->values();
	float& maxLinearAcceleration = mMaxLinearAccelerationPar->value();
	float& maxAngularAcceleration = mMaxAngularAccelerationPar->value();
	Eigen::VectorXf& acceleration = mAcceleration->backupValues();
	
	Eigen::VectorXf& prefAccel = mPrefAccel;
	Eigen::VectorXf& linearAccel = mLinearAccel;
	Eigen::VectorXf& angularAccel = mAngularAccel;
    
	prefAccel = force / mass;
	linearAccel = velocity;
	linearAccel.normalize();
    
	float dotProd = prefAccel.dot(linearAccel);
	linearAccel *= dotProd;
	angularAccel = prefAccel - linearAccel;

	linearAccel = linearAccel.cwiseMin(maxLinearAcceleration).cwiseMax(-maxLinearAcceleration);
	angularAccel = angularAccel.cwiseMin(maxAngularAcceleration).cwiseMax(-maxAngularAcceleration);

	acceleration = (linearAccel + angularAccel);
    
	//std::cout << "AccelerationBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
    
	//assert(std::isnan(acceleration[0]) == false && "isNan");
}