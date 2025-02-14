/** \file dab_flock_parameter_mag_behavior.cpp
 */

#include "dab_flock_parameter_mag_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterMagBehavior::ParameterMagBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterMagBehavior";
}

ParameterMagBehavior::ParameterMagBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterMagBehavior";
	
	if(mInputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    if(mOutputParameters[0]->dim() != 1 )  throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()) + " must be " + std::to_string(1), __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameters
	mParameter = mInputParameters[0];
	
	// output parameters
	mMagParameter = mOutputParameters[0];
}

ParameterMagBehavior::~ParameterMagBehavior()
{}

Behavior*
ParameterMagBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ParameterMagBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterMagBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterMagBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterMagBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ParameterMagBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
	
    Eigen::VectorXf& parValues = mParameter->values();
	float& parMag = mMagParameter->backupValue();
	
	parMag = parValues.norm();
	
}