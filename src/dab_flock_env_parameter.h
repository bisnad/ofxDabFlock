/** \file dab_flock_env_parameter.h
 *
 *  Created by Daniel Bisig on 2/21/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/08/17.
 */

#ifndef _dab_flock_env_parameter_h_
#define _dab_flock_env_parameter_h_

#include "dab_exception.h"
#include "dab_flock_parameter.h"
#include "dab_space.h"
#include "dab_space_alg_grid.h"
#include "dab_space_grid.h"

namespace dab
{

namespace flock
{

class Env;

class EnvParameter : public Parameter
{
public:
    /**
     \brief create environment parameter
     \param pEnv environment this parameter belongs to
     \param pName name of environment parameter
     \param pValueDim dimension of parameter value
     \param pSubdivisionCount grid subdivision of parameter value
     \param pMinPos minimum position of parameter value grid
     \param pMaxPos maximum position of parameter value grid
     \exception Exception failed to create env parameter
     */
    EnvParameter(Env* pEnv, const std::string& pName, unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    
    /**
     \brief create environment parameter
     \param pEnv environment this parameter belongs to
     \param pName name of environment parameter
     \param pValues environment parameter value values
     \param pSubdivisionCount grid subdivision of parameter value
     \param pMinPos minimum position of parameter value grid
     \param pMaxPos maximum position of parameter value grid
     \exception Exception failed to create env parameter
     */
    EnvParameter(Env* pEnv, const std::string& pName, const Eigen::VectorXf& pValues, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    
    /**
     \brief create environment parameter
     \param pEnv environment this parameter belongs to
     \param pGridSpace grid space
     \param pValueDim dimension of parameter value
     \exception Exception space does not contain a grid or valuedim of grid does not match specified value
     */
    EnvParameter( Env* pEnv, std::shared_ptr<space::Space> pGridSpace, unsigned int pValueDim ) throw (Exception);
    
    /**
     \brief copy constructor
     \param pEnv env this parameter belongs to
     \param pParameter parameter to copy
     */
    EnvParameter(Env* pEnv, EnvParameter& pParameter);
    
    /**
     \brief destructor
     */
    ~EnvParameter();
    
    /**
     \brief return parameter grid dimension
     \return parameter grid dimension
     */
    unsigned int gridDim() const;
    
    /**
     \brief return parameter grid size
     \return parameter grid size
     */
    const dab::Array<unsigned int>& gridSize() const;
    
    /**
     \brief return parameter value dimension
     \returns parameter value dimension
     */
    unsigned int valueDim() const;
    
    /**
     \brief return minimum position
     \return minimum position
     */
    const Eigen::VectorXf& minPos() const;
    
    /**
     \brief return maximum position
     \return maximum position
     */
    const Eigen::VectorXf& maxPos() const;
    
    /**
     \brief return environment parameter grid
     \return environment parameter grid
     */
    space::SpaceGrid* grid();
    
    /**
     \brief return environment parameter grid
     \return environment parameter grid
     */
    space::SpaceGrid* backupGrid();
    
    /**
     \brief return environment space
     \return environment space
     */
    std::shared_ptr<space::Space> space();
    
    /**
     \brief set values
     \param pValue single value
     */
    void set(float pValue);
    
    /**
     \brief set parameter values
     \param pValues parameter values
     
     set entire parameter value grid to same value
     */
    void set( const Eigen::VectorXf& pValues ) throw (Exception);
    
    /**
     \brief set parameter values
     \param pValues parameter values
     \exception failed to set values
     */
    void set( const math::VectorField<float>& pValues ) throw (Exception);
    
    /**
     \brief set parameter values
     \param pPosition position
     \param pValues parameter values
     \param pSetMode set mode
     
     set parameter value grid at specified position
     */
    void set( const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValues, space::GridValueSetMode pSetMode ) throw (Exception);
    
    /**
     \brief change parameter values
     \param pValues parameter values
     
     change entire parameter value grid by same value
     */
    void change( const Eigen::VectorXf& pValues ) throw (Exception);
    
    /**
     \brief change parameter values
     \param pPosition position
     \param pValues parameter values
     \param pSetMode set mode
     
     change parameter value grid at specified position
     */
    void change( const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValues, space::GridValueSetMode pSetMode ) throw (Exception);
    
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
     */
    virtual void randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception);
    
    /**
     \brief randomize parameter value
     \param pMinParameterValue minimum parameter value
     \param pMaxParameterValue minimum parameter value
     \param pThresholdValue threshold value
     
     salt and pepper randomization
     parameter value will be set to min value for random values below threshold and to max value for random values above threshold
     */
    virtual void randomize(float pMinParameterValue, float pMaxParameterValue, float pThresholdValue);
    
    /**
     \brief randomize parameter value
     \param pMinParameterValues minimum parameter values
     \param pMaxParameterValues minimum parameter values
     \param pThresholdValues threshold values
     
     salt and pepper randomization
     parameter value will be set to min value for random values below threshold and to max value for random values above threshold
     */
    virtual void randomize(const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues, const Eigen::VectorXf& pThresholdValues ) throw (Exception);
    
    /**
     \brief copy backup value into current value
     */
    void flush();
    
    /**
     \brief return vector of space objects
     \param pEnvObjects vector of space objects
     
     returns space objects that exist within the boundaries of the environment grid 
     */
    void spaceObjects( std::vector< SpaceObject* >& pEnvObjects );
    
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
    friend std::ostream& operator << ( std::ostream& pOstream, const EnvParameter& pParameter )
    {
        pOstream << pParameter.info();
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    EnvParameter();
    
    space::SpaceGrid* mValueGrid; ///\brief value grid
    space::SpaceGrid* mBackupValueGrid; ///\brief backup value grid;
    space::GridAlg* mGridAlg; ///\brief 
    std::shared_ptr<space::Space> mGridSpace; ///\brief space associated with value grid
};

};

};

#endif