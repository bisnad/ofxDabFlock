/** \file dab_flock_parameter_threshold_to_event_behavior.cpp
*/

#include "dab_flock_parameter_threshold_to_event_behavior.h"
#include "dab_flock_parameter.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

ParameterThresholdToEventBehavior::ParameterThresholdToEventBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
	: Behavior(pInputParameterString, pOutputParameterString)
	, mInitialized(false)
{
	mClassName = "ParameterThresholdToEventBehavior";
}

ParameterThresholdToEventBehavior::ParameterThresholdToEventBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
	: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
	, mInitialized(false)
{
	mClassName = "ParameterThresholdToEventBehavior";

	if (mInputParameters.size() < 2) throw Exception("FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(2) + " needed", __FILE__, __FUNCTION__, __LINE__);
	if (mOutputParameters.size() < 1) throw Exception("FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__);
	if (mInputParameters[1]->dim() != mOutputParameters[0]->dim())  throw Exception("FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__);

	// input parameters
	mObservePar = mInputParameters[0];
	mSendPar = mInputParameters[1];

	// output parameter
	mTargetPar = mOutputParameters[0];

	// create internal parameters
	mLowerThresholdPar = createInternalParameter("lowerThreshold", std::vector<float>(mInputParameters[0]->dim(), 0.0));
	mUpperThresholdPar = createInternalParameter("upperThreshold", std::vector<float>(mInputParameters[0]->dim(), 1.0));
	mEventTriggerCriteriaPar = createInternalParameter("eventTriggerCriteria", { 1.0 });
	mThresholdComparisonModePar = createInternalParameter("thresholdComparisonCriteria", { 1.0 });
	mEventTriggerFrequencyPar = createInternalParameter("eventTriggerFrequency", { 1.0 });

	// other member variables
	mThresholdExceededFlag = false;
	mInitialized = false;
}

ParameterThresholdToEventBehavior::~ParameterThresholdToEventBehavior()
{}

Behavior*
ParameterThresholdToEventBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if (pAgent != NULL) return new ParameterThresholdToEventBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new ParameterThresholdToEventBehavior(mInputParameterString, mOutputParameterString);
	}
	catch (Exception& e)
	{
		throw;
	}
}

Behavior*
ParameterThresholdToEventBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new ParameterThresholdToEventBehavior(pInputParameterString, pOutputParameterString);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void
ParameterThresholdToEventBehavior::init()
{
	mEventTriggerCriteria = static_cast<EventTriggerCriteria>(static_cast<int>(mEventTriggerCriteriaPar->value()));
	mThresholdComparisonMode = static_cast<ThresholdComparisonMode>(static_cast<int>(mThresholdComparisonModePar->value()));
	mEventTriggerFrequency = static_cast<EventTriggerFrequency>(static_cast<int>(mEventTriggerFrequencyPar->value()));

	mInitialized = true;
}

void
ParameterThresholdToEventBehavior::act()
{
	if (mInitialized == false) init();
	if (mActivePar->value() <= 0.0) return;

	Eigen::VectorXf& observeValues = mObservePar->values();
	Eigen::VectorXf& sendValues = mSendPar->values();
	Eigen::VectorXf& targetValues = mTargetPar->backupValues();
	Eigen::VectorXf& lowerThresholdValues = mLowerThresholdPar->values();
	Eigen::VectorXf& upperThresholdValues = mUpperThresholdPar->values();
	EventTriggerCriteria& eventTriggerCriteria = mEventTriggerCriteria;
	ThresholdComparisonMode& thresholdComparisonMode = mThresholdComparisonMode;
	EventTriggerFrequency& eventTriggerFrequency = mEventTriggerFrequency;
	bool& thresholdExceededFlag = mThresholdExceededFlag;
	bool tmpExceeededFlag = false;
	unsigned int dim = observeValues.rows();
	unsigned int d;

	// test whether parameter values exceed threshold
	if (eventTriggerCriteria == EnterThresholdRange)
	{
		if (thresholdComparisonMode == SignedComponentComparisonMode)
		{
			for (d = 0; d<dim && (observeValues[d] < lowerThresholdValues[d] || observeValues[d] > upperThresholdValues[d]); ++d) {}
			if (d < dim) tmpExceeededFlag = true;
		}
		else if (thresholdComparisonMode == UnsignedComponentComparisonMode)
		{
			for (d = 0; d<dim && (fabs(observeValues[d]) < fabs(lowerThresholdValues[d]) || fabs(observeValues[d]) > fabs(upperThresholdValues[d])); ++d) {}
			if (d < dim) tmpExceeededFlag = true;
		}
		else if (thresholdComparisonMode == LengthComparisonMode)
		{
			if (observeValues.squaredNorm() >= lowerThresholdValues.squaredNorm() && observeValues.squaredNorm() <= upperThresholdValues.squaredNorm()) tmpExceeededFlag = true;
		}

	}
	else if (eventTriggerCriteria == LeaveThresholdRange)
	{
		if (thresholdComparisonMode == SignedComponentComparisonMode)
		{
			for (d = 0; d<dim && (observeValues[d] > lowerThresholdValues[d] || observeValues[d] < upperThresholdValues[d]); ++d) {}
			if (d < dim) tmpExceeededFlag = true;
		}
		else if (thresholdComparisonMode == UnsignedComponentComparisonMode)
		{
			for (d = 0; d<dim && (fabs(observeValues[d]) > fabs(lowerThresholdValues[d]) || fabs(observeValues[d]) < fabs(upperThresholdValues[d])); ++d) {}
			if (d < dim) tmpExceeededFlag = true;
		}
		else if (thresholdComparisonMode == LengthComparisonMode)
		{
			if (observeValues.squaredNorm() <= lowerThresholdValues.squaredNorm() || observeValues.squaredNorm() >= upperThresholdValues.squaredNorm()) tmpExceeededFlag = true;
		}
	}

	//std::cout << "vel " << observeValues.lengthSquared() << " low thresh " << lowerThresholdValues.lengthSquared() << " high thresh " << upperThresholdValues.lengthSquared() << " tmpExceeededFlag " << tmpExceeededFlag << "\n";


	// decide whether to create event
	if (tmpExceeededFlag == true)
	{
		if (thresholdExceededFlag == false || eventTriggerFrequency == ContinuousTrigger)
		{

			//Simulation::get().event().addEvent( new SetParameterEvent( 0.0, mTargetPar->agent()->name(), "alignment_amount", sendValues ) );
			//mTargetPar->schedule(0.0, sendValues );
			// TODO: broken for the moment
			thresholdExceededFlag = true;

			//std::cout << "observeValues " << observeValues << " lowT " << lowerThresholdValues << " upT " << upperThresholdValues << " sendValues " << sendValues <<  "\n";
		}
	}
	else thresholdExceededFlag = false;
}