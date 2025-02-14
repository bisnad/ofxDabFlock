/**	\file dab_flock_agent.cpp
 */

#include "dab_flock_agent.h"
#include "dab_flock_simulation.h"
#include "dab_space_neighbor_group_alg.h"

using namespace dab;
using namespace dab::flock;

const std::string Agent::sClassName = "Agent";
unsigned int Agent::sInstanceCount = 0;

Agent::Agent()
: mIndex( sInstanceCount++ )
{
    mName = sClassName + std::to_string(mIndex);"\n";
    
	addParameter( new Parameter( this, "active", Eigen::Matrix<float, 1, 1>(1.0) ) );
}

Agent::Agent(const std::string& pName)
: mIndex( sInstanceCount++ )
, mName(pName)
{
	addParameter( new Parameter( this, "active", Eigen::Matrix<float, 1, 1>(1.0) ) );
}

Agent::Agent(const Agent& pAgent)
: mIndex( sInstanceCount++ )
{
    mName = sClassName + std::to_string(mIndex);
    
	// copy parameters
	unsigned int parCount = pAgent.parameterCount();
	for(unsigned int i=0; i<parCount; ++i)
	{
		addParameter( new Parameter(this, *(pAgent.parameter(i))) );
	}
    
	// copy behaviors
	unsigned int behCount = pAgent.behaviorCount();
	for(unsigned int i=0; i<behCount; ++i)
	{
		const Behavior* _behavior = pAgent.behavior(i);
		addBehavior( _behavior->create( _behavior->name(), this ) );
	}
}

Agent::Agent(const std::string& pName, const Agent& pAgent)
: mIndex( sInstanceCount++ )
, mName(pName)
{
	// copy parameters
	unsigned int parCount = pAgent.parameterCount();
	for(unsigned int i=0; i<parCount; ++i)
	{
		addParameter( new Parameter(this, *(pAgent.parameter(i))) );
	}
    
	// copy behaviors
	unsigned int behCount = pAgent.behaviorCount();
	for(unsigned int i=0; i<behCount; ++i)
	{
		const Behavior* _behavior = pAgent.behavior(i);
		addBehavior( _behavior->create( _behavior->name(), this ) );
	}
}

Agent::~Agent()
{}

unsigned int
Agent::index() const
{
	return mIndex;
}

const std::string&
Agent::name() const
{
	return mName;
}

unsigned int
Agent::parameterCount() const
{
	return mParameterList.parameterCount();
}

bool
Agent::checkParameter(const std::string& pParameterName) const
{
	return mParameterList.contains(pParameterName);
}

unsigned int
Agent::parameterIndex(const std::string& pParameterName) const throw (Exception)
{
	try
	{
		return mParameterList.parameterIndex(pParameterName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed get parameter index for parameter name " + pParameterName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Parameter*
Agent::parameter(unsigned int pParameterIndex) throw (Exception)
{
	try
	{
		return mParameterList.parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get parameter for parameter index " + std::to_string(pParameterIndex), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

const Parameter*
Agent::parameter(unsigned int pParameterIndex) const throw (Exception)
{
	try
	{
		return mParameterList.parameter(pParameterIndex);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get parameter for parameter index " + std::to_string(pParameterIndex), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

Parameter*
Agent::parameter(const std::string& pParameterName) throw (Exception)
{
	try
	{
		return mParameterList.parameter(pParameterName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get parameter for parameter name " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

const Parameter*
Agent::parameter(const std::string& pParameterName) const throw (Exception)
{
	try
	{
		return mParameterList.parameter(pParameterName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get parameter for parameter name " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::addParameter(Parameter* pParameter) throw (Exception)
{
	try
	{
		mParameterList.addParameter(pParameter);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add parameter " + pParameter->name(), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::addParameter(const std::string& pName, unsigned int pDim) throw (Exception)
{
	try
	{
		addParameter( new Parameter(this, pName, pDim) );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add parameter " + pName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::addParameter(const std::string& pName, unsigned int pDim, float pValue) throw (Exception)
{
	try
	{
        Eigen::VectorXf parValue(pDim);
        parValue.setConstant(pValue);
		addParameter( new Parameter(this, pName, parValue ) );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add parameter " + pName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::addParameter(const std::string& pName, const std::initializer_list<float>& pValues) throw (Exception)
{
	try
	{
		addParameter( new Parameter(this, pName, pValues) );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add parameter " + pName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::addParameter(const std::string& pName, const Eigen::VectorXf& pValues) throw (Exception)
{
	try
	{
		addParameter( new Parameter(this, pName, pValues) );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add parameter " + pName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::removeParameter(const std::string& pParameterName) throw (Exception)
{
	try
	{
		mParameterList.removeParameter( pParameterName );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to remove parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::setParameterValue(const std::string& pParameterName, float pParameterValue) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValue);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}
void
Agent::setParameterValues(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::setParameterValues(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::set(const std::string& pParameterName, float pParameterValue) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValue);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::set(const std::string& pParameterName, const std::initializer_list<float>& pParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::set(const std::string& pParameterName, const Eigen::VectorXf& pParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->set(pParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to set value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::randomize(const std::string& pParameterName, float pMinParameterValue, float pMaxParameterValue) throw (Exception)
{
	try
	{
		parameter(pParameterName)->randomize(pMinParameterValue, pMaxParameterValue);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to randomize value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::randomize(const std::string& pParameterName, const std::initializer_list<float>& pMinParameterValues, const std::initializer_list<float>& pMaxParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->randomize(pMinParameterValues, pMaxParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to randomize value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::randomize(const std::string& pParameterName, const Eigen::VectorXf& pMinParameterValues, const Eigen::VectorXf& pMaxParameterValues) throw (Exception)
{
	try
	{
		parameter(pParameterName)->randomize(pMinParameterValues, pMaxParameterValues);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to randomize value for parameter " + pParameterName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::assignNeighbors(const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, space::NeighborGroupAlg* pNeighborGroupAlg ) throw (Exception)
{
	try
	{
        std::shared_ptr<space::Space> space = space::SpaceManager::get().space(pSpaceName);
		Parameter* parameter = mParameterList.parameter(pParameterName);
		
		if( space->checkObject( parameter ) == true ) space->setObject( parameter, pVisible, pNeighborGroupAlg  );
		else space->addObject( parameter, pVisible, pNeighborGroupAlg);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to assign neighbors for parameter " + pParameterName + " and space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible = false ) throw (Exception)
{
	try
	{
        std::shared_ptr<space::Space> space = space::SpaceManager::get().space(pSpaceName);
		Parameter* parameter = mParameterList.parameter(pParameterName);
        
		if( space->checkObject( parameter ) == true ) space->setObject( parameter, pVisible, nullptr  );
		else space->addObject( parameter, pVisible, nullptr);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to assign neighbors for parameter " + pParameterName + " and space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
Agent::assignNeighbors( const std::string& pParameterName, const std::string& pSpaceName, bool pVisible, float pNeighborRadius, int pMaxNeighborCount, bool pReplaceNeighborMode )
{
	try
	{
        std::shared_ptr<space::Space> space = space::SpaceManager::get().space(pSpaceName );
		Parameter* parameter = mParameterList.parameter(pParameterName );
		
		if( space->checkObject( parameter ) == true ) space->setObject( parameter, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
		else space->addObject( parameter, pVisible, new space::NeighborGroupAlg( pNeighborRadius, pMaxNeighborCount, pReplaceNeighborMode ) );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to assign neighbors for parameter " + pParameterName + " and space " + pSpaceName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

unsigned int
Agent::behaviorCount() const
{
	return mBehaviorList.behaviorCount();
}

bool
Agent::checkBehavior(const std::string& pBehaviorName) const
{
	return mBehaviorList.contains(pBehaviorName);
}

Behavior*
Agent::behavior(unsigned int pBehaviorIndex) throw (Exception)
{
	try
	{
		return mBehaviorList.behavior(pBehaviorIndex);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get behaviour for behaviour index " + std::to_string(pBehaviorIndex), __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

const Behavior*
Agent::behavior(unsigned int pBehaviorIndex) const throw (Exception)
{
	try
	{
		return mBehaviorList.behavior(pBehaviorIndex);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get behaviour for behaviour index " + std::to_string(pBehaviorIndex), __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Behavior*
Agent::behavior(const std::string& pBehaviorName) throw (Exception)
{
	try
	{
		return mBehaviorList.behavior(pBehaviorName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get behaviour for behaviour name " + pBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

const Behavior*
Agent::behavior(const std::string& pBehaviorName) const throw (Exception)
{
	try
	{
		return mBehaviorList.behavior(pBehaviorName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to get behaviour for behaviour name " + pBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::addBehavior(Behavior* pBehavior) throw (Exception)
{
	try
	{
		mBehaviorList.addBehavior(pBehavior);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to add behaviour " + pBehavior->name(), __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::addBehavior(const std::string& pBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	Behavior* _behavior = nullptr;
	
	try
	{
		_behavior = pBehavior.create(pBehaviorName, this);
		mBehaviorList.addBehavior(_behavior);
	}
	catch(Exception& e)
	{
		if(_behavior != nullptr) delete _behavior;
		
        e += Exception("FLOCK ERROR: failed to add behaviour " + pBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::addBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition, const Behavior& pBehavior) throw (Exception)
{
	Behavior* _behavior = nullptr;
	
	try
	{
		_behavior = pBehavior.create(pBehaviorName, this);
		mBehaviorList.addBehavior(_behavior, pBehaviorPosition);
	}
	catch(Exception& e)
	{
		if(_behavior != nullptr) delete _behavior;
		
        e += Exception("FLOCK ERROR: failed to add behaviour " + pBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::addBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName, const Behavior& pBehavior) throw (Exception)
{
	Behavior* _behavior = nullptr;
	
	try
	{
		int pSuccessorBehaviorPosition = mBehaviorList.behaviorIndex(pSuccessorBehaviorName);
		_behavior = pBehavior.create(pBehaviorName, this);
		mBehaviorList.addBehavior(_behavior, pSuccessorBehaviorPosition);
	}
	catch (Exception& e)
	{
		if(_behavior != nullptr) delete _behavior;
		
        e += Exception("FLOCK ERROR: failed to add behaviour " + pBehaviorName + " in front of behaviour " + pSuccessorBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::moveBehavior(const std::string& pBehaviorName, unsigned int pBehaviorPosition) throw (Exception)
{
	try
	{
		int oldBehaviorPosition = mBehaviorList.behaviorIndex(pBehaviorName);
		mBehaviorList.moveBehavior(oldBehaviorPosition, pBehaviorPosition);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to move behaviour " + pBehaviorName + " to position " + std::to_string(pBehaviorPosition), __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::moveBehavior(const std::string& pBehaviorName, const std::string& pSuccessorBehaviorName) throw (Exception)
{
	try
	{
		int oldBehaviorPosition = mBehaviorList.behaviorIndex(pBehaviorName);
		int newBehaviorPosition = mBehaviorList.behaviorIndex(pSuccessorBehaviorName);
		
		//std::cout << "oldBehaviorPosition " << oldBehaviorPosition << " newBehaviorPosition " << newBehaviorPosition << "\n";
		
		if( oldBehaviorPosition == newBehaviorPosition ) return;
		else if( oldBehaviorPosition < newBehaviorPosition ) mBehaviorList.moveBehavior(oldBehaviorPosition, newBehaviorPosition - 1 );
		else mBehaviorList.moveBehavior(oldBehaviorPosition, newBehaviorPosition );
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to move behaviour " + pBehaviorName + " in front of behaviour  " + pSuccessorBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::removeBehavior(const std::string& pBehaviorName) throw (Exception)
{
	try
	{
		mBehaviorList.removeBehavior(pBehaviorName);
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to remove behaviour " + pBehaviorName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Agent::act()
{
	//std::cout << "agent " << mName.toStdString() << " act\n";
	
	if( mParameterList.parameter(0)->value() == 0.0 ) return; // check if agent is active
    
	mBehaviorList.act();
}

void
Agent::flush()
{
	mParameterList.flush();
}

Agent::operator std::string() const
{
    return info(0);
}

std::string
Agent::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    ss << mName << "\n";
    
    if(pPropagationLevel == 0)
    {
        ss << "parameter Count " << std::to_string(mParameterList.parameterCount()) << "\n";
    }
    else if(pPropagationLevel == 1)
    {
        ss << "parameters:\n";
        int parCount = mParameterList.parameterCount();
        for(int pI=0; pI<parCount; ++pI)
        {
            const Parameter* par = mParameterList.parameter(pI);
            ss << par->name() << " : " << par->values().transpose() << "\n";
        }
    }
    else
    {
        ss << "ParameterList\n";
        ss << mParameterList.info(pPropagationLevel - 1) << "\n";
    }
    
    if(pPropagationLevel == 0)
    {
        ss << "behavior Count " << mBehaviorList.behaviorCount() << "\n";
    }
    else if(pPropagationLevel == 1)
    {
        ss << "behaviors\n";
        int behaviorCount = mBehaviorList.behaviorCount();
        for(int bI=0; bI<behaviorCount; ++bI)
        {
            const Behavior* beh = mBehaviorList.behavior(bI);
            ss << beh->name() << " in: " << beh->inputParameterString() << " out: " << beh->outputParameterString() << "\n";
        }
    }
    else
    {
        ss << "BehaviorList\n";
        ss << mBehaviorList.info(pPropagationLevel - 1) << "\n";
    }

    
//	if(pPropagationLevel != 0)
//	{
//        ss << "BehaviorList\n";
//        ss << mBehaviorList.info(pPropagationLevel - 1) << "\n";
//	}
//	else
//	{
//        ss << "behavior Count " << mBehaviorList.behaviorCount() << "\n";
//	}
    
	return ss.str();
}