/** \file dab_flock_alignment_behavior.h
 *  \class dab::flock::AlignmentBehavior velocity alignment behavior
 *	\brief velocity alignment behavior
 *
 *  Behavior causes Agent to adapt its velocity to the velocities of its neighbors.\n
 *  The behavior derives a force vector to adapt the velocity.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  type: velocity dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_mindist dim: 1D defaultValue: 0.0\n
 *  name: xxx_maxdist dim: 1D defaultValue: 0.5\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/21/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_alignment_behavior_h_
#define _dab_flock_alignment_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class AlignmentBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    AlignmentBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    AlignmentBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~AlignmentBehavior();
    
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
    Parameter* mPositionPar; /// \brief position parameter (input)
    Parameter* mVelocityPar; /// \brief velocity parameter (input)
    Parameter* mForcePar; /// \brief force parameter (output)
    Parameter* mMinDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    
    space::NeighborGroup* mPositionNeighbors; /// \brief position neighbor group
    
    Eigen::VectorXf mAvgVelocity; /// \brief avg of neighboring velocities
    Eigen::VectorXf mTmpForce; /// \brief temporary force
};

};

};

#endif