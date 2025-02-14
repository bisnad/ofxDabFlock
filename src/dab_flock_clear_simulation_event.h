/** \file dab_flock_clear_simulation_event.h
 *
 *  Created by Daniel Bisig on 5/20/10.
 *  Ported to OpenFrameworks by Daniel Bisig on 11/10/2017.
 */

#ifndef _dab_flock_clear_simulation_event_h_
#define _dab_flock_clear_simulation_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class ClearSimulationEvent : public event::Event
{
public:
    ClearSimulationEvent();
    ClearSimulationEvent(double pTime);
    ClearSimulationEvent( const ClearSimulationEvent& pEvent );
    ClearSimulationEvent( double pTime, const ClearSimulationEvent& pEvent );
    virtual ~ClearSimulationEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
};

};

};

#endif