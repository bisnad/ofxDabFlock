/** \file dab_flock_target_parameter_behavior.cpp
 */

#include "dab_flock_target_parameter_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

TargetParameterBehavior::TargetParameterBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "TargetParameterBehavior";
}

TargetParameterBehavior::TargetParameterBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "TargetParameterBehavior";
	
	if(mInputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mInputPar = mInputParameters[0];
	
	// output parameter
	mOutputPar = mOutputParameters[0];
	
	// create internal parameters
    mTargetPar = createInternalParameter("target", std::vector<float>(mInputParameters[0]->dim(), 0.0));
    mAdaptPar = createInternalParameter("adapt", std::vector<float>(mInputParameters[0]->dim(), 1.0));
	mAmountPar = createInternalParameter("amount", { 0.1f } );
	mAbsPar = createInternalParameter("absolute", { 0.0f } );
    mDiffValues.resize(mInputParameters[0]->dim());
}

TargetParameterBehavior::~TargetParameterBehavior()
{}

Behavior*
TargetParameterBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new TargetParameterBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new TargetParameterBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
TargetParameterBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new TargetParameterBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
TargetParameterBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
	
    Eigen::VectorXf& inputValues = mInputPar->values();
	Eigen::VectorXf& outputValues = mOutputPar->backupValues();
	Eigen::VectorXf& targetValues = mTargetPar->values();
	Eigen::VectorXf& adaptValues = mAdaptPar->values();
    
	Eigen::VectorXf& diffValues = mDiffValues;
	float& amount = mAmountPar->value();
	float& absMode = mAbsPar->value();
	
	unsigned int dim = inputValues.rows();
	
	if(absMode == 0.0)
	{
		for(unsigned int d=0; d<dim; ++d)
		{
			if( adaptValues[d] > 0.0 )
			{
                diffValues[d] = targetValues[d] - inputValues[d];
				diffValues[d] *= adaptValues[d] * amount;
				outputValues[d] += diffValues[d];
			}
			else diffValues[d] = 0.0;
		}
	}
	else
	{
		for(unsigned int d=0; d<dim; ++d)
		{
			if( adaptValues[d] > 0.0 & inputValues[d] != 0.0)
			{
				diffValues[d] = ( fabs ( targetValues[d] ) - fabs( inputValues[d] ) ) * inputValues[d] / fabs( inputValues[d] );
				diffValues[d] *= adaptValues[d] * amount;
				outputValues[d] += diffValues[d];
                
			}
			else diffValues[d] = 0.0;
		}
	}
	
	//std::cout << "in " << inputValues << " target " << targetValues << " out " << outputValues << "\n";
}