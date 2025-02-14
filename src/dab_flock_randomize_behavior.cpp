/** \file dab_flock_randomize_behavior.cpp
 */

#include "dab_flock_randomize_behavior.h"
#include "dab_flock_agent.h"
#include "dab_math.h"

using namespace dab;
using namespace dab::flock;

RandomizeBehavior::RandomizeBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "RandomizeBehavior";
}

RandomizeBehavior::RandomizeBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "RandomizeBehavior";
	
	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// output parameter
	mRandomizePar = mOutputParameters[0];
	
	// create internal parameters
	mRangePar = createInternalParameter("range", mOutputParameters[0]->dim(), 1.0);
}

RandomizeBehavior::~RandomizeBehavior()
{}

Behavior*
RandomizeBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new RandomizeBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new RandomizeBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
RandomizeBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new RandomizeBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
RandomizeBehavior::act()
{
	//std::cout << "RandomizeBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& output = mRandomizePar->backupValues();
	Eigen::VectorXf& range = mRangePar->values();
    
	math::Math<>& math = math::Math<>::get();
	unsigned int dim = output.rows();
	
	for(unsigned int i=0; i<dim; ++i)
	{
		output[i] += math.uniformRandom(-range[i], range[i]);
	}
    
	//std::cout << "RandomizeBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	//assert(std::isnan(output[0]) == false && "isNan");
}