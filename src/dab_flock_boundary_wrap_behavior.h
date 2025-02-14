/** \file dab_flock_boundary_wrap_behavior.h
 *  \class dab::flock::BoundaryWrapBehavior creates periodic boundary for Agent 's position
 *	\brief  creates periodic boundary for Agent 's position
 *
 *  Behavior creates periodic boundary for Agent 's position.\n
 *  The behavior is normally meant to restrict an Agent 's position to a cubic region\n
 *  by mirroring the position to the opposite site of the cube whenever it exceeds the cube's boundaries.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: position dim: nD write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_lowerBoundary dim: nD defaultValue: -1.0\n
 *  name: xxx_upperBoundary dim: nD defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 8/31/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017.
 */

#ifndef _dab_flock_boundary_wrap_behavior_h_
#define _dab_flock_boundary_wrap_behavior_h_

#include "dab_flock_behavior.h"

namespace dab
{

namespace flock
{

class BoundaryWrapBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    BoundaryWrapBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    BoundaryWrapBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~BoundaryWrapBehavior();
    
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
    Parameter* mPositionInPar; /// \brief position parameter (input)
    Parameter* mPositionOutPar; /// \brief position parameter (output)
    Parameter* mLowerBoundaryPar; /// \brief lower boundary parameter (internal)
    Parameter* mUpperBoundaryPar; /// \brief upper boundary parameter (internal)
};

};

};

#endif