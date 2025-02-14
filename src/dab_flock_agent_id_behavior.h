/** \file dab_flock_agent_id_behavior.h
 *  \class dab::flock::AgentIdBehavior agent id behavior
 *	\brief agent id behavior
 *
 *  The Behavior stores agent id in output parameter\n
 *  \n
 *  Input Parameter:\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: 1 write: replace\n
 *  \n
 *  Created by Daniel Bisig on 6/22/11.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017
 */

#ifndef _dab_flock_agent_id_behavior_h_
#define _dab_flock_agent_id_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class AgentIdBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    AgentIdBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    AgentIdBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~AgentIdBehavior();
    
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
    Parameter* mIdPar; /// \brief reset parameter (output)
};

};

};

#endif