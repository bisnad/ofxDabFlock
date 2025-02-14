/** \file dab_flock_boundary_repulsion_behavior.cpp
 */

#include "dab_flock_boundary_repulsion_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

BoundaryRepulsionBehavior::BoundaryRepulsionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryRepulsionBehavior";
}

BoundaryRepulsionBehavior::BoundaryRepulsionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryRepulsionBehavior";
	
	if(mInputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );

	// input parameter
	mPositionPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mLowerBoundaryPar = createInternalParameter("lowerBoundary", mInputParameters[0]->dim(), -1.0);
	mUpperBoundaryPar = createInternalParameter("upperBoundary", mInputParameters[0]->dim(), 1.0);
	mMaxDistPar = createInternalParameter("maxDist", { 1.0f } );
	mAmountPar = createInternalParameter("amount", { 0.1f });
    
	// remaining stuff
	unsigned int dim = mForcePar->dim();
	mTmpForce.resize(dim, 1);
}

BoundaryRepulsionBehavior::~BoundaryRepulsionBehavior()
{}

Behavior*
BoundaryRepulsionBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new BoundaryRepulsionBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new BoundaryRepulsionBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
BoundaryRepulsionBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new BoundaryRepulsionBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
BoundaryRepulsionBehavior::act()
{
	//std::cout << "BoundaryRepulsionBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& position = mPositionPar->values();
	Eigen::VectorXf& force = mForcePar->backupValues();
	Eigen::VectorXf& lowerBoundary = mLowerBoundaryPar->values();
	Eigen::VectorXf& upperBoundary = mUpperBoundaryPar->values();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
	Eigen::VectorXf& tmpForce = mTmpForce;
	
	tmpForce.setConstant(0.0);
	
	unsigned int dim = position.rows();
	
	for(unsigned int i=0; i<dim; ++i)
	{
		if(position[i] < lowerBoundary[i])
		{
			tmpForce[i] += fabs ( lowerBoundary[i] - position[i]) * amount;
		}
		else if(position[i] < lowerBoundary[i] + maxDist)
		{
			tmpForce[i] += fabs ( (position[i] - lowerBoundary[i]) / maxDist ) * amount;
		}
		
		if(position[i] > upperBoundary[i] )
		{
			tmpForce[i] -= fabs ( upperBoundary[i] - position[i] ) * amount;
		}
		else if(position[i] > upperBoundary[i] - maxDist)
		{
			tmpForce[i] -= fabs ( (position[i] - upperBoundary[i]) / maxDist ) * amount;
		}
	}
	
	//std::cout << "boundary repulsion lowerB " << lowerBoundary << " upperB " << upperBoundary << " maxDist " << maxDist << " amount " << amount << " pos " << position << " force " << tmpForce << "\n";
	
	force += tmpForce;
    
	//std::cout << "BoundaryRepulsionBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	//assert(std::isnan(force[0]) == false && "isNan");
}