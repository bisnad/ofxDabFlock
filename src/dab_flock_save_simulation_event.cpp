/** \dab_flock_save_simulation_event.cpp
 */

#include "dab_flock_save_simulation_event.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

SaveSimulationEvent::SaveSimulationEvent( const std::string& pFileName, SerializeTools::SerializeMode pMode )
: event::Event(0.0, -1.0, event::RelativeTime)
, mFileName( pFileName )
, mMode( pMode )
{}

SaveSimulationEvent::SaveSimulationEvent(double pTime, const std::string& pFileName, SerializeTools::SerializeMode pMode )
: event::Event(pTime, -1.0, event::RelativeTime)
, mFileName( pFileName )
, mMode( pMode )
{}

SaveSimulationEvent::SaveSimulationEvent( const SaveSimulationEvent& pEvent )
: event::Event( pEvent )
, mFileName( pEvent.mFileName )
, mMode( pEvent.mMode )
{}

SaveSimulationEvent::SaveSimulationEvent( double pTime, const SaveSimulationEvent& pEvent )
: event::Event( pTime, pEvent )
, mFileName( pEvent.mFileName )
, mMode( pEvent.mMode )
{}

SaveSimulationEvent::~SaveSimulationEvent()
{}

event::Event*
SaveSimulationEvent::copy() const
{
	return new SaveSimulationEvent( *this );
}

event::Event*
SaveSimulationEvent::copy( double pTime ) const
{
	return new SaveSimulationEvent( pTime, *this );
}

void
SaveSimulationEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
	
	try
	{
		//std::cout << "SaveSimulationEvent::execute() begin\n";
		
		SerializeTools::get().save( mFileName, mMode );
		
		//std::cout << "SaveSimulationEvent::execute() end\n";
	}
	catch( Exception& e )
	{
		Simulation::get().exceptionReport( e );
	}
	
	mStarted = false;
	mFinished = true;
}