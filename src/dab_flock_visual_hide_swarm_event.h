/** \file dab_flock_hide_swarm_event.h
 *
 *  Created by Daniel Bisig on 6/18/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 04/11/2017.
 *
 */

#ifndef _dab_flock_visual_hide_swarm_event_h_
#define _dab_flock_visual_hide_swarm_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class HideSwarmEvent : public event::Event
{
public:
    HideSwarmEvent( const std::string& pSwarmName );
    HideSwarmEvent( const HideSwarmEvent& pEvent );
    virtual ~HideSwarmEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    HideSwarmEvent();
    
    std::string mSwarmName;
};

};

};

#endif