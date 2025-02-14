/** \file dab_flock_visual_agent_shape.h
 */

#ifndef _dab_flock_visual_agent_shape_h_
#define _dab_flock_visual_agent_shape_h_

#include <Eigen/Dense>
#include "ofVectorMath.h"
#include "ofShader.h"
#include "dab_geom_includes.h"
#include "dab_space_includes.h"

namespace dab
{
    
namespace flock
{

class VisAgentShape
{
public:
    VisAgentShape( std::shared_ptr<geom::PolyLine> pGeometry, bool pFilled = false );
    VisAgentShape( std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled );
    ~VisAgentShape();
    
    const std::array<float, 4>& color() const;
    float lineWidth() const;
    const glm::vec3& scale() const;
    
    void setColor( const std::array<float,4>& pColor );
    void setPosition(const glm::vec3& pPosition);
    void setOrientation(const glm::vec3& pVelocity);
    void setOrientation(const glm::quat& pOrientation);
    void setScale(const glm::vec3& pScale);
    void setLineWidth( float pLineWidth );
    
    void display(const ofShader& pShader);
    
protected:
    static glm::vec3 sRefVelocity;
    static std::array<float, 4> sColor;
    static float sLineWidth;
    
    //std::shared_ptr<space::SpaceShape> mShape;
    
    std::vector<bool> mClosed;
    std::vector<ofVbo> mVbos;
    
    std::array<float,4> mColor;
    float mLineWidth;
    //Eigen::Vector3f mVelocity;
    //Eigen::Quaternionf mOrientation;
    
    glm::vec3 mPosition;
	glm::vec3 mVelocity;
    glm::quat mOrientation;
	glm::vec3 mScale;
    glm::mat4x4 mModelMatrix;
    
    void create( std::shared_ptr<geom::PolyLine> pGeometry, bool pFilled );
    void create( std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled ) throw (Exception);
};

};
    
};

#endif