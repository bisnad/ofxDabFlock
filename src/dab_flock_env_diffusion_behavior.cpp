/** \file dab_flock_env_diffusion_behavior.cpp
 */

#include "dab_flock_env_diffusion_behavior.h"
#include "dab_flock_env.h"
#include "dab_flock_swarm.h"

using namespace dab;
using namespace dab::flock;

EnvDiffusionBehavior::EnvDiffusionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvDiffusionBehavior";
}

EnvDiffusionBehavior::EnvDiffusionBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pEnv, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvDiffusionBehavior";
	
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mInputEnvPar = dynamic_cast<EnvParameter*>( mInputParameters[0] );
    
	// output parameter
	mOutputEnvPar = dynamic_cast<EnvParameter*>( mOutputParameters[0] );
    
	if( mInputEnvPar == NULL ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputEnvPar == NULL ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[1]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
    
	unsigned int inputGridDim = mInputEnvPar->gridDim();
	unsigned int inputValueDim = mInputEnvPar->valueDim();
	const dab::Array<unsigned int>& inputGridSize = mInputEnvPar->gridSize();
	
	if( mOutputEnvPar->gridDim() != inputGridDim ) throw Exception( "FLOCK ERROR: grid dimensions of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputEnvPar->valueDim() != inputValueDim ) throw Exception( "FLOCK ERROR: value dimensions of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputEnvPar->gridSize() != inputGridSize ) throw Exception( "FLOCK ERROR: grid size of parameters don't match", __FILE__, __FUNCTION__, __LINE__ );
	
	// create internal parameter
	mDiffusionPar = createInternalParameter("diffusion", inputValueDim, 0.01 );
}

EnvDiffusionBehavior::~EnvDiffusionBehavior()
{}

Behavior*
EnvDiffusionBehavior::create(const std::string& pBehaviorName, Agent* pEnv) const
{
	try
	{
		Env* env = dynamic_cast<Env*>(pEnv);
        
		if(env != NULL)
		{
			return new EnvDiffusionBehavior(env, pBehaviorName, mInputParameterString, mOutputParameterString);
		}
		else return new EnvDiffusionBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EnvDiffusionBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return new EnvDiffusionBehavior(pInputParameterString, pOutputParameterString);
}

void
EnvDiffusionBehavior::act()
{
	space::SpaceGrid* inputGrid = mInputEnvPar->grid();
	space::SpaceGrid* outputGrid = mOutputEnvPar->backupGrid();
    const Eigen::VectorXf& diffusion = mDiffusionPar->values();
    
	math::VectorField<float>& inputField = inputGrid->vectorField();
	math::VectorField<float>& outputField = outputGrid->vectorField();
    std::vector<Eigen::VectorXf>& inputVectors = inputField.vectors();
	std::vector<Eigen::VectorXf>& outputVectors = outputField.vectors();
	unsigned int vectorCount = inputField.vectorCount();
	unsigned int vectorDim = inputField.vectorDim();
	
	const dab::Array<unsigned int>& gridSize = inputField.size();
	
	// 2D grids only for the moment
	if( gridSize.size() != 2 ) return;
    
	int vI;
	int gridWidth = gridSize[0];
	int gridHeight = gridSize[1];
	int gridWidth_1 = gridWidth - 1;
	int gridHeight_1 = gridHeight - 1;
    Eigen::VectorXf gridValue( vectorDim );
	
	// boundary conditions : corners
	// top left
	vI = 0;
	//std::cout << "top left vI\n" << vI << "\n";
	for(int d=0; d<vectorDim; ++d)
	{
		outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI + 1][d] + inputVectors[vI + gridWidth][d] - 2.0 * inputVectors[vI][d] );
	}
	// top right
	vI = gridWidth_1;
	//std::cout << "top right vI\n" << vI << "\n";
	for(int d=0; d<vectorDim; ++d)
	{
		outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI + gridWidth][d] - 2.0 * inputVectors[vI][d] );
	}
	// bottom left
	vI = gridHeight_1 * gridWidth;
	//std::cout << "bottom left vI\n" << vI << "\n";
	for(int d=0; d<vectorDim; ++d)
	{
		outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI + 1][d] + inputVectors[vI - gridWidth][d] - 2.0 * inputVectors[vI][d] );
	}
	// bottom right
	vI = gridHeight_1 * gridWidth + gridWidth_1;
	//std::cout << "bottom right vI\n" << vI << "\n";
	for(int d=0; d<vectorDim; ++d)
	{
		outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI - gridWidth][d] - 2.0 * inputVectors[vI][d] );
	}
	// boundary conditions : edges
	// top border
	vI = 1;
	//std::cout << "top border vI\n";
	for( int x=1; x < gridWidth_1; ++x, ++vI )
	{
		//std::cout << vI << " ";
		for(int d=0; d<vectorDim; ++d)
		{
			outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI + 1][d] + inputVectors[vI + gridWidth][d] - 3.0 * inputVectors[vI][d] );
		}
	}
	//std::cout << "\n";
	// bottom border
	vI = gridHeight_1 * gridWidth + 1;
	//std::cout << "bottom border vI\n";
	for( int x=1; x < gridWidth_1; ++x, ++vI )
	{
		//std::cout << vI << " ";
		for(int d=0; d<vectorDim; ++d)
		{
			outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI + 1][d] + inputVectors[vI - gridWidth][d] - 3.0 * inputVectors[vI][d] );
		}
	}
	//std::cout << "\n";
	// left border
	vI = gridWidth;
	//std::cout << "left border vI\n";
	for( int y=1; y < gridHeight_1; ++y, vI += gridWidth )
	{
		//std::cout << vI << " ";
		for(int d=0; d<vectorDim; ++d)
		{
			outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI + 1][d] + inputVectors[vI - gridWidth][d] + inputVectors[vI + gridWidth][d] - 3.0 * inputVectors[vI][d] );
		}
	}
	//std::cout << "\n";
	// right border
	vI = gridWidth + gridWidth_1;
	//std::cout << "right border vI\n";
	for( int y=1; y < gridHeight_1; ++y, vI += gridWidth )
	{
		//std::cout << vI << " ";
		for(int d=0; d<vectorDim; ++d)
		{
			outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI - gridWidth][d] + inputVectors[vI + gridWidth][d] - 3.0 * inputVectors[vI][d] );
		}
	}
	
	//std::cout << "\n";
	// remaining grid
	vI = gridWidth + 1;
	//std::cout << "remaining grid vI\n";
	for( int y=1; y<gridHeight_1; ++y )
	{
		for( int x=1; x<gridWidth_1; ++x, ++vI )
		{
			//std::cout << vI << " ";
			for(int d=0; d<vectorDim; ++d)
			{
				outputVectors[vI][d] += diffusion[d] * ( inputVectors[vI - 1][d] + inputVectors[vI + 1][d] + inputVectors[vI - gridWidth][d] + inputVectors[vI + gridWidth][d] - 4.0 * inputVectors[vI][d] );
			}			
		}
		//std::cout << "\n";
		
		vI += 2;
	}
	//std::cout << "\n";
	
	//mOutputEnvPar->flush();
}