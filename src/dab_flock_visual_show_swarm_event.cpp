/** \file dab_flock_visual_show_swarm_event.cpp
 */

#include "dab_flock_visual_show_swarm_event.h"
#include "dab_flock_visual.h"

using namespace dab;
using namespace dab::flock;

ShowSwarmEvent::ShowSwarmEvent()
: event::Event()
{}

ShowSwarmEvent::ShowSwarmEvent(const std::string& pSwarmName, const std::string& pPosParName )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mPosParName(pPosParName)
, mVelParName()
, mTrailLength(-1)
{}

ShowSwarmEvent::ShowSwarmEvent(const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mPosParName(pPosParName)
, mVelParName(pVelParName)
, mTrailLength(-1)
{}

ShowSwarmEvent::ShowSwarmEvent(const std::string& pSwarmName, const std::string& pPosParName, unsigned int pTrailLength )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mPosParName(pPosParName)
, mVelParName()
, mTrailLength(pTrailLength)
{}

ShowSwarmEvent::ShowSwarmEvent(const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, unsigned int pTrailLength )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mPosParName(pPosParName)
, mVelParName(pVelParName)
, mTrailLength(pTrailLength)
{}

ShowSwarmEvent::ShowSwarmEvent( const ShowSwarmEvent& pEvent )
: event::Event(pEvent)
, mSwarmName(pEvent.mSwarmName)
, mPosParName(pEvent.mPosParName)
, mVelParName(pEvent.mVelParName)
, mTrailLength(pEvent.mTrailLength)
{}

ShowSwarmEvent::~ShowSwarmEvent()
{}

event::Event*
ShowSwarmEvent::copy() const
{
	return new ShowSwarmEvent( *this );
}

event::Event*
ShowSwarmEvent::copy( double pTime ) const
{
	return new ShowSwarmEvent( *this );
}

void
ShowSwarmEvent::execute() throw (Exception)
{
    mStarted = true;
	mFinished = false;
	    
    if(mTrailLength >= 0) FlockVisuals::get().showSwarm( mSwarmName, mPosParName, mVelParName, mTrailLength );
    else FlockVisuals::get().showSwarm( mSwarmName, mPosParName, mVelParName );
    
	mStarted = false;
	mFinished = true;
}