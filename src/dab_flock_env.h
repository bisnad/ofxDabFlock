/** \file dab_flock_env.h
 *
 *  Created by Daniel Bisig on 2/21/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/08/17.
 */

#ifndef _dab_flock_env_h_
#define _dab_flock_env_h_

#include "dab_exception.h"
#include "dab_array.h"
#include "dab_index_map.h"
#include "dab_space_grid.h"
#include "dab_flock_agent.h"
#include "dab_flock_env_parameter.h"
#include <map>

namespace dab
{

namespace flock
{

class Env : public Agent
{
public:
    /**
     \brief create environment
     */
    Env(const std::string& pName, unsigned int pDim);
    
    /**
     \brief destructor
     */
    ~Env();
    
    /**
     \brief clear environment
     \remark triggers destructor
     */
    void clear();
    
    unsigned int dim() const;
    
    using Agent::addParameter;
    
    /**
     \brief create parameter
     \param pParameterName parameter name
     \param pValueDim parameter value dimension
     \param pSubdivisionCount parameter grid value subdivision count
     \param pMinPos paramter grid minimum position
     \param pMaxPos parameter grid maximum position
     \exception Exception failed to add parameter
     */
    void addParameter(const std::string& pParameterName, unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    
    /**
     \brief create parameter
     \param pParameterName parameter name
     \param pValues parameter values
     \param pSubdivisionCount parameter grid value subdivision count
     \param pMinPos paramter grid minimum position
     \param pMaxPos parameter grid maximum position
     \exception Exception failed to add parameter
     */
    void addParameter(const std::string& pParameterName, const Eigen::VectorXf& pValues, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    
    /**
     \brief create parameter
     \param pGridSpace grid space
     \param pValueDim parameter value dimension
     \exception Exception failed to add parameter
     */
    void addParameter( std::shared_ptr<space::Space> pGridSpace, unsigned int pValueDim ) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pValues parameter values
     \param pPosition position of parameter values
     \param pSetMode set mode
     \exception Exception failed to set parameter
     */
    void setParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pValues, const Eigen::VectorXf& pPosition, space::GridValueSetMode pSetMode) throw (Exception);
    
    /**
     \brief change parameter values
     \param pParameterName parameter name
     \param pValues parameter values
     \param pPosition position of parameter values
     \param pSetMode set mode
     \exception Exception FlockException failed to set parameter
     */
    void changeParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pValues, const Eigen::VectorXf& pPosition, space::GridValueSetMode pSetMode) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValue parameter value
     \exception Exception parameter name is not found
     */
    void set(const std::string& pParameterName, float pParameterValue) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pParameterValues parameter values
     \exception Exception parameter name is not found or parameter values has wrong dimension
     */
    void set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception);
    
    /**
     \brief set parameter values
     \param pParameterName parameter name
     \param pValues parameter values
     \param pPosition position of parameter values
     \param pSetMode set mode
     \exception Exception failed to set parameter
     */
    void set(const std::string& pParameterName, const math::VectorField<float>& pValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pParameterName parameter name
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue maximum parameter value
     \exception Exception parameter name not found
     */
    void randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \exception Exception parameter name not found or parameter values has wrong dimension
     */
    void randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pParameterName parameter name
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue maximum parameter value
     \param pThresholdValue threshold value
     \exception Exception parameter name not found
     
     salt and pepper randomization. If random number between 0.0 and 1.0 is below threshold, the parameter will be set to min value, otherwise it will be set to max value
     */
    void randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue, float pThresholdValue) throw (Exception);
    
    /**
     \brief randomize parameter values
     \param pParameterName parameter name
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues maximum parameter values
     \param pThresholdValues threshold values
     \exception Exception parameter name not found or parameter values has wrong dimension 
     
     salt and pepper randomization. If random number between 0.0 and 1.0 is below threshold, the parameter will be set to min value, otherwise it will be set to max value
     */
    void randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues, const Eigen::VectorXf& pThresholdValues) throw (Exception);
    
protected:
    /**
     \brief default constructor
     */
    Env();
    
    unsigned int mDim; ///\brief dimension of environments
};

};

};

#endif