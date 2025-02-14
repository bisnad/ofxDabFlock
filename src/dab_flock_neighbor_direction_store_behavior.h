/** \file dab_flock_neighbor_direction_store_behavior.h
 *  \class dab::flock::NeighborStoreBehavior store neighborhood information in agent parameter
 *	\brief store neighborhood information in agent parameter
 *
 *  The Behavior stores the direction of neighbors in an output parameter\n
 *  The dimension of the output parameter is fixed and must be nD + nD * maxNeighborCount\n
 *  If there are fewer neighbors than the parameter can hold, the remaining parameter positions are filled with FLT_MIN\n
 *  If there are more neighbors than the parameter can hold, they will be ignored\n
 *  The content of the output Parameter is as follows:\n
 *  [0 - nD-1] : agent direction ( always 0.0 )\n
 *  [nD - 2nD-1] : neighbor1 direction\n
 *  [2nD - 3nD-1] : neighbor2 direction\n
 *  etc\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD + nD * maxNeighborCount write: replace\n
 *  \n
 *  Created by Daniel Bisig on 30/12/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/16/2017.
 */


#ifndef _dab_flock_neighbor_direction_store_behavior_h_
#define _dab_flock_neighbor_direction_store_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class NeighborDirectionStoreBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    NeighborDirectionStoreBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    NeighborDirectionStoreBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~NeighborDirectionStoreBehavior();
    
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
    Parameter* mParameter; /// \brief input parameter (input)
    Parameter* mNeighborStorePar; /// \brief neighbor store parameter (output)
    space::NeighborGroup* mNeighbors; /// \brief neighbor group
};

};

};

#endif