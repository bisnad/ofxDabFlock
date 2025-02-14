/** \file iso_flock_env_behavior.cpp
 *
 *  Created by Daniel Bisig on 2/22/09.
 */

#include "dab_flock_env_behavior.h"
#include "dab_flock_env.h"
#include "dab_tokenizer.h"

using namespace dab;
using namespace dab::flock;

EnvBehavior::EnvBehavior()
	: Behavior()
	, mEnv(nullptr)
{
	mClassName = "EnvBehavior";
}

EnvBehavior::EnvBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
	: Behavior(pInputParameterString, pOutputParameterString)
	, mEnv(NULL)
{
	mClassName = "EnvBehavior";
}

EnvBehavior::EnvBehavior(Env* pEnv, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString)
	: Behavior(pInputParameterString, pOutputParameterString)
	, mEnv(pEnv)
{
	mName = pBehaviorName;
	mClassName = "EnvBehavior";
	mAgent = pEnv;
    
    mActivePar = createInternalParameter("active", { 1.0 });

	createInputParameters();
	createOutputParameters();
}
		
EnvBehavior::~EnvBehavior()
{
	mInputAgentParameterNames.clear();
	mOutputAgentParameterNames.clear();
}

void 
EnvBehavior::createInputParameters() throw (Exception)
{
	//std::cout << "EnvBehavior createInputParameters begin\n";

	try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> inputParameterNames;
        tokenizer.split(mInputParameterString, inputParameterNames, ' ');
		unsigned int inputParameterCount = inputParameterNames.size();
			
		for(unsigned int i=0; i<inputParameterCount; ++i)
		{
            // environment parameter
            std::vector<std::string> envParSplit;
            tokenizer.split(inputParameterNames[i], envParSplit, ':');
            unsigned int envParSplitCount = envParSplit.size();

			Parameter* envParameter = mEnv->parameter( envParSplit[0]);
			
			mInputParameters.push_back( envParameter );
			
			// agent parameters associated with env parameter
			if( dynamic_cast<EnvParameter*>(envParameter) != nullptr && envParSplitCount >= 2)
			{
                std::vector<std::string> agentParSplit;
                tokenizer.split(envParSplit[1], agentParSplit, ',');
				unsigned int agentParameterCount = agentParSplit.size();
                const std::string& envParameterName = envParameter->name();
                
                mInputAgentParameterNames[envParameterName] = std::vector<std::string>();
				
				for(unsigned int j=0; j<agentParameterCount; ++j) 
				{
                    mInputAgentParameterNames[envParameterName].push_back(agentParSplit[j]);
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create input parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
	//std::cout << "EnvBehavior createInputParameters end\n";
}

void 
EnvBehavior::createOutputParameters() throw (Exception)
{
	//std::cout << "EnvBehavior createOutputParameters begin\n";

	try
	{
        Tokenizer& tokenizer = Tokenizer::get();
        std::vector<std::string> outputParameterNames;
        tokenizer.split(mOutputParameterString, outputParameterNames, ' ');
		unsigned int outputParameterCount = outputParameterNames.size();
			
		for(unsigned int i=0; i<outputParameterCount; ++i)
		{
			// environment parameter
            std::vector<std::string> envParSplit;
            tokenizer.split(outputParameterNames[i], envParSplit, ':');
			unsigned int envParSplitCount = envParSplit.size();
			Parameter* envParameter = mEnv->parameter( envParSplit[0]);
			
			mOutputParameters.push_back( envParameter );
			
			// agent parameters associated with env parameter
			if( dynamic_cast<EnvParameter*>(envParameter) != NULL && envParSplitCount >= 2)
			{
                std::vector<std::string> agentParSplit;
                tokenizer.split(envParSplit[1], agentParSplit, ',');
                unsigned int agentParameterCount = agentParSplit.size();
                
                const std::string& envParameterName = envParameter->name();
				
                mOutputAgentParameterNames[envParameterName] = std::vector<std::string>();
                
				for(unsigned int j=0; j<agentParameterCount; ++j) 
				{
					mOutputAgentParameterNames[envParameterName].push_back(agentParSplit[j]);
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to create output parameters", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
	//std::cout << "EnvBehavior createOutputParameters end\n";
}	

EnvBehavior::operator std::string() const
{
	return info();
}

std::string
EnvBehavior::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << ": ";
	
	return ss.str();
}