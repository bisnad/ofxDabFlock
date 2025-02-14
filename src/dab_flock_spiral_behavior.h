/** \file dab_flock_spiral_behavior.h
 *  \class dab::flock::SpiralBehavior cause Agent to move in a spiral around center position of their neighbors
 *	\brief cause Agent to move in a spiral around center position of their neighbors
 *
 *  The Behavior causes Agent to move in a spiral around center position of their neighbors\n
 *  Input Parameter:\n
 *  type: position dim: 3D neighbors: required\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: 3D write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_minDist dim: 1D defaultValue: 0.0\n
 *  name: xxx_maxDist dim: 1D defaultValue: 0.5\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/24/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_spiral_behavior_h_
#define _dab_flock_spiral_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class SpiralBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    SpiralBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    SpiralBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~SpiralBehavior();
    
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
    Parameter* mPositionPar; /// \brief position parameter (input)
    Parameter* mForcePar; /// \brief force parameter (output)
    Parameter* mMinDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mMaxDistPar; /// \brief maximum distance parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    space::NeighborGroup* mPositionNeighbors; /// \brief position neighbor group
    
    Eigen::Vector3f mTmpForce; /// \brief temporary force
    Eigen::Vector3f mDirection1; /// \brief neighbor direction 1
    Eigen::Vector3f mDirection2; /// \brief neighbor direction 2
};

};

};

#endif

