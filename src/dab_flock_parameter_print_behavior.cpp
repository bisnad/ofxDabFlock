/** \file dab_flock_parameter_print_behavior.cpp
 */

#include "dab_flock_parameter_print_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterPrintBehavior::ParameterPrintBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterPrintBehavior";
}

ParameterPrintBehavior::ParameterPrintBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterPrintBehavior";
	
	if(mInputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameters
	mParameter = mInputParameters[0];
}

ParameterPrintBehavior::~ParameterPrintBehavior()
{}

Behavior*
ParameterPrintBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ParameterPrintBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterPrintBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterPrintBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterPrintBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ParameterPrintBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
	
	std::cout << "agent " << mAgent->name()<< " par " << mParameter->name() << " values " << mParameter->values() << " bValues " << mParameter->backupValues() << "\n";
}