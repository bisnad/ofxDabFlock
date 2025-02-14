/** \file dab_flock_add_agents_event.h
 *
 *  Created by Daniel Bisig on 1/28/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_add_agents_event_h_
#define _dab_flock_add_agents_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class AddAgentsEvent : public event::Event
{
public:
    AddAgentsEvent(const std::string& pSwarmName, unsigned int pAgentCount);
    AddAgentsEvent(double pTime, const std::string& pSwarmName, unsigned int pAgentCount);
    AddAgentsEvent(const AddAgentsEvent& pEvent);
    AddAgentsEvent(double pTime, const AddAgentsEvent& pEvent);
    virtual ~AddAgentsEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    std::string mSwarmName;
    unsigned int mAgentCount;
};

};

};

#endif