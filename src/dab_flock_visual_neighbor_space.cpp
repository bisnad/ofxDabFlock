/** \file dab_flock_visual_neighbor_space.cpp
 */

#include "dab_flock_visual_neighbor_space.h"
#include "dab_flock_simulation.h"
#include "dab_space_manager.h"
#include "dab_space.h"
#include "ofVectorMath.h"
#include <algorithm>

using namespace dab;
using namespace dab::flock;

std::array<float, 4> VisNeighborSpace::sColor = { 0.0, 0.0, 0.0, 1.0 };
float VisNeighborSpace::sLineWidth = 1.0;

VisNeighborSpace::VisNeighborSpace(const std::string& pSpaceName)
: mSpaceName(pSpaceName)
, mColor(sColor)
, mLineWidth(sLineWidth)
{
    try
    {
        create();
    }
    catch(Exception& e)
    {
        std::cout << e << "\n";
    }

}

VisNeighborSpace::~VisNeighborSpace()
{}
    
const std::string&
VisNeighborSpace::spaceName() const
{
    return mSpaceName;
}

const std::array<float, 4>&
VisNeighborSpace::color() const
{
    return mColor;
}


float
VisNeighborSpace::lineWidth() const
{
    return mLineWidth;
}
    
void
VisNeighborSpace::setColor(const std::array<float, 4>& pColor)
{
    mColor = pColor;
}

void
VisNeighborSpace::setLineWidth(float pLineWidth)
{
    mLineWidth = pLineWidth;
}

void
VisNeighborSpace::create() throw (Exception)
{
    mVertices.clear();
    mIndices.clear();
    
    Eigen::VectorXf agentPos;
    Eigen::VectorXf neighborPos;
    glm::vec3 agentPos3D;
	glm::vec3 neighorPos3D;
    
    int spaceDim;
    int spaceDimLim;
    
    try
    {
        std::shared_ptr<space::Space> space = Simulation::get().space().space(mSpaceName);
        spaceDim = space->dim();
        spaceDimLim = std::min(spaceDim, 3);
        
        const std::vector<space::SpaceProxyObject*>& spaceObjects = space->objects();
        
        int spaceObjectCount = spaceObjects.size();
        
        for(int soI=0; soI<spaceObjectCount; ++soI)
        {
            space::SpaceProxyObject* spaceObject = spaceObjects[soI];
            
            agentPos = spaceObject->position();
            
            for(int d=0; d<spaceDimLim; ++d)
            {
                agentPos3D[d] = agentPos[d];
            }
            
            const space::NeighborGroup* neighborGroup = spaceObject->neighborGroup();
            
            int neighborCount = neighborGroup->neighborCount();
            
            for(int nI=0; nI<neighborCount; ++nI)
            {
                neighborPos = neighborGroup->neighbor(nI)->position();
                
                for(int d=0; d<spaceDimLim; ++d)
                {
                    neighorPos3D[d] = neighborPos[d];
                }
                
                mVertices.push_back(agentPos3D);
                mVertices.push_back(neighorPos3D);
                mIndices.push_back(mIndices.size());
                mIndices.push_back(mIndices.size());
            }
        }
        
        mVbo.setVertexData(mVertices.data(), mVertices.size(), GL_STATIC_DRAW);
        mVbo.setIndexData(mIndices.data(), mIndices.size(), GL_STATIC_DRAW);
    }
    catch(Exception& e)
    {
        std::cout << e << "\n";
    }
}

void
VisNeighborSpace::display(const ofShader& pShader)
{
    mModelMatrix = glm::mat4(1.0f);
    
    pShader.setUniform4f("Color", mColor[0], mColor[1], mColor[2], mColor[3]);
    pShader.setUniformMatrix4f("ModelMatrix", mModelMatrix);
    
    glLineWidth(mLineWidth);
    
    mVbo.bind();
    mVbo.drawElements(GL_LINES, mVbo.getNumIndices());
    mVbo.unbind();
}
