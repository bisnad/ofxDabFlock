/** \file dab_flock_circular_behavior.h
 *  \class dan::flock::CircularBehavior cause Agent to move withing a ring
 *	\brief cause Agent to move withing a ring
 *
 *  The Behavior causes Agent to move withing a ring\n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_innerRadius dim: 1D defaultValue: 2.0\n
 *  name: xxx_outerRadius dim: 1D defaultValue: 3.0\n
 *  name: xxx_radialAmount dim: 1D defaultValue: 0.1\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig, Tasuo Unemi, Koichiro Mori, John Flury on 11/16/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_circular_behavior_h_
#define _dab_flock_circular_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class CircularBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    CircularBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    CircularBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~CircularBehavior();
    
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
    Parameter* mPosition; /// \brief position parameter (input)
    Parameter* mVelocity; /// \brief velocity parameter (input)
    Parameter* mForce; /// \brief force parameter (output)
    Parameter* mCenterPositionPar; /// \brief center position parameter (internal)
    Parameter* mInnerRadiusPar; /// \brief inner radius parameter (internal)
    Parameter* mOuterRadiusPar; /// \brief outer radius parameter (internal)
    Parameter* mOrtAmountPar; /// \brief orthogonal amount parameter (internal)
    Parameter* mTanAmountPar; /// \brief tangential amount parameter (internal)
    Eigen::Vector3f mNormVelocity; /// \brief normalized velocity
    Eigen::Vector3f mCenterVec; /// \brief vector pointing from agent position towards origin
    Eigen::Vector3f mNormZVec; /// \brief noralized vector pointing in positive z-direction
    Eigen::Vector3f mCrossVec;
    Eigen::Vector3f mNormTangentVec; /// \brief normalized tangent vector to circle
};

};

};

#endif