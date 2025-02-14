/** \file dab_flock_circular_behavior.cpp
 */

#include "dab_flock_circular_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

CircularBehavior::CircularBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "CircularBehavior";
}

CircularBehavior::CircularBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
, mNormZVec( 0.0, 0.0, 1.0 )
{
	mClassName = "CircularBehavior";
	
	if( mInputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    if( mInputParameters[0]->dim() != 3 )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must be 3 ", __FILE__, __FUNCTION__, __LINE__ );
    if( mInputParameters[1]->dim() != 3 )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()) + " must be 3 ", __FILE__, __FUNCTION__, __LINE__ );
    if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPosition = mInputParameters[0];
    mVelocity = mInputParameters[1];
	
	// output parameter
	mForce = mOutputParameters[0];
	
	// create internal parameters
    mCenterPositionPar = createInternalParameter("centerPosition", { 0.0f, 0.0f, 0.0f });
	mInnerRadiusPar = createInternalParameter("innerRadius", { 2.0f });
	mOuterRadiusPar = createInternalParameter("outerRadius", { 3.0f });
    mOrtAmountPar = createInternalParameter("ortAmount", {0.1f});
    mTanAmountPar = createInternalParameter("tanAmount", {0.1f});
}

CircularBehavior::~CircularBehavior()
{}

Behavior*
CircularBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new CircularBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new CircularBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
CircularBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new CircularBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
CircularBehavior::act()
{
	//std::cout << "CopyBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
	
    Eigen::VectorXf& position = mPosition->values();
    Eigen::VectorXf& velocity = mVelocity->values();
	Eigen::VectorXf& force = mForce->backupValues();
    Eigen::VectorXf& centerPosition = mCenterPositionPar->values();
	
	float& innerRadius = mInnerRadiusPar->value();
	float& outerRadius = mOuterRadiusPar->value();
	float& ortAmount = mOrtAmountPar->value();
    float& tanAmount = mTanAmountPar->value();
    
    // calculate tangential force
	mCenterVec = centerPosition - position;
    
    // calculate radial force
	float posLength = mCenterVec.norm();
	mCenterVec.normalize();
    
    mNormVelocity = velocity;
    mNormVelocity.normalize();
    mCrossVec = mCenterVec.cross(mNormVelocity);
    mCrossVec.normalize();
    mNormTangentVec = mCrossVec.cross(mCenterVec);
    
	force += mNormTangentVec * tanAmount;
	
	if(posLength < innerRadius)
	{
		force += mCenterVec * (posLength - innerRadius) * ortAmount;
	}
	else if(posLength > outerRadius)
	{
		force += mCenterVec * (posLength - outerRadius) * ortAmount;
	}
}