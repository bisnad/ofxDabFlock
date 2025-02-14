/** \file dab_flock_visual_show_swarm_event.h
 *
 *  Created by Daniel Bisig on 6/18/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 04/11/2017.
 *
 */


#ifndef _dab_flock_visual_show_swarm_event_h_
#define _dab_flock_visual_show_swarm_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class ShowSwarmEvent : public event::Event
{
public:
    ShowSwarmEvent( const std::string& pSwarmName, const std::string& pPosParName );
    ShowSwarmEvent( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName );
    ShowSwarmEvent( const std::string& pSwarmName, const std::string& pPosParName, unsigned int pMaxTrailLength );
    ShowSwarmEvent( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, unsigned int pTrailLength );
    
    ShowSwarmEvent( const ShowSwarmEvent& pEvent );
    virtual ~ShowSwarmEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    ShowSwarmEvent();
    
    std::string mSwarmName;
    std::string mPosParName;
    std::string mVelParName;
    int mTrailLength;
};

};

};

#endif