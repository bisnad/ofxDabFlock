/** \file dab_flock_copy_behavior.h
 *  \class dab::flock::CohesionBehavior copy parameter values into other parameter
 *	\brief copy parameter values into other parameter
 *
 *  The Behavior copies parameter values into other parameter\n
 *  Input Parameter:\n
 *  type: input dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/25/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_copy_behavior_h_
#define _dab_flock_copy_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class CopyBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    CopyBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    CopyBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~CopyBehavior();
    
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
    Parameter* mParIn; /// \brief input parameter (input)
    Parameter* mParOut; /// \brief output parameter (output)
};

};

};

#endif