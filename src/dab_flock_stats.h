/** \file dab_flock_stats.h
 *  \class dab::flock::FlockStats flock analyzer and statistics manager
 *  \brief flock analyzer and statistics manager
 *
 *  Created by Daniel Bisig on 6/5/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 09/08/17.
 */

#ifndef _dab_flock_stats_h_
#define _dab_flock_stats_h_

#include "dab_singleton.h"
#include "dab_flock_parameter.h"
#include "dab_space_objects_analyze_manager.h"

namespace dab
{

namespace flock
{

class FlockStats : public space::SpaceObjectsAnalyzeManager , public Singleton<FlockStats>
{
    friend class Singleton<FlockStats>;
    
public:
    /**
     \brief register parameter for analysis
     \param pAnalyzerName name of analyzer
     \param pGroupName name of group
     \param pParameter parameter
     \exception Exception failed to add parameter
     */
    void registerParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const Parameter& pParameter) throw (Exception);
    
    /**
     \brief register parameter neighbor group for analysis
     \param pAnalyzerName name of analyzer
     \param pGroupName name of group
     \param pNeighborGroup parameter neighbor group
     \exception Exception failed to add neighborgroup
     */
    void registerParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const space::NeighborGroup& pNeighborGroup ) throw (Exception);
    
    /**
     \brief deregister parameter
     \param pAnalyzerName name of analyzer
     \param pGroupName name of group
     \param pParameter parameter
     \exception Exception failed to remove parameter
     */
    void deregisterParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const Parameter& pParameter) throw (Exception);
    
    /**
     \brief deregister parameter neighborgroup
     \param pAnalyzerName name of analyzer
     \param pGroupName name of group
     \param pNeighborGroup parameter neighbor group
     \exception Exception failed to remove neighborgroup
     */
    void deregisterParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const space::NeighborGroup& pNeighborGroup ) throw (Exception);
    
    
protected:
    /**
     \brief default constructor
     */
    FlockStats();
    
    /**
     \brief destructor
     */
    ~FlockStats();
};

};

};

#endif