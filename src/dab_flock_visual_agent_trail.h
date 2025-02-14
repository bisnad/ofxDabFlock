/** dab_flock_visual_agent_trail.h
*/

#ifndef _dab_flock_visual_agent_trail_h_
#define _dab_flock_visual_agent_trail_h_

#include <iostream>
#include <array>
#include <Eigen/Dense>
#include "ofShader.h"
#include "ofVbo.h"
#include "ofVectorMath.h"
#include "dab_ringbuffer.h"

namespace dab
{

namespace flock
{

class VisAgentTrail
{
public:
	VisAgentTrail(unsigned int pMaxLength);
	~VisAgentTrail();
    
	float decay() const;
	float lineWidth() const;
	unsigned int length() const;
    unsigned int maxLength() const;
	const std::array<float,4>& color() const;
    const std::vector< glm::vec3 >& positions() const;
	
	void setDecay(float pDecay);
	void setLineWidth(float pLineWidth);
	void setLength( unsigned int pLength );
	void setColor(const std::array<float,4>& pColor);
	void update(const glm::vec3& pPosition);
	
	void display(const ofShader& pShader);
	
protected:
    std::vector< glm::vec3 > mPositions;
    std::vector< ofIndexType > mIndices;
    std::vector< float > mAlphaValues;
	std::array<float,4> mColor;
	float mDecay;
	float mLineWidth;
	unsigned int mLength;
    unsigned int mMaxLength;
	unsigned int mMaxTrailSegLength;
    
    int mCurrentPositionIndex;
    glm::mat4x4 mModelMatrix;
    ofVbo mVbo;
    
    void create();
};
    
};
    
};


#endif
