/** \file dab_flock_env_agent_interact_behavior.cpp
 */

#include "dab_flock_env_agent_interact_behavior.h"
#include "dab_flock_env.h"
#include "dab_flock_swarm.h"

using namespace dab;
using namespace dab::flock;

EnvAgentInteractBehavior::EnvAgentInteractBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvAgentInteractBehavior";
}

EnvAgentInteractBehavior::EnvAgentInteractBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: EnvBehavior(pEnv, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "EnvAgentInteractBehavior";
	
	// input parameter
	mEnvPar = dynamic_cast<EnvParameter*>( mInputParameters[0] );
	
	if( mEnvPar == nullptr ) throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " is not an environment parameter", __FILE__, __FUNCTION__, __LINE__ );
    
	mAgentParName = mInputAgentParameterNames[mInputParameters[0]->name()][0];
    
	// create internal parameters
	unsigned int envValueDim = mEnvPar->valueDim();
	mAmountPar = createInternalParameter("amount", envValueDim, 1.0 );
}

EnvAgentInteractBehavior::~EnvAgentInteractBehavior()
{}

Behavior*
EnvAgentInteractBehavior::create(const std::string& pBehaviorName, Agent* pEnv) const
{
	try
	{
		Env* env = dynamic_cast<Env*>(pEnv);
        
		if(env != nullptr)
		{
			return new EnvAgentInteractBehavior(env, pBehaviorName, mInputParameterString, mOutputParameterString);
		}
		else return new EnvAgentInteractBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
EnvAgentInteractBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return new EnvAgentInteractBehavior(pInputParameterString, pOutputParameterString);
}

void
EnvAgentInteractBehavior::act()
{
	//std::cout << "TerrainDepressionBehavior::act() begin\n";
	
	//space::SpaceGrid& envGrid = mEnvPar->backupGrid();
	const Eigen::VectorXf& amount = mAmountPar->values();
    
    std::shared_ptr<space::Space> envSpace = mEnvPar->space();
    std::vector<space::SpaceObject*> spaceObjects;
	mEnvPar->spaceObjects( spaceObjects );
    
	unsigned oC = spaceObjects.size();
	
	for(unsigned int oI=0; oI<oC; ++oI)
	{
		// filter space objects (only agent  parameters and no swarm parameters)
		Parameter* agentPositionPar = dynamic_cast<Parameter*>( spaceObjects[oI] );
		if(agentPositionPar == nullptr) continue;
		Agent* agent = agentPositionPar->agent();
		if( dynamic_cast<Swarm*>(agent) != nullptr ) continue;
		
		// get agent parameter values
		const Eigen::VectorXf& agentPosition = agentPositionPar->values();
		const Eigen::VectorXf& agentParValue = agent->parameter( mAgentParName )->values();
        
		//std::cout << "agent " << oI << " name " << agent->name().toStdString() << " par " << agentPositionPar->name().toStdString() << " value "<< agentPosition << " weight " << agentWeight << "\n";
        
		// change grid
		mEnvPar->change( agentPosition, agentParValue * amount, space::Interpol );
	}
	
	//mEnvPar->flush();
    
	//std::cout << "TerrainDepressionBehavior::act() end\n";
}