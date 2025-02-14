/** \dab_flock_clear_simulation_event.cpp
 */

#include "dab_flock_clear_simulation_event.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

ClearSimulationEvent::ClearSimulationEvent()
: event::Event(0.0, -1.0, event::RelativeTime)
{}

ClearSimulationEvent::ClearSimulationEvent(double pTime)
: event::Event(pTime, -1.0, event::RelativeTime)
{}

ClearSimulationEvent::ClearSimulationEvent( const ClearSimulationEvent& pEvent )
: event::Event( pEvent )
{}

ClearSimulationEvent::ClearSimulationEvent( double pTime, const ClearSimulationEvent& pEvent )
: event::Event( pTime, pEvent )
{}

ClearSimulationEvent::~ClearSimulationEvent()
{}

event::Event*
ClearSimulationEvent::copy() const
{
	return new ClearSimulationEvent( *this );
}

event::Event*
ClearSimulationEvent::copy( double pTime ) const
{
	return new ClearSimulationEvent( pTime, *this );
}

void
ClearSimulationEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
	
	try
	{
		//std::cout << "ClearSimulationEvent::execute() begin\n";
		
		Simulation::get().clear();
		
		//std::cout << "ClearSimulationEvent::execute() end\n";
	}
	catch( Exception& e )
	{
		Simulation::get().exceptionReport( e );
	}
	
	mStarted = false;
	mFinished = true;
}