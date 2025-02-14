/** \file  dab_flock_neighbor_parameter_store_behavior.h
 *  iso_flock
 *
 *  Created by Daniel Bisig on 7/6/12.
 *  Ported to OpenFrameworks by Daniel Bisig on 17/10/2017.
 *
 */

#ifndef _dab_flock_neighbor_parameter_store_behavior_h_
#define _dab_flock_neighbor_parameter_store_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class NeighborParameterStoreBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    NeighborParameterStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    NeighborParameterStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~NeighborParameterStoreBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception Exception wrong number of type of parameters
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pAgent) const  throw (Exception);
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    /**
     \brief perform behavior
     */
    void act();
    
protected:
    std::string mNeighborParameterName; /// \brief input parameter name (input)
    Parameter* mNeighborStorePar; /// \brief neighbor store parameter (output)
    space::NeighborGroup* mNeighbors; /// \brief neighbor group
};

};

};


#endif