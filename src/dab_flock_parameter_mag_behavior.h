/** \file dab_flock_parameter_mag_behavior.h
 *  \class dab::flock::ParameterMagBehavior stores magnitude of input parameter in output parameter
 *	\brief stores magnitude of input parameter in output parameter
 *
 *  \n
 *  Input Parameter:\n
 *  type: any dim: nD neighbors: ignored\n
 *  \n
 *  Output Parameter:\n
 *  type: magnitude dim: 1D write: replace\n
 *  \n
 *  Internal Parameter:\n
 *  \n
 *  Created by Daniel Bisig on 09/11/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
 */

#ifndef _dab_flock_parameter_mag_behavior_h_
#define _dab_flock_parameter_mag_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{
    
    namespace flock
    {
        
        class Agent;
        
        class ParameterMagBehavior : public Behavior
        {
        public:
            /**
             \brief create behavior
             \param pInputParameterString input parameter string (parameters are space separated)
             \param pOutputParameterString output paramaters are space separated)
             */
            ParameterMagBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);
            
            /**
             \brief create behavior
             \param pAgent agent this behavior belongs to
             \param pBehaviorName name of behavior
             \param pInputParameterString input parameter string (parameters are space separated)
             \param pOutputParameterString output paramaters are space separated)
             \exception Exception wrong number of type of parameters
             */
            ParameterMagBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);
            
            /**
             \brief destructor
             */
            ~ParameterMagBehavior();
            
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
            Parameter* mParameter; ///\brief parameter (input)
            Parameter* mMagParameter; ///\brief magnitude parameter (output)
        };
        
    };
    
};

#endif