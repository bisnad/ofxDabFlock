/** \file dab_flock_grid_avg_behavior.h
 *  \class dab::flock::GridAvgBehavior add grid value at Agent position to output parameter
 *	\brief add grid value at Agent position to output parameter
 *
 *  The Behavior adds grid value at Agent position to output parameter\n
 *  The grid value is linearly interpolated.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  \n
 *  Output Parameter:\n
 *  type: output dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_minDist dim: 1D defaultValue: 0.0\n
 *  name: xxx_maxDist dim: 1D defaultValue: 1.0\n
 *  name: xxx_amount dim: 1D defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 0.1\n
 *  \n
 *  Created by Daniel Bisig on 5/24/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_grid_avg_behavior_h_
#define _dab_flock_grid_avg_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class GridAvgBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    GridAvgBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    GridAvgBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~GridAvgBehavior();
    
    /**
     \brief create copy of behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \return new behavior
     \exception Exception wrong number of type of parameters
     */
    virtual Behavior* create(const std::string& pBehaviorName, Agent* pAgent) const  throw (Exception);
    
    /**
     \brief create copy of behavior
     \param pInputParameterString input parameter string
     \param pOutputParameterString output parameter string
     \return new behavior
     */
    virtual Behavior* create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const;
    
    /**
     \brief perform behavior
     */
    void act();
    
protected:
    Parameter* mParIn; /// \brief input parameter (input)
    Parameter* mParOut; /// \brief output parameter (output)
    Parameter* mMinDistPar; /// \brief mininum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal) 
    space::NeighborGroup* mGridValues; /// \brief grid values
    
    Eigen::VectorXf mTmpValueOut; /// \brief temporary output value
};

};

};

#endif