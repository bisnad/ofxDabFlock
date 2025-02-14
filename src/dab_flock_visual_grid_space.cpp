/** \file dab_flock_visual_grid_space.cpp
*/

#include "dab_flock_visual_grid_space.h"
#include "dab_flock_simulation.h"
#include "dab_space_manager.h"
#include "dab_space.h"
#include "dab_space_grid.h"
#include "dab_space_alg_grid.h"

using namespace dab;
using namespace dab::flock;

std::array<float, 4> VisGridSpace::sColor = { 0.0, 0.0, 0.0, 1.0 };
float VisGridSpace:: sLineWidth = 1.0;
float VisGridSpace::sValueScale = 1.0;

VisGridSpace::VisGridSpace(const std::string& pSpaceName)
: mSpaceName(pSpaceName)
, mColor(sColor)
, mLineWidth(sLineWidth)
, mValueScale(sValueScale)
{
    try
    {
        create();
    }
    catch (Exception& e)
    {
        std::cout << e << "\n";
    }
}

VisGridSpace::~VisGridSpace()
{}

const std::string&
VisGridSpace::spaceName() const
{
    return mSpaceName;
}

const std::array<float, 4>&
VisGridSpace::color() const
{
    return mColor;
}

float
VisGridSpace::lineWidth() const
{
    return mLineWidth;
}

float
VisGridSpace::valueScale() const
{
    return mValueScale;
}

void
VisGridSpace::setColor(const std::array<float, 4>& pColor)
{
    mColor = pColor;
}

void
VisGridSpace::setLineWidth(float pLineWidth)
{
    mLineWidth = pLineWidth;
}

void
VisGridSpace::setValueScale(float pValueScale)
{
    mValueScale = pValueScale;
    
    int vectorCount = mGridVectors.size();
    int vertexCount = mVertices.size();
    for(int gI=0, vI=0; vI<vectorCount; ++gI, vI += 2)
    {
        for(int d=0; d<3; ++d)
        {
            mVertices[vI+1][d] = mGridVectors[gI][d] + mGridValues[gI][d] * mValueScale;
        }
    }
    
    mVbo.setVertexData(mVertices.data(), vertexCount, GL_STATIC_DRAW);
}

void
VisGridSpace::create() throw (Exception)
{
    try
    {
        std::shared_ptr<space::Space> space = Simulation::get().space().space(mSpaceName);
        space::GridAlg* gridAlg = dynamic_cast< space::GridAlg* >( space->spaceAlg() );
        
        if( gridAlg == nullptr ) throw Exception("FLOCK ERROR: provided space " + mSpaceName + " is not of type grid space", __FILE__, __FUNCTION__, __LINE__);
        
        space::SpaceGrid& spaceGrid = gridAlg->grid();
        
        int valueDim = spaceGrid.valueDim();
        int valueDimLim = std::min(valueDim, 3);
        int gridDim = spaceGrid.gridDim();
        int gridDimLim = std::min(gridDim, 3);
        const dab::Array<unsigned int>& gridSubdivisonCount = spaceGrid.subdivisionCount();
        
        int gridPosCount = 1;
        for(int d=0; d<gridDim; ++d) gridPosCount *= gridSubdivisonCount[d];
        
        //std::cout << "gridPosCount " << gridPosCount << "\n";
        
        int vertexCount = gridPosCount*2;
        mGridVectors.resize(gridPosCount);
        mGridValues.resize(gridPosCount);
        mVertices.resize(vertexCount);
        
        Eigen::VectorXf gridPosition;
        Eigen::VectorXf gridValue;
        
        for(int gI=0, vI=0; gI<gridPosCount; ++gI, vI += 2)
        {
            gridPosition = spaceGrid.index2position(gI);
            gridValue = spaceGrid.gridValue(gI);
            
            //std::cout << "gI " << gI << " pos " << gridPosition.transpose() << " val " << gridValue.transpose() << "\n";
            
            for(int d=0; d<gridDimLim; ++d)
            {
                mGridVectors[gI][d] = gridPosition[d];
                mVertices[vI][d] = gridPosition[d];
            }
            for(int d=0; d<valueDimLim; ++d)
            {
                mGridValues[gI][d] = gridValue[d];
                mVertices[vI+1][d] = mVertices[vI][d] + mGridValues[gI][d] * mValueScale;
            }
        }
        
        mVbo.setVertexData(mVertices.data(), vertexCount, GL_STATIC_DRAW);
        
        int indexCount = gridPosCount*2;
        std::vector<ofIndexType> indices(indexCount);
        for(int i=0; i<indexCount; ++i) indices[i] = i;
        
        mVbo.setIndexData(indices.data(), indexCount, GL_STATIC_DRAW);
        
    }
    catch(Exception &e)
    {
        e += Exception("FLOCK ERROR: failed to create viusal grid space for space " + mSpaceName, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
VisGridSpace::display(const ofShader& pShader)
{
    mModelMatrix = glm::mat4(1.0f);
    
    pShader.setUniform4f("Color", mColor[0], mColor[1], mColor[2], mColor[3]);
    pShader.setUniformMatrix4f("ModelMatrix", mModelMatrix);
    
    glLineWidth(mLineWidth);
    
    mVbo.bind();
    mVbo.drawElements(GL_LINES, mVbo.getNumIndices());
    mVbo.unbind();
}
