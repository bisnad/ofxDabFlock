/** \file dab_flock_evasion_behavior.cpp
 */

#include "dab_flock_evasion_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

EvasionBehavior::EvasionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EvasionBehavior";
}

EvasionBehavior::EvasionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EvasionBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPositionPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mMaxDistPar = createInternalParameter("maxDist", { 0.2f } );
	mAmountPar = createInternalParameter("amount", { 0.1f } );
	
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
	
	// remaining stuff
	unsigned int dim = mForcePar->dim();
	mTmpForce.resize(dim, 1);
}

EvasionBehavior::~EvasionBehavior()
{}

Behavior*
EvasionBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new EvasionBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new EvasionBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EvasionBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new EvasionBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
EvasionBehavior::act()
{
	//std::cout << "EvasionBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	
	if(mActivePar->value() <= 0.0) return;
    
    Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
	Eigen::VectorXf& tmpForce = mTmpForce;
	
	tmpForce.setConstant(0.0);
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	unsigned int neighborCount = 0;
	
	//std::cout << "evasion for agent " << mAgent->name().toStdString() << " pos " << position << " neighborCount " << totalNeighborCount << "\n";
	
	if(totalNeighborCount == 0) return;
    
	float distance;
	float scale;
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		const Eigen::VectorXf& direction = positionNeighbors.direction(i);
		distance = positionNeighbors.distance(i);
        
		if(maxDist > 0.0 && distance > maxDist) continue;
		
		scale = (maxDist - distance) / maxDist;
		
		tmpForce += direction * scale;
		neighborCount++;
		
		//std::cout << "   neighbor agent " << positionPar.neighbor(i)->agent()->name().toStdString() << " pos " << positionPar.neighbor(i)->values() << " dir " << direction << "\n";
	}
	
	if(neighborCount == 0) return;
	
	tmpForce /= static_cast<float>(neighborCount);
	tmpForce *= -1.0 * amount;
	
	//std::cout << "force " << tmpForce << "\n";
	
	force += tmpForce;
    
	//std::cout << "EvasionBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	
	//assert(std::isnan(force[0]) == false && "isNan");
}