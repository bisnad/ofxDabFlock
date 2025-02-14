/** \file dab_flock_visual_hide_space_event.cpp
 */

#include "dab_flock_visual_hide_space_event.h"
#include "dab_flock_visual.h"

using namespace dab;
using namespace dab::flock;

HideSpaceEvent::HideSpaceEvent()
: event::Event()
{}

HideSpaceEvent::HideSpaceEvent(const std::string& pSpaceName )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
{}

HideSpaceEvent::HideSpaceEvent( const HideSpaceEvent& pEvent )
: event::Event(pEvent)
, mSpaceName(pEvent.mSpaceName)
{}

HideSpaceEvent::~HideSpaceEvent()
{}

event::Event*
HideSpaceEvent::copy() const
{
	return new HideSpaceEvent( *this );
}

event::Event*
HideSpaceEvent::copy( double pTime ) const
{
	return new HideSpaceEvent( *this );
}

void
HideSpaceEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
	
	FlockVisuals::get().hideSpace(mSpaceName);
    
	mStarted = false;
	mFinished = true;
}