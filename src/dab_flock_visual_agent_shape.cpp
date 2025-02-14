/** \file dab_flock_visual_agent_shape.cpp
 */

#include "dab_flock_visual_agent_shape.h"
#include "ofVectorMath.h"
#include "dab_math_vec.h"
#include <algorithm>

using namespace dab;
using namespace dab::flock;

glm::vec3 VisAgentShape::sRefVelocity(1.0, 0.0, 0.0);
std::array<float, 4> VisAgentShape::sColor = {0.0, 0.0, 0.0, 1.0};
float VisAgentShape::sLineWidth(1.0);

VisAgentShape::VisAgentShape( std::shared_ptr<geom::PolyLine> pGeometry, bool pFilled )
: mColor(sColor)
, mLineWidth( sLineWidth )
{
	create( pGeometry, pFilled );
}

VisAgentShape::VisAgentShape( std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled )
: mColor(sColor)
, mLineWidth( sLineWidth )
{
	create( pGeometryGroup, pFilled );
}

VisAgentShape::~VisAgentShape()
{}

void
VisAgentShape::create( std::shared_ptr<geom::PolyLine> pGeometry, bool pFilled )
{
    mClosed.push_back(pFilled);
    mVbos.resize(1);
    
	unsigned int vertexCount = pGeometry->vertexCount();
	if(vertexCount == 0) return;
    
    std::vector<glm::vec3>& vertices = pGeometry->vertices();
    mVbos[0].setVertexData(vertices.data(), vertexCount, GL_STATIC_DRAW);
    
    std::vector<ofIndexType> indices(vertexCount);
    for(int vI=0; vI<vertexCount; ++vI) indices[vI] = vI;
    mVbos[0].setIndexData(indices.data(), vertexCount, GL_STATIC_DRAW);
}

void
VisAgentShape::create( std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled ) throw (Exception)
{
    int geometryCount = pGeometryGroup->geometryCount();
    
    if(geometryCount != pFilled.size()) throw Exception("FLOCK ERROR: size of Geometry Group " + std::to_string(geometryCount) + " doesn't match size of bool array " + std::to_string(pFilled.size()), __FILE__, __FUNCTION__, __LINE__ );
    
	mClosed = pFilled;
    mVbos.resize(geometryCount);

    std::vector<geom::Geometry*>& geometries = pGeometryGroup->geometries();
	
	geom::PolyLine* polyLine;
	
	for( int gI=0; gI<geometryCount; ++gI )
	{
		polyLine = dynamic_cast< geom::PolyLine* >( geometries[gI] );
		bool filled = pFilled[gI];
		
		if( polyLine == NULL ) continue;
		
		unsigned int vertexCount = polyLine->vertexCount();
		
		if(vertexCount == 0) continue;
        
        std::vector<glm::vec3>& vertices = polyLine->vertices();
        mVbos[gI].setVertexData(vertices.data(), vertexCount, GL_STATIC_DRAW);
        
        std::vector<ofIndexType> indices(vertexCount);
        for(int vI=0; vI<vertexCount; ++vI) indices[vI] = vI;
        mVbos[gI].setIndexData(indices.data(), vertexCount, GL_STATIC_DRAW);
	}
}

const std::array<float, 4>&
VisAgentShape::color() const
{
    return mColor;
}

float
VisAgentShape::lineWidth() const
{
    return mLineWidth;
}

const glm::vec3&
VisAgentShape::scale() const
{
    return mScale;
}

void
VisAgentShape::setColor( const std::array<float,4>& pColor )
{
	mColor = pColor;
}

void
VisAgentShape::setPosition(const glm::vec3& pPosition)
{
    mPosition = pPosition;
}

void
VisAgentShape::setOrientation(const glm::vec3& pVelocity)
{
    //std::cout << "VisAgentShape::setOrientation " << pVelocity[0] << " " << pVelocity[1] << " " << pVelocity[2] << "\n";
    
	mVelocity = glm::normalize(pVelocity);
	mOrientation = dab::math::VectorMath::get().makeQuaternion(sRefVelocity, pVelocity);
}

void
VisAgentShape::setOrientation(const glm::quat& pOrientation)
{
    mOrientation = pOrientation;
}

void
VisAgentShape::setScale(const glm::vec3& pScale)
{
 	mScale = pScale;
}

void
VisAgentShape::setLineWidth( float pLineWidth )
{
	mLineWidth = pLineWidth;
}

void
VisAgentShape::display(const ofShader& pShader)
{
	// TODO: test if this works
	mModelMatrix = glm::mat4(1.0f);
	mModelMatrix *= glm::translate(mPosition);
	mModelMatrix *= glm::toMat4(mOrientation);
	mModelMatrix *= glm::scale(mScale);
    
    pShader.setUniform4f("Color", mColor[0], mColor[1], mColor[2], mColor[3]);
    pShader.setUniformMatrix4f("ModelMatrix", mModelMatrix);
    
    int vboCount = mVbos.size();
    
    for(int vboI=0; vboI<vboCount; ++vboI)
    {
        mVbos[vboI].bind();
        if(mClosed[vboI] == true)
        {
            //mVbos[vboI].drawElements(GL_TRIANGLE_STRIP, mVbos[vboI].getNumIndices());
            mVbos[vboI].drawElements(GL_LINE_STRIP, mVbos[vboI].getNumIndices());
        }
        else mVbos[vboI].drawElements(GL_LINE_STRIP, mVbos[vboI].getNumIndices());
        mVbos[vboI].unbind();
    }
}