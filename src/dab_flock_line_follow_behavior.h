/** \file dab_flock_line_follow_behavior.h
 * needs at least three line segments, segments should form a closed shape
 */

#ifndef _dab_flock_line_follow_behavior_h_
#define _dab_flock_line_follow_behavior_h_

#include "ofVectorMath.h"
#include "dab_flock_behavior.h"
#include "dab_geom_cuboid.h"
#include "dab_geom_geometry_group.h"
#include "dab_space_shape.h"

namespace dab
{

namespace flock
{

class LineFollowBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    LineFollowBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    LineFollowBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~LineFollowBehavior();
    
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
    Parameter* mForcePar; /// \brief force parameter (output)
    Parameter* mMinDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mContourMaintainDistPar; /// \brief contour maintain dist parameter (internal)
    Parameter* mOrtAmountPar; /// \brief behavior ortogonal amount parameter (internal)
    Parameter* mTanAmountPar; /// \brief behavior tangential amount parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    space::NeighborGroup* mPositionNeighbors; /// \brief position neighbor group
    
    glm::mat4x4 mWC2OC_Matrix;
    glm::vec3 mWC_AgentPosition;
	glm::vec3 mOC_AgentPosition;
	glm::vec3 mOC_ClosestPosition;
	glm::vec3 mWC_ClosestPosition;
	glm::vec3 mOrtDirection;
	glm::vec3 mTangDirection;
    
    geom::Cuboid mBoundingBox;
    space::SpaceShape* mPreviousSpaceShape;
    geom::GeometryGroup* mPreviousClosestGeomGroup;
    int mPreviousClosestLineIndex;
    
    void getClosestGeomGroup( geom::GeometryGroup** pGeomGroup, const glm::vec3& pOC_AgentPosition, float& pGroupDist );
};

};

};

#endif