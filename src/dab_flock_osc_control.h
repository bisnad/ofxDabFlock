/** \file iso_dab_osc_control.h
 *
 * Created by Daniel Bisig on 6/7/09.
 * Ported to OpenFrameworks by Daniel Bisig on 26.08.2017
 *
 */

#ifndef _dab_flock_osc_control_h_
#define _dab_flock_osc_control_h_

#include "dab_exception.h"
#include "dab_singleton.h"
#include "dab_osc_sender.h"
#include "dab_osc_receiver.h"
#include "dab_space_types.h"
#include "dab_space_alg.h"
#include "dab_space_alg_grid.h"
#include "dab_flock_behavior.h"
#include "dab_flock_add_space_event.h"
#include "ofTypes.h"

namespace dab
{

namespace event
{
class Event;
}

namespace flock
{

class OscControl : public Singleton<OscControl>, public OscListener
{
public:
    OscControl();
    OscControl( std::shared_ptr<OscSender> pErrorSender );
    ~OscControl();
    
    void setErrorSender( std::shared_ptr<OscSender> pErrorSender );
    
    virtual void notify(std::shared_ptr<OscMessage> pMessage);
    
protected:
    std::shared_ptr<OscSender> mErrorSender;
    
    std::map< std::string, space::SpaceAlgType > mSpaceAlgMap;
    std::map< std::string, space::GridAlg::GridNeighborMode > mGridNeighborModeMap;
    std::map< std::string, space::GridAlg::GridUpdateMode > mGridUpdateModeMap;
    std::map< std::string, Behavior* > mBehaviorMap;
    
    ofMutex mLock;
    
    void initCreationMaps();
    space::SpaceAlgType spaceAlgType( const std::string& pSpaceAlgType ) throw (Exception);
    space::GridAlg::GridNeighborMode gridNeighborMode( const std::string& pGridNeighborMode ) throw (Exception);
    space::GridAlg::GridUpdateMode gridUpdateMode( const std::string& pGridUpdateMode ) throw (Exception);
    
    void groupOscArgs( std::shared_ptr<OscMessage> pMessage, std::vector< std::shared_ptr<_OscArg> >& pGroupedArgs );
    
    
    void clearSimulation( ) throw (Exception);
    void restoreSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void saveSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setSimulationRate(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void freezeSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void addSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void removeSpace(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void addSender(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void removeSender(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void addReceiver(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void removeReceiver(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void addSwarm(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void removeSwarm(std::vector<_OscArg*>& pParameters()) throw (Exception);
    void addAgents(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void removeAgents(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void addParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
    void setParameter(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void randomizeParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void removeParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
	void assignNeighbors(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void removeNeigbors(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void registerParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void deregisterParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void addBehavior(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void moveBehavior(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void removeBehavior(std::vector<_OscArg*>& pParameters()) throw (Exception);
    void showSwarm(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void hideSwarm(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void showSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void hideSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void setDisplayColor(std::vector<_OscArg*>& pParameters()) throw (Exception);
	void setDisplayPosition(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
	void setDisplayOrientation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
	void changeDisplayOrientation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
	void setDisplayZoom(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void setWindowSettings(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    virtual void toggleFullScreen( ) throw (Exception);
    void setAgentColor(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setAgentScale(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setAgentLineWidth(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setTrailColor(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setTrailLength(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setTrailDecay(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
    void setTrailLineWidth(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception);
//    void setSpaceColor(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void setSpaceValueScale(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void saveImage(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void startRecordMovie(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void stopRecordMovie(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    
//    void queryAgentParameterCount(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void queryAgentParameterName(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void queryAgentParameterValue(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySwarmCount(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySpaceCount(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySenderCount(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySwarm(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySpace(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void querySender(std::vector<_OscArg*>& pParameters()) throw (Exception);
//    void queryParameter(std::vector<_OscArg*>& pParameters()) throw (Exception);
    
    /*
    base::String stringPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    int intPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    math::Vector<int> intVecPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    float floatPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    math::Vector<float> floatVecPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    bool boolPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    SpaceAlgType spaceAlgPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    com::Protocol comProtocolPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    com::Format comFormatPar( com::MessageValueGroup* pParameter ) throw (FlockException);
    base::String behaviorParameterPar( com::MessageValueGroup* pParameter ) throw (FlockException);
     */
    
    /*
    com::Message mMessage;
    base::String mOSCCommand;
    com::Message mOSCParameters;
    com::Sender* mErrorSender;
    
    QHash< base::String, SpaceAlgType > mSpaceAlgMap;
    QHash< base::String, space::GridAlg::GridNeighborMode > mGridNeighborModeMap;
    QHash< base::String, space::GridAlg::GridUpdateMode > mGridUpdateModeMap;
    QHash< base::String, com::Protocol > mComProtocolMap;
    QHash< base::String, com::Format > mComFormatMap;
    QHash< base::String, Behavior* > mBehaviorMap;
    */
    
    template<typename ValueType> void groupOscArgs( const std::vector<_OscArg*>& pInputArgs, int& pGoupStartIndex, std::vector<_OscArg*>& pGroupedArgs  );

};

};

};

#endif