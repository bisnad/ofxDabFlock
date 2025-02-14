/** \file dab_flock_parameter_threshold_to_event_behavior.h
*  \class dab::flock::ParameterThresholdToEventBehavior create events depending on whether the values an observed parameter enter or leave a value range
*	\brief create events depending on whether the values an observed parameter enter or leave a value range
*
*  The Behavior creates events depending on whether the values an observed parameter enter or leave a value range\n
*  The value range is defined by the two internal parameters : lowerThreshold & upperThreshold
*  Depending on the value of the internal parameter thresholdComparisonCriteria, an event is generated when the observed parameters values enter or leave the threshold range\n
*    EnterThresholdRange : event is created when observed parameter is within threshold range
*    LeaveThresholdRange : event is created when observed parameter is outside threshold range
*	Depending on the value of the internal parameter thresholdComparisonCriteria, the observed parameters values are compared differently against the treshold range
*    SignedComponentComparisonMode compare individual signed parameter components
*    UnsignedComponentComparisonMode compare individual unsigned parameter components
*    LengthComparisonMode compare length of parameter
*	Depending on the value of the internal parameter eventTriggerFrequency events are either always created or only once
*    SingleTrigger event is triggered only in the moment were a parameter that didn't fulfill the event criteria fulfills it now
*    ContinuousTrigger event is triggered always when a parameter fulfills event criteria
*  The value of the event is copied from the specified send input parameter\n
*  The event is sent to the specified target parameter\n
*  \n
*  Input Parameter:\n
*  type: observed dim: nD neighbors: ignored\n
*  type: send dim: nD neighbors: ignored\n
*  \n
*  Output Parameter:\n
*  type: target dim: nD write: replace via event\n
*  \n
*  Internal Parameter:\n
*  name: xxx_lowerThreshold dim: nD defaultValue: 0.0\n
*  name: xxx_upperThreshold dim: nD defaultValue: 1.0\n
*  name: xxx_eventTriggerCriteria dim: 1D defaultValue: LeaveThresholdRange\n
*  name: xxx_thresholdComparisonCriteria dim: 1D defaultValue: UnsignedComponentComparisonMode\n
*  name: xxx_eventTriggerFrequency dim: 1D defaultValue: ContinuousTrigger\n
*  name: xxx_active dim: 1D defaultValue: 0.1\n
*  \n
*  Created by Daniel Bisig on 05/12/08.
*  Ported to OpenFrameworks by Daniel Bisig on 20/10/2017.
*/

#ifndef _dab_flock_parameter_threshold_to_event_behavior_h_
#define _dab_flock_parameter_threshold_to_event_behavior_h_

#include "dab_flock_behavior.h"
#include <Eigen/Dense>

namespace dab
{

	namespace flock
	{

		class Agent;

		class ParameterThresholdToEventBehavior : public Behavior
		{
		public:
			/**
			\brief criteria for triggering event
			*/
			enum EventTriggerCriteria
			{
				EnterThresholdRange, ///\brief value enter treshold range
				LeaveThresholdRange	 ///\brief value leaves threshold range
			};

			/**
			\brief threshold comparison Mode
			*/
			enum ThresholdComparisonMode
			{
				SignedComponentComparisonMode, ///\brief compare signed component values of parameter and threshold
				UnsignedComponentComparisonMode, ///\brief compare unsigned component values of parameter and threshold
				LengthComparisonMode ///\brief compare length of parameter and threshold
			};

			/**
			\brief event trigger frequency
			*/
			enum EventTriggerFrequency
			{
				SingleTrigger, ///\brief event is triggered only in the moment were a parameter that didn't exceed the treshold beforehand exceed it now
				ContinuousTrigger///\brief event is triggered always when a parameter exceeds the treshold
			};

			/**
			\brief create behavior
			\param pInputParameterString input parameter string (parameters are space separated)
			\param pOutputParameterString output paramaters are space separated)
			*/
			ParameterThresholdToEventBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString);

			/**
			\brief create behavior
			\param pAgent agent this behavior belongs to
			\param pBehaviorName name of behavior
			\param pInputParameterString input parameter string (parameters are space separated)
			\param pOutputParameterString output paramaters are space separated)
			\exception Exception wrong number of type of parameters
			*/
			ParameterThresholdToEventBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception);

			/**
			\brief destructor
			*/
			~ParameterThresholdToEventBehavior();

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
			void init();

			Parameter* mObservePar; ///\brief observed parameter
			Parameter* mSendPar; ///\brief parameter that is sent
			Parameter* mTargetPar; ///\brief event parameter (the parameter the event is being sent to)
			Parameter* mLowerThresholdPar; ///\brief threshold for observed parameter (whenever it exceeds that, event is sent)
			Parameter* mUpperThresholdPar; ///\brief threshold for observed parameter (whenever it exceeds that, event is sent)
			Parameter* mEventTriggerCriteriaPar; ///\brief event trigger criteria
			Parameter* mThresholdComparisonModePar; ///\brief threshold comparison mode
			Parameter* mEventTriggerFrequencyPar; ///\brief event trigger frequency

			bool mInitialized;

			EventTriggerCriteria mEventTriggerCriteria; ///\brief event trigger criteria
			ThresholdComparisonMode mThresholdComparisonMode; ///\brief threshold comparison mode
			EventTriggerFrequency mEventTriggerFrequency; ///\brief event trigger frequency
			bool mThresholdExceededFlag; ///\brief flag whether threshold has previously been exceeded or not
		};

	};

};

#endif