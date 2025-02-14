/** \file dab_flock_env_parameter.cpp
 */

#include "dab_flock_env_parameter.h"
#include "dab_flock_env.h"
#include "dab_flock_simulation.h"
#include "dab_math.h"

using namespace dab;
using namespace dab::flock;

EnvParameter::EnvParameter()
: mValueGrid(nullptr)
, mBackupValueGrid(nullptr)
{}

EnvParameter::EnvParameter(Env* pEnv, const std::string& pName, unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
: Parameter(pEnv, pName, pValueDim)
{
    try
    {
        mValueGrid = new space::SpaceGrid(pValueDim, pSubdivisionCount, pMinPos, pMaxPos);
        mBackupValueGrid = new space::SpaceGrid(pValueDim, pSubdivisionCount, pMinPos, pMaxPos);
        
        std::string spaceName(pEnv->name() + "_" + mName);
        mGridAlg = new space::GridAlg( mValueGrid, space::GridAlg::AvgLocationMode, space::GridAlg::NoUpdateMode );
        mGridSpace = std::shared_ptr<space::Space>(new space::Space( spaceName, mGridAlg ));
        
        Simulation::get().space().addSpace(mGridSpace);
    }
    catch (Exception& e)
    {
        e += Exception("FLOCK ERROR: failed to create EnvParameter " + pName, __FILE__, __FUNCTION__, __LINE__);
        throw;
    }
}

EnvParameter::EnvParameter(Env* pEnv, const std::string& pName, const Eigen::VectorXf& pValues, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
: Parameter(pEnv, pName, pValues)
{
    try
    {
        mValueGrid = new space::SpaceGrid(pValues.rows(), pSubdivisionCount, pMinPos, pMaxPos);
        mBackupValueGrid = new space::SpaceGrid(pValues.rows(), pSubdivisionCount, pMinPos, pMaxPos);
        
        mValueGrid->setValues(pValues);
        mBackupValueGrid->setValues(pValues);
        
        std::string spaceName(pEnv->name() + "_" + mName);
        mGridAlg = new space::GridAlg( mValueGrid, space::GridAlg::AvgLocationMode, space::GridAlg::NoUpdateMode );
        mGridSpace = std::shared_ptr<space::Space>(new space::Space( spaceName, mGridAlg ));
        
        Simulation::get().space().addSpace(mGridSpace);
    }
    catch (Exception& e)
    {
        e += Exception("FLOCK ERROR: failed to create EnvParameter " + pName, __FILE__, __FUNCTION__, __LINE__);
        throw;
    }
}

EnvParameter::EnvParameter( Env* pEnv, std::shared_ptr<space::Space> pGridSpace, unsigned int pValueDim ) throw (Exception)
: Parameter( pEnv, pGridSpace->name(), pValueDim )
{
	mGridSpace = pGridSpace;
	
	space::GridAlg* gridAlg = dynamic_cast< space::GridAlg* >( pGridSpace->spaceAlg() );
	if(gridAlg == nullptr) throw Exception( "FLOCK ERROR: environment parameter space " + pGridSpace->name() + "does not contain a grid", __FILE__, __FUNCTION__, __LINE__ );
	
	mBackupValueGrid = &( gridAlg->grid() );
	
	if( mBackupValueGrid->valueDim() != pValueDim ) throw Exception( "FLOCK ERROR: environment parameter space " + pGridSpace->name() + " doesn't match specified dimension", __FILE__, __FUNCTION__, __LINE__ );
	
	mValueGrid = new space::SpaceGrid( mBackupValueGrid->valueDim(), mBackupValueGrid->subdivisionCount(), mBackupValueGrid->minPos(), mBackupValueGrid->maxPos() );
}

EnvParameter::EnvParameter(Env* pEnv, EnvParameter& pParameter)
: Parameter(pEnv, pParameter)
{
	mValueGrid = new space::SpaceGrid( *(pParameter.mValueGrid) );
	mBackupValueGrid = new space::SpaceGrid( *(pParameter.mBackupValueGrid) );
    
	std::string spaceName(pEnv->name() + "_" + mName);
	mGridAlg = new space::GridAlg( mValueGrid, space::GridAlg::AvgLocationMode, space::GridAlg::NoUpdateMode );
	mGridSpace = std::shared_ptr<space::Space>(new space::Space( spaceName, mGridAlg ));
	
	Simulation::get().space().addSpace(mGridSpace);
}

EnvParameter::~EnvParameter()
{
	Simulation::get().space().removeSpace(mGridSpace->name());
	delete mValueGrid;
	delete mBackupValueGrid;
}

unsigned int
EnvParameter::gridDim() const
{
	return mValueGrid->gridDim();
}

const dab::Array<unsigned int>&
EnvParameter::gridSize() const
{
	return mValueGrid->subdivisionCount();
}

unsigned int
EnvParameter::valueDim() const
{
	return mValueGrid->valueDim();
}

const Eigen::VectorXf&
EnvParameter::minPos() const
{
	return mValueGrid->minPos();
}

const Eigen::VectorXf&
EnvParameter::maxPos() const
{
	return mValueGrid->maxPos();
}

space::SpaceGrid*
EnvParameter::grid()
{
	return mValueGrid;
}

space::SpaceGrid*
EnvParameter::backupGrid()
{
	return mBackupValueGrid;
}

std::shared_ptr<space::Space>
EnvParameter::space()
{
	return mGridSpace;
}

void
EnvParameter::set( float pValue )
{
    Eigen::VectorXf _value(mDim);
    _value.setConstant(pValue);
    
	mBackupValueGrid->setValues( _value );
	
	flush();
}

void
EnvParameter::set( const Eigen::VectorXf& pValues ) throw (Exception)
{
	mBackupValueGrid->setValues(pValues);
	
	flush();
}

void
EnvParameter::set( const math::VectorField<float>& pValues ) throw (Exception)
{
	math::VectorField<float>& backupField = mBackupValueGrid->vectorField();
	
	try
	{
		backupField = pValues;
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: parameter " + mName + " vectorfield mismatch", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
	flush();
}

void
EnvParameter::set( const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValues, space::GridValueSetMode pSetMode ) throw (Exception)
{
	mBackupValueGrid->setValue(pPosition, pValues, pSetMode );
	
	//flush();
}

void
EnvParameter::change( const Eigen::VectorXf& pValues ) throw (Exception)
{
	mBackupValueGrid->changeValues(pValues);
	
	flush();
}

void
EnvParameter::change( const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValues, space::GridValueSetMode pSetMode ) throw (Exception)
{
	mBackupValueGrid->changeValue(pPosition, pValues, pSetMode );
	
	//flush();
}

void
EnvParameter::randomize(float pMinParameterValue, float pMaxParameterValue)
{
	if( isnan( pMinParameterValue ) == false && isnan( pMaxParameterValue ) == true ) return;
    
	math::Math<>& math = math::Math<>::get();
	
	math::VectorField<float>& vectorField = mBackupValueGrid->vectorField();
	unsigned int vC = vectorField.vectorCount();
    std::vector< Eigen::VectorXf >& vectors = vectorField.vectors();
	
	for(unsigned int vI=0; vI<vC; ++vI)
	{
		Eigen::VectorXf& vector = vectors[vI];
        
		for(unsigned int c=0; c<mDim; ++c)
		{
			vector[c] = math.random( pMinParameterValue, pMaxParameterValue );
		}
	}
	
	flush();
}

void
EnvParameter::randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
	if(pMinParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMinParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	if(pMaxParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMaxParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );

	math::Math<>& math = math::Math<>::get();
    
	math::VectorField<float>& vectorField = mBackupValueGrid->vectorField();
	unsigned int vC = vectorField.vectorCount();
	std::vector< Eigen::VectorXf >& vectors = vectorField.vectors();
	
	for(unsigned int vI=0; vI<vC; ++vI)
	{
		Eigen::VectorXf& vector = vectors[vI];
        
		for(unsigned int c=0; c<mDim; ++c)
		{
			if( isnan( pMinParameterValues[c] ) == false && isnan( pMaxParameterValues[c] ) == false ) vector[c] = math.random( pMinParameterValues[c], pMaxParameterValues[c] );
		}
	}
	
	flush();
}

void
EnvParameter::randomize(float pMinParameterValue, float pMaxParameterValue, float pThresholdValue)
{
	math::Math<>& math = math::Math<>::get();
	
	math::VectorField<float>& vectorField = mBackupValueGrid->vectorField();
	unsigned int vC = vectorField.vectorCount();
	std::vector< Eigen::VectorXf >& vectors = vectorField.vectors();
	
	for(unsigned int vI=0; vI<vC; ++vI)
	{
		Eigen::VectorXf& vector = vectors[vI];
        
		for(unsigned int c=0; c<mDim; ++c)
		{
			if( math.random(0.0, 1.0) >= pThresholdValue ) vector[c] = pMaxParameterValue;
			else vector[c] = pMinParameterValue;
		}
	}
	
	flush();
}

void
EnvParameter::randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues, const Eigen::VectorXf& pThresholdValues ) throw (Exception)
{
	if(pMinParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMinParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	if(pMaxParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMaxParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	if(pThresholdValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pThresholdValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
	math::Math<>& math = math::Math<>::get();
	
	math::VectorField<float>& vectorField = mBackupValueGrid->vectorField();
	unsigned int vC = vectorField.vectorCount();
	std::vector< Eigen::VectorXf >& vectors = vectorField.vectors();
	
	for(unsigned int vI=0; vI<vC; ++vI)
	{
		Eigen::VectorXf& vector = vectors[vI];
        
		for(unsigned int c=0; c<mDim; ++c)
		{
			if( math.random(0.0, 1.0) >= pThresholdValues[c] ) vector[c] = pMaxParameterValues[c];
			else vector[c] = pMinParameterValues[c];
		}
	}
	
	flush();
}

void
EnvParameter::flush()
{
	(*mValueGrid) = *mBackupValueGrid;
}

void
EnvParameter::spaceObjects( std::vector< SpaceObject* >& pEnvObjects )
{
	pEnvObjects.clear();
    
	const Eigen::VectorXf& gridMinPos = mGridAlg->grid().minPos();
	const Eigen::VectorXf& gridMaxPos = mGridAlg->grid().maxPos();
    std::vector<space::SpaceProxyObject*>& gridProxyObjects = mGridSpace->objects();
	unsigned int oC = gridProxyObjects.size();
	SpaceObject* spaceObject;
    
    bool withinEnvBoundary;

	for(unsigned int oI=0; oI<oC; ++oI)
	{
		spaceObject = gridProxyObjects[oI]->spaceObject();
		Eigen::VectorXf& objectPos = spaceObject->position();
        
        withinEnvBoundary = true;
        
        for(int d=0; d<mDim; ++d)
        {
            if(objectPos[d] < gridMinPos[d] || objectPos[d] > gridMaxPos[d])
            {
                withinEnvBoundary = false;
                break;
            }
        }
        
        if(withinEnvBoundary == true) pEnvObjects.push_back( gridProxyObjects[oI]->spaceObject() );
	}
}

EnvParameter::operator std::string() const
{
    return info(0);
}

std::string
EnvParameter::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << " ";
    ss << "values:\n";
    ss << (*mValueGrid);
    ss << "backup values:\n";
    ss << (*mBackupValueGrid);
    
    return ss.str();
}
