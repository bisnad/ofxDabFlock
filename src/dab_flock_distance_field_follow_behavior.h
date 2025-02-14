/** \file dab_flock_distance_field_follow_behavior.h
 *  \class dab::flock::DistanceFieldFollowBehavior cause Agent to along a surface represented by a distance field
 *	\brief cause Agent to along a surface represented by a distance field
 *
 *  The Behavior causes an Agent to move along the surface represented by a distance field\n
 *  The force created by the behavior acts both along the vectors of the distance field as well as tangential to them.\n
 *  The balance between the colinear and tangential force components depends on the length of the distance vector.\n
 *  The closer the distance vector length is to the value represented by the maximum distance parameter, the more the colinear component dominates.\n
 *  The closer the distance vector length is to the value represented by the minimum distance parameter, the more the tangention component dominates.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  type: velocity dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_minDist dim: 1D defaultValue: 0.0\n
 *  name: xxx_maxDist dim: 1D defaultValue: 1.0\n
 *  name: xxx_amount dim: 1D defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 9/23/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 14/10/2017.
 */

// TODO: Untested!!

#ifndef _dab_flock_distance_field_follow_behavior_h_
#define _dab_flock_distance_field_follow_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class DistanceFieldFollowBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    DistanceFieldFollowBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    DistanceFieldFollowBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~DistanceFieldFollowBehavior();
    
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
    Parameter* mMinDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mOrtAmountPar; /// \brief behavior amount parameter (internal)
    Parameter* mTanAmountPar; /// \brief behavior amount parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    space::NeighborGroup* mGridValues; /// \brief grid values
    
    Eigen::VectorXf mOrtDirection; /// \brief force along distance vector
    Eigen::VectorXf mTangDirection; /// \brief force perpendicular to distance vector
    Eigen::VectorXf mDistanceVector; /// \brief distance to surface vector
    Eigen::VectorXf mNormVelocity; /// \brief normalized velocity vector
};

};

};

#endif