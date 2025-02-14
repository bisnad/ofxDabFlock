/** \file dab_flock_add_agents_event.cpp
 */

#include "dab_flock_add_agents_event.h"

using namespace dab;
using namespace dab::flock;

AddAgentsEvent::AddAgentsEvent(const std::string& pSwarmName, unsigned int pAgentCount)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mAgentCount(pAgentCount)
{}

AddAgentsEvent::AddAgentsEvent(double pTime, const std::string& pSwarmName, unsigned int pAgentCount)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mAgentCount(pAgentCount)
{}

AddAgentsEvent::AddAgentsEvent(const AddAgentsEvent& pEvent)
: event::Event( pEvent )
, mSwarmName( pEvent.mSwarmName )
, mAgentCount( pEvent.mAgentCount )
{}

AddAgentsEvent::AddAgentsEvent(double pTime, const AddAgentsEvent& pEvent)
: event::Event( pTime, pEvent )
, mSwarmName( pEvent.mSwarmName )
, mAgentCount( pEvent.mAgentCount )
{}

AddAgentsEvent::~AddAgentsEvent()
{}

event::Event*
AddAgentsEvent::copy() const
{
	return new AddAgentsEvent( *this );
}

event::Event*
AddAgentsEvent::copy( double pTime ) const
{
	return new AddAgentsEvent( pTime, *this );
}

void
AddAgentsEvent::execute() throw (Exception)
{
	//std::cout << "AddAgentsEvent::execute() begin\n";
	
	mStarted = true;
	mFinished = false;
    
	try
	{
		Swarm* swarm = Simulation::get().swarm(mSwarmName);
		swarm->addAgents(mAgentCount);
	}
	catch(Exception& e)
	{
		Simulation::get().exceptionReport( e );
	}
    
	mStarted = false;
	mFinished = true;
	
	//std::cout << "AddAgentsEvent::execute() end\n";
}