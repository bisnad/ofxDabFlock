/** \file dab_flock_com.h
*
*  Created by Daniel Bisig on 5/26/07.
*  Ported to OpenFrameworks by Daniel Bisig on 09/08/17.
*/

#ifndef _dab_flock_com_h_
#define _dab_flock_com_h_

#include <array>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include "ofUtils.h"
#include "dab_exception.h"
#include "dab_singleton.h"
#include "dab_index_map.h"
#include "dab_osc_messenger.h"

namespace dab
{

	namespace flock
	{

		class Parameter;
		class OscControl;

#pragma mark communication type

#pragma mark parameter communication registration

		class ParameterRegistration
		{
		public:
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::vector<bool>& pParValueMask);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			ParameterRegistration(const std::string& pSwarmName, const std::string& pParameterName, const std::string& pSenderName, unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);

			ParameterRegistration(const ParameterRegistration& pRegistration);
			~ParameterRegistration();

			void set(unsigned int pSendInterval);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange) throw (Exception);
			void set(unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			void set(unsigned int pSendInterval, unsigned int pAgentGroupSize);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception);
			void set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			void set(unsigned int pSendInterval, const std::vector<bool>& pParValueMask);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception);
			void set(unsigned int pSendInterval, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			void set(unsigned int pSendInterval, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void set(unsigned int pSendInterval, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);

			const std::string& swarmName() const;
			const std::string& parameterName() const;
			const std::string& senderName() const;
			unsigned int sendInterval() const;
			unsigned int& currentSendInterval();
			const std::array<int, 2>& agentRange() const;
			unsigned int agentGroupSize() const;
			bool normalise() const;
			const Eigen::VectorXf& minParValue()const;
			const Eigen::VectorXf& maxParValue()const;
			const Eigen::VectorXf& parValueScale() const;
			const Eigen::VectorXf& parValueOffset() const;
			const std::vector<bool>& parValueMask() const;
			bool maskValues() const;

			bool operator==(const ParameterRegistration& pRegistration) const;
			bool match(const std::string& pSwarmName, const std::string& pParameterName) const;

		protected:
			std::string mSwarmName;
			std::string mParameterName;
			std::string mSenderName;
			std::array<int, 2> mAgentRange;
			unsigned int mAgentGroupSize;
			bool mNormalise;
			Eigen::VectorXf mMinParValue;
			Eigen::VectorXf mMaxParValue;
			Eigen::VectorXf mParValueScale;
			Eigen::VectorXf mParValueOffset;
			bool mMaskValues;
			std::vector<bool> mParValueMask;
			unsigned int mSendInterval;
			unsigned int mCurrentSendInterval;
		};

#pragma mark flock messenger

		class FlockCom : public OscMessenger, public Singleton<FlockCom>
		{
			friend class Singleton<FlockCom>;

		public:
			/**
			\brief create osc control
			\param pReceiverPort receiver port number
			\param pSenderIP sender ip address
			\param pSenderPort sender port number
			\throw Exception failed to create osc control
			*/
			void createOscControl(unsigned int pReceiverPort, const std::string& pSenderIP, unsigned int pSenderPort) throw (Exception);

			/**
			\brief create osc control
			\param pOscControl OSCControl
			\param pReceiverPort receiver port number
			\param pSenderIP sender ip address
			\param pSenderPort sender port number
			\throw Exception failed to create osc control
			*/
			void createOscControl(std::shared_ptr<OscControl> pOscControl, unsigned int pReceiverPort, const std::string& pSenderIP, unsigned int pSenderPort) throw (Exception);

			/**
			\brief create network sender
			\param pSenderName name of network sender
			\param pReceiverIP IP address of receiver
			\param pReceiverPort port of receiver
			\param pExtendedOscMode use extended OSC protocol
			\exception Exception failed to create network sender
			*/
			void createSender(const std::string& pSenderName, const std::string& pReceiverIP, unsigned int pReceiverPort, bool pExtendedOscMode) throw (Exception);

			/**
			\brief remove all senders with the exception of those involved in osc control
			*/
			void removeSenders();

			/**
			\brief remove sender
			\param pSenderName name of sender
			\exception failed to remove sender
			*/
			void removeSender(const std::string& pSenderName) throw (Exception);

			/**
			\brief query extended osc mode
			\param pSenderName name of sender
			\exception Exception sender not found
			*/
			bool extendedOscMode(const std::string& pSenderName) throw (Exception);

			/**
			\brief retrieve parameter registration information
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\exception ComException failed to retrieve paramater registration
			*/
			ParameterRegistration* parameterRegistration(std::shared_ptr<OscSender> pSender, const std::string& pSwarmName, const std::string& pParameterName) throw (Exception);

			/**
			\brief return parameter registration information
			\param pSwarmName name of swarm / env
			\param pParameterName name of parameter
			\return parameter registration information
			*/
			std::vector< ParameterRegistration* > parameterRegistrations(const std::string& pSwarmName, const std::string& pParameterName) const;

			/**
			\brief check if parameter is registered for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			*/
			bool checkParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize) throw (Exception);

			/**
			\brief register parameter for sending
			\param pSenderName name of sender
			\param pSendInterval send interval
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\param pAgentRange range of agents
			\param pAgentGroupSize number of parameters sent at once ( for OSC senders only )
			\param pMinParValue minimum parameter value
			\param pMaxParValue maximum parameter value
			\exception Exception failed to register parameter
			*/
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue) throw (Exception);


			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const std::vector<bool>& pParValueMask) throw (Exception);
			void registerParameter(const std::string& pSenderName, unsigned int pSendInterval, const std::string& pSwarmName, const std::string& pParameterName, const std::array<int, 2>& pAgentRange, unsigned int pAgentGroupSize, const Eigen::VectorXf& pMinParValue, const Eigen::VectorXf& pMaxParValue, const std::vector<bool>& pParValueMask) throw (Exception);


			/**
			\brief deregister parameter from sending
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			*/
			void deregisterParameter(const std::string& pSenderName, const std::string& pSwarmName, const std::string& pParameterName) throw (Exception);


			/**
			\brief update messenger
			\Exception Exception failed to update messenger
			*/
			virtual void update() throw (Exception);

		protected:
			std::shared_ptr<OscReceiver> mControlReceiver;
			std::shared_ptr<OscSender> mErrorSender;
			std::shared_ptr<OscControl> mOscControl;

			IndexMap< std::shared_ptr<OscSender>, std::vector<ParameterRegistration*> > mParameterRegistry; ///\brief parameter communication registry
			IndexMap< std::shared_ptr<OscSender>, bool > mExtendedOscMode;

			double mTime;


			/**
			\brief default constructor
			*/
			FlockCom();

			/**
			\brief destructor
			*/
			~FlockCom();

			/**
			\brief check if parameter registration for sender already exists
			\param pSenderName name of sender
			\param pSwarmName name of swarm
			\param pParameterName name of parameter
			\return true if parameter registration found, false otherwise
			*/
			bool checkParameterRegistration(std::shared_ptr<OscSender> pSender, const std::string& pSwarmName, const std::string& pParameterName);

			/**
			\brief update senders
			\exception ComException failed to update senders
			\remarks creates message based on registered parameters for all senders and sends them
			*/
			void send() throw (Exception);

			/**
			\brief send osc message
			\param pSender sender
			\exception ComException failed to update sender
			*/
			void sendMessage(std::shared_ptr<OscSender> pSender) throw (Exception);

			/**
			\brief update osc sender
			\param pSender sender
			\param pRegistration parameter registration
			\param pExtendedOscMode use extended osc protocol
			\exception ComException failed to update sender
			*/
			void sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, bool pExtendedOscMode) throw (Exception);

			/**
			\brief update osc sender
			\param pSender sender
			\param pRegistration parameter registration
			\param pAddress osc address pattern
			\param pParameter parameter
			\param pExtendedOscMode use extended osc protocol
			\exception ComException failed to update sender
			*/
			void sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, const std::string& pAddress, Parameter* pParameter, bool pExtendedOscMode) throw (Exception);

			void sendMessage(std::shared_ptr<OscSender> pSender, ParameterRegistration* pRegistration, const std::string& pAddress, std::vector<Parameter*> pParameters, bool pExtendedOscMode) throw (Exception);

			void addMessageParameter(std::shared_ptr<OscMessage> pMessage, ParameterRegistration* pRegistration, Parameter* pParameter, bool pExtendedOscMode) throw (Exception);

			void addMessageParameter(std::shared_ptr<OscMessage> pMessage, ParameterRegistration* pRegistration, std::vector<Parameter*> pParameters, bool pExtendedOscMode) throw (Exception);

		};

	};

};

#endif