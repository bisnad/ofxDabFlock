/** \file dab_flock_parameter_scale_behavior.cpp
 */

#include "dab_flock_parameter_scale_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterScaleBehavior::ParameterScaleBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterScaleBehavior";
}

ParameterScaleBehavior::ParameterScaleBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterScaleBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mInputPar = mInputParameters[0];
	
	// output parameter
	mOutputPar = mOutputParameters[0];
	
	// create internal parameters
	mMinInPar = createInternalParameter("minInput", std::vector<float>( mInputPar->dim(), 0.0 ) );
	mMaxInPar = createInternalParameter("maxInput", std::vector<float>( mInputPar->dim(), 1.0 ) );
	mMinOutPar = createInternalParameter("minOutput", std::vector<float>( mInputPar->dim(), 0.0 ) );
	mMaxOutPar = createInternalParameter("maxOutput", std::vector<float>( mInputPar->dim(), 1.0 ) );
}

ParameterScaleBehavior::~ParameterScaleBehavior()
{}

Behavior*
ParameterScaleBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ParameterScaleBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterScaleBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterScaleBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterScaleBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ParameterScaleBehavior::act()
{
	//std::cout << "ParameterScaleBehavior " << mName.toStdString() << " act()\n";
    
    Eigen::VectorXf& inputValues = mInputPar->values();
	Eigen::VectorXf& outputValues = mOutputPar->backupValues();
	Eigen::VectorXf& minInValues = mMinInPar->values();
	Eigen::VectorXf& maxInValues = mMaxInPar->values();
	Eigen::VectorXf& minOutValues = mMinOutPar->values();
	Eigen::VectorXf& maxOutValues = mMaxOutPar->values();
	
	unsigned int parDim = inputValues.rows();
	float parValue;
	
	for(unsigned int d=0; d<parDim; ++d)
	{
		parValue = ( inputValues[d] - minInValues[d] ) / ( maxInValues[d] - minInValues[d] );
		
		if(parValue < 0.0) parValue = 0.0;
		if(parValue > 1.0) parValue = 1.0;
		
		outputValues[d] = minOutValues[d] + parValue * (maxOutValues[d] - minOutValues[d]);
	}
	
	//std::cout << "in " <<  inputValues << " out " <<  outputValues << "\n";
}