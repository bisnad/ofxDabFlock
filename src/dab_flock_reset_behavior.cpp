/** \file dab_flock_reset_behavior.cpp
 */

#include "dab_flock_reset_behavior.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ResetBehavior::ResetBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "ResetBehavior";
}

ResetBehavior::ResetBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "ResetBehavior";

	if(mOutputParameters.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );

	// output parameter
	mResetPar = mOutputParameters[0];
	
	// create internal parameters
	mResetValuePar = createInternalParameter("resetValue", mResetPar->dim(), 0.0);
	mResetAmountPar = createInternalParameter("resetAmount", mResetPar->dim(), 1.0);
    
    // other stuff
    int dim = mResetPar->dim();
    
    mUnitVector.resize(dim, 1);
    mUnitVector.setConstant(1.0);
}

ResetBehavior::~ResetBehavior()
{}

Behavior*
ResetBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new ResetBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ResetBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
ResetBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ResetBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
ResetBehavior::act()
{
	//std::cout << "ResetBehavior begin: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	if(mActivePar->value() <= 0.0) return;
    
	Eigen::VectorXf& origValue = mResetPar->values();
	Eigen::VectorXf& newValue = mResetPar->backupValues();
	Eigen::VectorXf& resetValue = mResetValuePar->values();
	Eigen::VectorXf& resetAmount = mResetAmountPar->values();
    Eigen::VectorXf& unitVector = mUnitVector;
    
    newValue = resetAmount.cwiseProduct(resetValue) + origValue.cwiseProduct(resetAmount - unitVector);
	
	//std::cout << "par " << mResetPar->name().toStdString() << " amt " << resetAmount << " orig " << origValue << " new " << newValue << "\n";
    
	//std::cout << "ResetBehavior end: out values" << mOutputParameters[0]->values() << " bValues " << mOutputParameters[0]->backupValues() << "\n";
	//assert(std::isnan(valueOut[0]) == false && "isNan");
}