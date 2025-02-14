/** \file dab_flock_visual_swarm.cpp
*/

#include "dab_flock_visual_swarm.h"
#include "dab_flock_visual_agent_shape.h"
#include "dab_flock_visual_agent_trail.h"
#include "dab_flock_swarm.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

VisSwarm::VisSwarm(const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName)
: mSwarmName(pSwarmName)
, mPosParName(pPosParName)
, mVelParName(pVelParName)
, mAgentShape(nullptr)
, mAgentTrail(nullptr)
{}

VisSwarm::~VisSwarm()
{
    if(mAgentShape != nullptr) delete mAgentShape;
    if(mAgentTrail != nullptr) delete mAgentTrail;
    
    int trailCount = mAgentTrails.size();
    for(int tI=0; tI<trailCount; ++tI)
    {
        delete mAgentTrails[tI];
    }
    
    mAgentTrails.clear();
    mAgentPositions.clear();
    mAgentVelocities.clear();
}

const std::string&
VisSwarm::swarmName() const
{
    return mSwarmName;
}

const std::string&
VisSwarm::posParmName() const
{
    return mPosParName;
}

const std::string&
VisSwarm::velParName() const
{
    return mVelParName;
}

std::array<float,4>
VisSwarm::agentColor() const
{
    if(mAgentShape != nullptr) return mAgentShape->color();
    return std::array<float, 4>();
}

glm::vec3
VisSwarm::agentScale() const
{
    if(mAgentShape != nullptr) return mAgentShape->scale();
    return glm::vec3();
}

float
VisSwarm::agentLineWidth() const
{
    if(mAgentShape != nullptr) return mAgentShape->lineWidth();
    return 0.0;
}

std::array<float,4>
VisSwarm::trailColor() const
{
    if(mAgentTrail != nullptr) return mAgentTrail->color();
    return std::array<float, 4>();
}

int
VisSwarm::trailLength() const
{
    if(mAgentTrail != nullptr) return mAgentTrail->length();
    return 0;
}

float
VisSwarm::trailWidth() const
{
    if(mAgentTrail != nullptr) return mAgentTrail->lineWidth();
    return 0.0;
}

float
VisSwarm::trailDecay() const
{
    if(mAgentTrail != nullptr) return mAgentTrail->decay();
    return 0.0;
}

std::vector<glm::vec3>&
VisSwarm::agentPositions()
{
    return mAgentPositions;
}

std::vector<glm::vec3>&
VisSwarm::agentVelocities()
{
    return mAgentVelocities;
}

std::vector< VisAgentTrail*>&
VisSwarm::agentTrails()
{
    return mAgentTrails;
}

void
VisSwarm::createAgentShape(std::shared_ptr<geom::GeometryGroup> pGeometryGroup, const std::vector< bool >& pFilled)
{
    mAgentShape = new VisAgentShape(pGeometryGroup, pFilled);
}

void
VisSwarm::createAgentTrail(unsigned int pMaxTrailSize)
{
    mAgentTrail = new VisAgentTrail(pMaxTrailSize);
}

void
VisSwarm::setAgentColor(const std::array<float,4>& pColor)
{
    if(mAgentShape == nullptr) return;
    
    mAgentShape->setColor(pColor);
}

void
VisSwarm::setAgentScale(const glm::vec3& pScale)
{
    if(mAgentShape == nullptr) return;
    
    mAgentShape->setScale(pScale);
}

void
VisSwarm::setAgentLineWidth(float pLineWidth)
{
    if(mAgentShape == nullptr) return;
    
    mAgentShape->setLineWidth(pLineWidth);
}

void
VisSwarm::setTrailColor(const std::array<float,4>& pColor)
{
    if(mAgentTrail == nullptr) return;
    
    mAgentTrail->setColor(pColor);
    
    int trailCount = mAgentTrails.size();
    
    for(int tI=0; tI<trailCount; ++tI)
    {
        mAgentTrails[tI]->setColor(pColor);
    }
}

void
VisSwarm::setTrailLength(int pLength)
{
    if(mAgentTrail == nullptr) return;
    
    mAgentTrail->setLength(pLength);
    
    int trailCount = mAgentTrails.size();
    
    for(int tI=0; tI<trailCount; ++tI)
    {
        mAgentTrails[tI]->setLength(pLength);
    }
}

void
VisSwarm::setTrailWidth(float pWidth)
{
    if(mAgentTrail == nullptr) return;
    
    mAgentTrail->setLineWidth(pWidth);
    
    int trailCount = mAgentTrails.size();
    
    for(int tI=0; tI<trailCount; ++tI)
    {
        mAgentTrails[tI]->setLineWidth(pWidth);
    }
}

void
VisSwarm::setTrailDecay(float pDecay)
{
    if(mAgentTrail == nullptr) return;
    
    mAgentTrail->setDecay(pDecay);
    
    int trailCount = mAgentTrails.size();
    
    for(int tI=0; tI<trailCount; ++tI)
    {
        mAgentTrails[tI]->setDecay(pDecay);
    }
}

void
VisSwarm::setAgentCount(unsigned int pAgentCount)
{
    if(mAgentPositions.size() == pAgentCount) return;
    
    mAgentPositions.resize(pAgentCount);
    mAgentVelocities.resize(pAgentCount);
    
    int trailCount = mAgentTrails.size();
    
    if(trailCount > pAgentCount)
    {
        for(int tI = trailCount - 1; tI>=(int)pAgentCount; --tI)
        {
            delete mAgentTrails[tI];
            mAgentTrails.erase(mAgentTrails.begin() + tI);
        }
    }
    else
    {
        for(int tI = trailCount; tI<pAgentCount; ++tI)
        {
            VisAgentTrail* newVisAgentTrail = new VisAgentTrail(mAgentTrail->maxLength());
            newVisAgentTrail->setColor(mAgentTrail->color());
            newVisAgentTrail->setDecay(mAgentTrail->decay());
            newVisAgentTrail->setLength(mAgentTrail->length());
            newVisAgentTrail->setLineWidth(mAgentTrail->lineWidth());
            
            mAgentTrails.push_back(newVisAgentTrail);
        }
    }
}

void
VisSwarm::update()
{
	Swarm* swarm;
	int agentCount;
	unsigned int posParIndex;
	unsigned int velParIndex;
	unsigned int posParDim;
    
    swarm = Simulation::get().swarm( mSwarmName );
    std::vector<Agent*>& agents = swarm->agents();
    agentCount = agents.size();
    
    if(agentCount != mAgentTrails.size()) setAgentCount(agentCount);
    if(agentCount == 0) return;
    
    posParIndex = agents[0]->parameterIndex( mPosParName );
    posParDim = agents[0]->parameter( posParIndex )->dim();
    if(posParDim > 3) posParDim = 3;
    
    if( mVelParName.empty() == false )
    {
        //std::cout << "posParName " << mPosParName << " posParIndex " << posParIndex << " velParName " << mVelParName << " velParIndex " << swarm->parameterIndex( mVelParName ) << "\n";
        
        velParIndex = swarm->parameterIndex( mVelParName );
        
        for(unsigned int aI=0; aI<agentCount; ++aI)
        {
            const Eigen::VectorXf& agentPosition = agents[aI]->parameter( posParIndex )->values();
            const Eigen::VectorXf& agentVelocity = agents[aI]->parameter( velParIndex )->values();
            
            for(unsigned int d=0; d<posParDim; ++d)
            {
                mAgentPositions[aI][d] = agentPosition[d];
                mAgentVelocities[aI][d] = agentVelocity[d];
            }
            
            mAgentTrails[aI]->update( mAgentPositions[aI] );
        }
    }
    else
    {
        for(unsigned int aI=0; aI<agentCount; ++aI)
        {
            const Eigen::VectorXf& agentPosition = agents[aI]->parameter( posParIndex )->values();
            
            for(unsigned int d=0; d<posParDim; ++d)
            {
                mAgentPositions[aI][d] = agentPosition[d];
            }
            
            mAgentTrails[aI]->update( mAgentPositions[aI] );
        }
    }
    
    //	std::cout << "GLPanel::updateVisSwarms() end\n";
    
}

void
VisSwarm::displayAgents(const ofShader &pShader)
{
	//std::cout << "displayAgents swarm " << mSwarmName << " pos " << mPosParName << " vel " << mVelParName << "\n";

    // display agent shapes
    glLineWidth(mAgentShape->lineWidth());
    int agentCount = mAgentPositions.size();
    for(int aI=0; aI<agentCount; ++aI)
    {
        const glm::vec3& agentPos = mAgentPositions[aI];
        const glm::vec3& agentVel = mAgentVelocities[aI];

        mAgentShape->setPosition(agentPos);
        mAgentShape->setOrientation(agentVel);
        mAgentShape->display(pShader);
        
        //if(mSwarmName == "mocap_swarm" && aI == 35) std::cout << "display Agent " << aI << " pos " << agentPos << "\n";
    }
}

void
VisSwarm::displayTrails(const ofShader &pShader)
{
    glLineWidth(mAgentTrail->lineWidth());
    int trailCount = mAgentTrails.size();
    
    for(int tI=0; tI<trailCount; ++tI)
    {
        mAgentTrails[tI]->display(pShader);
    }
}

