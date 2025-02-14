/** \dab_flock_set_simulation_rate_event.cpp
 */

#include "dab_flock_set_simulation_rate_event.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

SetSimulationRateEvent::SetSimulationRateEvent( float pSimulationRate )
: event::Event(0.0, -1.0, event::RelativeTime)
, mSimulationRate( pSimulationRate )
{}

SetSimulationRateEvent::SetSimulationRateEvent( double pTime, float pSimulationRate )
: event::Event(pTime, -1.0, event::RelativeTime)
, mSimulationRate( pSimulationRate )
{}

SetSimulationRateEvent::SetSimulationRateEvent( const SetSimulationRateEvent& pEvent )
: event::Event( pEvent )
, mSimulationRate( pEvent.mSimulationRate )
{}

SetSimulationRateEvent::SetSimulationRateEvent( double pTime, const SetSimulationRateEvent& pEvent )
: event::Event( pTime, pEvent )
, mSimulationRate( pEvent.mSimulationRate )
{}

SetSimulationRateEvent::~SetSimulationRateEvent()
{}

event::Event*
SetSimulationRateEvent::copy() const
{
	return new SetSimulationRateEvent( *this );
}

event::Event*
SetSimulationRateEvent::copy( double pTime ) const
{
	return new SetSimulationRateEvent( pTime, *this );
}

void
SetSimulationRateEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
	
	try
	{
		Simulation::get().setUpdateInterval( mSimulationRate );
	}
	catch( Exception& e )
	{
		Simulation::get().exceptionReport( e );
	}
	
	mStarted = false;
	mFinished = true;
}