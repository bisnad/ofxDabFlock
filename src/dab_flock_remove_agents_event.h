/** \file dab_flock_remove_agents_event.h
 *
 *  Created by Daniel Bisig on 1/28/09.
 *  Ported to OpenFraemworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_remove_agents_event_h_
#define _dab_flock_remove_agents_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class RemoveAgentsEvent : public event::Event
{
public:
    RemoveAgentsEvent( const std::string& pSwarmName );
    RemoveAgentsEvent( const std::string& pSwarmName, int pAgentCount );
    RemoveAgentsEvent( const std::string& pSwarmName, int pStartAgentIndex, int pEndAgentIndex );
    RemoveAgentsEvent( double pTime, const std::string& pSwarmName );
    RemoveAgentsEvent( double pTime, const std::string& pSwarmName, int pAgentCount );
    RemoveAgentsEvent( double pTime, const std::string& pSwarmName, int pStartAgentIndex, int pEndAgentIndex );
    RemoveAgentsEvent( const RemoveAgentsEvent& pEvent );
    RemoveAgentsEvent( double pTime, const RemoveAgentsEvent& pEvent );
    virtual ~RemoveAgentsEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    std::string mSwarmName;
    int mStartAgentIndex;
    int mAgentCount;
};

};

};

#endif