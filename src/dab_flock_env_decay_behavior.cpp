/** \file dab_flock_env_decay_behavior.cpp
 */

#include "dab_flock_env_decay_behavior.h"
#include "dab_flock_env.h"
#include "dab_flock_swarm.h"

using namespace dab;
using namespace dab::flock;

EnvDecayBehavior::EnvDecayBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvDecayBehavior";
}

EnvDecayBehavior::EnvDecayBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pEnv, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvDecayBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mInputEnvPar = dynamic_cast<EnvParameter*>( mInputParameters[0] );
    
	// output parameter
	mOutputEnvPar = dynamic_cast<EnvParameter*>( mOutputParameters[0] );
    
	if( mInputEnvPar == nullptr ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputEnvPar == nullptr ) throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
    
	unsigned int inputGridDim = mInputEnvPar->gridDim();
	unsigned int inputValueDim = mInputEnvPar->valueDim();
	const dab::Array<unsigned int>& inputGridSize = mInputEnvPar->gridSize();
    
	unsigned int outputGridDim = mOutputEnvPar->gridDim();
	unsigned int outputValueDim = mOutputEnvPar->valueDim();
	const dab::Array<unsigned int>& outputGridSize = mOutputEnvPar->gridSize();
    
	if( inputGridDim != outputGridDim ) throw Exception( "FLOCK ERROR: input environment parameter " + mInputEnvPar->name() + " grid dimension " + std::to_string(inputGridDim) + " does not match output environment parameter " + mOutputEnvPar->name() + " grid dimension " + std::to_string(outputGridDim), __FILE__, __FUNCTION__, __LINE__ );
	if( inputValueDim != outputValueDim ) throw Exception( "FLOCK ERROR: input environment parameter " + mInputEnvPar->name() + " value dimension " + std::to_string(inputValueDim) + " does not match output environment parameter " + mOutputEnvPar->name() + " value dimension " + std::to_string(outputValueDim), __FILE__, __FUNCTION__, __LINE__ );
	if( inputGridSize != outputGridSize ) throw Exception( "FLOCK ERROR: input environment parameter " + mInputEnvPar->name() + " grid size " + inputGridSize.info() + " does not match output environment parameter " + mOutputEnvPar->name() + " grid size " + outputGridSize.info(), __FILE__, __FUNCTION__, __LINE__ );
	
	// create internal parameters
	mDecayPar = createInternalParameter("decay", { 0.01f } );
}

EnvDecayBehavior::~EnvDecayBehavior()
{}

Behavior*
EnvDecayBehavior::create(const std::string& pBehaviorName, Agent* pEnv) const
{
	try
	{
		Env* env = dynamic_cast<Env*>(pEnv);
        
		if(env != nullptr)
		{
			return new EnvDecayBehavior(env, pBehaviorName, mInputParameterString, mOutputParameterString);
		}
		else return new EnvDecayBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EnvDecayBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return new EnvDecayBehavior(pInputParameterString, pOutputParameterString);
}

void
EnvDecayBehavior::act()
{
	space::SpaceGrid* inputGrid = mInputEnvPar->grid();
	space::SpaceGrid* outputGrid = mOutputEnvPar->backupGrid();
    Eigen::VectorXf decay = mDecayPar->values();
    
	math::VectorField<float>& inputField = inputGrid->vectorField();
	math::VectorField<float>& outputField = outputGrid->vectorField();
    std::vector<Eigen::VectorXf>& inputVectors = inputField.vectors();
	std::vector<Eigen::VectorXf>& outputVectors = outputField.vectors();
	unsigned int vectorCount = inputField.vectorCount();
	unsigned int vectorDim = inputField.vectorDim();
	
	for( int vI=0; vI<vectorCount; ++vI )
	{
		for( int d=0; d<vectorDim; ++d )
		{
			outputVectors[vI][d] -= inputVectors[vI][d] * decay[d];
			
			//std::cout << "vI " << vI << " decay " << decay[d] << " in " <<  inputVectors[vI][d] << " out " << outputVectors[vI][d] << "\n";
		}
	}
}