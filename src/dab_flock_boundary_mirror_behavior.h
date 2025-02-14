/** \file dab_flock_boundary_mirror_behavior.h
 *  \class dab::flock::BoundaryMirrorBehavior creates reflective boundary for Agent 's position
 *	\brief creates reflective boundary for Agent 's position
 *
 *  Behavior creates reflective boundary for Agent 's position.\n
 *  The behavior is normally meant to restrict an Agent 's position to a cubic region\n
 *  by reflecting the velocity and acceleration at the cube's boundaries.\n
 *  \n
 *  Input Parameter:\n
 *  type: position dim: nD neighbors: ignore\n
 *  type: velocity dim: nD neighbors: ignore\n
 *  type: acceleration dim: nD neighbors: ignore\n
 *  \n
 *  Output Parameter:\n
 *  type: velocity dim: nD write: add\n
 *  type: acceleration dim: nD write: add\n
 *  \n
 *  Internal Parameter:\n
 *  name: xxx_lowerBoundary dim: nD defaultValue: -1.0\n
 *  name: xxx_upperBoundary dim: nD defaultValue: 1.0\n
 *  name: xxx_active dim: 1D defaultValue: 1.0\n
 *  \n
 *  Created by Daniel Bisig on 4/19/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 01/10/2017.
 */

#ifndef _dab_flock_boundary_mirror_behavior_h_
#define _dab_flock_boundary_mirror_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

namespace flock
{

class BoundaryMirrorBehavior : public Behavior
{
public:
    /**
     \brief create behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     */
    BoundaryMirrorBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
    
    /**
     \brief create behavior
     \param pAgent agent this behavior belongs to
     \param pBehaviorName name of behavior
     \param pInputParameterString input parameter string (parameters are space separated)
     \param pOutputParameterString output paramaters are space separated)
     \exception Exception wrong number of type of parameters
     */
    BoundaryMirrorBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
    
    /**
     \brief destructor
     */
    ~BoundaryMirrorBehavior();
    
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
    Parameter* mDerivative0InPar; /// \brief zero order derivative parameter (input)
    Parameter* mDerivative1InPar; /// \brief first order derivative parameter (input)
    Parameter* mDerivative2InPar; /// \brief second order derivative parameter (input)
    Parameter* mDerivative1OutPar; /// \brief first order derivative parameter (output)
    Parameter* mDerivative2OutPar; /// \brief second order derivative parameter (output)
    Parameter* mLowerBoundaryPar; /// \brief lower boundary parameter (internal)
    Parameter* mUpperBoundaryPar; /// \brief upper boundary parameter (internal)
};

};

};

#endif