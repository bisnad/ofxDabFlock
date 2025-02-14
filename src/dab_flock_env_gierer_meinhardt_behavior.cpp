/** \file dab_flock_env_gierer_meinhardt_behavior.cpp
 */

#include "dab_flock_env_gierer_meinhardt_behavior.h"
#include "dab_flock_env.h"
#include "dab_flock_swarm.h"

using namespace dab;
using namespace dab::flock;

EnvGiererMeinhardtBehavior::EnvGiererMeinhardtBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvGiererMeinhardtBehavior";
}

EnvGiererMeinhardtBehavior::EnvGiererMeinhardtBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pEnv, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvGiererMeinhardtBehavior";
	
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
	mChem1ProdPar = createInternalParameter("chem1Production", chemValueDim, 0.01 );
	mChem2ProdPar = createInternalParameter("chem2Production", chemValueDim, 0.01 );
	mChem1DecayPar = createInternalParameter("chem1Decay", chemValueDim, 0.15 );
	mChem2DecayPar = createInternalParameter("chem2Decay", chemValueDim, 0.1 );
	mReactRatePar = createInternalParameter("reactionRate", chemValueDim, 0.1 );
}

EnvGiererMeinhardtBehavior::~EnvGiererMeinhardtBehavior()
{}

Behavior*
EnvGiererMeinhardtBehavior::create(const std::string& pBehaviorName, Agent* pEnv) const
{
	try
	{
		Env* env = dynamic_cast<Env*>(pEnv);
        
		if(env != NULL)
		{
			return new EnvGiererMeinhardtBehavior(env, pBehaviorName, mInputParameterString, mOutputParameterString);
		}
		else return new EnvGiererMeinhardtBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EnvGiererMeinhardtBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return new EnvGiererMeinhardtBehavior(pInputParameterString, pOutputParameterString);
}

void
EnvGiererMeinhardtBehavior::act()
{
	space::SpaceGrid* inputChem1Grid = mInputChem1Par->grid();
	space::SpaceGrid* inputChem2Grid = mInputChem2Par->grid();
	space::SpaceGrid* outputChem1Grid = mOutputChem1Par->backupGrid();
	space::SpaceGrid* outputChem2Grid = mOutputChem2Par->backupGrid();
    const Eigen::VectorXf& chem1Prod = mChem1ProdPar->values();
	const Eigen::VectorXf& chem2Prod = mChem2ProdPar->values();
	const Eigen::VectorXf& chem1Decay = mChem1DecayPar->values();
	const Eigen::VectorXf& chem2Decay = mChem2DecayPar->values();
	const Eigen::VectorXf& reactRate = mReactRatePar->values();
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
	float conc1, conc2, conc11;
	
	for( int vI=0; vI<vectorCount; ++vI )
	{
		for( int d=0; d<vectorDim; ++d )
		{
			conc1 = inputChem1Vectors[vI][d];
			conc2 = inputChem2Vectors[vI][d];
			
			if( conc1 < 0.0 ) conc1 = 0.0;
			if( conc2 < 0.0 ) conc2 = 0.0;
			
			conc11 = inputChem1Vectors[vI][d] * inputChem1Vectors[vI][d];
			outputChem1Vectors[vI][d] += reactRate[d] * conc11 / conc2 - chem1Decay[d] * conc1 + chem1Prod[d];
			outputChem2Vectors[vI][d] += reactRate[d] * conc11 - chem2Decay[d] * conc2 + chem2Prod[d];
		}
	}	
}