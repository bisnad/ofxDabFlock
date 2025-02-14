/** \file dab_flock_boundary_wrap_behavior.cpp
 */

#include "dab_flock_boundary_wrap_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

BoundaryWrapBehavior::BoundaryWrapBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryWrapBehavior";
}

BoundaryWrapBehavior::BoundaryWrapBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryWrapBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mPositionInPar = mInputParameters[0];
	
	// output parameter
	mPositionOutPar = mOutputParameters[0];
	
	// create internal parameters
	mLowerBoundaryPar = createInternalParameter("lowerBoundary", mInputParameters[0]->dim(), -1.0);
	mUpperBoundaryPar = createInternalParameter("upperBoundary", mInputParameters[0]->dim(), 1.0);
}

BoundaryWrapBehavior::~BoundaryWrapBehavior()
{}

Behavior*
BoundaryWrapBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new BoundaryWrapBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new BoundaryWrapBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
BoundaryWrapBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new BoundaryWrapBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
BoundaryWrapBehavior::act()
{
	//std::cout << "BoundaryRepulsionBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& inValues = mPositionInPar->values();
	Eigen::VectorXf& outValues = mPositionOutPar->backupValues();
	Eigen::VectorXf& lowerBoundary = mLowerBoundaryPar->values();
	Eigen::VectorXf& upperBoundary = mUpperBoundaryPar->values();
	
	Eigen::VectorXf boundarySize = upperBoundary - lowerBoundary;
	
	unsigned int dim = inValues.rows();
	
	outValues = inValues;
	
	for(unsigned int i=0; i<dim; ++i)
	{
		while(outValues[i] < lowerBoundary[i])
		{
			outValues[i] += boundarySize[i];
		}
		while(outValues[i] > upperBoundary[i])
		{
			outValues[i] -= boundarySize[i];
		}
	}
}