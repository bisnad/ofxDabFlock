/** \file dab_flock_com.cpp
*/

#include "dab_flock_com.h"
#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_flock_parameter.h"
#include "dab_flock_osc_control.h"
#include "dab_exception.h"
#include "dab_osc_messenger.h"
#include "dab_osc_sender.h"

using namespace dab;
using namespace dab::flock;

#pragma mark parameter communication registration

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(1)
	, mNormalise(false)
	, mMaskValues(false)
{
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(1)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(false)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(1)
	, mNormalise(false)
	, mMaskValues(false)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(1)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(false)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(false)
	, mMaskValues(false)
{
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(false)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(false)
	, mMaskValues(false)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(false)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::vector<bool>& pParValueMask)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(1)
	, mNormalise(false)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(1)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(1)
	, mNormalise(false)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(1)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(false)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange({ -1, -1 })
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(false)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
}

ParameterRegistration::ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
	: mSwarmName(pSwarmName)
	, mParameterName(pParameterName)
	, mSenderName(pSenderName)
	, mSendInterval(pSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pAgentRange)
	, mAgentGroupSize(pAgentGroupSize)
	, mNormalise(true)
	, mMinParValue(pMinParValue)
	, mMaxParValue(pMaxParValue)
	, mParValueScale(pMaxParValue - pMinParValue)
	, mParValueOffset(pMinParValue)
	, mMaskValues(true)
	, mParValueMask(pParValueMask)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}


ParameterRegistration::ParameterRegistration(const ParameterRegistration& pRegistration)
	: mSwarmName(pRegistration.mSwarmName)
	, mParameterName(pRegistration.mParameterName)
	, mSenderName(pRegistration.mSenderName)
	, mSendInterval(pRegistration.mSendInterval)
	, mCurrentSendInterval(mSendInterval)
	, mAgentRange(pRegistration.mAgentRange)
	, mAgentGroupSize(pRegistration.mAgentGroupSize)
	, mNormalise(pRegistration.mNormalise)
	, mMinParValue(pRegistration.mMinParValue)
	, mMaxParValue(pRegistration.mMaxParValue)
	, mParValueScale(pRegistration.mParValueScale)
	, mParValueOffset(pRegistration.mParValueOffset)
	, mMaskValues(pRegistration.mMaskValues)
	, mParValueMask(pRegistration.mParValueMask)
{
}

ParameterRegistration::~ParameterRegistration()
{}

void
ParameterRegistration::set(unsigned int pSendInterval)
{
	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = 1;
	mNormalise = false;
	mMaskValues = false;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = 1;
	mNormalise = false;
	mMaskValues = false;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = 1;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mMaskValues = false;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = 1;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mMaskValues = false;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, unsigned int pAgentGroupSize)
{
	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = false;
	mMaskValues = false;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = false;
	mMaskValues = false;
}

void
ParameterRegistration::set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mMaskValues = false;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mMaskValues = false;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::vector<bool>& pParValueMask)
{
	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = 1;
	mNormalise = false;
	mParValueMask = pParValueMask;
	mMaskValues = true;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = 1;
	mNormalise = false;
	mParValueMask = pParValueMask;
	mMaskValues = true;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = 1;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mParValueMask = pParValueMask;
	mMaskValues = true;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = 1;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mParValueMask = pParValueMask;
	mMaskValues = true;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask)
{
	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = false;
	mParValueMask = pParValueMask;
	mMaskValues = true;
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = false;
	mParValueMask = pParValueMask;
	mMaskValues = true;
}

void
ParameterRegistration::set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = { -1, -1 };
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mParValueMask = pParValueMask;
	mMaskValues = true;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

void
ParameterRegistration::set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	if (pAgentRange[1] >= 0 && pAgentRange[0] > pAgentRange[1]) throw Exception("FLOCK ERROR: illegal agent range, min value " + std::to_string(pAgentRange[0]) + " exceeds max value " + std::to_string(pAgentRange[1]), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != pMaxParValue.rows()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of max value " + std::to_string(pMaxParValue.rows()), __FILE__, __FUNCTION__, __LINE__);
	if (pMinParValue.rows() != mParValueMask.size()) throw Exception("FLOCK ERROR: dim of min value " + std::to_string(pMinParValue.rows()) + " doesn't match dim of mask value " + std::to_string(pParValueMask.size()), __FILE__, __FUNCTION__, __LINE__);

	mSendInterval = pSendInterval;
	mCurrentSendInterval = mSendInterval;
	mAgentRange = pAgentRange;
	mAgentGroupSize = pAgentGroupSize;
	mNormalise = true;
	mMinParValue = pMinParValue;
	mMaxParValue = pMaxParValue;
	mParValueScale = pMaxParValue - pMinParValue;
	mParValueOffset = pMinParValue;
	mParValueMask = pParValueMask;
	mMaskValues = true;

	unsigned int dim = mParValueScale.rows();
	for (unsigned int d = 0; d<dim; ++d) mParValueScale[d] = 1.0 / (mParValueScale[d]);
}

const std::string&
ParameterRegistration::swarmName() const
{
	return mSwarmName;
}

const std::string&
ParameterRegistration::parameterName() const
{
	return mParameterName;
}

const std::string&
ParameterRegistration::senderName() const
{
	return mSenderName;
}

unsigned int
ParameterRegistration::sendInterval() const
{
	return mSendInterval;
}

unsigned int&
ParameterRegistration::currentSendInterval()
{
	return mCurrentSendInterval;
}

const std::array<int, 2>&
ParameterRegistration::agentRange() const
{
	return mAgentRange;
}

unsigned int
ParameterRegistration::agentGroupSize() const
{
	return mAgentGroupSize;
}

bool
ParameterRegistration::normalise() const
{
	return mNormalise;
}

const Eigen::VectorXf&
ParameterRegistration::minParValue()const
{
	return mMinParValue;
}

const Eigen::VectorXf&
ParameterRegistration::maxParValue()const
{
	return mMaxParValue;
}

const Eigen::VectorXf&
ParameterRegistration::parValueScale() const
{
	return mParValueScale;
}

const Eigen::VectorXf&
ParameterRegistration::parValueOffset() const
{
	return mParValueOffset;
}

bool
ParameterRegistration::maskValues() const
{
	return mMaskValues;
}

const std::vector<bool>&
ParameterRegistration::parValueMask() const
{
	return mParValueMask;
}

bool
ParameterRegistration::operator==(const ParameterRegistration& pRegistration) const
{
	return match(pRegistration.mSwarmName, pRegistration.mParameterName);
}

bool
ParameterRegistration::match(const std::string& pSwarmName, const std::string& pParameterName) const
{
	return (mSwarmName == pSwarmName && mParameterName == pParameterName);
}

#pragma mark flock messenger

FlockCom::FlockCom()
	: OscMessenger()
{}

FlockCom::~FlockCom()
{
	// delete all parameter registrations
	std::map< std::shared_ptr<OscSender>, std::vector<ParameterRegistration*> >& parameterMap = mParameterRegistry.map();

	for (auto iter = parameterMap.begin(); iter != parameterMap.end(); ++iter)
	{
		std::vector<ParameterRegistration*>& parameterRegistration = iter->second;

		int regCount = parameterRegistration.size();

		for (int rI = 0; rI<regCount; ++rI)
		{
			delete parameterRegistration[rI];
		}
	}

	mParameterRegistry.clear();
	mExtendedOscMode.clear();
}

void
FlockCom::createOscControl(unsigned int pReceiverPort, const std::string& pSenderIP, unsigned int pSenderPort) throw (Exception)
{
	try
	{
		mErrorSender = std::shared_ptr<OscSender>(new OscSender("ErrorSender", pSenderIP, pSenderPort));
		mControlReceiver = std::shared_ptr<OscReceiver>(new OscReceiver("ControlReceiver", pReceiverPort));
		mOscControl = std::shared_ptr<OscControl>(new OscControl(mErrorSender));
		mOscControl->setErrorSender(mErrorSender);
		mControlReceiver->registerOscListener(mOscControl);
		mControlReceiver->start();

	}
	catch (Exception& e)
	{
		throw;
	}
}

void
FlockCom::createOscControl(std::shared_ptr<OscControl> pOscControl, unsigned int pReceiverPort, const std::string& pSenderIP, unsigned int pSenderPort) throw (Exception)
{
	try
	{
		mErrorSender = std::shared_ptr<OscSender>(new OscSender("ErrorSender", pSenderIP, pSenderPort));
		mControlReceiver = std::shared_ptr<OscReceiver>(new OscReceiver("ControlReceiver", pReceiverPort));
		mOscControl = pOscControl;
		mOscControl->setErrorSender(mErrorSender);
		mControlReceiver->registerOscListener(pOscControl);
		mControlReceiver->start();
	}
	catch (Exception& e)
	{
		throw;
	}
}

bool
FlockCom::checkParameterRegistration(std::shared_ptr<OscSender> pSender, const std::string& pSwarmName, const std::string& pParameterName)
{
	//std::cout << "FlockCom::checkParameterRegistration()\n";
	if (mParameterRegistry.contains(pSender) == false) return false;
	std::vector< ParameterRegistration* >& regs = mParameterRegistry[pSender];

	unsigned int regCount = regs.size();
	for (unsigned int rI = 0; rI<regCount; ++rI) if (regs[rI]->match(pSwarmName, pParameterName) == true) return true;
	return false;
}

ParameterRegistration*
FlockCom::parameterRegistration(std::shared_ptr<OscSender> pSender, const std::string& pSwarmName, const std::string& pParameterName) throw (Exception)
{
	//std::cout << "FlockCom::parameterRegistration\n";

	if (mParameterRegistry.contains(pSender) == false) throw Exception("FLOCK ERROR: sender " + pSender->name() + " does not exist", __FILE__, __FUNCTION__, __LINE__);
	std::vector< ParameterRegistration* >& regs = mParameterRegistry[pSender];

	unsigned int regCount = regs.size();
	for (unsigned int rI = 0; rI<regCount; ++rI) if (regs[rI]->match(pSwarmName, pParameterName) == true)
	{
		return regs[rI];
	}

	throw Exception("FLOCK ERROR: parameter " + pParameterName + " of swarm " + pSwarmName + " not registered for sender " + pSender->name(), __FILE__, __FUNCTION__, __LINE__);
}

std::vector< ParameterRegistration* >
FlockCom::parameterRegistrations(const std::string& pSwarmName, const std::string& pParameterName) const
{
	std::vector< ParameterRegistration* > result;

	const std::map< std::shared_ptr<OscSender>, std::vector<ParameterRegistration*> >& parameterMap = mParameterRegistry.map();

	for (auto iter = parameterMap.begin(); iter != parameterMap.end(); ++iter)
	{
		const std::vector<ParameterRegistration*>& parRegs = iter->second;

		int regCount = parRegs.size();

		for (int rI = 0; rI<regCount; ++rI)
		{
			if (parRegs[rI]->swarmName() == pSwarmName && parRegs[rI]->parameterName() == pParameterName) result.push_back(parRegs[rI]);
		}
	}

	return result;
}

bool
FlockCom::checkParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName)
{
	if (checkSender(pSenderName) == false) return false;

	std::shared_ptr<OscSender> pSender = sender(pSenderName);
	std::vector< ParameterRegistration* > regs = mParameterRegistry[pSender];

	unsigned int regCount = regs.size();
	for (unsigned int rI = 0; rI<regCount; ++rI) if (regs[rI]->match(pSwarmName, pParameterName) == true) return true;
	return false;
}

void
FlockCom::createSender(const std::string& pSenderName, const std::string& pReceiverIP, unsigned int pReceiverPort, bool pExtendedOscMode) throw (Exception)
{
	try
	{
		std::shared_ptr<OscSender> _sender = OscMessenger::createSender(pSenderName, pReceiverIP, pReceiverPort);

		mExtendedOscMode.add(_sender, pExtendedOscMode);
		mParameterRegistry.add(_sender, std::vector<ParameterRegistration*>());
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::removeSender(const std::string& pSenderName) throw (dab::Exception)
{
	try
	{
		//std::cout << "removeSender name " << pSenderName.toStdString() << " begin\n";

		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		mParameterRegistry.remove(_sender);
		mExtendedOscMode.remove(_sender);

		OscMessenger::removeSender(pSenderName);

		//std::cout << "removeSender name " << pSenderName.toStdString() << " end\n";
	}
	catch (dab::Exception& e)
	{
		throw;
	}
}

void
FlockCom::removeSenders()
{
	std::map<std::string, std::shared_ptr<OscSender> >& senderMap = mSenders.map();
	for (auto iter = senderMap.begin(); iter != senderMap.end(); ++iter)
	{
		std::shared_ptr<OscSender> _sender = iter->second;

		mParameterRegistry.remove(_sender);
		mExtendedOscMode.remove(_sender);
	}

	OscMessenger::removeSenders();
}

bool
FlockCom::extendedOscMode(const std::string& pSenderName) throw (Exception)
{
	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);
		return mExtendedOscMode[_sender];
	}
	catch (Exception& e)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, 1);
	}
	catch (dab::Exception&)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, 1, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, 1);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, 1, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, 1);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, 1, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, pAgentRange, 1);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, pAgentRange, 1, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentGroupSize);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentGroupSize, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, pAgentGroupSize);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize) throw (dab::Exception)
{
	//std::cout << "FlockCom::registerParameter( " << pSenderName << ", " << pSwarmName << ", " << pParameterName << " )\n";

	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentGroupSize);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentGroupSize));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	//std::cout << "FlockCom::registerParameter( " << pSenderName << ", " << pSwarmName << ", " << pParameterName << ", " << pMinParValue << ", " << pMaxParValue <<  " )\n";

	//std::cout << "FlockCom::registerParameter( " << pSenderName << ", " << pSwarmName << ", " << pParameterName << " )\n";

	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentGroupSize);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentGroupSize));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}




	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentGroupSize, pMinParValue, pMaxParValue);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentGroupSize, pMinParValue, pMaxParValue));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception)
{
	try
	{
		//std::cout << "FlockCom::registerParameter( pSenderName " << pSenderName.toStdString() << " , pSendInterval " << pSendInterval << " , pSwarmName " << pSwarmName.toStdString() << " , pParameterName " << pParameterName.toStdString() << " , pAgentRange " << pAgentRange << " )\n";

		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentRange, pAgentGroupSize);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentRange, pAgentGroupSize));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (dab::Exception)
{
	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

// <<<<<

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::vector<bool>& pParValueMask) throw (dab::Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, 1, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, 1, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, 1, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, 1, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, 1, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, 1, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, pAgentRange, 1, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, pSendInterval, pSwarmName, pParameterName, pAgentRange, 1, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentGroupSize, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, pAgentGroupSize, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		registerParameter(pSenderName, 0, pSwarmName, pParameterName, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask);
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
{
	//std::cout << "FlockCom::registerParameter( " << pSenderName.toStdString() << ", " << pSwarmName.toStdString() << ", " << pParameterName.toStdString() << " )\n";

	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentGroupSize, pParValueMask);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentGroupSize, pParValueMask));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	//	std::cout << "FlockCom::registerParameter( " << pSenderName.toStdString() << ", " << pSwarmName.toStdString() << ", " << pParameterName.toStdString() << ", " << pMinParValue << ", " << pMaxParValue <<  " )\n";

	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		//std::cout << "FlockCom::registerParameter( pSenderName " << pSenderName.toStdString() << " , pSendInterval " << pSendInterval << " , pSwarmName " << pSwarmName.toStdString() << " , pParameterName " << pParameterName.toStdString() << " , pAgentRange " << pAgentRange << " )\n";

		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentRange, pAgentGroupSize, pParValueMask);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentRange, pAgentGroupSize, pParValueMask));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception)
{
	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		//		// dirty fix
		//		if( checkParameterRegistration( snd , pSwarmName, pParameterName) == true ) return;
		//		// dirty fix end

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == true)
		{
			ParameterRegistration* parReg = parameterRegistration(_sender, pSwarmName, pParameterName);
			parReg->set(pSendInterval, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask);
		}
		else
		{
			mParameterRegistry[_sender].push_back(new ParameterRegistration(pSwarmName, pParameterName, pSenderName, pSendInterval, pAgentRange, pAgentGroupSize, pMinParValue, pMaxParValue, pParValueMask));
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to register parameter for sender " + pSenderName + " swarm " + pSwarmName + " parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

// <<<<<

void
FlockCom::deregisterParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName) throw (dab::Exception)
{
	try
	{
		std::shared_ptr<OscSender> _sender = sender(pSenderName);

		if (checkParameterRegistration(_sender, pSwarmName, pParameterName) == false)
		{
			throw Exception("COM ERROR: parameter " + pParameterName + " of swarm " + pSwarmName + " not registered for sender " + pSenderName, __FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			std::vector<ParameterRegistration*>& regs = mParameterRegistry[_sender];

			int regCount = regs.size();
			for (int rI = regCount - 1; rI >= 0; --rI)
			{
				ParameterRegistration* reg = regs[rI];
				if (reg->match(pSwarmName, pParameterName) == true)
				{
					regs.erase(regs.begin() + rI);
					delete reg;
					return;
				}
			}
		}
	}
	catch (dab::Exception& e)
	{
		e += Exception("COM ERROR: failed to deregister swarm " + pSwarmName + " parameter " + pParameterName + " for sender " + pSenderName, __FILE__, __FUNCTION__, __LINE__);

		throw e;
	}
}

void
FlockCom::update() throw (dab::Exception)
{
	try
	{
		send();
	}
	catch (dab::Exception& e)
	{
		std::cout << e << "\n";
	}
}

void
FlockCom::send() throw (dab::Exception)
{
	try
	{
		std::vector< std::shared_ptr<OscSender> > _senders = mSenders.values();

		unsigned int sndCount = _senders.size();
		std::shared_ptr<OscSender> _sender;

		for (unsigned int sI = 0; sI<sndCount; ++sI)
		{
			_sender = _senders[sI];

			if (mParameterRegistry[_sender].size() > 0)
			{
				sendMessage(_sender);
			}
		}
	}
	catch (dab::Exception)
	{
		throw;
	}
}

void
FlockCom::sendMessage(std::shared_ptr<OscSender> pSender) throw (dab::Exception)
{
	//std::cout << "FlockCom::sendMessage( com::Sender* pSender ) begin\n";

	try
	{
		std::vector<ParameterRegistration*>& regs = mParameterRegistry[pSender];
		bool extOscMode = mExtendedOscMode[pSender];

		unsigned int regCount = regs.size();

		for (unsigned int rI = 0; rI<regCount; ++rI)
		{
			ParameterRegistration* reg = regs[rI];

			if (reg->sendInterval() > 0 && reg->currentSendInterval() < reg->sendInterval()) reg->currentSendInterval() += 1;
			else
			{
				sendMessage(pSender, reg, extOscMode);
				reg->currentSendInterval() = 0;
			}
		}

		//pSender->send();
	}
	catch (dab::Exception& e)
	{
		e += Exception("COM ERROR: failed to send message with sender " + pSender->name(), __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}

	//std::cout << "FlockCom::sendMessage( com::Sender* pSender ) end\n";
}

//void
//FlockCom::sendMessage( std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, bool pExtendedOscMode ) throw (dab::Exception)
//{
//	//std::cout << "FlockCom::sendOSCMessage( com::Sender* pSender, ParameterRegistration* pRegistration ) throw (FlockException)\n";
//	
//	try
//	{
//		const std::string& swarmName = pRegistration->swarmName();
//		const std::string& parameterName = pRegistration->parameterName();
//		
//		Swarm* swarm = Simulation::get().swarm(swarmName);
//		
//		if( swarm->checkSwarmParameter( parameterName ) == true )
//		{
//			sendMessage( pSender, pRegistration, std::string( "/" + swarmName + "/" + parameterName ), swarm->swarmParameter( parameterName ), pExtendedOscMode );
//		}
//		else if( swarm->checkParameter( parameterName ) == true )
//		{
//            unsigned int parameterIndex = swarm->parameterIndex(parameterName);
//			std::array<int, 2> agentRange = pRegistration->agentRange();
//			
//			std::vector<Agent*>& agents = swarm->agents();
//			int agentCount = agents.size();
//			
//			if( agentRange[0] < 0) agentRange[0] = 0;
//			else if( agentRange[0] >= agentCount ) return;
//			if( agentRange[1] < 0 || agentRange[1] >= agentCount ) agentRange[1] = agentCount;
//			
//			int agentGroupSize = pRegistration->agentGroupSize();
//			
//			//std::cout << "agentGroupSize " << agentGroupSize << "\n";
//            //std::cout << "ar0 " << agentRange[0] << " ar1 " << agentRange[1] << "\n";
//			
//			for( unsigned int aI = agentRange[0]; aI < agentRange[1]; ++aI )
//			{
//                sendMessage( pSender, pRegistration, std::string( "/" + swarmName + "/" + std::to_string(aI) + "/" + parameterName ), agents[aI]->parameter( parameterIndex ), pExtendedOscMode );
//			}
//		}
//		else
//		{
//			throw dab::Exception( "COM ERROR: parameter " + parameterName + " not found for swarm " + swarmName, __FILE__, __FUNCTION__, __LINE__ );
//		}
//	}
//	catch( dab::Exception& e )
//	{
//        e += dab::Exception("COM ERROR: failed to send message", __FILE__, __FUNCTION__, __LINE__);
//		throw e;
//	}
//}

void
FlockCom::sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, bool pExtendedOscMode) throw (dab::Exception)
{
	//std::cout << "FlockCom::sendOSCMessage( com::Sender* pSender, ParameterRegistration* pRegistration ) throw (FlockException)\n";

	try
	{
		const std::string& swarmName = pRegistration->swarmName();
		const std::string& parameterName = pRegistration->parameterName();

		Swarm* swarm = Simulation::get().swarm(swarmName);

		if (swarm->checkSwarmParameter(parameterName) == true)
		{
			sendMessage(pSender, pRegistration, std::string("/" + swarmName + "/" + parameterName), swarm->swarmParameter(parameterName), pExtendedOscMode);
		}
		else if (swarm->checkParameter(parameterName) == true)
		{
			unsigned int parameterIndex = swarm->parameterIndex(parameterName);
			std::array<int, 2> agentRange = pRegistration->agentRange();

			std::vector<Agent*>& agents = swarm->agents();
			int agentCount = agents.size();

			if (agentRange[0] < 0) agentRange[0] = 0;
			else if (agentRange[0] >= agentCount) return;
			if (agentRange[1] < 0 || agentRange[1] >= agentCount) agentRange[1] = agentCount;

			int agentGroupSize = pRegistration->agentGroupSize();

			//			std::cout << "agentGroupSize " << agentGroupSize << "\n";
			//            std::cout << "ar0 " << agentRange[0] << " ar1 " << agentRange[1] << "\n";

			for (int aI = agentRange[0]; aI < agentRange[1]; )
			{
				int startAI = aI;
				int parameterCount = std::min(agentGroupSize, agentRange[1] - aI);
				std::vector<Parameter*> _parameters(parameterCount);

				for (int i = 0; i<parameterCount; ++i, ++aI)
				{
					//std::cout << "aI " << aI << " i " << i << " par " << agents[aI]->parameter( parameterIndex ) << "\n";

					_parameters[i] = agents[aI]->parameter(parameterIndex);
				}

				if (agentGroupSize > 1)
				{
					int endAI = aI - 1;
					sendMessage(pSender, pRegistration, std::string("/" + swarmName + "/" + std::to_string(startAI) + "/" + std::to_string(endAI) + "/" + parameterName), _parameters, pExtendedOscMode);
				}
				else
				{
					sendMessage(pSender, pRegistration, std::string("/" + swarmName + "/" + std::to_string(startAI) + "/" + parameterName), _parameters, pExtendedOscMode);
				}
			}
		}
		else
		{
			throw dab::Exception("COM ERROR: parameter " + parameterName + " not found for swarm " + swarmName, __FILE__, __FUNCTION__, __LINE__);
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to send message", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
FlockCom::sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, const std::string& pAddress, std::vector<Parameter*> pParameters, bool pExtendedOscMode) throw (Exception)
{
	//std::cout << "sendMessage sender " << pSender->name() << " address " << pAddress << "\n";

	try
	{
		std::shared_ptr<OscMessage> message(new OscMessage());
		message->setAddress(pAddress);
		addMessageParameter(message, pRegistration, pParameters, pExtendedOscMode);

		//pSender->queue( message ); // TODO: check why queing doesn't work

		pSender->send(message);
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to send message", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
FlockCom::sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, const std::string& pAddress, Parameter* pParameter, bool pExtendedOscMode) throw (Exception)
{
	// deprecated

	try
	{
		std::shared_ptr<OscMessage> message(new OscMessage());
		message->setAddress(pAddress);

		addMessageParameter(message, pRegistration, pParameter, pExtendedOscMode);

		//pSender->queue( message ); // TODO: check why queing doesn't work
		pSender->send(message);
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to send message", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
FlockCom::addMessageParameter(std::shared_ptr<OscMessage> pMessage, ParameterRegistration* pRegistration, std::vector<Parameter*> pParameters, bool pExtendedOscMode) throw (Exception)
{
	try
	{
		int parameterCount = pParameters.size();
		if (parameterCount == 0) return;

		unsigned int parDim = pParameters[0]->values().rows();
		bool normalise = pRegistration->normalise();
		bool maskValues = pRegistration->maskValues();
		const std::vector<bool>& parValueMask = pRegistration->parValueMask();

		if (pExtendedOscMode == false)
		{
			if (normalise == false)
			{
				if (maskValues == false)
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							pMessage->add(parValues[d]);
						}
					}
				}
				else
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true)
							{
								pMessage->add(parValues[d]);
							}
						}
					}
				}
			}
			else
			{
				const Eigen::VectorXf& parValueScale = pRegistration->parValueScale();
				const Eigen::VectorXf& parValueOffset = pRegistration->parValueOffset();
				float parSendNormValue;

				if (maskValues == false)
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							parSendNormValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
							parSendNormValue = std::max(std::min(parSendNormValue, 1.0f), 0.0f);
							pMessage->add(parSendNormValue);
						}
					}
				}
				else
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true)
							{
								parSendNormValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
								parSendNormValue = std::max(std::min(parSendNormValue, 1.0f), 0.0f);
								pMessage->add(parSendNormValue);
							}
						}
					}
				}
			}
		}
		else
		{
			int parameterValueCount = 0;

			if (maskValues == true)
			{
				int maskValueCount = 0;
				for (int d = 0; d<parDim; ++d)
				{
					if (parValueMask[d] == true) maskValueCount++;
				}

				parameterValueCount = parameterCount * maskValueCount;
			}
			else
			{
				parameterValueCount = parameterCount * parDim;
			}

			std::vector<float> parameterValues(parameterValueCount);

			if (normalise == false)
			{
				if (maskValues == false)
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							parameterValues[i++] = parValues[d];
						}
					}
				}
				else
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true) parameterValues[i++] = parValues[d];
						}
					}
				}
			}
			else
			{
				const Eigen::VectorXf& parValueScale = pRegistration->parValueScale();
				const Eigen::VectorXf& parValueOffset = pRegistration->parValueOffset();
				float parSendNormValue;

				if (maskValues == false)
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							parSendNormValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
							parSendNormValue = std::max(std::min(parSendNormValue, 1.0f), 0.0f);
							parameterValues[i++] = parSendNormValue;
						}
					}
				}
				else
				{
					for (int pI = 0, i = 0; pI<parameterCount; ++pI)
					{
						Parameter* parameter = pParameters[pI];
						const Eigen::VectorXf& parValues = parameter->values();

						for (int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true)
							{
								parSendNormValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
								parSendNormValue = std::max(std::min(parSendNormValue, 1.0f), 0.0f);
								parameterValues[i++] = parSendNormValue;
							}
						}
					}
				}
			}

			pMessage->add(parameterValues.data(), parameterValues.size());
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to add message parameter", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
FlockCom::addMessageParameter(std::shared_ptr<OscMessage> pMessage, ParameterRegistration* pRegistration, Parameter* pParameter, bool pExtendedOscMode) throw (dab::Exception)
{
	//deprecated

	try
	{
		const Eigen::VectorXf& parValues = pParameter->values();
		unsigned int parDim = parValues.rows();

		bool maskValues = pRegistration->maskValues();
		const std::vector<bool>& parValueMask = pRegistration->parValueMask();

		bool normalise = pRegistration->normalise();

		if (normalise == false)
		{
			if (maskValues == true)
			{
				if (pExtendedOscMode == true)
				{
					std::vector< float > parSendValues;
					for (int d = 0; d<parDim; ++d) if (parValueMask[d] == true) parSendValues.push_back(parValues[d]);
					pMessage->add(parSendValues.data(), parSendValues.size());
				}
				else
				{
					for (int d = 0; d<parDim; ++d)
					{
						if (parValueMask[d] == true)
						{
							pMessage->add(parValues[d]);
						}
					}
				}
			}
			else
			{
				if (pExtendedOscMode == true)
				{
					pMessage->add(parValues.data(), parDim);
				}
				else
				{
					for (int d = 0; d<parDim; ++d)
					{
						pMessage->add(parValues[d]);
					}
				}
			}
		}
		else
		{
			const Eigen::VectorXf& parValueScale = pRegistration->parValueScale();
			const Eigen::VectorXf& parValueOffset = pRegistration->parValueOffset();

			if (parDim != parValueScale.rows()) throw dab::Exception("COM ERROR: value scale dimension (" + std::to_string(parDim) + ") does not match parameter dimension (" + std::to_string(parValues.rows()) + ")", __FILE__, __FUNCTION__, __LINE__);
			else
			{
				if (maskValues == true)
				{
					std::vector< float > parSendValues;
					float parSendValue;

					if (pExtendedOscMode == true)
					{
						for (unsigned int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true)
							{
								parSendValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
								if (parSendValue < 0.0) parSendValue = 0.0;
								else if (parSendValue > 1.0) parSendValue = 1.0;
								parSendValues.push_back(parSendValue);
							}
						}

						pMessage->add(parSendValues.data(), parSendValues.size());
					}
					else
					{
						for (unsigned int d = 0; d<parDim; ++d)
						{
							if (parValueMask[d] == true)
							{
								parSendValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
								if (parSendValue < 0.0) parSendValue = 0.0;
								else if (parSendValue > 1.0) parSendValue = 1.0;

								pMessage->add(parSendValue);
							}
						}
					}
				}
				else
				{
					if (pExtendedOscMode == true)
					{
						std::vector<float> scaledValues(parDim);

						for (unsigned int d = 0; d<parDim; ++d)
						{
							scaledValues[d] = (parValues[d] - parValueOffset[d]) * parValueScale[d];
							if (scaledValues[d] < 0.0) scaledValues[d] = 0.0;
							else if (scaledValues[d] > 1.0) scaledValues[d] = 1.0;
						}

						pMessage->add(scaledValues.data(), scaledValues.size());
					}
					else
					{
						float scaledValue;

						for (unsigned int d = 0; d<parDim; ++d)
						{
							scaledValue = (parValues[d] - parValueOffset[d]) * parValueScale[d];
							if (scaledValue < 0.0) scaledValue = 0.0;
							else if (scaledValue > 1.0) scaledValue = 1.0;
							pMessage->add(scaledValue);
						}
					}
				}
			}
		}
	}
	catch (dab::Exception& e)
	{
		e += dab::Exception("COM ERROR: failed to add message parameter", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}