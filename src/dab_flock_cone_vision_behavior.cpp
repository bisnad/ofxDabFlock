/** \file dab_flock_cone_vision_behavior.cpp
 */

#include "dab_flock_cone_vision_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::flock;

ConeVisionBehavior::ConeVisionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ConeVisionBehavior";
}

ConeVisionBehavior::ConeVisionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ConeVisionBehavior";
	
	if( mInputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mInputParameters[1]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ ) ;

    
	// input parameter
	mPositionInPar = mInputParameters[0];
	mVelocityPar = mInputParameters[1];
	
	// output parameter
	mPositionOutPar = mOutputParameters[0];
	
	// create internal parameters
	mVisionAnglePar = createInternalParameter("visionAngle", { 0.0 } );
	
	// input neighbor groups
	mPositionInNeighbors = mInputNeighborGroups[0];
    
	// input neighbor groups
	mPositionOutNeighbors = mOutputNeighborGroups[0];
	
	// remaining stuff
	unsigned int dim = mVelocityPar->dim();
	mNormVelocity.resize(dim,1);
	mNormNeighborDirection.resize(dim,1);
}

ConeVisionBehavior::~ConeVisionBehavior()
{}

Behavior*
ConeVisionBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ConeVisionBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ConeVisionBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ConeVisionBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ConeVisionBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ConeVisionBehavior::act()
{
	//std::cout << "ConeVisionBehavior::act() begin\n";
    
	//std::cout << "AlignmentBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	space::NeighborGroup& positionInNeighbors = *mPositionInNeighbors;
    Eigen::VectorXf& velocity = mVelocityPar->values();
	const std::string& velocityName = mVelocityPar->name();
	space::NeighborGroup& positionOutNeighbors = *mPositionOutNeighbors;
	space::SpaceNeighborRelation* neighbor;
	space::SpaceObject* spaceObject;
	
	positionOutNeighbors.removeNeighbors();
	
	float& visionAngle = mVisionAnglePar->value();
	
	mNormVelocity = velocity;
	mNormVelocity.normalize();
    
	
	unsigned int totalNeighborCount = positionInNeighbors.neighborCount();
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		neighbor = positionInNeighbors.neighborRelation(i);
		spaceObject = neighbor->neighbor();
		const Eigen::VectorXf& neighborDirection = neighbor->direction();
		
		mNormNeighborDirection = neighborDirection;
		mNormNeighborDirection.normalize();
		
		//std::cout << "vel " << velocity << " nVel " << neighborVelocity << " dot " << mNormVelocity.dot( mNormNeighVelocity ) << "\n";
		
		if( mNormVelocity.dot( mNormNeighborDirection ) < visionAngle ) continue;
		
		positionOutNeighbors.addNeighbor( spaceObject, neighbor->distance(), neighbor->direction() );
	}
	
	//std::cout << "in neighbors " <<  positionInNeighbors << "\n";
	//std::cout << "out neighbors " <<  positionOutNeighbors << "\n";
    
	//std::cout << "ConeVisionBehavior::act() end\n";
}