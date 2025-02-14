/** \file dab_flock_visual_agent_trail.cpp
 */

#include "dab_flock_visual_agent_trail.h"

using namespace dab;
using namespace dab::flock;

VisAgentTrail::VisAgentTrail(unsigned int pMaxLength)
: mPositions(pMaxLength)
, mIndices(pMaxLength)
, mAlphaValues(pMaxLength)
, mDecay(0.99)
, mLineWidth(1.0)
, mColor({0.0, 0.0, 0.0, 1.0})
, mLength(pMaxLength)
, mMaxLength(pMaxLength)
, mCurrentPositionIndex(0)
{}

VisAgentTrail::~VisAgentTrail()
{}

void
VisAgentTrail::create()
{
    mVbo.setVertexData(mPositions.data(), mMaxLength, GL_STATIC_DRAW);
    mVbo.setIndexData(mIndices.data(), mMaxLength, GL_STATIC_DRAW);
}

float
VisAgentTrail::decay() const
{
	return mDecay;
}

float
VisAgentTrail::lineWidth() const
{
	return mLineWidth;
}

unsigned int
VisAgentTrail::length() const
{
	return mLength;
}

unsigned int
VisAgentTrail::maxLength() const
{
	return mMaxLength;
}

const std::array<float,4>&
VisAgentTrail::color() const
{
	return mColor;
}

const std::vector< glm::vec3 >&
VisAgentTrail::positions() const
{
	return mPositions;
}

void
VisAgentTrail::setDecay(float pDecay)
{
    //std::cout << "VisAgentTrail " << this << " set decay " << pDecay << "\n";
    
	mDecay = pDecay;
}

void
VisAgentTrail::setLineWidth(float pLineWidth)
{
	mLineWidth = pLineWidth;
}

void
VisAgentTrail::setLength(unsigned int pLength)
{
	if(pLength <= mPositions.size() ) mLength = pLength;
}

void
VisAgentTrail::setColor(const std::array<float,4>& pColor)
{
    mColor = pColor;
    
	for(unsigned int i=0; i<4; ++i)
	{
		if( mColor[i] < 0.0 ) mColor[i] = 0.0;
		else if( mColor[i] > 1.0 ) mColor[i] = 1.0;
	}
}

void
VisAgentTrail::update(const glm::vec3& pPosition)
{
    //std::cout << "VisAgentTrail::update: decay " << mDecay << "\n";
    
    //std::cout << "VisAgentTrail " << this << " update decay " << mDecay << "\n";
    
    mCurrentPositionIndex += 1;
    if(mCurrentPositionIndex >= mLength) mCurrentPositionIndex = 0;
    
    mPositions[mCurrentPositionIndex] = pPosition;
    
    // update indices and alpha values
    // TODO: figure out if there is a more efficient method that rewriting the entire alpha and index buffers
    float alphaValue = 1.0;
    int index = 0;
    for(int i=mCurrentPositionIndex; i>=0; --i)
    {
        mIndices[index] = i;
        mAlphaValues[i] = alphaValue;
        
        index++;
        alphaValue *= mDecay;
    }
    for(int i=mLength-1; i>mCurrentPositionIndex; --i)
    {
        mIndices[index] = i;
        mAlphaValues[i] = alphaValue;
        
        index++;
        alphaValue *= mDecay;
    }
    
    mVbo.setVertexData(mPositions.data(), mLength, GL_STATIC_DRAW);
    mVbo.setIndexData(mIndices.data(), mLength, GL_STATIC_DRAW);
}

void
VisAgentTrail::display(const ofShader& pShader)
{
	mModelMatrix = glm::mat4(1.0f);
    
    pShader.setUniform4f("Color", mColor[0], mColor[1], mColor[2], mColor[3]);
    pShader.setUniformMatrix4f("ModelMatrix", mModelMatrix);
    
    int attrLoc = pShader.getAttributeLocation("alpha");
    //mVbo.setAttributeData(attrLoc, mAlphaValues.data(), 1, mMaxLength, GL_STATIC_DRAW);
    mVbo.setAttributeData(attrLoc, mAlphaValues.data(), 1, mLength, GL_STATIC_DRAW);
    
    mVbo.bind();
    mVbo.drawElements(GL_LINE_STRIP, mVbo.getNumIndices());
    mVbo.unbind();
}