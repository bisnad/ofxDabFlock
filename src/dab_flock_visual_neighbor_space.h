/** \file dab_flock_visual_neighbor_space.h
 */

#ifndef _dab_flock_visual_neighbor_space_h_
#define _dab_flock_visual_neighbor_space_h_

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
        
        class VisNeighborSpace
        {
        public:
            friend class FlockVisuals;
            
            VisNeighborSpace(const std::string& pSpaceName);
            ~VisNeighborSpace();
            
            const std::string& spaceName() const;
            const std::array<float, 4>& color() const;
            float lineWidth() const;
            
            void setColor(const std::array<float, 4>& pColor);
            void setLineWidth(float pLineWidth);
            
            void display(const ofShader& pShader);
            
        protected:
            static std::array<float,4> sColor;
            static float sLineWidth;
            
            std::string mSpaceName;
            std::array<float, 4> mColor;
            float mLineWidth;
            
            std::vector<glm::vec3> mVertices;
            std::vector<ofIndexType> mIndices;
            
            ofVbo mVbo;
			glm::mat4x4 mModelMatrix;
            
            void create() throw (Exception);
        };
        
    };
    
};

#endif
