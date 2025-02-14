/** \file dab_flock_set_parameter_event.h
 *
 *  Created by Daniel Bisig on 02/08/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 11/10/2017.
 */

#ifndef _dab_flock_set_parameter_event_h_
#define _dab_flock_set_parameter_event_h_

#include <array>
#include <Eigen/Dense>
#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class SetParameterEvent : public event::Event
{
public:
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pParameterValues, double pDuration = -1.0);
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    SetParameterEvent( double pTime, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues);
    
    SetParameterEvent( const SetParameterEvent& pEvent );
    SetParameterEvent( double pTime, const SetParameterEvent& pEvent );
    virtual ~SetParameterEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    std::string mSwarmName;
    std::string mParameterName;
    int mAgentRangeStartIndex;
    int mAgentRangeEndIndex;
    Eigen::VectorXf mParameterValues;
    Eigen::VectorXf mParameterValues2;
    Eigen::VectorXf mUpdatedParameterValues;
    float mStartTime;
    float mRemainingDuration;
    float mPreviousEventTime;
    float mCurrentEventTime;
    float mEventInterval;
    bool mRandomize;
};

};

};

#endif