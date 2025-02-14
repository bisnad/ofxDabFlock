/** \file dab_flock_damping_behavior.cpp
 */

#include "dab_flock_damping_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

DampingBehavior::DampingBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "DampingBehavior";
}

DampingBehavior::DampingBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "DampingBehavior";
	
	if(mInputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mVelocityPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mPrefVelocityPar = createInternalParameter("prefVelocity", { 0.0f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
	
	// remaining stuff
	unsigned int dim = mForcePar->dim();
	mTmpForce.resize(dim, 1);
}

DampingBehavior::~DampingBehavior()
{}

Behavior*
DampingBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new DampingBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new DampingBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
DampingBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new DampingBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
DampingBehavior::act()
{
	//std::cout << "DampingBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& velocity = mVelocityPar->values();
	float& prefVelocity = mPrefVelocityPar->value();
	float& amount = mAmountPar->value();
	Eigen::VectorXf& force = mForcePar->backupValues();
	Eigen::VectorXf& tmpForce = mTmpForce;
    
	float velMag = velocity.norm();
	
	if(velMag == 0) return;
	
	float velDiff = prefVelocity - velMag;
    
	tmpForce = velocity;
	tmpForce.normalize();
	tmpForce *= velDiff;
	tmpForce *= amount;
	
	force += tmpForce;
    
	//std::cout << "DampingBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	//assert(std::isnan(force[0]) == false && "isNan");
}