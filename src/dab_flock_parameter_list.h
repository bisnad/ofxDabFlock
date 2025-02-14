/**	\file dab_flock_parameter_list.h
 *	\class dab::flock::ParameterList list of parameters
 *	\brief list of parameters
 *
 *
 *  Created by Daniel Bisig on 3/29/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 3/08/17.
 *  Copyright 2006 Daniel Bisig All rights reserved.
 */

#ifndef _dab_flock_parameter_list_h_
#define _dab_flock_parameter_list_h_

#include <map>
#include "dab_exception.h"
#include "dab_index_map.h"
#include "dab_flock_parameter.h"

namespace dab
{

namespace flock
{

class ParameterList
{
public:
    /**
     \brief default name
     */
    static const std::string sClassName;
    
    /**
     \brief default constructor
     */
    ParameterList();
    
    /**
     \brief destructor
     */
    ~ParameterList();
    
    /**
     \brief return number of parameters this list contains
     \return number of parameters this list contains
     */
    unsigned int parameterCount() const;
    
    /**
     \brief checks if parameter list contains parameter
     \param pName parameter name
     \return true, if parameter name exists
     */
    bool contains(const std::string& pName) const;
    
    /**
     \brief return index of parameter
     \param pName parameter name
     \return index of parameter
     \exception Exception parameter not found
     */
    unsigned int parameterIndex(const std::string& pName) const throw (Exception);
    
    /**
     \brief get parameter
     \param pIndex parameter index
     \return parameter
     \exception Exception parameter not found
     */
    Parameter* parameter(unsigned int pIndex) throw (Exception);
    
    /**
     \brief get parameter
     \param pIndex parameter index
     \return parameter
     \exception Exception parameter not found
     */
    const Parameter* parameter(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief get parameter
     \param pName parameter name
     \return parameter
     \exception Exception parameter not found
     */
    Parameter* parameter(const std::string& pName) throw (Exception);
    
    /**
     \brief get parameter
     \param pName parameter name
     \return parameter
     \exception Exception parameter not found
     */
    const Parameter* parameter(const std::string& pName) const throw (Exception);
    
    /**
     \brief add parameter
     \param pParameter parameter
     \exception Exception parameter already exists
     */
    void addParameter(Parameter* pParameter) throw (Exception);
    
    /**
     \brief add parameter
     \param pAgent agent this parameter list belongs to
     \param pName parameter name
     \param pDim dimension of parameter
     \exception Exception parameter already exists
     */
    void addParameter(Agent* pAgent, const std::string& pName, unsigned int pDim) throw (Exception);
    
    /**
     \brief remove parameter
     \param pName parameter name
     \exception Exception parameter not found
     */
    void removeParameter(const std::string& pName) throw (Exception);
    
    /**
     \brief return parameter values
     \param pName parameter name
     \return parameter values
     \exception FlockException parameter not found
     */
    Eigen::VectorXf& values(const std::string& pName) throw (Exception);
    
    /**
     \brief set parameter value
     \param pName parameter name
     \param pValue parameter value
     \exception Exception parameter not found
     */
    void setValue(const std::string& pName, float pValue) throw (Exception);
    
    /**
     \brief set parameter values
     \param pName parameter name
     \param pValues parameter values
     \exception Exception parameter not found or incompatible dimension
     */
    void setValues(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception);
    
    /**
     \brief update all parameters
     
     copies their backup values into their current values
     */
    void flush();
    
    /**
     \brief print parameter list information
     */
    virtual operator std::string() const;
    
    /**
     \brief print parameter list information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief print parameter list information
     */	
    friend std::ostream& operator<< (std::ostream& pOstream, const ParameterList& pParameterList)
    {
        pOstream << pParameterList.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief parameters
     */
    IndexMap<std::string, Parameter*> mParameters;
};

};

};

#endif