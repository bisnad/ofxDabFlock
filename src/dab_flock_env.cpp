/** \file dab_flock_env.cpp
 */

#include "dab_flock_env.h"
#include "dab_flock_simulation.h"
#include "dab_space_neighbor_group_alg.h"
#include "dab_exception.h"

using namespace dab;
using namespace dab::flock;

Env::Env()
: Agent()
, mDim(0)
{}

Env::Env(const std::string& pName, unsigned int pDim)
: Agent(pName)
, mDim(pDim)
{
	Simulation::get().addAgent(this);
	Simulation::get().addEnv(this);
}

Env::~Env()
{
	Simulation::get().removeAgent(this);
	Simulation::get().removeEnv(this);
}

unsigned int
Env::dim() const
{
	return mDim;
}

void
Env::addParameter(const std::string& pParameterName, unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
{
	if( pSubdivisionCount.size() != mDim ) throw Exception("FLOCK ERROR: subdivision dimension " + std::to_string(pSubdivisionCount.size()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	if( pMinPos.rows() != mDim ) throw Exception( "FLOCK ERROR: minPos dimension " + std::to_string(pMinPos.rows()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	if( pMaxPos.rows() != mDim ) throw Exception( "FLOCK ERROR: maxPos dimension " + std::to_string(pMaxPos.rows()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	
	try
	{
		EnvParameter* parameter = new EnvParameter( this, pParameterName, pValueDim, pSubdivisionCount, pMinPos, pMaxPos );
		addParameter(parameter);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::addParameter(const std::string& pParameterName, const Eigen::VectorXf& pValues, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
{
	if( pSubdivisionCount.size() != mDim ) throw Exception( "FLOCK ERROR: subdivision dimension " + std::to_string(pSubdivisionCount.size()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	if( pMinPos.rows() != mDim ) throw Exception( "FLOCK ERROR: minPos dimension " + std::to_string(pMinPos.rows()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );
	if( pMaxPos.rows() != mDim ) throw Exception( "FLOCK ERROR: maxPos dimension " + std::to_string(pMaxPos.rows()) + " doesn't match environment dimension " + std::to_string(mDim), __FILE__, __FUNCTION__, __LINE__ );

	try
	{
		EnvParameter* parameter = new EnvParameter( this, pParameterName, pValues, pSubdivisionCount, pMinPos, pMaxPos );
		addParameter(parameter);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::addParameter( std::shared_ptr<space::Space> pGridSpace, unsigned int pValueDim ) throw (Exception)
{
	try
	{
		EnvParameter* parameter = new EnvParameter( this, pGridSpace, pValueDim );
		addParameter(parameter);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::setParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pValues, const Eigen::VectorXf& pPosition, space::GridValueSetMode pSetMode) throw (Exception)
{
	try
	{
		Parameter* par = parameter(pParameterName);
		EnvParameter* envPar = dynamic_cast<EnvParameter*>( par );
		if(envPar != nullptr) envPar->set(pValues, pPosition, pSetMode);
		else par->set(pValues);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::changeParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pValues, const Eigen::VectorXf& pPosition, space::GridValueSetMode pSetMode) throw (Exception)
{
	try
	{
		Parameter* par = parameter(pParameterName);
		EnvParameter* envPar = dynamic_cast<EnvParameter*>( par );
		if(envPar != nullptr) envPar->change(pValues, pPosition, pSetMode);
		else par->change(pValues);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::set(const std::string& pParameterName, float pParameterValue) throw (Exception)
{
	try
	{
		Agent::set( pParameterName, pParameterValue );
	}
	catch( Exception& e )
	{
		throw;
	}
}

void
Env::set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception)
{
	try
	{
		Agent::set( pParameterName, pParameterValues );
	}
	catch( Exception& e )
	{
		throw;
	}
}

void
Env::set(const std::string& pParameterName, const math::VectorField<float>& pValues) throw (Exception)
{
	try
	{
		Parameter* par = parameter(pParameterName);
		EnvParameter* envPar = dynamic_cast<EnvParameter*>( par );
		if(envPar != nullptr)
		{
			envPar->set( pValues );
		}
		else throw Exception( "FLOCK ERROR: Environment paramter name " + pParameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception)
{
	try
	{
		Agent::randomize( pParameterName, pMinParameterValue, pMaxParameterValue );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
	try
	{
		Agent::randomize( pParameterName, pMinParameterValues, pMaxParameterValues );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue, float pThresholdValue ) throw (Exception)
{
	try
	{
		Parameter* par = parameter(pParameterName);
		EnvParameter* envPar = dynamic_cast<EnvParameter*>( par );
		if(envPar != nullptr)
		{
			envPar->randomize( pMinParameterValue, pMaxParameterValue, pThresholdValue );
		}
		else throw Exception( "FLOCK ERROR: Environment paramter name " + pParameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
Env::randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues, const Eigen::VectorXf& pThresholdValues) throw (Exception)
{
	try
	{
		Parameter* par = parameter(pParameterName);
		EnvParameter* envPar = dynamic_cast<EnvParameter*>( par );
		if(envPar != nullptr)
		{
			envPar->randomize( pMinParameterValues, pMaxParameterValues, pThresholdValues );
		}
		else throw Exception( "FLOCK ERROR: Environment paramter name " + pParameterName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	}
	catch(Exception& e)
	{
		throw;
	}
}

