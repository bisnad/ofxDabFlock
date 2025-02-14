/** \file dab_flock_parameter_combine_behavior.h
 *  \class dab::flock::ParameterCombineBehavior combine several input parameters into single output parameter
 *	\brief combine several input parameters into single output parameter
 *
 *  The behavior concatenations all input parameter values into a single output parameter.\n
 *
 *
 *  Input Parameters:\n
 *  type: InputParameter1 dim: nD neighbors: ignore\n
 *  type: InputParameter2 dim: nD neighbors: ignore\n
 *  etc\n
 *  \n
 *  Output Parameter:\n
 *  type: OutputParameter dim: the sum of the dimensions of all input parameters write: replace\n
 *  \n
 *  Created by Daniel Bisig on 2/7/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_parameter_combine_behavior_h_
#define _dab_flock_parameter_combine_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/DEnse>

namespace dab
{

namespace flock
{

class ParameterCombineBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ParameterCombineBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ParameterCombineBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ParameterCombineBehavior();
    
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
    Parameter* mOutputPar; /// \brief output parameter
};

};

};

#endif