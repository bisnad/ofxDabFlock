/** \file dab_flock_parameter_combine_behavior.cpp
 */

#include "dab_flock_parameter_combine_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterCombineBehavior::ParameterCombineBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterCombineBehavior";
}

ParameterCombineBehavior::ParameterCombineBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterCombineBehavior";
	
	if( mInputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, at least " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	unsigned int outputDim = 0;
	for( unsigned int i=0; i<mInputParameters.size(); ++i ) outputDim += mInputParameters[i]->dim();
	if(mOutputParameters[0]->dim() != outputDim)  throw Exception( "FLOCK ERROR: illegal output parameter dim, is " + std::to_string( mOutputParameters[0]->dim()) + " should be " + std::to_string(outputDim), __FILE__, __FUNCTION__, __LINE__ );
	
	// output parameter
	mOutputPar = mOutputParameters[0];
    
	//std::cout << "scales " << mapValues << "\n";
}

ParameterCombineBehavior::~ParameterCombineBehavior()
{}

Behavior*
ParameterCombineBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ParameterCombineBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterCombineBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterCombineBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterCombineBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ParameterCombineBehavior::act()
{
    Eigen::VectorXf& outputValues = mOutputPar->backupValues();
    
	unsigned int inputParCount = mInputParameters.size();
	unsigned int inputParDim;
	for(unsigned int outIndex=0, inputParNr=0; inputParNr < inputParCount; ++inputParNr)
	{
        Eigen::VectorXf& inputValues = mInputParameters[inputParNr]->values();
		inputParDim = inputValues.rows();
		
		for(unsigned int inIndex=0; inIndex < inputParDim; ++inIndex, ++outIndex)
		{
			outputValues[outIndex] = inputValues[inIndex];
		}
	}
}