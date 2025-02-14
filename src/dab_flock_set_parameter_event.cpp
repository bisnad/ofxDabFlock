/** \file dab_flock_set_parameter_event.cpp
 */

#include "dab_flock_set_parameter_event.h"
#include "dab_flock_env.h"
#include "dab_math.h"

using namespace dab;
using namespace dab::flock;

SetParameterEvent::SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(0.0, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(-1)
, mAgentRangeEndIndex(-1)
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent(const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(0.0, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(pAgentIndex)
, mAgentRangeEndIndex(pAgentIndex)
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(0.0, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex( pAgentRange[0] )
, mAgentRangeEndIndex( pAgentRange[1] )
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(pTime, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(-1)
, mAgentRangeEndIndex(-1)
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(pTime, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(pAgentIndex)
, mAgentRangeEndIndex(pAgentIndex)
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pParameterValues, double pDuration)
: event::Event(pTime, pDuration, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex( pAgentRange[0] )
, mAgentRangeEndIndex( pAgentRange[1] )
, mParameterValues(pParameterValues)
, mParameterValues2(pParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(false)
{}

SetParameterEvent::SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(-1)
, mAgentRangeEndIndex(-1)
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent( const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(pAgentIndex)
, mAgentRangeEndIndex(pAgentIndex)
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent(const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex( pAgentRange[0] )
, mAgentRangeEndIndex( pAgentRange[1] )
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(-1)
, mAgentRangeEndIndex(-1)
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, int pAgentIndex, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex(pAgentIndex)
, mAgentRangeEndIndex(pAgentIndex)
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent(double pTime, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int,2>& pAgentRange, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSwarmName(pSwarmName)
, mParameterName(pParameterName)
, mAgentRangeStartIndex( pAgentRange[0] )
, mAgentRangeEndIndex( pAgentRange[1] )
, mParameterValues(pMinParameterValues)
, mParameterValues2(pMaxParameterValues)
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize(true)
{}

SetParameterEvent::SetParameterEvent( const SetParameterEvent& pEvent )
: event::Event( pEvent )
, mSwarmName( pEvent.mSwarmName )
, mParameterName( pEvent.mParameterName )
, mAgentRangeStartIndex( pEvent.mAgentRangeStartIndex )
, mAgentRangeEndIndex( pEvent.mAgentRangeEndIndex )
, mParameterValues( pEvent.mParameterValues )
, mParameterValues2( pEvent.mParameterValues2 )
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize( pEvent.mRandomize )
{}

SetParameterEvent::SetParameterEvent( double pTime, const SetParameterEvent& pEvent )
: event::Event( pTime, pEvent )
, mSwarmName( pEvent.mSwarmName )
, mParameterName( pEvent.mParameterName )
, mAgentRangeStartIndex( pEvent.mAgentRangeStartIndex )
, mAgentRangeEndIndex( pEvent.mAgentRangeEndIndex )
, mParameterValues( pEvent.mParameterValues )
, mParameterValues2( pEvent.mParameterValues2 )
, mUpdatedParameterValues( mParameterValues.rows() )
, mRandomize( pEvent.mRandomize )
{}

SetParameterEvent::~SetParameterEvent()
{}

event::Event*
SetParameterEvent::copy() const
{
	return new SetParameterEvent( *this );
}

event::Event*
SetParameterEvent::copy( double pTime ) const
{
	return new SetParameterEvent( pTime, *this );
}

void
SetParameterEvent::execute() throw (Exception)
{
	//std::cout << "SetParameterEvent::execute()\n";
	
	mFinished = false;
	
	try
	{
		if( mStarted == true && mRemainingDuration <= 0.0 && mFinished == true )
		{
			return;
		}
		
		mPreviousEventTime = mCurrentEventTime;
		mCurrentEventTime = ofGetElapsedTimef();
		if( mStarted == true ) mEventInterval = mCurrentEventTime - mPreviousEventTime;
		else mEventInterval = FLT_MIN;
		
		if( mStarted == false )
		{
			mStartTime = mTime;
			mStarted = true;
		}
		
		double stepCount = 1.0;
		double invStepCount = 1.0;
		
		mRemainingDuration = ( mStartTime + mDuration ) - mCurrentEventTime;
		mRemainingDuration -= mEventInterval; // safety: be one step ahead so we rather finish an event with duration early than later
		
		if( mRemainingDuration > 0.0 )
		{
			stepCount = mRemainingDuration / mEventInterval;
			invStepCount = 1.0 / stepCount;
		}
		
		// simple version of parameter set for env
		if( Simulation::get().checkEnv(mSwarmName) == true )
		{
			Env* env = Simulation::get().env( mSwarmName );
			env->set( mParameterName, mParameterValues );
			mFinished = true;
			
			return;
		};
		// env done
		
		
		Swarm* swarm = Simulation::get().swarm(mSwarmName);
        std::vector<Agent*>& agents = swarm->agents();
		
		///////////////////////////////
		// set swarm parameter value //
		///////////////////////////////
		
		if(mAgentRangeStartIndex == -1 && mAgentRangeEndIndex == -1)
		{
			if( mRemainingDuration > 0.0 )
			{
				unsigned int valueDim = mParameterValues.rows();
				
				Parameter* parameter = ( static_cast<Agent*>( swarm ) )->parameter(mParameterName);
				Eigen::VectorXf& curParValues = parameter->values();
				
				//std::cout << "curParValues " << curParValues << "\n";
				
				for(int d=0; d<valueDim; ++d)
				{
					mUpdatedParameterValues[d] = curParValues[d] + ( mParameterValues[d] - curParValues[d] ) * invStepCount;
				}
				
				parameter->setValues(mUpdatedParameterValues);
			}
			else if( mRandomize == true )
			{
				math::Math<>& math = math::Math<>::get();
				unsigned int valueDim = mParameterValues.rows();
				Eigen::VectorXf randomValues( valueDim );
				
				for(int d=0; d<valueDim; ++d) randomValues[d] = math.random( mParameterValues[d], mParameterValues2[d] );
				( static_cast<Agent*>( swarm ) )->parameter(mParameterName)->setValues(randomValues);
			}
			else
			{
				( static_cast<Agent*>( swarm ) )->parameter(mParameterName)->setValues(mParameterValues);
			}
		}
		
		///////////////////////////////
		// set agent parameter value //
		///////////////////////////////
		
		if(agents.size() != 0)
		{
			unsigned int parameterIndex = agents[0]->parameterIndex(mParameterName);
			int agentRangeStartIndex = mAgentRangeStartIndex;
			int agentRangeEndIndex = mAgentRangeEndIndex;
			
			
			if(agentRangeStartIndex == -1 || agentRangeStartIndex < 0) agentRangeStartIndex = 0;
			if(agentRangeEndIndex == -1 || agentRangeEndIndex >= agents.size()) agentRangeEndIndex = agents.size() - 1;
			
			//std::cout << "set agent par from " << mAgentRangeStartIndex << " to " << mAgentRangeEndIndex << "\n";
			
			if( mRemainingDuration > 0.0 )
			{
				unsigned int valueDim = mParameterValues.rows();
				
				for(int agentNr = agentRangeStartIndex; agentNr <= agentRangeEndIndex; ++agentNr)
				{
					Parameter* parameter = agents[agentNr]->parameter(parameterIndex);
					Eigen::VectorXf& curParValues = parameter->values();
					
					for(int d=0; d<valueDim; ++d)
					{
						mUpdatedParameterValues[d] = curParValues[d] + ( mParameterValues[d] - curParValues[d] ) * invStepCount;
					}
					
					//std::cout << "agentNr " << agentNr << " curPar " << curParValues << " targetPar " << mParameterValues << " updatePar " << mUpdatedParameterValues << "\n";
					
					parameter->setValues(mUpdatedParameterValues);
				}
				
				//std::cout << "before: duration " << mDuration << " remainingDuration " << mRemainingDuration << "\n";
				//std::cout << "after: duration " << mDuration << " remainingDuration " << mRemainingDuration << "\n";
			}
			else if( mRandomize == true )
			{
				math::Math<>& math = math::Math<>::get();
				unsigned int valueDim = mParameterValues.rows();
				Eigen::VectorXf randomValues( valueDim );
				
				for(int agentNr = agentRangeStartIndex; agentNr <= agentRangeEndIndex; ++agentNr)
				{
					for(int d=0; d<valueDim; ++d) randomValues[d] = math.random( mParameterValues[d], mParameterValues2[d] );
					
					agents[agentNr]->parameter(parameterIndex)->setValues(randomValues);
					
					//std::cout << "set agent " << agents[agentNr]->name().toStdString() << " par " << agents[agentNr]->parameter(parameterIndex).name().toStdString() << " to " << randomValues << "\n";
				}
				
				mFinished = true;
			}
			else
			{
				for(int agentNr = agentRangeStartIndex; agentNr <= agentRangeEndIndex; ++agentNr)
				{
					agents[agentNr]->parameter(parameterIndex)->setValues(mParameterValues);
				}
				
				mFinished = true;
				
				//std::cout << "planned finish time " << mStartTime + mDuration << " current finish time " << static_cast<double>( base::Tools::get().time() ) * 0.001 << "\n";
				
			}
		}
	}
	catch(Exception& e)
	{
		Simulation::get().exceptionReport( e );
		
		mStarted = false;
		mFinished = true;
	}
}