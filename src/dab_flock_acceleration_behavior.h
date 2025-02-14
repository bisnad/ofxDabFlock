/** \file dab_flock_acceleration_behavior.h
 *  \class dab::flock::AccelerationBehavior acceleration behavior
 *	\brief acceleration behavior
 *
 *  Behavior for calculating an Agent 's acceleration from a force acting on it.\n
 *  can limit the resulting acceleration by clamping its linear and angular components.\n
 *  \n
 *  Input Parameter:\n
 *  type: mass dim: 1D neighbors: ignore\n
 *  type: velocity dim: nD neighbors: ignore\n
 *  type: force dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: acceleration dim: nD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_maxLinearAcceleration dim: 1D defaultValue: 0.5\n
 *  name: xxx_maxAngularAcceleration dim: 1D defaultValue: 0.5\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/17/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017
 */

#ifndef _dab_flock_acceleration_behavior_h_
#define _dab_flock_acceleration_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class AccelerationBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    AccelerationBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    AccelerationBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~AccelerationBehavior();
    
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
     \brief perform behavior action
     */
    virtual void act();
    
protected:
    Parameter* mMassPar; /// \brief mass parameter (input)
    Parameter* mVelocityPar; /// \brief velocity parameter (input)
    Parameter* mForcePar; /// \brief force parameter (input)
    Parameter* mAcceleration; /// \brief acceleration parameter (output)
    Parameter* mMaxLinearAccelerationPar; /// \brief maximum linear acceleration parameter (internal)
    Parameter* mMaxAngularAccelerationPar; /// \brief maximum angular acceleration parameter (internal)
    
    Eigen::VectorXf mPrefAccel; /// \brief preferred acceleration
    Eigen::VectorXf mLinearAccel; /// \brief linear acceleration
    Eigen::VectorXf mAngularAccel; /// \brief angular acceleration
};

};

};

#endif