/** \file dab_flock_spiral_behavior.cpp
 */

#include "dab_flock_spiral_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::flock;

SpiralBehavior::SpiralBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "SpiralBehavior";
}

SpiralBehavior::SpiralBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "SpiralBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != 3 )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must be 3", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters[0]->dim() != 3 )  throw Exception( "FLOCK EROR: output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()) + " must be 3", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPositionPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f } );
	mMaxDistPar = createInternalParameter("maxDist", { 0.5f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
    
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
}

SpiralBehavior::~SpiralBehavior()
{}

Behavior*
SpiralBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new SpiralBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new SpiralBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
SpiralBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new SpiralBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
SpiralBehavior::act()
{
	//std::cout << "SpiralBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
    Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
    
	Eigen::Vector3f& tmpForce = mTmpForce;
	
	tmpForce.setConstant(0.0);
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	unsigned int neighborCount = 0;
    
	if(totalNeighborCount == 0) return;
	
	space::SpaceNeighborRelation* neighborRelation1 = nullptr;
	space::SpaceNeighborRelation* neighborRelation2 = nullptr;
	float distance1, distance2;
	float scale;
	
	for(unsigned int i=0; i<totalNeighborCount - 1; ++i)
	{
		neighborRelation1 = positionNeighbors.neighborRelation(i);
		mDirection1 = neighborRelation1->direction();
		distance1 = neighborRelation1->distance();
        
		neighborRelation2 = positionNeighbors.neighborRelation(i + 1);
		mDirection2 = neighborRelation2->direction();
		distance2 = neighborRelation1->distance();
        
		if(minDist > 0.0 && distance1 < minDist) continue;
		if(maxDist > 0.0 && distance1 > maxDist) continue;
		if(minDist > 0.0 && distance2 < minDist) continue;
		if(maxDist > 0.0 && distance2 > maxDist) continue;
        
		scale = (maxDist - distance1) / maxDist * (maxDist - distance2) / maxDist;
        
		tmpForce += mDirection1.cross(mDirection2) * scale;
		
		neighborCount++;
		
		//std::cout << "   neighbor agent " << positionPar.neighbor(i)->agent()->name().toStdString() << " pos " << positionPar.neighbor(i)->values() << " dir " << direction << "\n";
	}
	
	if(neighborCount == 0) return;
	
	tmpForce /= static_cast<float>(neighborCount);
	tmpForce *= -1.0 * amount;
	
	//std::cout << "force " << tmpForce << "\n";
	
	force += tmpForce;
	
	//std::cout << "SpiralBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	//assert(std::isnan(force[0]) == false && "isNan");
}
