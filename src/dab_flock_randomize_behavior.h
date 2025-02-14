/** \file dab_flock_randomize_behavior.h
 *  \class dab::flock::RandomizeBehavior randomize parameter value
 *	\brief randomize parameter value
 *
 *  The Behavior randomizes parameter value\n
 *  \n
 *  Input Parameter:\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_range dim: nD defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 4/16/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017.
 */

#ifndef _dab_flock_randomize_behavior_h_
#define _dab_flock_randomize_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class RandomizeBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    RandomizeBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    RandomizeBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~RandomizeBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception Exception wrong number of type of parameters
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception);
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    /**
     \brief perform behavior action
     */
    virtual void act();
    
protected:
    Parameter* mRandomizePar; /// \brief randomize parameter (output)
    Parameter* mRangePar; /// \brief randomize range parameter (internal)
};

};

};

#endif