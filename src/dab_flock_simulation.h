/** \file dab_flock_simulation.h
 *
 *  Created by Daniel Bisig on 4/19/07.
 *  Ported to Open Frameworks by Daniel Bisig on 4/08/17.
 */

#ifndef _dab_flock_simulation_h_
#define _dab_flock_simulation_h_

#include "ofUtils.h"
#include "ofThread.h"
#include <vector>

#include "dab_singleton.h"
#include "dab_exception.h"
#include "dab_listener.h"
#include "dab_event_manager.h"
#include "dab_flock_com.h"
#include "dab_flock_stats.h"
//#include <iso_base/iso_base_notifier.h>
//#include <iso_math/iso_math_rectangle.h>
//#include <iso_event/iso_event_includes.h>
#include "dab_space_manager.h"

namespace dab
{

namespace flock
{

class Agent;
class Swarm;
class Env;

class Simulation : public Singleton<Simulation>, public UpdateNotifier, public ofThread
{
friend class Singleton<Simulation>;
    
public:
    /**
     \brief return simulation step
     */
    long simulationStep() const;
    
    /**
     \brief return simulation update rate
     \return simulation update rate (simulation steps / sec)
     */
    float updateRate() const;
    
    /**
     \brief return update interval
     \return update interval
     */
    float updateInterval() const;
    
    /**
     \brief set update interval
     \param pUpdateInterval update interval in milisecs
     */
    void setUpdateInterval(float pUpdateInterval);
    
    /**
     \brief return event manager
     \return event manager
     */
    event::EventManager& event();
    
    /**
     \brief return space manager
     \return space manager
     */
    space::SpaceManager& space();
    
    /**
     \brief return communcation manager
     */
    FlockCom& com();
    
    /**
     \brief return analyzer manager
     */
    FlockStats& stats();
    
    /**
     \brief return agent count
     \return agent count
     */
    unsigned int agentCount() const;
    
    /**
     \brief return agents
     \return agents
     */
    std::vector<Agent*>& agents();
    
    /**
     \brief return agent
     \param pIndex agent index
     \return agent
     \exception Exception agent not found
     */
    Agent* agent(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return agent
     \param pName agent name
     \return agent
     \exception Exception agent not found
     */
    Agent* agent(std::string& pName) throw (Exception);
    
    /**
     \brief add agent to simulation
     \param pAgent agent
     */
    void addAgent(Agent* pAgent);
    
    /**
     \brief remove agent from simulation
     \param pAgent agent
     */
    void removeAgent(Agent* pAgent);
    
    /**
     \brief check swarm
     \param pName swarm name
     \return true if swarm exists, false otherwise
     */
    bool checkSwarm( std::string pName );
    
    /**
     \brief return swarms
     \return swarms
     */
    std::vector<Swarm*>& swarms();
    
    /**
     \brief return swarm
     \param pIndex swarm index
     \return swarm
     \exception Exception swarm not found
     */
    Swarm* swarm(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return swarm
     \param pName swarm name
     \return swarm
     \exception FlockException swarm not found
     */
    Swarm* swarm(std::string pName) throw (Exception);
    
    /**
     \brief add swarm to simulation
     \param pSwarm swarm
     */
    void addSwarm(Swarm* pSwarm);
    
    /**
     \brief remove swarm from simulation
     \param pSwarm swarm
     */
    void removeSwarm(Swarm* pSwarm);
    
    /**
     \brief check environment
     \param pName environment name
     \return true if environment exists, false otherwise
     */
    bool checkEnv( std::string pName );
    
    /**
     \brief return environments
     \return environments
     */
    std::vector<Env*>& envs();
    
    /**
     \brief return environment
     \param pIndex environment index
     \return environment
     \exception Exception environment not found
     */
    Env* env(unsigned int pIndex) throw (Exception);
    
    /**
     \brief return environment
     \param pName environment name
     \return environment
     \exception Exception environment not found
     */
    Env* env(std::string pName) throw (Exception);
    
    /**
     \brief add environment to simulation
     \param pEnv environment
     */
    void addEnv(Env* pEnv);
    
    /**
     \brief remove environment from simulation
     \param pEnv environment
     */
    void removeEnv(Env* pEnv);
    
    /**
     \brief clear entire simulation
     */
    void clear();
    
    /**
     \brief perform one simulation step
     */
    void update();
    
    void notifyListeners();
    
	void start();
    void stop();
    
    /**
     \brief check whether simulation is paused or not
     \return true, if simulation is paused, false otherwise
     */
    bool paused();
    
    /**
     \brief check whether simulation is frozen or not
     \return true, if simulation is frozen, false otherwise
     */
    bool frozen();
    
    /**
     \brief switch simulation between paused and non-paused mode
     */
    void switchPaused();
    
    void switchFrozen();
    
    /**
     \brief report exception
     \param pException exception
     */
    void exceptionReport( const Exception& pException );
    
protected:
    /**
     \brief default contructor
     */
    Simulation();
    
    /**
     \brief destructor
     */
    ~Simulation();

	void threadedFunction();
    
    static Simulation* sSimulation; /// \brief singleton instance
    
    std::vector<Agent*> mAgents; /// \brief agents
    std::vector<Swarm*> mSwarms;/// \brief swarms
    std::vector<Env*> mEnvs; ///\brief environments
    double mUpdateInterval; /// \brief simulation update interval (milliseconds)
    float mUpdateRate; ///\brief simulation update rate
    bool mTerminated; /// \brief simulation termination flag
    double mTime; /// \brief simulation running time (milliseconds)
    event::EventManager mEventManager; /// \brief event manager
    long mSimulationStep;
    
    bool mPaused;
    bool mFrozen;
};

};

};

#endif