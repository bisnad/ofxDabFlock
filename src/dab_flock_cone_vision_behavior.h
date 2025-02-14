/** \file dab_flock_cone_vision_behavior.h
 *  \class dab::flock::ConeVisionBehavior velocity alignment behavior
 *	\brief cone vision behavior
 *
 *  The behavior implements cone vision for agents.\n
 *  It does so by copying those neighbors from an input neighbor group into an output neighbor group that fall within the interior of an agent's vision cone.\n.
 *  The tip of vision cone is placed at the agent's position and oriented in the direction of the agent's velocity.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  type: velocity dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_visionAngle dim: 1D defaultValue: 0.0\n
 *  \n
 *  Created by Daniel Bisig on 4/15/09.
 */

// TODO: Untested

#ifndef _dab_flock_cone_vision_behavior_h_
#define _dab_flock_cone_vision_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class ConeVisionBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    ConeVisionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    ConeVisionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~ConeVisionBehavior();
    
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
    Parameter* mPositionInPar; /// \brief position parameter (input)
    Parameter* mVelocityPar; /// \brief velocity parameter (input)
    Parameter* mPositionOutPar; /// \brief position parameter (output)
    Parameter* mVisionAnglePar; /// \brief force parameter (internal)
    space::NeighborGroup* mPositionInNeighbors; /// \brief position neighbor group
    space::NeighborGroup* mPositionOutNeighbors; /// \brief position neighbor group
    
    Eigen::VectorXf mNormVelocity;
    Eigen::VectorXf mNormNeighborDirection;
};

};

};

#endif