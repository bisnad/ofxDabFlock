/** \file dab_flock_remove_agents_event.cpp
 */

#include "dab_flock_remove_agents_event.h"

using namespace dab;
using namespace dab::flock;

RemoveAgentsEvent::RemoveAgentsEvent(const std::string& pSwarmName)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( -1 )
, mAgentCount(-1)
{}

RemoveAgentsEvent::RemoveAgentsEvent(const std::string& pSwarmName, int pAgentCount)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( -1 )
, mAgentCount(pAgentCount)
{}

RemoveAgentsEvent::RemoveAgentsEvent(const std::string& pSwarmName, int pStartAgentIndex, int pEndAgentIndex)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( pStartAgentIndex )
, mAgentCount( pEndAgentIndex - pStartAgentIndex )
{}

RemoveAgentsEvent::RemoveAgentsEvent(double pTime, const std::string& pSwarmName)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( -1 )
, mAgentCount(-1)
{}

RemoveAgentsEvent::RemoveAgentsEvent(double pTime, const std::string& pSwarmName, int pAgentCount)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( -1 )
, mAgentCount(pAgentCount)
{}

RemoveAgentsEvent::RemoveAgentsEvent(double pTime, const std::string& pSwarmName, int pStartAgentIndex, int pEndAgentIndex )
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mStartAgentIndex( pStartAgentIndex )
, mAgentCount( pEndAgentIndex - pStartAgentIndex )
{}

RemoveAgentsEvent::RemoveAgentsEvent( const RemoveAgentsEvent& pEvent )
: event::Event( pEvent )
, mSwarmName( pEvent.mSwarmName )
, mStartAgentIndex( pEvent.mStartAgentIndex )
, mAgentCount( pEvent.mAgentCount )
{}

RemoveAgentsEvent::RemoveAgentsEvent( double pTime, const RemoveAgentsEvent& pEvent )
: event::Event( pTime, pEvent )
, mSwarmName( pEvent.mSwarmName )
, mStartAgentIndex( pEvent.mStartAgentIndex )
, mAgentCount( pEvent.mAgentCount )
{}

RemoveAgentsEvent::~RemoveAgentsEvent()
{}

event::Event*
RemoveAgentsEvent::copy() const
{
	return new RemoveAgentsEvent( *this );
}

event::Event*
RemoveAgentsEvent::copy( double pTime ) const
{
	return new RemoveAgentsEvent( pTime, *this );
}

void
RemoveAgentsEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
    
	try
	{
		Swarm* swarm = Simulation::get().swarm(mSwarmName);
		
		if( mAgentCount < 0 ) swarm->removeAgents();
		if( mStartAgentIndex >= 0 ) swarm->removeAgents(mStartAgentIndex, mAgentCount);
		else swarm->removeAgents(mAgentCount);
	}
	catch(Exception& e)
	{
		Simulation::get().exceptionReport( e );
	}
    
	mStarted = false;
	mFinished = true;
}