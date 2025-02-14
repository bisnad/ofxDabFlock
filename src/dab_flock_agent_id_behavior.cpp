/** \file dab_flock_agent_id_behavior.cpp
 */

#include "dab_flock_agent_id_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

AgentIdBehavior::AgentIdBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "AgentIdBehavior";
}

AgentIdBehavior::AgentIdBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "AgentIdBehavior";
	
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters[0]->dim() != 1 )  throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()) + " must match be " + std::to_string(1), __FILE__, __FUNCTION__, __LINE__ );
	
	// output parameter
	mIdPar = mOutputParameters[0];
}

AgentIdBehavior::~AgentIdBehavior()
{}

Behavior*
AgentIdBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new AgentIdBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new AgentIdBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
AgentIdBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new AgentIdBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
AgentIdBehavior::act()
{
	//std::cout << "AgentIdBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
	
	mIdPar->set( static_cast<float>( mAgent->index() ) );
}