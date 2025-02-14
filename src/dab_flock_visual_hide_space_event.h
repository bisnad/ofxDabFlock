/** \file dab_flock_visual_hide_space_event.h
 *
 *  Created by Daniel Bisig on 6/19/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 04/11/2017.
 *
 */

#ifndef _dab_flock_visual_hide_space_event_h_
#define _dab_flock_visual_hide_space_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class HideSpaceEvent : public event::Event
{
public:
    HideSpaceEvent( const std::string& pSpaceName );
    HideSpaceEvent( const HideSpaceEvent& pEvent );
    virtual ~HideSpaceEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    HideSpaceEvent();
    
    std::string mSpaceName;
};

};

};

#endif