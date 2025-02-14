/** dab_flock_save_simulation_event.h
 *
 *  Created by Daniel Bisig on 5/20/10.
 *  Ported to OpenFrameworks by Daniel Bisig on 02/11/2017.
 *
 */

#ifndef _dab_flock_save_simulation_event_h_
#define _dab_flock_save_simulation_event_h_

#include "dab_flock_serialize.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class SaveSimulationEvent : public event::Event
{
public:
    SaveSimulationEvent( const std::string& pFileName, SerializeTools::SerializeMode pMode = SerializeTools::AllMode );
    SaveSimulationEvent( double pTime, const std::string& pFileName, SerializeTools::SerializeMode pMode = SerializeTools::AllMode );
    SaveSimulationEvent( const SaveSimulationEvent& pEvent );
    SaveSimulationEvent( double pTime, const SaveSimulationEvent& pEvent );
    virtual ~SaveSimulationEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    std::string mFileName;
    SerializeTools::SerializeMode mMode;
};

};

};

#endif