/** \file iso_flock_behavior.cpp
 */

#include "dab_flock_behavior.h"
#include "dab_flock_agent.h"
#include "dab_flock_swarm.h"
#include "dab_flock_parameter.h"
#include "dab_tokenizer.h"

using namespace dab;
using namespace dab::flock;

Behavior::Behavior()
: mAgent(nullptr)
{}

Behavior::Behavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: mAgent(nullptr)
, mClassName( "Behavior" )
, mInputParameterString(pInputParameterString)
, mOutputParameterString(pOutputParameterString)
{}

Behavior::Behavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
: mAgent(pAgent)
, mName(pBehaviorName)
, mClassName( "Behavior" )
, mInputParameterString(pInputParameterString)
, mOutputParameterString(pOutputParameterString)
{
	//std::cout << "Behavior name " << pBehaviorName.toStdString() << " agent name " << pAgent->name().toStdString() << "\n";
    
	mActivePar = createInternalParameter("active", { 1.0 });
    
	createInputParameters();
	createOutputParameters();
}

Behavior::~Behavior()
{
	mInputParameters.clear();
	mInputNeighborGroups.clear();
	mOutputParameters.clear();
	
	unsigned int internalParameterCount = mInternalParameters.size();
	for(unsigned int i=0; i<internalParameterCount; ++i)
	{
		mAgent->removeParameter( mInternalParameters[i]->name() );
	}
	mInternalParameters.clear();
}

void
Behavior::createInputParameters() throw (Exception)
{
	try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> inputParameterNames;
        tokenizer.split(mInputParameterString, inputParameterNames, ' ');
		unsigned int inputParameterCount = inputParameterNames.size();
        
		for(unsigned int i=0; i<inputParameterCount; ++i)
		{
			if( inputParameterNames[i].find('@') == std::string::npos ) // parameter_name
			{
				std::string parameterName = inputParameterNames[i];
				Parameter* parameter;
				
                // check if parameter is an AgentParameter
				if( mAgent->checkParameter( parameterName ) ) parameter = mAgent->parameter(parameterName);
				else // check if parameter is a SwarmParameter
				{
					Swarm* swarm = static_cast< Swarm* > (mAgent);
					if( swarm->checkSwarmParameter( parameterName ) ) parameter = swarm->swarmParameter( parameterName );
					else throw Exception( "FLOCK ERROR: parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
				}
                
				mInputParameters.push_back(parameter);
			}
			else // parameter_name @ space_name
			{
				std::string parameterName;
				std::string spaceName;
				std::string neighborParName;
				Parameter* parameter;
                
                std::vector<std::string> parSpacePairNames;
                tokenizer.split(inputParameterNames[i], parSpacePairNames, '@');
            
				parameterName = parSpacePairNames[0];
				
                // check if parameter is an AgentParameter
				if( mAgent->checkParameter( parameterName ) ) parameter = mAgent->parameter(parameterName);
				else // check if parameter is a SwarmParameter
				{
					Swarm* swarm = static_cast< Swarm* > (mAgent);
					if( swarm->checkSwarmParameter( parameterName ) ) parameter = swarm->swarmParameter( parameterName );
					else throw Exception( "FLOCK ERROR: parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
				}
				
				mInputParameters.push_back(parameter);
				
				if(parSpacePairNames.size() > 1) // neighbors requested
				{
					for(unsigned int j=1; j<parSpacePairNames.size(); ++j)
					{
						if( parSpacePairNames[j].find(':') != std::string::npos) // parameter_name @ space_name : neighbor_parameter_name
						{
                            std::vector< std::string > spaceNeighborParPairNames;
                            tokenizer.split(parSpacePairNames[j], spaceNeighborParPairNames, ':');
							spaceName = spaceNeighborParPairNames[0];
							neighborParName = spaceNeighborParPairNames[1];
						}
						else
						{
							spaceName = parSpacePairNames[j];
							neighborParName = "";
						}
                        
						
						if( parameter->checkNeighborGroup( spaceName ) == false ) throw Exception( "FLOCK ERROR: Parameter does not contain neighbor group " + spaceName, __FILE__, __FUNCTION__, __LINE__ );
						space::NeighborGroup* neighborGroup = parameter->neighborGroup(spaceName);
						mInputNeighborGroups.push_back( neighborGroup );
						mNeighborInputParameterNames.push_back( neighborParName );
					}
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create input parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Behavior::createOutputParameters() throw (Exception)
{
	try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> outputParameterNames;
        tokenizer.split(mOutputParameterString, outputParameterNames, ' ');
		unsigned int outputParameterCount = outputParameterNames.size();
		
		for(unsigned int i=0; i<outputParameterCount; ++i)
		{
			if( outputParameterNames[i].find('@') == std::string::npos ) // parameter_name
			{
				std::string parameterName = outputParameterNames[i];
				Parameter* parameter;
				
				if( mAgent->checkParameter( parameterName ) ) parameter = mAgent->parameter(parameterName);
				else
				{
					Swarm* swarm = static_cast< Swarm* > (mAgent);
					if( swarm->checkSwarmParameter( parameterName ) ) parameter = swarm->swarmParameter(parameterName);
					else throw Exception( "FLOCK ERROR: parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
				}
				
				mOutputParameters.push_back(parameter);
			}
			else // parameter_name @ space_name
			{
				std::string parameterName;
				std::string spaceName;
				std::string neighborParName;
				Parameter* parameter;
				
                std::vector<std::string> parSpacePairNames;
                tokenizer.split(outputParameterNames[i], parSpacePairNames, '@');
				parameterName = parSpacePairNames[0];
				
				if( mAgent->checkParameter( parameterName ) ) parameter = mAgent->parameter(parameterName);
				else
				{
					Swarm* swarm = static_cast< Swarm* > (mAgent);
					if( swarm->checkSwarmParameter( parameterName ) ) parameter = swarm->swarmParameter( parameterName );
					else throw Exception( "FLOCK ERROR: parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
				}
				
				mOutputParameters.push_back(parameter);
				
				if(parSpacePairNames.size() > 1) // neighbors requested
				{
					for(unsigned int j=1; j<parSpacePairNames.size(); ++j)
					{
						if( parSpacePairNames[j].find(':') != std::string::npos) // parameter_name @ space_name : neighbor_parameter_name
						{
                            std::vector<std::string> spaceNeighborParPairNames;
                            tokenizer.split(parSpacePairNames[j], spaceNeighborParPairNames, ':');
							spaceName = spaceNeighborParPairNames[0];
							neighborParName = spaceNeighborParPairNames[1];
						}
						else
						{
							spaceName = parSpacePairNames[j];
							neighborParName = "";
						}
						
						if( parameter->checkNeighborGroup( spaceName ) == false ) throw Exception( "FLOCK ERROR: Parameter does not contain neighbor group " + spaceName, __FILE__, __FUNCTION__, __LINE__ );
                        
						space::NeighborGroup* neighborGroup = parameter->neighborGroup(spaceName);
						mOutputNeighborGroups.push_back( neighborGroup );
						mNeighborOutputParameterNames.push_back( neighborParName );
					}
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create output parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Parameter*
Behavior::createInternalParameter(const std::string& pParameterName, const std::vector<float>& pValues)
{
	std::string fullParameterName =  mName + "_" + pParameterName;
	Parameter* internalParameter;
    
	// create internal parameter only if it doesn't exist already
	if( mAgent->checkParameter(fullParameterName) == true ) internalParameter = mAgent->parameter(fullParameterName);
	else
	{
		internalParameter = new Parameter(mAgent, fullParameterName, pValues);
		mInternalParameters.push_back(internalParameter);
		mAgent->addParameter(internalParameter);
	}
	
	return internalParameter;
}

Parameter*
Behavior::createInternalParameter(const std::string& pParameterName, int pDim, float pValue )
{
	std::string fullParameterName =  mName + "_" + pParameterName;
	Parameter* internalParameter;
    
	// create internal parameter only if it doesn't exist already
	if( mAgent->checkParameter(fullParameterName) == true ) internalParameter = mAgent->parameter(fullParameterName);
	else
	{
        Eigen::VectorXf values(pDim);
        values.setConstant(pValue);
        
		internalParameter = new Parameter(mAgent, fullParameterName, values);
		mInternalParameters.push_back(internalParameter);
		mAgent->addParameter(internalParameter);
	}
	
	return internalParameter;
}

Parameter*
Behavior::createInternalParameter(const std::string& pParameterName, const std::initializer_list<float>& pValues)
{
	std::string fullParameterName =  mName + "_" + pParameterName;
	Parameter* internalParameter;
    
	// create internal parameter only if it doesn't exist already
	if( mAgent->checkParameter(fullParameterName) == true ) internalParameter = mAgent->parameter(fullParameterName);
	else
	{
		internalParameter = new Parameter(mAgent, fullParameterName, pValues);
		mInternalParameters.push_back(internalParameter);
		mAgent->addParameter(internalParameter);
	}
	
	return internalParameter;
}

std::vector< Parameter* >&
Behavior::internalParameters()
{
	return mInternalParameters;
}

const std::string&
Behavior::name() const
{
	return mName;
}

std::string
Behavior::className() const
{
	return mClassName;
}

const std::string&
Behavior::inputParameterString() const
{
	return mInputParameterString;
}

const std::string&
Behavior::outputParameterString() const
{
	return mOutputParameterString;
}

Behavior::operator std::string() const
{
    return info();
}

std::string
Behavior::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << ": ";
	
	unsigned int inputParameterCount = mInputParameters.size();
	if(inputParameterCount > 0)
	{
        ss << "Input: [ ";
		for(unsigned int i=0; i<inputParameterCount; ++i) ss << mInputParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int outputParameterCount = mOutputParameters.size();
	if(outputParameterCount > 0)
	{
        ss << "Output: [ ";
		for(unsigned int i=0; i<outputParameterCount; ++i) ss << mOutputParameters[i]->name() << " ";
        ss << "] ";
	}
    
	unsigned int internalParameterCount = mInternalParameters.size();
	if(internalParameterCount > 0)
	{
        ss << "Internal: [ ";
		for(unsigned int i=0; i<internalParameterCount; ++i) ss << mInternalParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int inputNeighborGroupCount = mInputNeighborGroups.size();
	if(inputNeighborGroupCount > 0)
	{
        ss << "NeighborGroup: [ ";
		for(unsigned int i=0; i<inputNeighborGroupCount; ++i) ss << mInputNeighborGroups[i]->name() << " ";
		ss << "] ";
	}
    
	return ss.str();
}
