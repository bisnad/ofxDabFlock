/** \file dab_flock_visual_swarm.h
*/

#ifndef _dab_flock_visual_swarm_h_
#define _dab_flock_visual_swarm_h_

#include <iostream>
#include <array>
#include "ofVectorMath.h"
#include "ofShader.h"
#include "dab_geom_geometry_group.h"

namespace dab
{

namespace flock
{

class VisAgentShape;
class VisAgentTrail;

class VisSwarm
{
public:
    VisSwarm(const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName);
    ~VisSwarm();
    
    const std::string& swarmName() const;
    const std::string& posParmName() const;
    const std::string& velParName() const;
    std::array<float,4> agentColor() const;
    glm::vec3 agentScale() const;
    float agentLineWidth() const;
    std::array<float,4> trailColor() const;
    int trailLength() const;
    float trailWidth() const;
    float trailDecay() const;
    
    std::vector< glm::vec3 >& agentPositions();
    std::vector< glm::vec3 >& agentVelocities();
    std::vector< VisAgentTrail* >& agentTrails();
    
    void createAgentShape(std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled);
    void createAgentTrail(unsigned int pMaxTrailSize);
    void setAgentColor(const std::array<float,4>& pColor);
    void setAgentScale(const glm::vec3& pScale);
    void setAgentLineWidth(float pLineWidth);
    void setTrailColor(const std::array<float,4>& pColor);
    void setTrailLength(int pLength);
    void setTrailWidth(float pWidth);
    void setTrailDecay(float pDecay);
    
    void setAgentCount(unsigned int pAgentCount);
    
    void update();
    void displayAgents(const ofShader &pShader);
    void displayTrails(const ofShader &pShader);
    
protected:
    std::string mSwarmName;
    std::string mPosParName;
    std::string mVelParName;
    
    std::vector< glm::vec3 > mAgentPositions;
    std::vector< glm::vec3 > mAgentVelocities;
    
    VisAgentShape* mAgentShape;
    VisAgentTrail* mAgentTrail;
    std::vector<VisAgentTrail*> mAgentTrails;
};

};
    
};

#endif
