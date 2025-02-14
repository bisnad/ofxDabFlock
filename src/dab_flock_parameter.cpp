/**	\file dab_flock_parameter.cpp
 */

#include "dab_flock_parameter.h"
#include "dab_math.h"
#include "dab_space_manager.h"
#include "dab_space_neighbor_group_alg.h"
#include "dab_flock_agent.h"
#include "dab_flock_simulation.h"

using namespace dab;
using namespace dab::flock;

const std::string Parameter::sClassName = "Parameter";

Parameter::Parameter()
: SpaceObject(1)
, mAgent(nullptr)
, mValues(mPosition)
, mBackupValues(mPosition)
{}

Parameter::Parameter(Agent* pAgent, const std::string& pName, unsigned int pDim)
: SpaceObject(pDim)
, mName(pName)
, mAgent(pAgent)
, mValues(mPosition)
, mBackupValues(mPosition)
{}

Parameter::Parameter(Agent* pAgent, const std::string& pName, const std::initializer_list<float>& pValues)
: SpaceObject(pValues.size())
, mName(pName)
, mValues(mPosition)
, mBackupValues(mPosition)
{
    auto iter = pValues.begin();
    
    for( int d=0; d < mDim; ++d, ++iter)
    {
        mValues[d] = *iter;
        mBackupValues[d] = *iter;
    }
}

Parameter::Parameter(Agent* pAgent, const std::string& pName, const std::vector<float>& pValues)
: SpaceObject(pValues.size())
, mName(pName)
, mValues(mPosition)
, mBackupValues(mPosition)
{
    auto iter = pValues.begin();
    
    for( int d=0; d < mDim; ++d, ++iter)
    {
        mValues[d] = *iter;
        mBackupValues[d] = *iter;
    }
}

Parameter::Parameter(Agent* pAgent, const std::string& pName, unsigned int pValueCount, const float* pValues)
: SpaceObject(pValueCount)
, mName(pName)
, mValues(mPosition)
, mBackupValues(mPosition)
{
    for(int d=0; d<mDim; ++d)
    {
        mValues[d] = pValues[d];
        mBackupValues[d] = pValues[d];
    }
}

Parameter::Parameter(Agent* pAgent, const std::string& pName, const Eigen::VectorXf& pValues)
: SpaceObject(pValues)
, mName(pName)
, mAgent(pAgent)
, mValues(mPosition)
, mBackupValues(mPosition)
{}

Parameter::Parameter(Agent* pAgent, const Parameter& pParameter)
: SpaceObject( pParameter )
, mName( pParameter.mName )
, mAgent( pAgent )
, mValues( mPosition )
, mBackupValues( mPosition )
{}

Parameter::~Parameter()
{}

const std::string&
Parameter::name() const
{
	return mName;
}

unsigned int
Parameter::dim() const
{
	return mDim;
}

unsigned int
Parameter::count() const
{
	return mDim;
}

void
Parameter::set(float pValue)
{
    mBackupValues.setConstant(pValue);
}

void
Parameter::set(const std::initializer_list<float>& pValues) throw (Exception)
{
    if(pValues.size() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValues.size()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
    auto iter = pValues.begin();
    
    for( int d=0; d < mDim; ++d, ++iter)
    {
        if( isnan( *iter ) == false )mBackupValues[d] = *iter;
    }
}

void
Parameter::set(unsigned int pValueCount, const float* pValues) throw (Exception)
{
	if(pValueCount != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValueCount) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	
	for(unsigned int d=0; d<mDim; ++d)
	{
		if( isnan( pValues[d] ) == false ) mBackupValues[d] = pValues[d];
	}
}

void
Parameter::set(const Eigen::VectorXf& pValues) throw (Exception)
{
	if(pValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );

	
	for(unsigned int d=0; d<mDim; ++d)
	{
		mBackupValues[d] = pValues[d];
	}
}

void
Parameter::change(float pValue)
{
    for(int d=0; d<mDim; ++d) mBackupValues[d] += pValue;
}

void
Parameter::change(const std::initializer_list<float>& pValues) throw (Exception)
{
	if(pValues.size() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValues.size()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
    auto iter = pValues.begin();
    
    for( int d=0; d < mDim; ++d, ++iter)
    {
        if( isnan( *iter ) == false ) mBackupValues[d] += *iter;
    }
}

void
Parameter::change(unsigned int pValueCount, float* pValues) throw (Exception)
{
	if(pValueCount != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValueCount) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
	for(unsigned int d=0; d<mDim; ++d)
	{
		if( isnan( pValues[d] ) == false ) mBackupValues[d] += pValues[d];
	}
}

void
Parameter::change(const Eigen::VectorXf& pValues) throw (Exception)
{
	if(pValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
	for(unsigned int d=0; d<mDim; ++d)
	{
		mBackupValues[d] += pValues[d];
	}
}

void
Parameter::randomize(float pMinParameterValue, float pMaxParameterValue)
{
	if( isnan( pMinParameterValue ) == true || isnan( pMaxParameterValue ) == true ) return;
    
	math::Math<>& math = math::Math<>::get();
    
	for(unsigned int d=0; d<mDim; ++d)
	{
		mBackupValues[d] = math.random( pMinParameterValue, pMaxParameterValue );
	}
}

void
Parameter::randomize(const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception)
{
    if(pMinParameterValues.size() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMinParameterValues.size()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	if(pMaxParameterValues.size() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMaxParameterValues.size()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
    
	math::Math<>& math = math::Math<>::get();
    
    auto minIter = pMinParameterValues.begin();
    auto maxIter = pMaxParameterValues.begin();
    
    for(int d=0; d<mDim; ++d, ++minIter, ++maxIter)
    {
        if( isnan( *minIter ) == false && isnan( *maxIter ) == false ) mBackupValues[d] = math.random( *minIter, *maxIter );
    }
}

void
Parameter::randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
    if(pMinParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMinParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );
	if(pMaxParameterValues.rows() != mDim) throw Exception( "FLOCK ERROR: valueCount mismatch: " + std::to_string(mDim) + " != " + std::to_string(pMaxParameterValues.rows()) + " for parameter " + mName, __FILE__, __FUNCTION__, __LINE__ );

	math::Math<>& math = math::Math<>::get();
    
	for(unsigned int d=0; d<mDim; ++d)
	{
		mBackupValues[d] = math.random( pMinParameterValues[d], pMaxParameterValues[d] );
	}
}

Agent*
Parameter::agent()
{
	return mAgent;
}

void
Parameter::flush()
{
    //	mValues = mBackupValues;
	
	for(unsigned int d=0; d<mDim; ++d) mValues[d] = mBackupValues[d];
    
	unsigned int neighborListCount = mNeighborLists.size();
	for(unsigned int i=0; i<neighborListCount; ++i) mNeighborLists[i]->setValues(mValues);
}

Eigen::VectorXf&
Parameter::values()
{
	return mValues;
}

const Eigen::VectorXf&
Parameter::values() const
{
	return mValues;
}

float&
Parameter::value(unsigned int pIndex) throw (Exception)
{
	if( pIndex >= mDim ) throw Exception( "FLOCK ERROR: Index " + std::to_string(pIndex) + " Out Of Bounds (Parameter Dim " + std::to_string(mDim) + ") ", __FILE__, __FUNCTION__, __LINE__ );
	
	return mValues[pIndex];
}

Eigen::VectorXf&
Parameter::backupValues()
{
	return mBackupValues;
}

float&
Parameter::backupValue(unsigned int pIndex) throw (Exception)
{
	if( pIndex >= mDim ) throw Exception( "FLOCK ERROR: Index " + std::to_string(pIndex) + " Out Of Bounds (Parameter Dim " + std::to_string(mDim) + ") ", __FILE__, __FUNCTION__, __LINE__ );
	
	return mBackupValues[pIndex];
}

const Eigen::VectorXf&
Parameter::operator=( const Eigen::VectorXf& pValues) throw (Exception)
{
    if( pValues.rows() != mBackupValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mBackupValues.rows()) + " Values Dimension " + std::to_string(pValues.rows()) + ") ", __FILE__, __FUNCTION__, __LINE__ );
    
	mBackupValues = pValues;
    
    return mBackupValues;
}

const Eigen::VectorXf&
Parameter::operator=( const std::initializer_list<float>& pValues) throw (Exception)
{
    if( pValues.size() != mBackupValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mBackupValues.rows()) + " Values Dimension " + std::to_string(pValues.size()) + ") ", __FILE__, __FUNCTION__, __LINE__ );
    
    auto iter = pValues.begin();
    
    for(int d=0; d<mDim; ++d, ++iter)
    {
        mBackupValues[d] = *iter;
    }
    
    return mBackupValues;
}

const Eigen::VectorXf&
Parameter::operator+=( const Eigen::VectorXf& pValues) throw (Exception)
{
    if( pValues.rows() != mBackupValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mBackupValues.rows()) + " Values Dimension " + std::to_string(pValues.rows()) + ") ", __FILE__, __FUNCTION__, __LINE__ );
    
	mBackupValues += pValues;
    
    return mBackupValues;
}

void
Parameter::setValue(float pValue)
{
	mValues.setConstant(pValue);
	mBackupValues.setConstant(pValue);
}

void
Parameter::setValues(const Eigen::VectorXf& pValues) throw (Exception)
{
    if( pValues.rows() != mValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mValues.rows()) + " Values Dimension " + std::to_string(pValues.rows()) + ") ", __FILE__, __FUNCTION__, __LINE__ );
	
	for(unsigned int d=0; d<mDim; ++d)
	{
        mValues[d] = pValues[d];
        mBackupValues[d] = pValues[d];
	}
}

void
Parameter::setValues(const std::initializer_list<float>& pValues) throw (Exception)
{
    if( pValues.size() != mValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mValues.rows()) + " Values Dimension " + std::to_string(pValues.size()) + ") ", __FILE__, __FUNCTION__, __LINE__ );
	
    auto iter = pValues.begin();
	int d = 0;

	for(auto iter = pValues.begin(); iter != pValues.end(); ++iter, ++ d)
	{
		if( isnan( *iter ) == false )
		{
			mValues[d] = *iter;
			mBackupValues[d] = *iter;
		}
	}
}

void
Parameter::setValues(unsigned int pValueCount, float* pValues) throw (Exception)
{
    if( pValueCount != mValues.rows() ) throw Exception( "FLOCK ERROR: Incompatible Parameter Dimension " + std::to_string(mValues.rows()) + " Values Dimension " + std::to_string(pValueCount) + ") ", __FILE__, __FUNCTION__, __LINE__ );
	
	for(unsigned int d=0; d<mDim; ++d)
	{
		if( isnan( pValues[d] ) == false )
		{
			mValues[d] = pValues[d];
			mBackupValues[d] = pValues[d];
		}
	}
}

Parameter::operator std::string() const
{
    return info(0);
}

std::string
Parameter::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << " " << SpaceObject::info(pPropagationLevel);
    
    return ss.str();
}