/**	\file dab_flock_parameter.h
 *	\class dab::flock::Parameter parameter class
 *	\brief parameter class
 *
 *
 *  Created by Daniel Bisig on 3/23/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 3/08/17.
 */

#ifndef _dab_flock_parameter_h_
#define _dab_flock_parameter_h_

#include <iostream>
#include <Eigen/Dense>
#include "dab_exception.h"
#include "dab_index_map.h"
#include "dab_space_object.h"

namespace dab
{

namespace flock
{

class Agent;
    
class Parameter : public space::SpaceObject
{
public:
    /**
     \brief class name
     */
	static const std::string sClassName;
    
    /**
     \brief create parameter
     \param pAgent agent this parameter belongs to
     \param pName parameter name
     \param pDim dimension of parameter (> 0)
     */
	Parameter(Agent* pAgent, const std::string& pName, unsigned int pDim);
    
    /**
     \brief create parameter
     \param pAgent agent this parameter belongs to
     \param pName parameter name
     \param pValueCount number of values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	Parameter(Agent* pAgent, const std::string& pName, unsigned int pValueCount, const float* pValues);
    
    /**
     \brief create parameter
     \param pAgent agent this parameter belongs to
     \param pName parameter name
     \param pValues parameter values
     */
	Parameter(Agent* pAgent, const std::string& pName, const std::initializer_list<float>& pValues);
    
    /**
     \brief create parameter
     \param pAgent agent this parameter belongs to
     \param pName parameter name
     \param pValues parameter values
     */
	Parameter(Agent* pAgent, const std::string& pName, const std::vector<float>& pValues);
    
    /**
     \brief create parameter
     \param pAgent agent this parameter belongs to
     \param pName parameter name
     \param pValues parameter values
     */
	Parameter(Agent* pAgent, const std::string& pName, const Eigen::VectorXf& pValues);
    
    /**
     \brief copy constructor
     \param pAgent agent this parameter belongs to
     \param pParameter parameter to copy
     */
	Parameter(Agent* pAgent, const Parameter& pParameter);
    
    /**
     \brief destructor
     */
	virtual ~Parameter();
    
    /**
     \brief return name
     \return name
     */
	const std::string& name() const;
    
    /**
     \brief returns expected number of values
     \return expected number of values
     */
	virtual unsigned int dim() const;
    
    /**
     \brief returns expected number of values
     \return expected number of values
     */
	virtual unsigned int count() const;
    
    /**
     \brief set values
     \param pValue single value
     */
	virtual void set(float pValue);
    
    /**
     \brief set values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	virtual void set(const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief set values
     \param pValueCount number of values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	virtual void set(unsigned int pValueCount, const float* pValues) throw (Exception);
    
    /**
     \brief set values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	virtual void set(const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief change values
     \param pValue single value
     */
	virtual void change(float pValue);

    /**
     \brief change values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	virtual void change(const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief change values
     \param pValueCount number of values
     \param pValues values
     \exception Exception number of values cannot be changed
     */
	virtual void change(unsigned int pValueCount, float* pValues) throw (Exception);
    
    /**
     \brief change values
     \param pValues values
     \exception FlockException number of values cannot be changed
     */
	virtual void change(const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue minimum parameter value
     */
	virtual void randomize(float pMinParameterValue, float pMaxParameterValue);
    
    /**
     \brief randomize parameter value
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues minimum parameter values
     \exception Exception number of values cannot be changed
     */
	virtual void randomize(const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues minimum parameter values
     \exception Exception number of values cannot be changed
     */
	virtual void randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    /**
     \brief return agent this parameter belongs to
     */
	Agent* agent();
    
    /**
     \brief return parameter values
     \return parameter values
     */
	Eigen::VectorXf& values();
    
    /**
     \brief return parameter values
     \return parameter values
     */
	const Eigen::VectorXf& values() const;
    
    /**
     \brief return parameter value
     \param pIndex value index
     \return parameter value
     \exception Exception index out of bounds
     */
	float& value(unsigned int pIndex = 0) throw (Exception);
    
    /**
     \brief return parameter backup values
     \return parameter backup values
     */
	Eigen::VectorXf& backupValues();
    
    /**
     \brief return parameter backup value
     \param pIndex backup value index
     \return parameter backup value
     \exception Exception index out of bounds
     */
	float& backupValue(unsigned int pIndex = 0) throw (Exception);
    
    /**
     \brief copy backup value into current value
     */
	virtual void flush();
    
    /**
     \brief parameter = vector
     \param pValues vector to copy values from
     \return Vector
     \exception Exception dimension mismatch
     
     copy vector values to backup values
     */
    const Eigen::VectorXf& operator=( const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief parameter = vector
     \param pValues vector to copy values from
     \return Vector
     \exception Exception dimension mismatch
     
     copy vector values to backup values
     */
    const Eigen::VectorXf& operator=( const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief parameter += vector
     \param pValues vector to add values from
     \return Vector
     \exception Exception dimension mismatch
     
     add vector values to backup values
     */
    const Eigen::VectorXf& operator+=( const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief set parameter value
     \param pValue parameter value
     
     sets both value and backup value
     */
	void setValue(float pValue);
    
    /**
     \brief set parameter values
     \param pValues parameter values
     \exception Exception dimension mismatch
     
     sets both values and backup values
     */
	void setValues(const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pValues parameter values
     \exception Exception dimension mismatch
     
     sets both values and backup values
     */
	void setValues(const std::initializer_list<float>& pValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pValueCount value count
     \param pValues parameter values
     \exception FlockException dimension mismatch
     
     sets both values and backup values
     */
	void setValues(unsigned int pValueCount, float* pValues) throw (Exception);
    
    /**
     \brief print parameter information
     */
	virtual operator std::string() const;
    
    /**
     \brief print parameter information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
	virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual parameter info
     \param pOstream output text stream
     \param pParameter parameter
     */
 	friend std::ostream& operator << ( std::ostream& pOstream, const Parameter& pParameter )
    {
		pOstream << pParameter.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
	Parameter();
	
	/**
     \brief parameter name
     */
	std::string mName;
	
	/**
     \brief agent this parameter belongs to
     */
	Agent* mAgent;
	
	/**
     \brief parameter values
     */
	Eigen::VectorXf& mValues;
	
	/**
     \brief parameter backup values
     
     parameter backup values are required for correctly updating parameter values
     */
	Eigen::VectorXf mBackupValues;
	
	/**
     \brief neighbor lists
     */
	IndexMap<std::string, Parameter*> mNeighborLists;

};
    
};
    
};

#endif