/** \file dab_flock_visual_show_space_event.cpp
 */

#include "dab_flock_visual_show_space_event.h"
#include "dab_flock_visual.h"

using namespace dab;
using namespace dab::flock;

ShowSpaceEvent::ShowSpaceEvent()
: event::Event()
{}

ShowSpaceEvent::ShowSpaceEvent( const std::string& pSpaceName )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceColor( { 0.0, 0.0, 0.0, 1.0 } )
, mValueScale(1.0)
{}

ShowSpaceEvent::ShowSpaceEvent( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceColor(pSpaceColor)
, mValueScale(1.0)
{}

ShowSpaceEvent::ShowSpaceEvent( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor, float pValueScale )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceColor(pSpaceColor)
, mValueScale(pValueScale)
{}

ShowSpaceEvent::ShowSpaceEvent( const ShowSpaceEvent& pEvent )
: event::Event(pEvent)
, mSpaceName(pEvent.mSpaceName)
, mSpaceColor(pEvent.mSpaceColor)
, mValueScale(pEvent.mValueScale)
{}

ShowSpaceEvent::~ShowSpaceEvent()
{}

event::Event*
ShowSpaceEvent::copy() const
{
	return new ShowSpaceEvent( *this );
}

event::Event*
ShowSpaceEvent::copy( double pTime ) const
{
	return new ShowSpaceEvent( *this );
}

void
ShowSpaceEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
    
	FlockVisuals::get().showSpace( mSpaceName, mSpaceColor, mValueScale );
    
	mStarted = false;
	mFinished = true;
}