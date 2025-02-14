/** \file dab_flock_evasion_behavior.h
 *  \class dab::flock::EvasionBehavior cause Agent to move away from their neighbors
 *	\brief cause Agent to move away from their neighbors
 *
 *  The Behavior causes Agent to move away from their neighbors\n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: required\n
 *  \n
 *  Output Parameter:\n
 *  type: force dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_maxDist dim: 1D defaultValue: 0.2\n
 *  name: xxx_amount dim: 1D defaultValue: 0.1\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/21/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 13/10/2017.
 */

#ifndef _dab_flock_evasion_behavior_h_
#define _dab_flock_evasion_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class EvasionBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    EvasionBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    EvasionBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~EvasionBehavior();
    
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
    Parameter* mMaxDistPar; /// \brief minimum distance parameter (internal)
    Parameter* mAmountPar; /// \brief behavior amount parameter (internal)
    space::NeighborGroup* mPositionNeighbors; /// \brief position neighbor group
    
    Eigen::VectorXf mTmpForce; /// \brief temporary force
};

};

};

#endif