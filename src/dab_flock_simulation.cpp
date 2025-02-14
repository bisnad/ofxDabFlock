/** \file dab_flock_simulation.cpp
 */

#include "dab_flock_simulation.h"
#include "dab_flock_behavior.h"
#include "dab_flock_agent.h"
#include "dab_flock_swarm.h"
#include "dab_flock_env.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace dab;
using namespace dab::flock;

void
Simulation::setUpdateInterval(float pUpdateInterval)
{
	mUpdateInterval = pUpdateInterval;
}

Simulation::Simulation()
: mUpdateInterval(10000) // 100 times per second
, mSimulationStep( 0 )
, mTerminated(false)
, mEventManager()
, mPaused(false)
, mFrozen(false)
{
	mTime = ofGetElapsedTimeMillis();
}

Simulation::~Simulation()
{}

long
Simulation::simulationStep() const
{
	return mSimulationStep;
}

float
Simulation::updateRate() const
{
	return mUpdateRate;
}

float
Simulation::updateInterval() const
{
	return mUpdateInterval;
}

event::EventManager&
Simulation::event()
{
	return mEventManager;
}

space::SpaceManager&
Simulation::space()
{
	return space::SpaceManager::get();
}

FlockCom&
Simulation::com()
{
	return FlockCom::get();
}

FlockStats&
Simulation::stats()
{
	return FlockStats::Singleton<FlockStats>::get();
}

unsigned int
Simulation::agentCount() const
{
	return mAgents.size();
}

std::vector<Agent*>&
Simulation::agents()
{
	return mAgents;
}

Agent*
Simulation::agent(unsigned int pIndex) throw (Exception)
{
	if(pIndex >= mAgents.size()) throw Exception( "FLOCK ERROR: agent index " + std::to_string(pIndex) + " not found", __FILE__, __FUNCTION__, __LINE__ );
    
	return mAgents[pIndex];
}

Agent*
Simulation::agent(std::string& pName) throw (Exception)
{
	unsigned int agentCount = mAgents.size();
	for(unsigned int i=0; i<agentCount; ++i)
	{
		if(mAgents[i]->name() == pName) return mAgents[i];
	}
	
	throw Exception( "FLOCK ERROR: agent " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
}

void
Simulation::addAgent(Agent* pAgent)
{
	mAgents.push_back(pAgent);
    
	// register all parameter of agent as event targets in the event manager
    // ???
	unsigned int parameterCount = pAgent->parameterCount();
	
	for(unsigned int parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
	{
		Parameter* parameter = pAgent->parameter(parameterIndex);
	}
}

void
Simulation::removeAgent(Agent* pAgent)
{
	int agentCount = mAgents.size();
	for(int i=agentCount-1; i>=0; --i)
    {
        if( mAgents[i] == pAgent ) mAgents.erase(mAgents.begin() + i);
    }
}

bool
Simulation::checkSwarm( std::string pName )
{
	unsigned int swarmCount = mSwarms.size();
	for(unsigned int i=0; i<swarmCount; ++i)
	{
		if(mSwarms[i]->name() == pName) return true;
	}
	
	return false;
}

std::vector<Swarm*>&
Simulation::swarms()
{
	return mSwarms;
}

Swarm*
Simulation::swarm(unsigned int pIndex) throw (Exception)
{
	if(pIndex >= mSwarms.size()) throw Exception( "FLOCK ERROR: swarm index " + std::to_string(pIndex) + " not found", __FILE__, __FUNCTION__, __LINE__ );
    
	return mSwarms[pIndex];
}

Swarm*
Simulation::swarm(std::string pName) throw (Exception)
{
	unsigned int swarmCount = mSwarms.size();
	for(unsigned int i=0; i<swarmCount; ++i)
	{
		if(mSwarms[i]->name() == pName) return mSwarms[i];
	}
	
	throw Exception( "FLOCK ERROR: swarm " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
}

void
Simulation::addSwarm(Swarm* pSwarm)
{
	mSwarms.push_back(pSwarm);
}

void
Simulation::removeSwarm(Swarm* pSwarm)
{
	int swarmCount = mSwarms.size();
	for(int i=swarmCount-1; i>=0; --i)
    {
        if( mSwarms[i] == pSwarm ) mSwarms.erase(mSwarms.begin() + i);
    }
}

bool
Simulation::checkEnv( std::string pName )
{
	unsigned int envCount = mEnvs.size();
	for(unsigned int i=0; i<envCount; ++i)
	{
		if(mEnvs[i]->name() == pName) return true;
	}
	
	return false;
}

std::vector<Env*>&
Simulation::envs()
{
	return mEnvs;
}

Env*
Simulation::env(unsigned int pIndex) throw (Exception)
{
	if(pIndex >= mEnvs.size()) throw Exception( "FLOCK ERROR: environment index " + std::to_string(pIndex) + " not found", __FILE__, __FUNCTION__, __LINE__ );
    
	return mEnvs[pIndex];
}

Env*
Simulation::env(std::string pName) throw (Exception)
{
	unsigned int envCount = mEnvs.size();
	for(unsigned int i=0; i<envCount; ++i)
	{
		if(mEnvs[i]->name() == pName) return mEnvs[i];
	}
	
	throw Exception( "FLOCK ERROR: environment " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
}

void
Simulation::addEnv(Env* pEnv)
{
	mEnvs.push_back(pEnv);
}

void
Simulation::removeEnv(Env* pEnv)
{
	int envCount = mEnvs.size();
	for(int i=envCount-1; i>=0; --i)
    {
        if( mEnvs[i] == pEnv ) mEnvs.erase(mEnvs.begin() + i);
    }
}

void
Simulation::clear()
{
	//std::cout << "Simulation::clear() begin\n";
	
	mPaused = true;
	
	// hide swarms
	//GLWindow::get().panel().hideSwarms();
	
	// hide spaces
	//GLWindow::get().panel().hideSpaces();
    
	// remove swarms
    int swarmCount = mSwarms.size();
    for(int sI=0; sI<swarmCount; ++sI)
    {
        //delete mSwarms[sI];
        mSwarms[sI]->clear();
    }
    mSwarms.clear();
	
	// remove envs
    int envCount = mEnvs.size();
    for(int eI=0; eI<envCount; ++eI)
    {
        delete mEnvs[eI];
    }
    mEnvs.clear();
	
	// remove com
	FlockCom::get().removeSenders();
	FlockCom::get().removeReceivers();
	
	// remove spaces
	space::SpaceManager::get().removeSpaces();
	
	Swarm::sInstanceCount = 0;
	Agent::sInstanceCount = 0;
	
	mPaused = false;
	
	//std::cout << "Simulation::clear() end\n";
}

void
Simulation::update()
{
	FlockCom::get().update();

	mEventManager.update();
    
	if( mFrozen == false )
	{
		notifyListeners();

		space::SpaceManager::get().update();
		
		unsigned int swarmCount = mSwarms.size();
		unsigned int agentCount = mAgents.size();
        
		//for(unsigned int i=0; i<swarmCount; ++i) mSwarms[i]->act();
		for(unsigned int i=0; i<agentCount; ++i) mAgents[i]->act();
        
		//for(unsigned int i=0; i<swarmCount; ++i) mSwarms[i]->flush();
		for(unsigned int i=0; i<agentCount; ++i) mAgents[i]->flush();
        
		FlockStats::Singleton<FlockStats>::get().update();
        
		//for(unsigned int i=0; i<agentCount; ++i) std::cout << "i " << i << " name " << mAgents[i]->name().toStdString() << "\n";
		//std::cout << "mass " << mAgents[0]->parameter("mass").values() << " prefVel " << mAgents[0]->parameter("damping_prefVelocity").values() << "\n";
        
		mSimulationStep++;
	}
	
	//std::cout << "Simulation::update() end\n";
}

void
Simulation::notifyListeners()
{
    UpdateNotifier::update();
    
    int listenerCount = mListeners.size();
    
    for(int lI=0; lI<listenerCount; ++lI)
    {
        mListeners[lI].lock()->notifyUpdate();
    }
}

void
Simulation::threadedFunction()
{
	while (isThreadRunning())
	{
		if (mPaused == false)
		{
			double currentTime = ofGetElapsedTimeMillis();
			if (currentTime - mTime < mUpdateInterval) continue;
			else mTime = currentTime;

			update();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	//while(mTerminated == false)
	//{
 //       if(mPaused == false)
 //       {
 //           double currentTime = ofGetElapsedTimeMillis();
 //           if(currentTime - mTime < mUpdateInterval) continue;
 //           else mTime = currentTime;
 //           
 //           update();
 //       }
 //       
	//	std::this_thread::sleep_for(std::chrono::microseconds(10));
	//}
	//
	//if( mTerminated == true ) mTerminated = false;
}

bool
Simulation::paused()
{
	return mPaused;
}

bool
Simulation::frozen()
{
	return mFrozen;
}

void
Simulation::start()
{
	if (isThreadRunning() == false) startThread();
}

void
Simulation::stop()
{
	if (isThreadRunning() == true) stopThread();

	//mTerminated = true;
}

void
Simulation::switchPaused()
{
	mPaused = !mPaused;
}

void
Simulation::switchFrozen()
{
	mFrozen = !mFrozen;
}

void
Simulation::exceptionReport( const Exception& pException )
{
	if( FlockCom::get().checkSender("OSCErrorSender") == true )
	{
        std::shared_ptr<OscMessage> errorMessage( new OscMessage("/FlockError") );
        
        std::string exceptionString = pException;
		errorMessage->add(exceptionString);
		
		try
		{
            FlockCom::get().OscMessenger::send("OSCErrorSender", errorMessage);
		}
		catch(Exception& e)
		{
			std::cout << e << "\n";
		}
	}
    
	std::cout << pException << "\n";
}
