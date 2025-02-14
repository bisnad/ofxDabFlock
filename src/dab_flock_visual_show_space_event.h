/** \file dab_flock_visual_show_space_event.h
 *
 *  Created by Daniel Bisig on 6/19/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 04/11/2017.
 *
 */

#ifndef _dab_flock_visual_show_space_event_h_
#define _dab_flock_visual_show_space_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class ShowSpaceEvent : public event::Event
{
public:
    ShowSpaceEvent( const std::string& pSpaceName );
    ShowSpaceEvent( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor );
    ShowSpaceEvent( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor, float pValueScale );
    ShowSpaceEvent( const ShowSpaceEvent& pEvent );
    virtual ~ShowSpaceEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    ShowSpaceEvent();
    
    std::string mSpaceName;
    std::array<float, 4> mSpaceColor;
    float mValueScale;
};

};

};

#endif