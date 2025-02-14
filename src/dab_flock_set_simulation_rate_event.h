/** dab_flock_set_simulation_rate_event.h
 *
 *  Created by Daniel Bisig on 6/19/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 24/10/2017.
 *
 */

#ifndef _dab_flock_set_simulation_rate_event_h_
#define _dab_flock_set_simulation_rate_event_h_

#include "dab_event.h"

namespace dab
{

namespace flock
{

class SetSimulationRateEvent : public event::Event
{
public:
    SetSimulationRateEvent( float pSimulationRate );
    SetSimulationRateEvent( double pTime, float pSimulationRate);
    SetSimulationRateEvent( const SetSimulationRateEvent& pEvent );
    SetSimulationRateEvent( double pTime, const SetSimulationRateEvent& pEvent );
    virtual ~SetSimulationRateEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    float mSimulationRate;
};

};

};

#endif