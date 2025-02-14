/**	\file dab_flock_parameter_list.cpp
 */

#include "dab_space_neighbor_group_alg.h"
#include "dab_flock_parameter_list.h"
#include "dab_space_manager.h"

using namespace dab;
using namespace dab::flock;

const std::string ParameterList::sClassName = "ParameterList";

ParameterList::ParameterList()
{}

ParameterList::~ParameterList()
{
	unsigned int parameterCount = mParameters.size();
	
	for(unsigned int i=0; i<parameterCount; ++i)
	{
		Parameter* par = mParameters[i];
		delete par;
	}
	
	mParameters.clear();
}

unsigned int
ParameterList::parameterCount() const
{
	return mParameters.size();
}

bool
ParameterList::contains(const std::string& pName) const
{
	return mParameters.contains(pName);
}

unsigned int
ParameterList::parameterIndex(const std::string& pName) const throw (Exception)
{
	try
	{
		return mParameters.index(pName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: parameter " + pName + " not found", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Parameter*
ParameterList::parameter(unsigned int pIndex) throw (Exception)
{
	if( pIndex >= mParameters.size() ) throw Exception( "FLOCK ERROR: parameter index " + std::to_string(pIndex) + " out of bounds", __FILE__, __FUNCTION__, __LINE__ );
	
	return mParameters[pIndex];
}

const Parameter*
ParameterList::parameter(unsigned int pIndex) const throw (Exception)
{
	if( pIndex >= mParameters.size() ) throw Exception( "FLOCK ERROR: parameter index " + std::to_string(pIndex) + " out of bounds", __FILE__, __FUNCTION__, __LINE__ );
	
	return mParameters[pIndex];
}

Parameter*
ParameterList::parameter(const std::string& pName) throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	return mParameters[pName];
}

const Parameter*
ParameterList::parameter(const std::string& pName) const throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	return mParameters[pName];
}

void
ParameterList::addParameter(Parameter* pParameter) throw (Exception)
{
    if( mParameters.contains(pParameter->name()) == true ) throw Exception( "FLOCK ERROR: parameter name " + pParameter->name() + " already exists", __FILE__, __FUNCTION__, __LINE__ );
    
	mParameters.add(pParameter->name(), pParameter);
}

void
ParameterList::addParameter(Agent* pAgent, const std::string& pName, unsigned int pDim) throw (Exception)
{
    if( mParameters.contains(pName) == true ) throw Exception( "FLOCK ERROR: parameter name " + pName + " already exists", __FILE__, __FUNCTION__, __LINE__ );
    
	Parameter* par = new Parameter(pAgent, pName, pDim);
	mParameters.add(pName, par);
}

void
ParameterList::removeParameter(const std::string& pName) throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	Parameter* par = mParameters[pName];
	mParameters.remove(pName);
    
	delete par;
}

Eigen::VectorXf&
ParameterList::values(const std::string& pName) throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	return mParameters[pName]->values();
}

void
ParameterList::setValue(const std::string& pName, float pValue) throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	Parameter* par = mParameters[pName];
	par->setValue(pValue);
}

void
ParameterList::setValues(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception)
{
	if( mParameters.contains(pName) == false ) throw Exception( "FLOCK ERROR: parameter name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	Parameter* par = mParameters[pName];

    try
    {
        par->setValues(pValues);
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
ParameterList::flush()
{
	unsigned int parameterCount = mParameters.size();
	
	for(unsigned int i=0; i<parameterCount; ++i) mParameters[i]->flush();
}


ParameterList::operator std::string() const
{
    return info(0);
}

std::string
ParameterList::info(int pPropagationLevel) const
{
	std::stringstream ss;
    
	unsigned int parameterCount = mParameters.size();
	
	for(unsigned int i=0; i<parameterCount; ++i)
	{
		const Parameter* par = mParameters[i];
        
		if(pPropagationLevel == 0)
		{
            ss << par->name() << " dim: " << par->dim() << " values: " << par->values().transpose() << "\n";
		}
		else if(pPropagationLevel < 0 || pPropagationLevel >= 1)
		{
            ss << par->info(pPropagationLevel - 1) << "\n";
		}
	}
    
	return ss.str();
}