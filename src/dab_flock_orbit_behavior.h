/** \file dab_flock_orbit_behavior.h
 *  \class dab::flock::OrbitBehavior cause agents to orbit around the positions of neighboring agents
 *	\brief cause agents to orbit around the positions of neighboring agents
 *
 *  This behavior causes agents to orbit around the positions of neighboring agents.\n
 *  A cosine function that extends from the position of the neighboring agent establishes rings of concentric peaks and valleys to which the oribiting agent respond to.\n
 *  Peaks in the cosine function are avoided by the orbiting agent.\n
 *  Valleys in the cosine function attract the orbiting agent.\n
 *  The frequency and position of the cosine function's peaks and valleys can be controlled via the phase abd frequency parameters.\n
 *  The radAmount parameter controls the attraction and repulsion of the orbiting agent perpendicular to the rings of peaks and valleys\n.
 *  The tanAmount parameter controls the acceleration of the orbiting agent in a tangential direction with respect to the rings of peaks and valleys\n.
 *  The orbiting behavior is active when an agent is within a distance from the neighbor agent that lies within the values of minDist and maxDist parameters.\n
 *  The amount parameter controls the strength of the orbiting behavior.\n
 *
 *  The Behavior causes Agent to move withing a ring\n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  type: velocity dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_frequency dim: 1D defaultValue: 2.0\n
 *  name: xxx_phase dim: 1D defaultValue: 0.0\n
 *  name: xxx_minDist dim: 1D defaultValue: 0.0\n
 *  name: xxx_maxDist dim: 1D defaultValue: 0.5\n
 *  name: xxx_radAmount dim: 1D defaultValue: 3.0\n
 *  name: xxx_tanAmount dim: 1D defaultValue: 6.0\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig, Tasuo Unemi, Koichiro Mori, John Flury on 11/16/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 22/10/2017.
 */

#ifndef _dab_flock_orbit_behavior_h_
#define _dab_flock_orbit_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class Agent;

class OrbitBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    OrbitBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    OrbitBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~OrbitBehavior();
    
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
    Parameter* mPositionPar; /// \brief position parameter (input)
    Parameter* mVelocityPar; /// \brief velocity parameter (input)
    Parameter* mForcePar; /// \brief force parameter (output)
    Parameter* mFrequencyPar; /// \brief frequency parameter (internal)
    Parameter* mPhasePar; /// \brief phase parameter (internal)
    Parameter* mMinDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mRadAmountPar; /// \brief radial amount parameter (internal)
    Parameter* mTanAmountPar; /// \brief tangential amount parameter (internal)
    Parameter* mAmountPar; /// \brief behaviour amount parameter (internal)
    space::NeighborGroup* mPositionNeighbors; /// \brief position neighbor group
    Eigen::Vector3f mNormDirection; ///brief normalized neighbor direction
    Eigen::Vector3f mNormVelocity; ///\brief normalized agent velocity
    Eigen::Vector3f mCrossVec; ///\brief vector cross product (norm dir, norm vel)
    Eigen::Vector3f mCrossVec2; ///\brief normalized agent velocity (cross vec, norm dir)
    Eigen::VectorXf mAvgDirection; /// \brief avg of neighboring value directions
    Eigen::VectorXf mTmpForce; /// \brief temporary force
};

};

};

#endif