/** \file dab_flock_parameter_print_behavior.h
 *  \class dab:flock::ParameterPrintBehavior print parameter to console
 *	\brief print parameter to console
 *
 *  The Behavior prints parameter to console\n
 *  It prints the agent name and the parameter's value and backup value\n
 *  Might come in handy for debugging purposes\n
 *  \n
 *  Input Parameter:\n
 *  type: any dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 05/12/08.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017.
 */

#ifndef _dab_flock_parameter_print_behavior_h_
#define _dab_flock_parameter_print_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class ParameterPrintBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ParameterPrintBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ParameterPrintBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ParameterPrintBehavior();
    
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
    Parameter* mParameter; ///\brief observed parameter
};

};

};

#endif