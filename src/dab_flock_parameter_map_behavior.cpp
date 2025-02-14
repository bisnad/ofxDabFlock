/** \file dab_flock_parameter_map_behavior.cpp
 */

#include "dab_flock_parameter_map_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterMapBehavior::ParameterMapBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterMapBehavior";
}

ParameterMapBehavior::ParameterMapBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ParameterMapBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );

	// input parameter
	mInputPar = mInputParameters[0];
	
	// output parameter
	mOutputPar = mOutputParameters[0];
	
	// create internal parameters
	mMapPar = createInternalParameter("map", std::vector<float>( mInputPar->dim() * mOutputPar->dim(), 0.0 ) );
	Eigen::VectorXf& mapValues = mMapPar->values();
	
	if(mInputPar->dim() < mOutputPar->dim()) for(unsigned int i=0; i<mInputPar->dim(); ++i ) mapValues[i * mInputPar->dim() + i] = 1.0;
	else for(unsigned int i=0; i<mOutputPar->dim(); ++i ) mapValues[i * mInputPar->dim() + i] = 1.0;
	
	//std::cout << "scales " << mapValues << "\n";
}

ParameterMapBehavior::~ParameterMapBehavior()
{}

Behavior*
ParameterMapBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ParameterMapBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterMapBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterMapBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterMapBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ParameterMapBehavior::act()
{
    Eigen::VectorXf& inputValues = mInputPar->values();
	Eigen::VectorXf& outputValues = mOutputPar->backupValues();
	Eigen::VectorXf& mapValues = mMapPar->values();
    
	unsigned int inputDim = inputValues.rows();
	unsigned int outputDim = outputValues.rows();
    
	/*
     for(unsigned int outIndex=0, scaleIndex=0; outIndex < outputDim; ++outIndex, scaleIndex += )
     {
     if(std::isnan(mapValues[scaleIndex]) == false) outputValues[outIndex] = 0.0;
     }
     */
	
	for(unsigned int scaleIndex=0, outIndex=0; outIndex < outputDim; ++outIndex)
	{
		for(unsigned int inIndex=0; inIndex < inputDim; ++scaleIndex, ++inIndex)
		{
			if(std::isnan(mapValues[scaleIndex]) == false) outputValues[outIndex] += inputValues[inIndex] * mapValues[scaleIndex];
            
			//std::cout << "inI " << inIndex << " outI " << outIndex << " scaleI " << scaleIndex << " inV " << inputValues[inIndex] << " scaleV " << mapValues[scaleIndex] << " outV " << outputValues[outIndex] << "\n";
		}
	}
	
	//std::cout << "--------------------\n";
}