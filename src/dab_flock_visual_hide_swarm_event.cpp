/** \file dab_flock_hide_swarm_event.cpp
 */

#include "dab_flock_visual_hide_swarm_event.h"
#include "dab_flock_visual.h"

using namespace dab;
using namespace dab::flock;

HideSwarmEvent::HideSwarmEvent()
: event::Event()
{}

HideSwarmEvent::HideSwarmEvent(const std::string& pSwarmName )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
{}

HideSwarmEvent::HideSwarmEvent( const HideSwarmEvent& pEvent )
: event::Event(pEvent)
, mSwarmName(pEvent.mSwarmName)
{}

HideSwarmEvent::~HideSwarmEvent()
{}

event::Event*
HideSwarmEvent::copy() const
{
	return new HideSwarmEvent( *this );
}

event::Event*
HideSwarmEvent::copy( double pTime ) const
{
	return new HideSwarmEvent( *this );
}

void
HideSwarmEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
    
    FlockVisuals::get().hideSwarm(mSwarmName);
    
	mStarted = false;
	mFinished = true;
}