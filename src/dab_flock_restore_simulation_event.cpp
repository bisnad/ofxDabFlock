/** \dab_flock_restore_simulation_event.cpp
 */

#include "dab_flock_restore_simulation_event.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

RestoreSimulationEvent::RestoreSimulationEvent( const std::string& pFileName, SerializeTools::SerializeMode pMode )
: event::Event(0.0, -1.0, event::RelativeTime)
, mFileName( pFileName )
, mMode( pMode )
{
	//std::cout << "RestoreSimulationEvent::RestoreSimulationEvent() \n";
}

RestoreSimulationEvent::RestoreSimulationEvent(double pTime, const std::string& pFileName, SerializeTools::SerializeMode pMode )
: event::Event(pTime, -1.0, event::RelativeTime)
, mFileName( pFileName )
, mMode( pMode )
{}

RestoreSimulationEvent::RestoreSimulationEvent( const RestoreSimulationEvent& pEvent )
: event::Event( pEvent )
, mFileName( pEvent.mFileName )
, mMode( pEvent.mMode )
{}

RestoreSimulationEvent::RestoreSimulationEvent( double pTime, const RestoreSimulationEvent& pEvent )
: event::Event( pTime, pEvent )
, mFileName( pEvent.mFileName )
, mMode( pEvent.mMode )
{}

RestoreSimulationEvent::~RestoreSimulationEvent()
{}

event::Event*
RestoreSimulationEvent::copy() const
{
	return new RestoreSimulationEvent( *this );
}

event::Event*
RestoreSimulationEvent::copy( double pTime ) const
{
	return new RestoreSimulationEvent( pTime, *this );
}

void
RestoreSimulationEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
	
	try
	{
		SerializeTools::get().restore( mFileName, mMode );
	}
	catch( Exception& e )
	{
		Simulation::get().exceptionReport( e );
	}
	
	mStarted = false;
	mFinished = true;
}