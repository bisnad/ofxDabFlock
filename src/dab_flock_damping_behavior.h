/** \file dab_flock_damping_behavior.h
 *  \class dab::flock::DampingBehavior adapt Agent velocity towards target speed
 *	\brief adapt Agent velocity towards target speed
 *
 *  The Behavior adapts Agent velocity towards target speed\n
 *  Input Parameter:\n
 *  type: velocity dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_prefVelocity dim: 1D defaultValue: 1.0\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/21/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/10/2017.
 */

#ifndef _dab_flock_damping_behavior_h_
#define _dab_flock_damping_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class DampingBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    DampingBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception FlockException wrong number of type of parameters
     */
    DampingBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~DampingBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception FlockException wrong number of type of parameters
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
    Parameter* mVelocityPar; /// \brief velocity parameter (input)
    Parameter* mForcePar; /// \brief force parameter (output)
    Parameter* mPrefVelocityPar; /// \brief preferred velocity parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    
    Eigen::VectorXf mTmpForce; /// \brief temporary force
};

};

};

#endif