/** \file iso_flock_swarm_behavior.cpp
 */


#include "dab_flock_swarm_behavior.h"
#include "dab_flock_swarm.h"
#include "dab_tokenizer.h"

using namespace dab;
using namespace dab::flock;

SwarmBehavior::SwarmBehavior()
: Behavior()
, mSwarm(nullptr)
, mInputAgentParameters(mInputParameters)
, mInputAgentNeighborGroups(mInputNeighborGroups)
, mOutputAgentParameters(mOutputParameters)
, mOutputAgentNeighborGroups(mOutputNeighborGroups)
, mInternalAgentParameters(mInternalParameters)
{
	mClassName = "SwarmBehavior";
}

SwarmBehavior::SwarmBehavior(const std::string& pInputSwarmParameterString, const std::string& pInputAgentParameterString, const std::string& pOutputSwarmParameterString, const std::string& pOutputAgentParameterString)
: Behavior(pInputAgentParameterString, pOutputAgentParameterString)
, mSwarm(nullptr)
, mInputSwarmParameterString(pInputSwarmParameterString)
, mOutputSwarmParameterString(pOutputSwarmParameterString)
, mInputAgentParameters(mInputParameters)
, mInputAgentNeighborGroups(mInputNeighborGroups)
, mOutputAgentParameters(mOutputParameters)
, mOutputAgentNeighborGroups(mOutputNeighborGroups)
, mInternalAgentParameters(mInternalParameters)
{
	mClassName = "SwarmBehavior";
}

SwarmBehavior::SwarmBehavior(Swarm* pSwarm, const std::string& pSwarmBehaviorName, const std::string& pInputSwarmParameterString, const std::string& pInputAgentParameterString, const std::string& pOutputSwarmParameterString, const std::string& pOutputAgentParameterString)
: Behavior(pInputAgentParameterString, pOutputAgentParameterString)
, mSwarm(pSwarm)
, mInputSwarmParameterString(pInputSwarmParameterString)
, mOutputSwarmParameterString(pOutputSwarmParameterString)
, mInputAgentParameters(mInputParameters)
, mInputAgentNeighborGroups(mInputNeighborGroups)
, mOutputAgentParameters(mOutputParameters)
, mOutputAgentNeighborGroups(mOutputNeighborGroups)
, mInternalAgentParameters(mInternalParameters)
{
	mAgent = pSwarm;
	mName = pSwarmBehaviorName;
	mClassName = "SwarmBehavior";
    
    try
    {
        createInputParameters();
        createOutputParameters();
        createInputSwarmParameters();
        createOutputSwarmParameters();
        createAgentParameterIndices();
    }
    catch(Exception& e)
    {
        e += Exception("FLOCK ERROR: failed to create behavior " + pSwarmBehaviorName + " for swarm " + pSwarm->name(), __FILE__, __FUNCTION__, __LINE__);
        std::cout << e << "\n";
    }
}

Behavior*
SwarmBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	return create("", pInputParameterString, "", pOutputParameterString);
}

SwarmBehavior::~SwarmBehavior()
{
	mInputSwarmParameters.clear();
	mInputSwarmNeighborGroups.clear();
	mOutputSwarmParameters.clear();
	
	unsigned int internalSwarmParameterCount = mInternalSwarmParameters.size();
	for(unsigned int i=0; i<internalSwarmParameterCount; ++i) delete mInternalSwarmParameters[i];
	mInternalSwarmParameters.clear();
}

void
SwarmBehavior::createAgentParameterIndices() throw (Exception)
{
	// will be repeated every time before the beavior is actually executed
	// this is a safety measure in case the number of agent parameters has changed in the meantime
    
	try
	{
		// remove previous agent parameter indices
		mInputAgentParameterIndices.clear();
		mOutputAgentParameterIndices.clear();
		mInternalAgentParameterIndices.clear();
		mInputAgentNeighborGroupIndices.clear();
		mOutputAgentNeighborGroupIndices.clear();
		
		// add new agent parameter indices
		for(unsigned int parNr=0; parNr<mInputAgentParameters.size(); ++parNr) mInputAgentParameterIndices.push_back( mSwarm->parameterIndex( mInputAgentParameters[parNr]->name() ) );
		for(unsigned int parNr=0; parNr<mOutputAgentParameters.size(); ++parNr) mOutputAgentParameterIndices.push_back( mSwarm->parameterIndex( mOutputAgentParameters[parNr]->name() ) );
		for(unsigned int parNr=0; parNr<mInternalAgentParameters.size(); ++parNr) mInternalAgentParameterIndices.push_back( mSwarm->parameterIndex( mInternalAgentParameters[parNr]->name() ) );
		for(unsigned int neighborNr=0; neighborNr<mInputAgentNeighborGroups.size(); ++neighborNr)
		{
			space::NeighborGroup* neighborGroup = mInputAgentNeighborGroups[neighborNr];
			space::SpaceObject* spaceObject = neighborGroup->spaceObject();
			mInputAgentNeighborGroupIndices.push_back( spaceObject->neighborGroupIndex(neighborGroup->name()) );
		}
		for(unsigned int neighborNr=0; neighborNr<mOutputAgentNeighborGroups.size(); ++neighborNr)
		{
			space::NeighborGroup* neighborGroup = mOutputAgentNeighborGroups[neighborNr];
			space::SpaceObject* spaceObject = neighborGroup->spaceObject();
			mOutputAgentNeighborGroupIndices.push_back( spaceObject->neighborGroupIndex(neighborGroup->name()) );
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create Agent Parameter Indices for swarm " + mSwarm->name() + " behaviour " + mName, __FILE__, __FUNCTION__, __LINE__);
		std::cout << e << "\n";
	}
}

void
SwarmBehavior::createInputSwarmParameters() throw (Exception)
{
	try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> inputSwarmParameterNames;
        tokenizer.split(mInputSwarmParameterString, inputSwarmParameterNames, ' ');
		unsigned int inputSwarmParameterCount = inputSwarmParameterNames.size();
        
        
        for(unsigned int i=0; i<inputSwarmParameterCount; ++i)
		{
			if( inputSwarmParameterNames[i].find('@') == std::string::npos ) // parameter_name
			{
				std::string parameterName = inputSwarmParameterNames[i];
				Parameter* parameter;
				
                if( mSwarm->checkSwarmParameter( parameterName ) ) parameter = mSwarm->swarmParameter( parameterName );
                else throw Exception( "FLOCK ERROR: swarm parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
                
				mInputSwarmParameters.push_back(parameter);
			}
			else // parameter_name @ space_name
			{
				std::string parameterName;
				std::string spaceName;
				std::string neighborParName;
				Parameter* parameter;
                
                std::vector<std::string> parSpacePairNames;
                tokenizer.split(inputSwarmParameterNames[i], parSpacePairNames, '@');
                
				parameterName = parSpacePairNames[0];
				
                if( mSwarm->checkSwarmParameter( parameterName ) ) parameter = mSwarm->swarmParameter( parameterName );
                else throw Exception( "FLOCK ERROR: swarm parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
				
				mInputSwarmParameters.push_back(parameter);
				
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
						mInputSwarmNeighborGroups.push_back( neighborGroup );
					}
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create input swarm parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SwarmBehavior::createOutputSwarmParameters() throw (Exception)
{
    try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> outputSwarmParameterNames;
        tokenizer.split(mOutputSwarmParameterString, outputSwarmParameterNames, ' ');
		unsigned int outputSwarmParameterCount = outputSwarmParameterNames.size();
		
		for(unsigned int i=0; i<outputSwarmParameterCount; ++i)
		{
            std::string parameterName = outputSwarmParameterNames[i];
            Parameter* parameter;
            
            if( mSwarm->checkSwarmParameter( parameterName ) ) parameter = mSwarm->swarmParameter(parameterName);
            else throw Exception( "FLOCK ERROR: swarm parameter " + parameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
            
            mOutputSwarmParameters.push_back(parameter);
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create output parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Parameter*
SwarmBehavior::createInternalSwarmParameter(const std::string& pSwarmParameterName, const std::initializer_list<float>& pValues) throw (Exception)
{
	std::string fullSwarmParameterName( mName + "_" + pSwarmParameterName);
	Parameter* internalSwarmParameter;
    
	// create internal swarm parameter only if it doesn't exist already
	if( mSwarm->checkSwarmParameter(fullSwarmParameterName) == true ) internalSwarmParameter = mSwarm->swarmParameter(fullSwarmParameterName);
	else
	{
		internalSwarmParameter = new Parameter(mSwarm, fullSwarmParameterName, pValues);
		mInternalSwarmParameters.push_back(internalSwarmParameter);
		mSwarm->addSwarmParameter(internalSwarmParameter);
	}
	
	return internalSwarmParameter;
}

const std::string&
SwarmBehavior::inputSwarmParameterString() const
{
	return mInputSwarmParameterString;
}

const std::string&
SwarmBehavior::outputSwarmParameterString() const
{
	return mOutputSwarmParameterString;
}

const std::string&
SwarmBehavior::inputAgentParameterString() const
{
	return mInputParameterString;
}

const std::string&
SwarmBehavior::outputAgentParameterString() const
{
	return mOutputParameterString;
}

SwarmBehavior::operator std::string() const
{
    return info();
}

std::string
SwarmBehavior::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << ": ";

	unsigned int inputSwarmParameterCount = mInputSwarmParameters.size();
	if(inputSwarmParameterCount > 0)
	{
        ss << "Input Swarm: [ ";
		for(unsigned int i=0; i<inputSwarmParameterCount; ++i) ss << mInputSwarmParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int inputAgentParameterCount = mInputAgentParameters.size();
	if(inputAgentParameterCount > 0)
	{
		ss << "Input Agent: [ ";
		for(unsigned int i=0; i<inputAgentParameterCount; ++i) ss << mInputAgentParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int outputSwarmParameterCount = mOutputSwarmParameters.size();
	if(outputSwarmParameterCount > 0)
	{
		ss << "Output Swarm: [ ";
		for(unsigned int i=0; i<outputSwarmParameterCount; ++i) ss << mOutputSwarmParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int outputAgentParameterCount = mOutputAgentParameters.size();
	if(outputAgentParameterCount > 0)
	{
		ss << "Output Agent: [ ";
		for(unsigned int i=0; i<outputAgentParameterCount; ++i) ss << mOutputAgentParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int internalSwarmParameterCount = mInternalSwarmParameters.size();
	if(internalSwarmParameterCount > 0)
	{
		ss << "Internal Swarm: [ ";
		for(unsigned int i=0; i<internalSwarmParameterCount; ++i) ss << mInternalSwarmParameters[i]->name() << "\n";
		ss << "] ";
	}
    
	unsigned int internalAgentParameterCount = mInternalAgentParameters.size();
	if(internalAgentParameterCount > 0)
	{
		ss << "Internal Agent: [ ";
		for(unsigned int i=0; i<internalAgentParameterCount; ++i) ss << mInternalAgentParameters[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int inputSwarmNeighborGroupCount = mInputSwarmNeighborGroups.size();
	if(inputSwarmNeighborGroupCount > 0)
	{
		ss << "NeighborGroup Swarm: [ ";
		for(unsigned int i=0; i<inputSwarmNeighborGroupCount; ++i) ss << mInputSwarmNeighborGroups[i]->name() << " ";
		ss << "] ";
	}
    
	unsigned int inputAgentNeighborGroupCount = mInputAgentNeighborGroups.size();
	if(inputAgentNeighborGroupCount > 0)
	{
		ss << "NeighborGroup Agent: [ ";
		for(unsigned int i=0; i<inputAgentNeighborGroupCount; ++i) ss << mInputAgentNeighborGroups[i]->name() << " ";
		ss << "] ";
	}
    
	return ss.str();
}