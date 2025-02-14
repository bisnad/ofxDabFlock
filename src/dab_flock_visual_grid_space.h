/** \file dab_flock_visual_grid_space.h
*/

#ifndef _dab_flock_visual_grid_space_h_
#define _dab_flock_visual_grid_space_h_

#include <iostream>
#include <array>
#include "ofVectorMath.h"
#include "ofShader.h"
#include "ofVbo.h"
#include "dab_exception.h"

namespace dab
{

namespace flock
{

class VisGridSpace
{
public:
    VisGridSpace(const std::string& pSpaceName);
    ~VisGridSpace();
    
    const std::string& spaceName() const;
    const std::array<float, 4>& color() const;
    float lineWidth() const;
    float valueScale() const;
    
    void setColor(const std::array<float, 4>& pColor);
    void setLineWidth(float pLineWidth);
    void setValueScale(float pValueScale);
    
    void display(const ofShader& pShader);
    
protected:
    static std::array<float,4> sColor;
    static float sLineWidth;
    static float sValueScale;
    
    std::string mSpaceName;
	std::array<float, 4> mColor;
	float mLineWidth;
	float mValueScale;
    
    std::vector<glm::vec3> mGridVectors;
    std::vector<glm::vec3> mGridValues;
    std::vector<glm::vec3> mVertices;
    
    ofVbo mVbo;
    glm::mat4x4 mModelMatrix;
    
    void create() throw (Exception);
    
};
    
};
    
};

#endif
