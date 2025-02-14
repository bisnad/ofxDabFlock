/** \file dab_flock_env_gray_scott_behavior.cpp
 */

#include "dab_flock_env_gray_scott_behavior.h"
#include "dab_flock_env.h"
#include "dab_flock_swarm.h"

using namespace dab;
using namespace dab::flock;

EnvGrayScottBehavior::EnvGrayScottBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvGrayScottBehavior";
}

EnvGrayScottBehavior::EnvGrayScottBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pEnv, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvGrayScottBehavior";
	
	if( mInputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 2 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__ );

	// input parameter
	mInputChem1Par = dynamic_cast<EnvParameter*>( mInputParameters[0] );
	mInputChem2Par = dynamic_cast<EnvParameter*>( mInputParameters[1] );
    
	// output parameter
	mOutputChem1Par = dynamic_cast<EnvParameter*>( mOutputParameters[0] );
	mOutputChem2Par = dynamic_cast<EnvParameter*>( mOutputParameters[1] );
    
	if( mInputChem1Par == nullptr ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputChem2Par == nullptr ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[1]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputChem1Par == nullptr ) throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputChem2Par == nullptr ) throw Exception( "FLOCK ERROR: output parameter " + mOutputParameters[1]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
    
	unsigned int chemGridDim = mInputChem1Par->gridDim();
	unsigned int chemValueDim = mInputChem1Par->valueDim();
    const dab::Array<unsigned int>& chemGridSize = mInputChem1Par->gridSize();
	
	if( mInputChem2Par->gridDim() != chemGridDim || mOutputChem1Par->gridDim() != chemGridDim ||  mOutputChem2Par->gridDim() != chemGridDim) throw Exception( "FLOCK ERROR: grid dimensions of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputChem2Par->valueDim() != chemValueDim || mOutputChem1Par->valueDim() != chemValueDim ||  mOutputChem2Par->valueDim() != chemValueDim) throw Exception( "FLOCK ERROR: value dimensions of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputChem2Par->gridSize() != chemGridSize || mOutputChem1Par->gridSize() != chemGridSize ||  mOutputChem2Par->gridSize() != chemGridSize) throw Exception( "FLOCK ERROR: grid size of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	
	// create internal parameters
	mFPar = createInternalParameter("F", chemValueDim, 0.1 );
	mKPar = createInternalParameter("k", chemValueDim, 0.1 );
}

EnvGrayScottBehavior::~EnvGrayScottBehavior()
{}

Behavior*
EnvGrayScottBehavior::create(const std::string& pBehaviorName, Agent* pEnv) const
{
	try
	{
		Env* env = dynamic_cast<Env*>(pEnv);
        
		if(env != nullptr)
		{
			return new EnvGrayScottBehavior(env, pBehaviorName, mInputParameterString, mOutputParameterString);
		}
		else return new EnvGrayScottBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EnvGrayScottBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return new EnvGrayScottBehavior(pInputParameterString, pOutputParameterString);
}

void
EnvGrayScottBehavior::act()
{
	space::SpaceGrid* inputChem1Grid = mInputChem1Par->grid();
	space::SpaceGrid* inputChem2Grid = mInputChem2Par->grid();
	space::SpaceGrid* outputChem1Grid = mOutputChem1Par->backupGrid();
	space::SpaceGrid* outputChem2Grid = mOutputChem2Par->backupGrid();
    const Eigen::VectorXf& F = mFPar->values();
	const Eigen::VectorXf& k = mKPar->values();
	math::VectorField<float>& inputChem1Field = inputChem1Grid->vectorField();
	math::VectorField<float>& inputChem2Field = inputChem2Grid->vectorField();
	math::VectorField<float>& outputChem1Field = outputChem1Grid->vectorField();
	math::VectorField<float>& outputChem2Field = outputChem2Grid->vectorField();
    std::vector<Eigen::VectorXf>& inputChem1Vectors = inputChem1Field.vectors();
	std::vector<Eigen::VectorXf>& inputChem2Vectors = inputChem2Field.vectors();
	std::vector<Eigen::VectorXf>& outputChem1Vectors = outputChem1Field.vectors();
	std::vector<Eigen::VectorXf>& outputChem2Vectors = outputChem2Field.vectors();
    
	unsigned int vectorCount = inputChem1Field.vectorCount();
	unsigned int vectorDim = inputChem1Field.vectorDim();
    
	float conc1, conc2, conc122;
	for( int vI=0; vI<vectorCount; ++vI )
	{
		for( int d=0; d<vectorDim; ++d )
		{
			conc1 = inputChem1Vectors[vI][d];
			conc2 = inputChem2Vectors[vI][d];
			
			if( conc1 < 0.0 ) conc1 = 0.0;
			else if( conc1 > 1.0 ) conc1 = 1.0;
			if( conc2 < 0.0 ) conc2 = 0.0;
			else if( conc2 > 1.0 ) conc2 = 1.0;
            
			conc122 = conc1 * conc2 * conc2;
			outputChem1Vectors[vI][d] += F[d] * ( 1.0  - conc1 ) - ( conc122 );
			outputChem2Vectors[vI][d] += conc122 - ( F[d] + k[d] ) * conc2;
		}
	}	
}