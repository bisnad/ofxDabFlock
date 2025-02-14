/** \file dab_flock_boundary_mirror_behavior.cpp
 */

#include "dab_flock_boundary_mirror_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

BoundaryMirrorBehavior::BoundaryMirrorBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryMirrorBehavior";
}

BoundaryMirrorBehavior::BoundaryMirrorBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "BoundaryMirrorBehavior";
	
	if( mInputParameters.size() < 3 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(3) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[1]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[1]->name() + " dim " + std::to_string(mInputParameters[1]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[2]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[2]->name() + " dim " + std::to_string(mInputParameters[2]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters[1]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[1]->name() + " dim " + std::to_string(mOutputParameters[1]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mDerivative0InPar = mInputParameters[0];
	mDerivative1InPar = mInputParameters[1];
	mDerivative2InPar = mInputParameters[2];
	
	// output parameter
	mDerivative1OutPar = mOutputParameters[0];
	mDerivative2OutPar = mOutputParameters[1];
	
	// create internal parameters
	mLowerBoundaryPar = createInternalParameter("lowerBoundary", mDerivative0InPar->dim(), -1.0);
	mUpperBoundaryPar = createInternalParameter("upperBoundary", mDerivative0InPar->dim(), 1.0);
}

BoundaryMirrorBehavior::~BoundaryMirrorBehavior()
{}

Behavior*
BoundaryMirrorBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new BoundaryMirrorBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new BoundaryMirrorBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
BoundaryMirrorBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new BoundaryMirrorBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
BoundaryMirrorBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
	
    Eigen::VectorXf& derivative0In = mDerivative0InPar->values();
	Eigen::VectorXf& derivative1In = mDerivative1InPar->values();
	Eigen::VectorXf& derivative2In = mDerivative2InPar->values();
	Eigen::VectorXf& derivative1Out = mDerivative1OutPar->backupValues();
	Eigen::VectorXf& derivative2Out = mDerivative2OutPar->backupValues();
	Eigen::VectorXf& lowerBoundary = mLowerBoundaryPar->values();
	Eigen::VectorXf& upperBoundary = mUpperBoundaryPar->values();
	
	unsigned int dim = derivative0In.rows();
	
	for(unsigned int i=0; i<dim; ++i)
	{
		if((derivative0In[i] < lowerBoundary[i] && derivative1In[i] < 0.0) || (derivative0In[i] > upperBoundary[i] && derivative1In[i] > 0.0))
		{
			derivative1Out[i] -= 2.0 * derivative1In[i];
		}
        
		if((derivative0In[i] < lowerBoundary[i] && derivative2In[i] < 0.0) || (derivative0In[i] > upperBoundary[i] && derivative2In[i] > 0.0))
		{
			derivative2Out[i] -= 2.0 * derivative2In[i];
		}
	}
}