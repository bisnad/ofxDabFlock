/** \file dab_flock_osc_control.cpp
*/

#include "dab_flock_osc_control.h"
#include "dab_flock_event_includes.h"
#include "dab_flock_visual_event_includes.h"
#include "dab_flock_euler_integration.h"
#include "dab_flock_behavior_includes.h"
#include "dab_flock_serialize.h"
#include "dab_flock_visual.h"
#include "dab_flock_visual_swarm.h"

using namespace dab;
using namespace dab::flock;

OscControl::OscControl()
: Singleton<OscControl>()
, OscListener()
, mErrorSender(nullptr)
{}

OscControl::OscControl( std::shared_ptr<OscSender> pErrorSender )
: Singleton<OscControl>()
, OscListener()
, mErrorSender(pErrorSender)
{}

OscControl::~OscControl()
{}

void
OscControl::setErrorSender( std::shared_ptr<OscSender> pErrorSender )
{
    mErrorSender = pErrorSender;
}

void
OscControl::initCreationMaps()
{
    mSpaceAlgMap["PermanentNeighbors"] = space::PermanentNeighborsAlgType;
    mSpaceAlgMap["NTree"] = space::NTreeAlgType;
    mSpaceAlgMap["KDTree"] = space::KDTreeAlgType;
    mSpaceAlgMap["ANN"] = space::ANNAlgType;
    mSpaceAlgMap["RTree"] = space::RTreeAlgType;
    mSpaceAlgMap["Grid"] = space::GridAlgType;
    
    mGridUpdateModeMap["NoUpdate"] = space::GridAlg::NoUpdateMode;
    mGridUpdateModeMap["NearestReplace"] = space::GridAlg::NearestReplaceMode;
    mGridUpdateModeMap["NearestAdd"] = space::GridAlg::NearestAddMode;
    mGridUpdateModeMap["AvgReplace"] = space::GridAlg::AvgReplaceMode;
    mGridUpdateModeMap["AvgAdd"] = space::GridAlg::AvgAddMode;
    
    mGridNeighborModeMap["CellLocation"] = space::GridAlg::CellLocationMode;
    mGridNeighborModeMap["GridLocation"] = space::GridAlg::GridLocationMode;
    mGridNeighborModeMap["AvgLocation"] = space::GridAlg::AvgLocationMode;
    mGridNeighborModeMap["AvgRegion"] = space::GridAlg::AvgRegionMode;

    mBehaviorMap["EulerIntegration"] = new EulerIntegration("", "");
    mBehaviorMap["Acceleration"] = new AccelerationBehavior("", "");
    mBehaviorMap["Alignment"] = new AlignmentBehavior("", "");
    mBehaviorMap["BoundaryMirror"] = new BoundaryMirrorBehavior("", "");
    mBehaviorMap["BoundaryRepulsion"] = new BoundaryRepulsionBehavior("", "");
    mBehaviorMap["BoundaryWrap"] = new BoundaryWrapBehavior("", "");
    mBehaviorMap["Circular"] = new CircularBehavior("", "") ;
    mBehaviorMap["Cohesion"] = new CohesionBehavior("", "");
    mBehaviorMap["Copy"] = new CopyBehavior("", "");
    mBehaviorMap["Damping"] = new DampingBehavior("", "");
    mBehaviorMap["DistanceFieldFollow"] = new DistanceFieldFollowBehavior("", "");
    mBehaviorMap["Evasion"] = new EvasionBehavior("", "");
    mBehaviorMap["GridAvg"] = new GridAvgBehavior("", "");
    mBehaviorMap["ConeVision"] = new ConeVisionBehavior("", "");
    mBehaviorMap["NeighborIndexStore"] = new NeighborIndexStoreBehavior("", "");
    mBehaviorMap["NeighborDistanceStore"] = new NeighborDistanceStoreBehavior("", "");
    mBehaviorMap["NeighborDirectionStore"] = new NeighborDirectionStoreBehavior("", "");
    mBehaviorMap["NeighborParameterStore"] = new NeighborParameterStoreBehavior("", "");
    mBehaviorMap["ParameterCombine"] = new ParameterCombineBehavior("", "");
    mBehaviorMap["ParameterMap"] = new ParameterMapBehavior("", "");
    mBehaviorMap["ParameterPrint"] = new ParameterPrintBehavior("", "");
    mBehaviorMap["ParameterScale"] = new ParameterScaleBehavior("", "");
    mBehaviorMap["ParameterThresholdToEvent"] = new ParameterThresholdToEventBehavior("", "");
    mBehaviorMap["Randomize"] = new RandomizeBehavior("", "");
    mBehaviorMap["Reset"] = new ResetBehavior("", "");
    mBehaviorMap["Spiral"] = new SpiralBehavior("", "");
    mBehaviorMap["LineFollow"] = new LineFollowBehavior("", "");
    mBehaviorMap["TargetParameter"] = new TargetParameterBehavior("", "");
}

space::SpaceAlgType
OscControl::spaceAlgType( const std::string& pSpaceAlgType ) throw (Exception)
{
    if( mSpaceAlgMap.find(pSpaceAlgType) == mSpaceAlgMap.end() ) throw Exception("FLOCK ERROR: space algorithm of type " + pSpaceAlgType + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    return mSpaceAlgMap[pSpaceAlgType];
}

space::GridAlg::GridNeighborMode
OscControl::gridNeighborMode( const std::string& pGridNeighborMode ) throw (Exception)
{
    if( mGridNeighborModeMap.find(pGridNeighborMode) == mGridNeighborModeMap.end() ) throw Exception("FLOCK ERROR: grid neighnor mode of type " + pGridNeighborMode + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    return mGridNeighborModeMap[pGridNeighborMode];
}

space::GridAlg::GridUpdateMode
OscControl::gridUpdateMode( const std::string& pGridUpdateMode ) throw (Exception)
{
    if( mGridUpdateModeMap.find(pGridUpdateMode) == mGridUpdateModeMap.end() ) throw Exception("FLOCK ERROR: grid update mode of type " + pGridUpdateMode + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    return mGridUpdateModeMap[pGridUpdateMode];
}

void
OscControl::groupOscArgs( std::shared_ptr<OscMessage> pMessage, std::vector< std::shared_ptr<_OscArg> >& pGroupedArgs )
{
//    std::cout << *pMessage << "\n";
//    
//    std::cout << "arrayCount " << pMessage->arrayCount() << "\n";
    
    const std::vector<_OscArg*> arguments = pMessage->arguments();
    int argCount = arguments.size();
    
    if(pMessage->arrayCount() > 0) // message alread contains value groups, simple copy them
    {
        for(int aI=0; aI<argCount; ++aI)
        {
            _OscArg* arg = arguments[aI];
            
            //std::cout << "arg " << aI << " " << *arg << "\n";
            //std::cout << "valueCount " << arg->valueCount() << "\n";
            
            pGroupedArgs.push_back( std::shared_ptr<_OscArg>(arg->copy()));
        }
    }
    else
    {
        std::vector< std::vector<_OscArg*> > groupedArgs;
        
        OscType currentType = OSC_TYPE_NONE;
        std::vector<_OscArg*> currentGroup;
        std::string tmp;
        
        for(int aI=0; aI<argCount; ++aI)
        {
            _OscArg* arg = arguments[aI];
            
//            std::cout << "arg " << aI << " " << *arg << "\n";
//            std::cout << "valueCount " << arg->valueCount() << "\n";
//            std::cout << "byteCount " << arg->byteCount() << "\n";
            
            if(arg->oscType() == currentType && arg->oscType() != OSC_TYPE_STRING)
            {
                currentGroup.push_back(arg);
            }
            else
            {
                if( currentGroup.size() > 0 )
                {
                    groupedArgs.push_back(currentGroup);
                    currentGroup.clear();
                }
                
                if(arg->oscType() == OSC_TYPE_CHAR)
                {
                    currentType = OSC_TYPE_NONE;
                }
                else if(arg->oscType() == OSC_TYPE_STRING && arg->valueCount() == 1 )
                {
                    currentType = OSC_TYPE_NONE;
                }
                else
                {
                    currentGroup.push_back(arg);
                    currentType = arg->oscType();
                }
            }
        }
        
        if(currentGroup.size() > 0) groupedArgs.push_back(currentGroup);
        
        int groupCount = groupedArgs.size();
        
        //std::cout << "groupCount " << groupCount << "\n";
        
        for(int gI=0; gI<groupCount; ++gI)
        {
            std::vector<_OscArg*>& group = groupedArgs[gI];
            int groupSize = group.size();
            OscType groupType = group[0]->oscType();
            
            //std::cout << "group " << gI << " size " << groupSize << " type " << groupType << "\n";
            
            if(groupSize == 1)
            {
                pGroupedArgs.push_back( std::shared_ptr<_OscArg>(group[0]->copy()));
            }
            else
            {
                if(groupType == OSC_TYPE_INT32)
                {
                    std::vector<int32_t> valueVec(groupSize);
                    for(int vI=0; vI<groupSize; ++vI)
                    {
                        valueVec[vI] = *(group[vI]);
                    }
                    
                    pGroupedArgs.push_back( std::shared_ptr<_OscArg>(new OscArg<int32_t>( valueVec.data(), valueVec.size())));
                }
                else if(groupType == OSC_TYPE_FLOAT)
                {
                    
                    
                    std::vector<float> valueVec(groupSize);
                    for(int vI=0; vI<groupSize; ++vI)
                    {
                        valueVec[vI] = *(group[vI]);
                    }
                    
                    pGroupedArgs.push_back(std::shared_ptr<_OscArg>(new OscArg<float>( valueVec.data(), valueVec.size())));
                }
                // etc more types
            }
            
            //std::cout << "new group " << *(pGroupedArgs[ pGroupedArgs.size() - 1]) << "\n";
        }
    }
}


void
OscControl::notify(std::shared_ptr<OscMessage> pMessage)
{
    mLock.lock();
    
    //std::cout << "OSCControl::notify\n";
    
    try
    {
        std::string oscCommand = pMessage->address();
        std::vector< std::shared_ptr<_OscArg> > groupedOscArgs;
        groupOscArgs(pMessage, groupedOscArgs);

		//std::cout << "oscCommand " << oscCommand << "\n";

        if(oscCommand == "/ClearSimulation") clearSimulation();
        else if(oscCommand == "/RestoreSimulation") restoreSimulation(groupedOscArgs);
		else if(oscCommand == "/SaveSimulation") saveSimulation(groupedOscArgs);
        else if(oscCommand == "/SetSimulationRate") setSimulationRate(groupedOscArgs);
        else if(oscCommand == "/FreezeSimulation") freezeSimulation(groupedOscArgs);
        else if(oscCommand == "/AddSpace") addSpace(groupedOscArgs);
        else if(oscCommand == "/AddAgents") addAgents(groupedOscArgs);
        else if(oscCommand == "/RemoveAgents") removeAgents(groupedOscArgs);
        else if(oscCommand == "/SetParameter") setParameter(groupedOscArgs);
		else if (oscCommand == "/AssignNeighbors") assignNeighbors(groupedOscArgs);
        else if(oscCommand == "/ShowSwarm") showSwarm(groupedOscArgs);
        else if(oscCommand == "/HideSwarm") hideSwarm(groupedOscArgs);
        else if(oscCommand == "/ShowSpace") showSpace(groupedOscArgs);
        else if(oscCommand == "/HideSpace") hideSpace(groupedOscArgs);
		else if (oscCommand == "/DisplayPosition") setDisplayPosition(groupedOscArgs);
		else if (oscCommand == "/DisplayOrientation") setDisplayOrientation(groupedOscArgs);
		else if (oscCommand == "/DisplayOrientationChange") changeDisplayOrientation(groupedOscArgs);
		else if (oscCommand == "/DisplayZoom") setDisplayZoom(groupedOscArgs);
        else if(oscCommand == "/AgentColor") setAgentColor(groupedOscArgs);
        else if(oscCommand == "/AgentScale") setAgentScale(groupedOscArgs);
        else if(oscCommand == "/AgentLineWidth") setAgentLineWidth(groupedOscArgs);
        else if(oscCommand == "/TrailColor") setTrailColor(groupedOscArgs);
        else if(oscCommand == "/TrailLength") setTrailLength(groupedOscArgs);
        else if(oscCommand == "/TrailDecay") setTrailDecay(groupedOscArgs);
        else if(oscCommand == "/TrailLineWidth") setTrailLineWidth(groupedOscArgs);
    }
    catch(dab::Exception& e)
    {
        e += dab::Exception("COM ERROR: OscControl notify failed", __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
    
    mLock.unlock();
}

void
OscControl::clearSimulation() throw (Exception)
{
    try
    {
        Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new ClearSimulationEvent()) );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::restoreSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            std::string fileName = pParameters[0]->operator const std::string&();
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RestoreSimulationEvent( fileName )) );
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING)
        {
            std::string fileName = pParameters[0]->operator const std::string&();
            std::string restoreModeString = pParameters[1]->operator const std::string&();
            
            if( restoreModeString == "Config" ) Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RestoreSimulationEvent( fileName, SerializeTools::ConfigMode )) );
            else if( restoreModeString == "Values" ) Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RestoreSimulationEvent( fileName, SerializeTools::ValuesMode )) );
            else Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RestoreSimulationEvent( fileName )) );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /RestoreSimulation", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::saveSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            std::string fileName = pParameters[0]->operator const std::string&();
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SaveSimulationEvent( fileName )) );
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING)
        {
            std::string fileName = pParameters[0]->operator const std::string&();
            std::string restoreModeString = pParameters[1]->operator const std::string&();
            
            if( restoreModeString == "Config" ) Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SaveSimulationEvent( fileName, SerializeTools::ConfigMode )) );
            else if( restoreModeString == "Values" ) Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SaveSimulationEvent( fileName, SerializeTools::ValuesMode )) );
            else Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SaveSimulationEvent( fileName )) );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /SaveSimulation", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setSimulationRate(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_FLOAT)
        {
            float simulationRate = *(pParameters[0]);
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetSimulationRateEvent(simulationRate)) );
        }
        else if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_INT32)
        {
            // simulation rate
            int simulationRate = *(pParameters[0]);
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetSimulationRateEvent(static_cast<float>(simulationRate))) );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /SetSimulationRate", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::freezeSimulation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_INT32)
        {
            int freeze = *(pParameters[0]);
             Simulation& simulation = Simulation::get();
            
            if( static_cast<bool>(freeze) == false && simulation.frozen() == true ) simulation.switchFrozen();
            else if( static_cast<bool>(freeze) == true && simulation.frozen() == false ) simulation.switchFrozen();
        }
        else if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_FLOAT)
        {
            float freeze = *(pParameters[0]);
            Simulation& simulation = Simulation::get();
            
            if( static_cast<bool>(freeze) == false && simulation.frozen() == true ) simulation.switchFrozen();
            else if( static_cast<bool>(freeze) == true && simulation.frozen() == false ) simulation.switchFrozen();
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /FreezeSimulation", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::addSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 3 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            std::string spaceAlgName = pParameters[1]->operator const std::string&();
            int spaceDim = *(pParameters[2]);
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new AddSpaceEvent( spaceName, spaceAlgType(spaceAlgName), spaceDim)) );

        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[3]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY )
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            std::string spaceAlgName = pParameters[1]->operator const std::string&();
            int spaceDim = pParameters[2]->valueCount();
            float* spaceMinPosValues = *(pParameters[2]);
            float* spaceMaxPosValues = *(pParameters[3]);
            
            Eigen::VectorXf spaceMinPos(spaceDim);
            for(int d=0; d<spaceDim; ++d) spaceMinPos[d] = spaceMinPosValues[d];
            
            Eigen::VectorXf spaceMaxPos(spaceDim);
            for(int d=0; d<spaceDim; ++d) spaceMaxPos[d] = spaceMaxPosValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new AddSpaceEvent( spaceName, spaceAlgType(spaceAlgName), spaceMinPos, spaceMaxPos)) );
        }
        else if(pParameters.size() == 8 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32 && pParameters[3]->oscType() == EXT_TYPE_ARG_INT32_ARRAY && pParameters[4]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[5]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[6]->oscType() == OSC_TYPE_STRING && pParameters[7]->oscType() == OSC_TYPE_STRING)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            std::string spaceAlgName = pParameters[1]->operator const std::string&();
            int gridValueDim = *(pParameters[2]);
            int spaceDim = pParameters[3]->valueCount();
            int* gridSubdivisionValues = *(pParameters[3]);
            float* spaceMinPosValues = *(pParameters[4]);
            float* spaceMaxPosValues = *(pParameters[5]);
            std::string gridNeighborModeName = pParameters[6]->operator const std::string&();
            std::string gridUpdateModeName = pParameters[7]->operator const std::string&();
            
            dab::Array<unsigned int> gridSubdivisionCount(spaceDim);
            for(int d=0; d<spaceDim; ++d) gridSubdivisionCount[d] = gridSubdivisionValues[d];
            
            Eigen::VectorXf spaceMinPos(spaceDim);
            for(int d=0; d<spaceDim; ++d) spaceMinPos[d] = spaceMinPosValues[d];
            
            Eigen::VectorXf spaceMaxPos(spaceDim);
            for(int d=0; d<spaceDim; ++d) spaceMaxPos[d] = spaceMaxPosValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new AddSpaceEvent(spaceName, spaceAlgType(spaceAlgName), gridValueDim, gridSubdivisionCount, spaceMinPos, spaceMaxPos, gridNeighborMode(gridNeighborModeName), gridUpdateMode(gridUpdateModeName))) );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /AddSpace", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::addAgents(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_INT32)
        {
            // swarm name / agent count
            std::string swarmName = pParameters[0]->operator const std::string&();
            int agentCount = *(pParameters[1]);
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new AddAgentsEvent( swarmName, agentCount )));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /AddAgents", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::removeAgents(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            // swarm name
            std::string swarmName = pParameters[0]->operator const std::string&();
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RemoveAgentsEvent( swarmName )));
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_INT32)
        {
            // swarm name / agent count
            std::string swarmName = pParameters[0]->operator const std::string&();
            int agentCount = *(pParameters[1]);
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RemoveAgentsEvent( swarmName, agentCount )));
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_INT32_ARRAY)
        {
            // swarm name / agent start index / agent end index
            std::string swarmName = pParameters[0]->operator const std::string&();
            int parameterDim = pParameters[1]->valueCount();
            int* agentIndices = *(pParameters[1]);
            int agentStartIndex = 0;
            int agentEndIndex = 0;
            
            if(parameterDim == 1)
            {
                agentStartIndex = agentIndices[0];
                agentEndIndex = agentStartIndex;
            }
            else if(parameterDim > 1)
            {
                agentStartIndex = agentIndices[0];
                agentEndIndex = agentIndices[1];
            }
    
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new RemoveAgentsEvent( swarmName, agentStartIndex, agentEndIndex )));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /RemoveAgents", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setParameter(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 3 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / parameter value
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            float parameterValue = *(pParameters[2]);
            
            Eigen::VectorXf parameterVec(1);
            parameterVec[0] = parameterValue;
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, parameterVec)));
        }
        else if(pParameters.size() == 3 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            // swarm name / parameter name / parameter values
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int parameterDim = pParameters[2]->valueCount();
            float* parameterValues = *(pParameters[2]);
            
            Eigen::VectorXf parameterVec(parameterDim);
            for(int d=0; d<parameterDim; ++d) parameterVec[d] = parameterValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, parameterVec)));
        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_FLOAT && pParameters[3]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / parameter value / duration
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            float parameterValue = *(pParameters[2]);
            float duration = *(pParameters[3]);
            
            Eigen::VectorXf parameterVec(1);
            parameterVec[0] = parameterValue;
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, parameterVec, duration)));
        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[3]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / parameter values / duration
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int parameterDim = pParameters[2]->valueCount();
            float* parameterValues = *(pParameters[2]);
            float duration = *(pParameters[3]);
            
            Eigen::VectorXf parameterVec(parameterDim);
            for(int d=0; d<parameterDim; ++d) parameterVec[d] = parameterValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, parameterVec, duration)));
        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32 && pParameters[3]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / agent index / parameter value
            
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int agentIndex = *(pParameters[2]);
            float parameterValue = *(pParameters[3]);
            
            Eigen::VectorXf parameterVec(1);
            parameterVec[0] = parameterValue;
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, agentIndex, parameterVec)));
        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32 && pParameters[3]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            // swarm name / parameter name / agent index / parameter values
            
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int agentIndex = *(pParameters[2]);
            int parameterDim = pParameters[3]->valueCount();
            float* parameterValues = *(pParameters[3]);
            
            Eigen::VectorXf parameterVec(parameterDim);
            for(int d=0; d<parameterDim; ++d) parameterVec[d] = parameterValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, agentIndex, parameterVec)));
        }
        else if(pParameters.size() == 5 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32 && pParameters[3]->oscType() == OSC_TYPE_FLOAT && pParameters[4]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / agent index / parameter value / interpolation duration
            
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int agentIndex = *(pParameters[2]);
            float parameterValue = *(pParameters[3]);
            float duration = *(pParameters[4]);
            
            Eigen::VectorXf parameterVec(1);
            parameterVec[0] = parameterValue;
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, agentIndex, parameterVec, duration)));
        }
        else if(pParameters.size() == 5 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_INT32 && pParameters[3]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[4]->oscType() == OSC_TYPE_FLOAT)
        {
            // swarm name / parameter name / agent index / parameter values / interpolation duration
            
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string parameterName = pParameters[1]->operator const std::string&();
            int agentIndex = *(pParameters[2]);
            int parameterDim = pParameters[3]->valueCount();
            float* parameterValues = *(pParameters[3]);
            float duration = *(pParameters[4]);
            
            Eigen::VectorXf parameterVec(parameterDim);
            for(int d=0; d<parameterDim; ++d) parameterVec[d] = parameterValues[d];
            
            Simulation::get().event().addEvent( std::shared_ptr<event::Event>(new SetParameterEvent(swarmName, parameterName, agentIndex, parameterVec, duration)));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /SetParameter", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void 
OscControl::assignNeighbors(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
	// TODO: create an assign neighbor event for this
	// TODO: this is marginal implementatiom only for changing the visibility of one parameter in one space only

	try
	{
		if (pParameters.size() == 5 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_INT32  && pParameters[2]->oscType() == OSC_TYPE_STRING && pParameters[3]->oscType() == OSC_TYPE_STRING  && pParameters[4]->oscType() == OSC_TYPE_INT32)
		{
			// swarm name / agent_index / parameter name / space name/ visible 
			std::string swarmName = pParameters[0]->operator const std::string&();
			int agentIndex = *(pParameters[1]);
			std::string parameterName = pParameters[2]->operator const std::string&();
			std::string spaceName = pParameters[3]->operator const std::string&();
			int visible = *(pParameters[4]);

			// TODO: create an assign neighbor event for this
			Simulation::get().swarm(swarmName)->agent(agentIndex)->assignNeighbors(parameterName, spaceName, static_cast<bool>(visible));
		}
		else throw Exception("FLOCK ERROR: Wrong Parameters for /AssignNeighbors", __FILE__, __FUNCTION__, __LINE__);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void
OscControl::showSwarm(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string posParName = pParameters[1]->operator const std::string&();
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSwarmEvent(swarmName, posParName)));
        }
        else if(pParameters.size() == 3 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_STRING)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string posParName = pParameters[1]->operator const std::string&();
            std::string velParName = pParameters[2]->operator const std::string&();
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSwarmEvent(swarmName, posParName, velParName)));
        }
        else if(pParameters.size() == 4 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_STRING && pParameters[2]->oscType() == OSC_TYPE_STRING && pParameters[3]->oscType() == OSC_TYPE_INT32)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            std::string posParName = pParameters[1]->operator const std::string&();
            std::string velParName = pParameters[2]->operator const std::string&();
            int trailLength = *(pParameters[3]);
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSwarmEvent(swarmName, posParName, velParName, trailLength)));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /ShowSwarm", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::hideSwarm(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new HideSwarmEvent(swarmName)));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /HideSwarm", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::showSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSpaceEvent(spaceName)));
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            
            int colorDim = pParameters[1]->valueCount();
            float* colorValues = *(pParameters[1]);
            
            if(colorDim != 4) throw Exception("FLOCK ERROR: iWrong Parameters for /ShowSwarm", __FILE__, __FUNCTION__, __LINE__ );
            
            std::array<float, 4> color = { colorValues[0], colorValues[1], colorValues[2], colorValues[3] };
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSpaceEvent(spaceName, color)));
        }
        else if(pParameters.size() == 3 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY && pParameters[2]->oscType() == OSC_TYPE_FLOAT)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            int colorDim = pParameters[1]->valueCount();
            float* colorValues = *(pParameters[1]);
            float valueScale = *(pParameters[2]);
            
            
            if(colorDim != 4) throw Exception("FLOCK ERROR: iWrong Parameters for /ShowSwarm", __FILE__, __FUNCTION__, __LINE__ );
            
            std::array<float, 4> color = { colorValues[0], colorValues[1], colorValues[2], colorValues[3] };
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new ShowSpaceEvent(spaceName, color, valueScale)));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /ShowSwarm", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::hideSpace(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_STRING)
        {
            std::string spaceName = pParameters[0]->operator const std::string&();
            
            FlockVisuals::get().event().addEvent( std::shared_ptr<event::Event>(new HideSpaceEvent(spaceName)));
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /HideSpace", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void 
OscControl::setDisplayPosition(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
	try
	{
		if (pParameters.size() == 1 && pParameters[0]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
		{
			int posDim = pParameters[0]->valueCount();
			float* posValues = *(pParameters[0]);
			if (posDim != 3) throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayPosition", __FILE__, __FUNCTION__, __LINE__);

			ofVec3f pos(posValues[0], posValues[1], posValues[2]);

			FlockVisuals::get().setDisplayPosition(pos);
		}
		else throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayPosition", __FILE__, __FUNCTION__, __LINE__);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void 
OscControl::setDisplayOrientation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
	try
	{
		if (pParameters.size() == 1 && pParameters[0]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
		{
			int orientDim = pParameters[0]->valueCount();
			float* orientValues = *(pParameters[0]);
			if (orientDim != 4) throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayOrientation", __FILE__, __FUNCTION__, __LINE__);

			ofQuaternion orient(orientValues[0], orientValues[1], orientValues[2], orientValues[3]);

			FlockVisuals::get().setDisplayOrientation(orient);
		}
		else throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayOrientation", __FILE__, __FUNCTION__, __LINE__);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void 
OscControl::changeDisplayOrientation(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
	try
	{
		if (pParameters.size() == 1 && pParameters[0]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
		{
			int orientDim = pParameters[0]->valueCount();
			float* orientValues = *(pParameters[0]);
			if (orientDim != 4) throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayOrientationChange", __FILE__, __FUNCTION__, __LINE__);

			ofQuaternion orient(orientValues[0], orientValues[1], orientValues[2], orientValues[3]);

			FlockVisuals::get().setDisplayOrientationChange(orient);
		}
		else throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayOrientationChange", __FILE__, __FUNCTION__, __LINE__);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void 
OscControl::setDisplayZoom(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
	try
	{
		if (pParameters.size() == 1 && pParameters[0]->oscType() == OSC_TYPE_FLOAT)
		{
			float zoom = *(pParameters[0]);
			
			FlockVisuals::get().setDisplayZoom(zoom);
		}
		else throw Exception("FLOCK ERROR: Wrong Parameters for /DisplayZoom", __FILE__, __FUNCTION__, __LINE__);
	}
	catch (Exception& e)
	{
		throw;
	}
}

void
OscControl::setAgentColor(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            
            int colorDim = pParameters[1]->valueCount();
            float* colorValues = *(pParameters[1]);
            
            if(colorDim != 4) throw Exception("FLOCK ERROR: Wrong Parameters for /AgentColor", __FILE__, __FUNCTION__, __LINE__ );
            
            std::array<float, 4> color = { colorValues[0], colorValues[1], colorValues[2], colorValues[3] };
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setAgentColor(color);
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /AgentColor", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setAgentScale(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_FLOAT)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            float agentScale = *(pParameters[1]);
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setAgentScale( ofVec3f(agentScale, agentScale, agentScale) );
        }
        else if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            
            int agentScaleDim = pParameters[1]->valueCount();
            float* agentScaleValues = *(pParameters[1]);
            
            if(agentScaleDim != 3) throw Exception("FLOCK ERROR: Wrong Parameters for /AgentScale", __FILE__, __FUNCTION__, __LINE__ );
            
            ofVec3f agentScale( agentScaleValues[0], agentScaleValues[1], agentScaleValues[2] );
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setAgentScale( agentScale );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /AgentScale", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setAgentLineWidth(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_FLOAT)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            float agentLineWidth = *(pParameters[1]);
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setAgentLineWidth( agentLineWidth );
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /AgentLineWidth", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setTrailColor(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == EXT_TYPE_ARG_FLOAT_ARRAY)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            
            int trailColorDim = pParameters[1]->valueCount();
            float* trailColorValues = *(pParameters[1]);
            
            if(trailColorDim != 4) throw Exception("FLOCK ERROR: Wrong Parameters for /TrailColor", __FILE__, __FUNCTION__, __LINE__ );
            
            std::array<float, 4> trailColor = { trailColorValues[0], trailColorValues[1], trailColorValues[2], trailColorValues[3] };
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setTrailColor(trailColor);
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /TrailColor", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setTrailLength(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_INT32)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            int trailLength = *(pParameters[1]);
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setTrailLength(trailLength);
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /TrailLength", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setTrailDecay(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_FLOAT)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            float trailDecay = *(pParameters[1]);
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setTrailDecay(trailDecay);
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /TrailFloat", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
OscControl::setTrailLineWidth(const std::vector< std::shared_ptr<_OscArg> >& pParameters) throw (Exception)
{
    try
    {
        if(pParameters.size() == 2 && pParameters[0]->oscType() == OSC_TYPE_STRING && pParameters[1]->oscType() == OSC_TYPE_FLOAT)
        {
            std::string swarmName = pParameters[0]->operator const std::string&();
            float trailWidth = *(pParameters[1]);
            
            VisSwarm* visSwarm = FlockVisuals::get().visualSwarm(swarmName);
            
            visSwarm->setTrailWidth(trailWidth);
        }
        else throw Exception( "FLOCK ERROR: Wrong Parameters for /TrailLineWidth", __FILE__, __FUNCTION__, __LINE__ );
    }
    catch(Exception& e)
    {
        throw;
    }
}