/** \file dab_flock_serialize.cpp
*/

#include "dab_flock_serialize.h"
#include "dab_file_io.h"
#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_flock_agent.h"
#include "dab_flock_env.h"
#include "dab_flock_behavior.h"
#include "dab_flock_swarm_behavior.h"
#include "dab_flock_behavior_includes.h"
#include "dab_flock_visual.h"
#include "dab_flock_visual_swarm.h"
#include "dab_flock_visual_agent_shape.h"
#include "dab_flock_visual_agent_trail.h"
#include "dab_flock_visual_neighbor_space.h"
#include "dab_flock_visual_grid_space.h"
#include "dab_space_includes.h"

using namespace dab;
using namespace dab::flock;

template<>
void
SerializeTools::getValues<unsigned int>(const Json::Value& pData, const std::string& pName, dab::Array<unsigned int>& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    pValues.setSize(dataDim);
    
    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asUInt();
    }
}

SerializeTools::SerializeTools()
{
    if( mBehaviorMap.size() == 0 ) createBehaviorMap();
}

SerializeTools::~SerializeTools()
{}

void
SerializeTools::createBehaviorMap()
{
    registerBehavior( new EulerIntegration("","") );
    registerBehavior( new AccelerationBehavior("", "") );
    registerBehavior( new AlignmentBehavior("", "") );
    registerBehavior( new BoundaryMirrorBehavior("", "") );
    registerBehavior( new BoundaryRepulsionBehavior("", "") );
    registerBehavior( new BoundaryWrapBehavior("", "") );
    registerBehavior( new CircularBehavior("", "") );
    registerBehavior( new CohesionBehavior("", "") );
    registerBehavior( new CopyBehavior("", "") );
    registerBehavior( new DampingBehavior("", "") );
    registerBehavior( new DistanceFieldFollowBehavior("", "") );
    registerBehavior( new EvasionBehavior("", "") );
    registerBehavior( new GridAvgBehavior("", "") );
    registerBehavior( new ConeVisionBehavior("", "") );
    registerBehavior( new NeighborStoreBehavior("", "") );
    registerBehavior( new NeighborIndexStoreBehavior("", "") );
    registerBehavior( new NeighborDistanceStoreBehavior("", "") );
    registerBehavior( new NeighborDirectionStoreBehavior("", "") );
    registerBehavior( new ParameterCombineBehavior("", "") );
    registerBehavior( new ParameterMagBehavior("", "") );
    registerBehavior( new ParameterMapBehavior("", "") );
    registerBehavior( new ParameterPrintBehavior("", "") );
    registerBehavior( new ParameterScaleBehavior("", "") );
    registerBehavior( new ParameterThresholdToEventBehavior("", "") );
    registerBehavior( new RandomizeBehavior("", "") );
    registerBehavior( new ResetBehavior("", "") );
    registerBehavior( new SpiralBehavior("", "") );
    registerBehavior( new LineFollowBehavior("", "") );
    registerBehavior( new TargetParameterBehavior("", "") );
    registerBehavior( new OrbitBehavior("", "") );
    
    //    registerBehavior( new EnvClampBehavior("","") );
    //    registerBehavior< EnvDecayBehavior >();
    //    registerBehavior< EnvDiffusionBehavior >();
    //    registerBehavior< EnvGrayScottBehavior >();
    //    registerBehavior< EnvGiererMeinhardtBehavior >();
    //    registerBehavior< EnvAgentInteractBehavior >();
}

void
SerializeTools::registerBehavior(Behavior* pBehavior)
{
    mBehaviorMap[pBehavior->className()] = pBehavior;
}

void
SerializeTools::save( const std::string& pFileName, SerializeMode pMode ) throw (Exception)
{
	//std::cout << "save begin\n";
	
	try
	{
		if( Simulation::get().paused() == false ) Simulation::get().switchPaused();
		
		Json::Value serializeData;
        serializeData["className"] = "Swarm";
        serializeData["configName"] = "Swarm";
		
		if( pMode == ConfigMode || pMode == AllMode )
		{
			saveConfig( serializeData );
		}
		
		if( pMode == ValuesMode || pMode == AllMode )
		{
			saveValues( serializeData );
		}
        
        if( Simulation::get().paused() == true ) Simulation::get().switchPaused();
        
        // write data
        try
        {
            Json::StyledWriter styledWriter;
            dab::FileIO::get().write(styledWriter.write(serializeData), ofToDataPath(pFileName));
        }
        catch(dab::Exception& e)
        {
            e += dab::Exception("FILE ERROR: failed to write file " + pFileName, __FILE__, __FUNCTION__, __LINE__);
            throw e;
        }
	}
	catch(Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to save to file " + pFileName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
	//std::cout << "save end\n";
}

void
SerializeTools::saveConfig( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		Json::Value configData;
		configData["className"] = "SwarmConfig";
		configData["configName"] = "SwarmConfig";
		
		serializeSim(configData);
		serializeCom(configData);
		serializeSpace(configData);
		serializeSwarms(configData);
		serializeVis(configData);
		
		pSerializeData["SwarmConfig"] = configData;
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to save configuration", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::saveValues( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		Json::Value valuesData;
		valuesData["className"] = "SwarmValues";
		valuesData["configName"] = "SwarmValues";
		
		serializeValues(valuesData);
		
		pSerializeData["SwarmValues"] = valuesData;
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to save values", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::restore( const std::string& pFileName, SerializeMode pMode ) throw (Exception)
{
	try
	{
		if( Simulation::get().paused() == false ) Simulation::get().switchPaused();
        
        std::string serializeString;
        dab::FileIO::get().read(ofToDataPath(pFileName), serializeString);
        
        Json::Reader reader;
        Json::Value serializeData;
        
        bool parsingSuccessful = reader.parse(serializeString, serializeData);
        
        if(parsingSuccessful == false) throw Exception("FLOCK ERROR: failed to parse file " + pFileName, __FILE__, __FUNCTION__, __LINE__);
		
		if( pMode == ConfigMode || pMode == AllMode )
		{
			const Json::Value& configData = getValue(serializeData,"SwarmConfig");
			
			restoreConfig( configData );
		}
		
		if( pMode == ValuesMode || pMode == AllMode )
		{
			const Json::Value& valuesData = getValue(serializeData,"SwarmValues");
			
			restoreValues( valuesData );
		}
        
		
		if( Simulation::get().paused() == true ) Simulation::get().switchPaused();
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to restore file " + pFileName, __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
SerializeTools::restoreConfig( const Json::Value&  pConfigData ) throw (Exception)
{
	try
	{
		deSerializeSim( pConfigData );
		deSerializeCom( pConfigData );
		deSerializeSpace( pConfigData );
		deSerializeSwarms( pConfigData );
		deSerializeVis( pConfigData );
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to restore config", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::restoreValues( const Json::Value& pValuesData ) throw (Exception)
{
	//std::cout << "SerializeTools::restoreValues begin\n";
	
	try
	{
		deSerializeValues( pValuesData );
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to restore values", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
	//std::cout << "SerializeTools::restoreValues end\n";
}

void
SerializeTools::serializeSim( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		float simUpdateInterval = Simulation::get().updateInterval();
		
		Json::Value simData;
		simData["className"] = "Simulation";
		simData["configName"] = "Simulation";
		simData["rate"] = simUpdateInterval;
		
		pSerializeData["Simulation"] = simData;
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to serialize simulation", __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SerializeTools::serializeCom( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		FlockCom& flockCom = Simulation::get().com();
        
        std::vector< std::string > senderNames = flockCom.senderNames();
        std::vector< std::string > receiverNames = flockCom.receiverNames();
		
		int comCount = senderNames.size() + receiverNames.size();
		
		if( comCount > 0 )
		{
            Json::Value comSerializeData;
			comSerializeData["className"] = "Communication";
			comSerializeData["configName"] = "Communication";
			
			int senderCount = senderNames.size();

			if( senderCount > 0 )
			{
				Json::Value sendersSerializeData;
				for( int sI=0; sI < senderCount; ++sI )
				{
					std::string senderName = senderNames[sI];
					std::string senderIP = flockCom.senderIP( senderName );
					unsigned senderPort = flockCom.senderPort( senderName );
                    bool extendedOscMode = flockCom.extendedOscMode(senderName);
					
					Json::Value  senderSerializeData;
					senderSerializeData["className"] = "Sender";
					senderSerializeData["configName"] = "Sender";
					senderSerializeData["configNr"] = sI;
					senderSerializeData["name"] = senderName;;
					senderSerializeData["ip"] = senderIP;
					senderSerializeData["port"] = senderPort;
                    senderSerializeData["extOscMode"] = extendedOscMode;
                    
					sendersSerializeData.append(senderSerializeData);
				}
                
				comSerializeData["Senders"] = sendersSerializeData;
			}
			
			int receiverCount = receiverNames.size();
			
			if( receiverCount > 0)
			{
				Json::Value receiversSerializeData;
				
				for( int rI=0; rI < receiverCount; ++rI )
				{
					std::string receiverName = receiverNames[rI];
					unsigned receiverPort = flockCom.receiverPort( receiverName );
					
					Json::Value receiverSerializeData;
					receiverSerializeData["className"] = "Receiver";
					receiverSerializeData["configName"] = "Receiver";
					receiverSerializeData["configNr"] = rI;
					receiverSerializeData["name"] = receiverName;
					receiverSerializeData["port"] = receiverPort;
					
					receiversSerializeData.append(receiverSerializeData);
				}
				
				comSerializeData["Receivers"] = receiversSerializeData;
			}
			
			pSerializeData["Communication"] = comSerializeData;
			
		}
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to serialize communication", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::serializeSpace( Json::Value& pSerializeData ) throw (Exception)
{
	//std::cout << "SerializeTools::serializeSpace begin\n";
	
	try
	{
		space::SpaceManager& spaceManager = space::SpaceManager::get();
		
        std::vector< std::string > spaceNames = spaceManager.spaceNames();
		int spaceCount = spaceNames.size();
		
		if( spaceCount > 0 )
		{
			Json::Value spacesSerializeData;
			
			for(int sI=0; sI<spaceCount; ++sI)
			{
                std::shared_ptr<space::Space> space = spaceManager.space(spaceNames[sI]);
				space::SpaceAlg* spaceAlg = space->spaceAlg();
				const std::string& spaceName = space->name();
				unsigned int spaceDim = spaceAlg->dim();
				
				// only deal with grid spaces that own their own grids. The other grid spaces are created from environments and will be dealt with there
				if( dynamic_cast< space::GridAlg* >( spaceAlg ) != NULL && ( dynamic_cast< space::GridAlg* >( spaceAlg ) )->gridOwner() == false ) continue;
				
                Json::Value spaceSerializeData;
				spaceSerializeData["className"] = "Space";
				spaceSerializeData["configName"] = "Space";
				spaceSerializeData["configNr"] = sI;
				spaceSerializeData["name"] = spaceName;
				spaceSerializeData["dim"] = spaceDim;
				
				std::cout << "space name " << spaceName << "\n";
				
				if( spaceAlg->fixedSize() == true )
				{
                    spaceSerializeData["minPos"] = addValues(spaceAlg->minPos());
                    spaceSerializeData["maxPos"] = addValues(spaceAlg->maxPos());
				}
				
				if( dynamic_cast< space::RTreeAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "RTree";
					
					if( ( dynamic_cast< space::RTreeAlg* >( spaceAlg ) )->closestShapePointType() == space::ClosestPointAABB )
                    {
                        spaceSerializeData["closestPointType"] = "AABB";
                    }
					else
                    {
                        spaceSerializeData["closestPointType"] = "Shape";
                    }
				}
				else if( dynamic_cast< space::PermanentNeighborsAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "Manual";
				}
				else if( dynamic_cast< space::NTreeAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "NTree";
				}
				else if( dynamic_cast< space::KDTreeAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "KDTree";
				}
				else if( dynamic_cast< space::ANNAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "ANN";
				}
				else if( dynamic_cast< space::GridAlg* >( spaceAlg ) != nullptr )
				{
					spaceSerializeData["algorithm"] = "Grid";
					
					space::GridAlg* gridAlg = dynamic_cast< space::GridAlg* >( spaceAlg );
					space::SpaceGrid* spaceGrid = &( gridAlg->grid() );
					
					spaceSerializeData["valueDim"] = spaceGrid->valueDim();
                    spaceSerializeData["subdivisionCount"] = addValues(spaceGrid->subdivisionCount());
					
					if( gridAlg->neighborMode() == space::GridAlg::CellLocationMode ) spaceSerializeData["neighborMode"] = "CellLocationMode";
					else if( gridAlg->neighborMode() == space::GridAlg::GridLocationMode ) spaceSerializeData["neighborMode"] = "GridLocationMode";
					else if( gridAlg->neighborMode() == space::GridAlg::AvgLocationMode ) spaceSerializeData["neighborMode"] = "AvgLocationMode";
					else if( gridAlg->neighborMode() == space::GridAlg::AvgRegionMode ) spaceSerializeData["neighborMode"] = "AvgRegionMode";
					
					if( gridAlg->updateMode() == space::GridAlg::NoUpdateMode ) spaceSerializeData["updateMode"] = "NoUpdateMode";
					else if( gridAlg->updateMode() == space::GridAlg::NearestReplaceMode ) spaceSerializeData["updateMode"] = "NearestReplaceMode";
					else if( gridAlg->updateMode() == space::GridAlg::NearestAddMode ) spaceSerializeData["updateMode"] = "NearestAddMode";
					else if( gridAlg->updateMode() == space::GridAlg::AvgReplaceMode ) spaceSerializeData["updateMode"] = "AvgReplaceMode";
					else if( gridAlg->updateMode() == space::GridAlg::AvgAddMode ) spaceSerializeData["updateMode"] = "AvgAddMode";
				}
				
				pSerializeData["Spaces"].append(spaceSerializeData);
			}
		}
	}
	catch (Exception& e)
	{
		e += Exception("FLOCK ERROR: failed to serialize spaces", __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
	
	//std::cout << "SerializeTools::serializeSpace end\n";
}

void
SerializeTools::serializeSwarms( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		
		// ****
		// Envs
		// ****
		
        std::vector<Env*>& envs = Simulation::get().envs();
		int envCount = envs.size();
		
		if( envCount > 0 )
		{
			for(int eI=0; eI<envCount; ++eI)
			{
				Json::Value envSerializeData;
				envSerializeData["className"] = "Env";
				envSerializeData["configName"] = "Env";
				envSerializeData["configNr"] = eI;
				
				Env* env = envs[eI];
				const std::string& envName = env->name();
				unsigned int envDim = env->dim();
				
				envSerializeData["name"] = envName;
				envSerializeData["dim"] = envDim;
				
				// ****************
				// Env Parameters
				// ****************
				
				int parameterCount = env->parameterCount();
				
				if( parameterCount > 0 )
				{
					for(int epI=0; epI < parameterCount; ++epI )
					{
						Json::Value parameterSerializeData;
						
						Parameter* parameter = env->parameter(epI);
						EnvParameter* envParameter = dynamic_cast< EnvParameter* >( parameter );
						const std::string& parameterName = parameter->name();
						unsigned int parameterDim = parameter->values().rows();
						
						parameterSerializeData["name"] = parameterName;
						
						if( envParameter != nullptr )
						{
							parameterSerializeData["className"] = "EnvParameter";
							parameterSerializeData["configName"] = "EnvParameter";
							parameterSerializeData["configNr"] = epI;
							parameterSerializeData["valueDim"] = parameterDim;
							parameterSerializeData["gridSize"] = addValues(envParameter->gridSize());
							parameterSerializeData["minPos"] = addValues(envParameter->minPos());
							parameterSerializeData["maxPos"] = addValues(envParameter->maxPos());
						}
						else
						{
							parameterSerializeData["className"] = "Parameter";
							parameterSerializeData["configName"] = "EnvParameter";
							parameterSerializeData["configNr"] = epI;
							parameterSerializeData["dim"] = parameterDim;
							parameterSerializeData["values"] = addValues(parameter->values());
						}
						
						//********************
						// Parameter Neighbors
						//********************
                        
						int neighborGroupCount = parameter->neighbors()->neighborGroupCount();
						
						if( neighborGroupCount > 0 )
						{
							for( int nI=0; nI<neighborGroupCount; ++nI )
							{
								Json::Value neighborSerializeData;
								neighborSerializeData["className"] = "Neighbor";
								neighborSerializeData["configName"] = "Neighbor";
								neighborSerializeData["configNr"] = nI;
								
								std::string neighborSpaceName = parameter->neighborGroup( nI )->space()->name();
								
								neighborSerializeData["spaceName"] = neighborSpaceName;
								neighborSerializeData["visible"] = parameter->visible( neighborSpaceName );
								
								if( parameter->canHaveNeighbors( neighborSpaceName ) == true )
								{
									neighborSerializeData["radius"] = parameter->neighborRadius( neighborSpaceName );
									neighborSerializeData["maxNeighborCount"] = parameter->maxNeighborCount( neighborSpaceName );
									neighborSerializeData["replaceNeighborMode"] = parameter->replaceNeighborMode( neighborSpaceName );
								}
								
								parameterSerializeData["Neighbors"].append( neighborSerializeData );
							}
						}
						
						//************************
						// Parameter Communication
						//************************
						
                        std::vector< ParameterRegistration* > comInfos = Simulation::get().com().parameterRegistrations( envName, parameterName );
						int cC = comInfos.size();
						
						if( cC > 0 )
						{
							for( int cI=0; cI<cC; ++cI )
							{
								Json::Value comSerializeData;
								comSerializeData["className"] = "Communication";
								comSerializeData["configName"] = "Communication";
								comSerializeData["configNr"] = cI;
								
								ParameterRegistration* comInfo = comInfos[cI];
								
								comSerializeData["senderName"] = comInfo->senderName();
								comSerializeData["sendInterval"] = comInfo->sendInterval();
								
								if( comInfo->normalise() == true )
								{
									comSerializeData["minParValue"] = addValues(comInfo->minParValue());
									comSerializeData["maxParValue"] = addValues(comInfo->maxParValue());
								}
								
								comSerializeData["agentRange"] = addValues<int, 2>(comInfo->agentRange());
								comSerializeData["agentGroupSize"] = comInfo->agentGroupSize();
								
								parameterSerializeData["Communications"].append( comSerializeData );
							}
						}
						
						envSerializeData["Parameters"].append( parameterSerializeData );
						
					}
				}
				
				// **************
				// Env behaviors
				// **************
				
				int behaviorCount = env->behaviorCount();
				
				if( behaviorCount > 0 )
				{
					for(int ebI=0; ebI < behaviorCount; ++ebI )
					{
						Json::Value envBehaviorSerializeData;
						
						Behavior* behavior = env->behavior( ebI );
						
						const std::string& behaviorName = behavior->name();
						const std::string& behaviorClassName = behavior->className();
						
						envBehaviorSerializeData["className"] = behaviorClassName;
						envBehaviorSerializeData["configName"] = behaviorClassName;
						envBehaviorSerializeData["configNr"] = ebI;
						
						envBehaviorSerializeData["name"] = behaviorName;
						
						envBehaviorSerializeData["inputParameters"] = behavior->inputParameterString();
						envBehaviorSerializeData["outputParameters"] = behavior->outputParameterString();
						
						
						envSerializeData["Behaviors"].append(envBehaviorSerializeData);
					}
				}
				
				pSerializeData["Envs"].append(envSerializeData);
				
			}
		}
		
		// ******
		// Swarms
		// ******
		
        std::vector<Swarm*>& swarms = Simulation::get().swarms();
		int swarmCount = swarms.size();
		
		if( swarmCount > 0 )
		{
			for(int sI=0; sI<swarmCount; ++sI)
			{
				Json::Value swarmSerializeData;
				swarmSerializeData["className"] = "Swarm";
				swarmSerializeData["configName"] = "Swarm";
				swarmSerializeData["configNr"] = sI;
				
				Swarm* swarm = swarms[sI];
				const std::string& swarmName = swarm->name();
				unsigned int agentCount = swarm->agentCount();
				
				//std::cout << "serialize swarm " << swarmName.toStdString() << "\n";
				
				swarmSerializeData["name"] = swarmName;
				swarmSerializeData["agentCount"] = agentCount;
				
				// ****************
				// Swarm Parameters
				// ****************
				
				int swarmParameterCount = swarm->swarmParameterCount();
				
				if( swarmParameterCount > 0 )
				{
					for(int spI=0; spI < swarmParameterCount; ++spI )
					{
						Json::Value swarmParameterSerializeData;
						swarmParameterSerializeData["className"] = "SwarmParameter";
						swarmParameterSerializeData["configName"] = "SwarmParameter";
						swarmParameterSerializeData["configNr"] = spI;
						
						Parameter* swarmParameter = swarm->swarmParameter( spI );
						const std::string& swarmParameterName = swarmParameter->name();
						unsigned int swarmParameterDim = swarmParameter->dim();
						const Eigen::VectorXf& swarmParameterValues = swarmParameter->values();
						
						swarmParameterSerializeData["name"] = swarmParameterName;
						swarmParameterSerializeData["dim"] = swarmParameterDim;
						swarmParameterSerializeData["values"] = addValues(swarmParameterValues);
						
						//********************
						// Parameter Neighbors
						//********************
						
                        std::vector< NeighborAssignInfo* > neighborInfos = swarm->swarmNeighborInfo( swarmParameterName );
						int nC = neighborInfos.size();
						
						if( nC > 0 )
						{
							for( int nI=0; nI<nC; ++nI )
							{
								Json::Value neighborSerializeData;
								neighborSerializeData["className"] = "Neighbor";
								neighborSerializeData["configName"] = "Neighbor";
								neighborSerializeData["configNr"] = nI;
								
								NeighborAssignInfo* neighborInfo = neighborInfos[nI];
								
								neighborSerializeData["spaceName"] = neighborInfo->mSpaceName;
								neighborSerializeData["visible"] = neighborInfo->mVisible;
								
								if( neighborInfo->mNeighborGroupAlg != nullptr )
								{
									neighborSerializeData["radius"] = neighborInfo->mNeighborGroupAlg->neighborRadius();
									neighborSerializeData["maxNeighborCount"] = neighborInfo->mNeighborGroupAlg->maxNeighborCount();
									neighborSerializeData["replaceNeighborMode"] = neighborInfo->mNeighborGroupAlg->replaceNeighborMode();
								}

								swarmParameterSerializeData["Neighbors"].append( neighborSerializeData );
							}
						}
						
						//************************
						// Parameter Communication
						//************************
						
                        std::vector< ParameterRegistration* > comInfos = Simulation::get().com().parameterRegistrations( swarmName, swarmParameterName );
						int cC = comInfos.size();
						
						if( cC > 0 )
						{
							for( int cI=0; cI<cC; ++cI )
							{
								Json::Value comSerializeData;
								comSerializeData["className"] = "Communication";
								comSerializeData["configName"] = "Communication";
								comSerializeData["configNr"] = cI;
								
								ParameterRegistration* comInfo = comInfos[cI];
								
								comSerializeData["senderName"] = comInfo->senderName();
								comSerializeData["sendInterval"] = comInfo->sendInterval();
								
								if( comInfo->normalise() == true )
								{
									comSerializeData["minParValue"] = addValues(comInfo->minParValue());
									comSerializeData["maxParValue"] = addValues(comInfo->maxParValue());
								}
								
								comSerializeData["agentRange"] = addValues<int, 2>(comInfo->agentRange());
								comSerializeData["agentGroupSize"] = comInfo->agentGroupSize();
								
								swarmParameterSerializeData["Communications"].append( comSerializeData );
							}
						}
						
						swarmSerializeData["SwarmParameters"].append( swarmParameterSerializeData );
					}
				}
				
				// ***************
				// Swarm behaviors
				// ***************
				
				int swarmBehaviorCount = swarm->swarmBehaviorCount();
				
				if( swarmBehaviorCount > 0 )
				{
					for(int sbI=0; sbI < swarmBehaviorCount; ++sbI )
					{
						Json::Value swarmBehaviorSerializeData;
						
						Behavior* behavior = swarm->swarmBehavior( sbI );
						SwarmBehavior* swarmBehavior = dynamic_cast< SwarmBehavior* >( behavior );
						
						const std::string& behaviorClassName = behavior->className();
						
						swarmBehaviorSerializeData["className"] = behaviorClassName;
						
						if( swarmBehavior != nullptr )
						{
							swarmBehaviorSerializeData["configName"] = "SwarmBehavior";
						}
						else
						{
							swarmBehaviorSerializeData["configName"] = "Behavior";
						}
						
						swarmBehaviorSerializeData["configNr"] = sbI;
						
						const std::string& swarmBehaviorName = behavior->name();
						
						swarmBehaviorSerializeData["name"] = swarmBehaviorName;
						
						if( swarmBehavior != nullptr )
						{
							swarmBehaviorSerializeData["inputAgentParameters"] = swarmBehavior->inputAgentParameterString();
							swarmBehaviorSerializeData["outputAgentParameters"] = swarmBehavior->outputAgentParameterString();
							swarmBehaviorSerializeData["inputSwarmParameters"] = swarmBehavior->inputSwarmParameterString();
							swarmBehaviorSerializeData["outputSwarmParameters"] = swarmBehavior->outputSwarmParameterString();
						}
						else
						{
							swarmBehaviorSerializeData["inputParameters"] = swarmBehavior->inputParameterString();
							swarmBehaviorSerializeData["outputParameters"] = swarmBehavior->outputParameterString();
						}
						
						swarmSerializeData["SwarmBehaviors"].append( swarmBehaviorSerializeData );
					}
				}
				
				// ****************
				// Agent Parameters
				// ****************
				
				int agentParameterCount = swarm->parameterCount();
				
				if( agentParameterCount > 0 )
				{
					for(int apI=0; apI<agentParameterCount; ++apI)
					{
						Json::Value agentParameterSerializeData;
						agentParameterSerializeData["className"] = "Parameter";
						agentParameterSerializeData["configName"] = "AgentParameter";
						agentParameterSerializeData["configNr"] = apI;
						
						Parameter* parameter = static_cast<Agent*>( swarm )->parameter( apI );
						const std::string& parameterName = parameter->name();
						unsigned int parameterDim = parameter->dim();
						const Eigen::VectorXf& parameterValues = parameter->values();
						
						agentParameterSerializeData["name"] = parameterName;
						agentParameterSerializeData["dim"] = parameterDim;
						agentParameterSerializeData["values"] = addValues(parameterValues);
						
						//********************
						// Parameter Neighbors
						//********************
						
                        std::vector< NeighborAssignInfo* > neighborInfos = swarm->agentNeighborInfo( parameterName );
						int nC = neighborInfos.size();
						
						if( nC > 0 )
						{
							for( int nI=0; nI<nC; ++nI )
							{
								Json::Value neighborSerializeData;
								neighborSerializeData["className"] = "Neighbor";
								neighborSerializeData["configName"] = "Neighbor";
								neighborSerializeData["configNr"] = nI;
								
								NeighborAssignInfo* neighborInfo = neighborInfos[nI];
								
								neighborSerializeData["spaceName"] = neighborInfo->mSpaceName;
								neighborSerializeData["visible"] = neighborInfo->mVisible;
								
								if( neighborInfo->mNeighborGroupAlg != nullptr )
								{
									neighborSerializeData["radius"] = neighborInfo->mNeighborGroupAlg->neighborRadius();
									neighborSerializeData["maxNeighborCount"] = neighborInfo->mNeighborGroupAlg->maxNeighborCount();
									neighborSerializeData["replaceNeighborMode"] = neighborInfo->mNeighborGroupAlg->replaceNeighborMode();
								}
								
								agentParameterSerializeData["Neighbors"].append( neighborSerializeData );
							}
						}
						
						//************************
						// Parameter Communication
						//************************
						
                        std::vector< ParameterRegistration* > comInfos = Simulation::get().com().parameterRegistrations( swarmName, parameterName );
						int cC = comInfos.size();
						
						if( cC > 0 )
						{
							Json::Value communicationsSerializeData;
							
							for( int cI=0; cI<cC; ++cI )
							{
								Json::Value comSerializeData;
								comSerializeData["className"] = "Communication";
								comSerializeData["configName"] = "Communication";
								comSerializeData["configNr"] = cI;
								
								ParameterRegistration* comInfo = comInfos[cI];
								
								//std::cout << "cI " << cI << " swarm " << comInfo->swarmName().toStdString() << " par " << comInfo->parameterName().toStdString() << " sender " << comInfo->senderName().toStdString() << "\n";
								
								comSerializeData["senderName"] = comInfo->senderName();
								comSerializeData["sendInterval"] = comInfo->sendInterval();
								
								if( comInfo->normalise() == true )
								{
									comSerializeData["minParValue"] = addValues(comInfo->minParValue());
									comSerializeData["maxParValue"] = addValues(comInfo->maxParValue());
								}
								
								comSerializeData["agentRange"] = addValues<int, 2>(comInfo->agentRange());
								comSerializeData["agentGroupSize"] = comInfo->agentGroupSize();
								
								communicationsSerializeData[comInfo->senderName()].append( comSerializeData );
							}
							
							agentParameterSerializeData["Communications"] = communicationsSerializeData;
						}
						
						swarmSerializeData["AgentParameters"].append( agentParameterSerializeData );
					}
				}
				
				// ***************
				// Agent behaviors
				// ***************
				
				int behaviorCount = swarm->behaviorCount();
				
				if( behaviorCount > 0 )
				{
					for(int abI=0; abI < behaviorCount; ++abI )
					{
						Json::Value agentBehaviorSerializeData;
						
						Behavior* behavior = swarm->behavior( abI );
						
						const std::string& behaviorName = behavior->name();
						const std::string& behaviorClassName = behavior->className();
						
						agentBehaviorSerializeData["className"] = behaviorClassName;
						agentBehaviorSerializeData["configName"] = "AgentBehavior";
						agentBehaviorSerializeData["configNr"] = abI;
						
						agentBehaviorSerializeData["name"] = behaviorName;
						
						agentBehaviorSerializeData["inputParameters"] = behavior->inputParameterString();
						agentBehaviorSerializeData["outputParameters"] = behavior->outputParameterString();
						
						swarmSerializeData["AgentBehaviors"].append( agentBehaviorSerializeData );
					}
				}
				
				pSerializeData["Swarms"].append(swarmSerializeData);
			}
		}
        
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: Failed to serialize swarms", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::serializeVis( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
        FlockVisuals& flockVisuals = FlockVisuals::get();
        
		Json::Value visualSerializeData;
		visualSerializeData["className"] = "Visual";
		visualSerializeData["configName"] = "Visual";
		
		//*****************
		// Display Settings
		//*****************
        
		const glm::vec3& displayPosition = flockVisuals.displayPosition();
		const ofQuaternion& displayOrientation = flockVisuals.displayOrientation();
        const ofQuaternion& displayOrientationChange = flockVisuals.displayOrientationChange();
        
		float displayZoom = flockVisuals.displayZoom();
		const std::array<float,4>& displayColor = flockVisuals.displayColor();
		
		visualSerializeData["displayPosition"] = addValues(displayPosition);
		visualSerializeData["displayOrientation"] = addValues(displayOrientation);
		visualSerializeData["displayOrientationChange"] = addValues(displayOrientationChange);
		visualSerializeData["displayZoom"] = displayZoom;
		visualSerializeData["displayColor"] = addValues<float, 4>(displayColor);
        
		const std::vector< VisSwarm* >& visualSwarms = flockVisuals.visualSwarms();
		unsigned int visualSwarmCount = visualSwarms.size();
		
		if( visualSwarmCount > 0 )
		{
			//**************
			// Visual Swarms
			//**************
			
			for(int i=0; i<visualSwarmCount; ++i)
			{
				VisSwarm* visualSwarm = visualSwarms[i];
                
				Json::Value visualSwarmSerializeData;
				visualSwarmSerializeData["className"] = "VisualSwarm";
				visualSwarmSerializeData["configName"] = "VisualSwarm";
				visualSwarmSerializeData["configNr"] = i;
				visualSwarmSerializeData["swarmName"] = visualSwarm->swarmName();
				visualSwarmSerializeData["posParName"] = visualSwarm->posParmName();
				visualSwarmSerializeData["velParName"] = visualSwarm->velParName();
				visualSwarmSerializeData["agentColor"] = addValues<float, 4>(visualSwarm->agentColor());
				visualSwarmSerializeData["agentScale"] = addValues(visualSwarm->agentScale());
				visualSwarmSerializeData["agentLineWidth"] = visualSwarm->agentLineWidth();
				visualSwarmSerializeData["trailColor"] = addValues<float, 4>(visualSwarm->trailColor());
				visualSwarmSerializeData["trailLength"] = visualSwarm->trailLength();
				visualSwarmSerializeData["trailWidth"] = visualSwarm->trailWidth();
				visualSwarmSerializeData["trailDecay"] = visualSwarm->trailDecay();
				
				visualSerializeData["VisualSwarms"].append(visualSwarmSerializeData);
			}
		}
		
		const std::vector< VisNeighborSpace* >& visualNeighbors = flockVisuals.visualNeighbors();
		unsigned int visualNeighborCount = visualNeighbors.size();
		
		if( visualNeighborCount > 0 )
		{
			//*****************
			// Visual Neighbors
			//*****************
			
			for(int i=0; i<visualNeighborCount; ++i)
			{
				VisNeighborSpace* visualNeighbor = visualNeighbors[i];
                
				Json::Value visualNeighborSerializeData;
				visualNeighborSerializeData["className"] = "VisualNeighbor";
				visualNeighborSerializeData["configName"] = "VisualNeighbor";
				visualNeighborSerializeData["configNr"] = i;
				visualNeighborSerializeData["spaceName"] = visualNeighbor->spaceName();
				visualNeighborSerializeData["spaceColor"] = addValues<float, 4>(visualNeighbor->color());
				visualNeighborSerializeData["lineWidth"] = visualNeighbor->lineWidth();
                
				visualSerializeData["VisualNeighbors"].append(visualNeighborSerializeData);
			}
		}
        
		const std::vector< VisGridSpace* >& visualGrids = flockVisuals.visualGrids();
		unsigned int visualGridCount = visualGrids.size();
		
		if( visualGridCount > 0 )
		{
			//*************
			// Visual Grids
			//*************
			
			Json::Value visualGridsSerializeData;
			
			for(int i=0; i<visualGridCount; ++i)
			{
				VisGridSpace* visualGrid = visualGrids[i];
				
				Json::Value visualGridSerializeData;
				visualGridSerializeData["className"] = "VisualGrid";
				visualGridSerializeData["configName"] = "VisualGrid";
				visualGridSerializeData["configNr"] = i;
				visualGridSerializeData["spaceName"] = visualGrid->spaceName();
				visualGridSerializeData["spaceColor"] = addValues<float, 4>(visualGrid->color());
                visualGridSerializeData["lineWidth"] = visualGrid->lineWidth();
				visualGridSerializeData["valueScale"] = visualGrid->valueScale();
				
				visualSerializeData["VisualGrids"].append(visualGridSerializeData);
			}
		}
		
		pSerializeData["Visual"] = visualSerializeData;
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to serialize visualization", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::serializeValues( Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
        std::vector<Swarm*>& swarms = Simulation::get().swarms();
		int swarmCount = swarms.size();
		
		if( swarmCount > 0 )
		{
			// *****************
			// Swarms Parameters
			// *****************
			
			for(int sI=0; sI<swarmCount; ++sI)
			{
				Json::Value swarmSerializeData;
				swarmSerializeData["className"] = "Swarm";
				swarmSerializeData["configName"] = "Swarm";
				
				Swarm* swarm = swarms[sI];
				const std::string& swarmName = swarm->name();
				unsigned int agentCount = swarm->agentCount();
				
				swarmSerializeData["name"] = swarmName;
				swarmSerializeData["agentCount"] = agentCount;
				
				// ****************
				// Swarm Parameters
				// ****************
				
				int swarmParameterCount = swarm->swarmParameterCount();
				
				if( swarmParameterCount > 0 )
				{
					Json::Value swarmParametersSerializeData;
					
					int swarmParameterCount = swarm->swarmParameterCount();
					
					for(int spI=0; spI < swarmParameterCount; ++spI )
					{
						Json::Value swarmParameterSerializeData;
						swarmParameterSerializeData["className"] = "SwarmParameter";
						swarmParameterSerializeData["configName"] = "SwarmParameter";
						
						Parameter* swarmParameter = swarm->swarmParameter( spI );
						const std::string& swarmParameterName = swarmParameter->name();
						
						swarmParameterSerializeData["name"] = swarmParameterName;
						swarmParameterSerializeData["values"] = addValues(swarmParameter->values());
						
						swarmSerializeData["SwarmParameters"].append(swarmParameterSerializeData);
					}
				}
				
				// ****************
				// Agent Parameters
				// ****************
				
				int agentParameterCount = swarm->parameterCount();
				
				if( agentParameterCount > 0 )
				{
					
					Json::Value agentParametersSerializeData;
					
					int agentParameterCount = swarm->parameterCount();
					
					for(int apI=0; apI<agentParameterCount; ++apI)
					{
						Json::Value agentParameterSerializeData;
						agentParameterSerializeData["className"] = "AgentParameter";
						agentParameterSerializeData["configName"] = "AgentParameter";
						
						Parameter* agentParameter = static_cast<Agent*>( swarm )->parameter( apI );
						const std::string& agentParameterName = agentParameter->name();
						
						agentParameterSerializeData["name"] = agentParameterName;
						agentParameterSerializeData["values"] = addValues(agentParameter->values());
						
						bool saveIndividualAgentParameterValues = false;
						
						for(int aI=0; aI < agentCount; ++aI)
						{
							const Parameter* individualAgentParameter = swarm->parameter(aI,agentParameterName );
							
							if( individualAgentParameter->values() != agentParameter->values() )
							{
								saveIndividualAgentParameterValues = true;
								break;
							}
						}
						
						if( saveIndividualAgentParameterValues == true )
						{
							for(int aI=0; aI < agentCount; ++aI)
							{
								const Parameter* individualAgentParameter = swarm->parameter(aI,agentParameterName );
								agentParameterSerializeData[ "value" + std::to_string(aI) ] = addValues(individualAgentParameter->values());							}
						}
						
						swarmSerializeData["AgentParameters"].append(agentParameterSerializeData);
					}
				}
				
				pSerializeData["Swarms"].append( swarmSerializeData );
			}
		}
		
		// ***************
		// Envs Parameters
		// ***************
		
		
        std::vector<Env*>& envs = Simulation::get().envs();
		int envCount = envs.size();
		
		if( envCount > 0 )
		{
			Json::Value envsSerializeData;
			
			for(int eI=0; eI<envCount; ++eI)
			{
				Json::Value envSerializeData;
				envSerializeData["className"] = "Env";
				envSerializeData["configName"] = "Env";
				
				Env* env = envs[eI];
				const std::string& envName = env->name();
				unsigned int envDim = env->dim();
				
				envSerializeData["name"] = envName;
				envSerializeData["dim"] = envDim;
				
				// ****************
				// Env Parameters
				// ****************
				
				int parameterCount = env->parameterCount();
				
				if( parameterCount > 0 )
				{
					Json::Value envParametersSerializeData;
					
					for(int epI=0; epI < parameterCount; ++epI )
					{
						Json::Value parameterSerializeData;
						
						Parameter* parameter = env->parameter(epI);
						EnvParameter* envParameter = dynamic_cast< EnvParameter* >( parameter );
						const std::string& parameterName = parameter->name();
						unsigned int parameterDim = parameter->values().rows();
						
						parameterSerializeData["name"] = parameterName;
						
						if( envParameter != nullptr )
						{
							parameterSerializeData["className"] = "EnvParameter";
							parameterSerializeData["configName"] = "EnvParameter";
							parameterSerializeData["valueDim"] = parameterDim;
							parameterSerializeData["gridSize"] = addValues(envParameter->gridSize());
							
							space::SpaceGrid* parameterGrid = envParameter->grid();
							const math::VectorField<float>& parameterField = parameterGrid->vectorField();
							unsigned int parameterVectorCount = parameterField.vectorCount();
                            const std::vector<Eigen::VectorXf>& parameterVectors = parameterField.vectors();
                            
							for(unsigned int pvI=0; pvI < parameterVectorCount; ++pvI)
							{
                                parameterSerializeData[ "vec" + std::to_string(pvI) ] = addValues(parameterVectors[pvI]);
							}
						}
						else
						{
							parameterSerializeData["className"] = "Parameter";
							parameterSerializeData["configName"] = "EnvParameter";
							parameterSerializeData["dim"] = parameterDim;
							parameterSerializeData["values"] = addValues(parameter->values());
						}
						
						envSerializeData["Parameters"].append(parameterSerializeData);
					}                    
				}
				
				pSerializeData["Envs"].append(envSerializeData);
			}
		}
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: Failed to serialize value", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeSim( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		if( pSerializeData.isMember("Simulation") == true )
		{
            const Json::Value& simSerializeData = pSerializeData["Simulation"];
            float simRate = simSerializeData["rate"].asFloat();
			
			Simulation::get().setUpdateInterval( simRate );
		}
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to deserialize simulation", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeCom( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		if( pSerializeData.isMember("Communication") == true )
		{
			FlockCom& flockCom = Simulation::get().com();
            
			const Json::Value& comSerializeData = pSerializeData["Communication"];
			
			if( comSerializeData.isMember( "Senders" ) )
			{
				const Json::Value& sendersSerializeData = comSerializeData["Senders"];
                
				int senderCount = sendersSerializeData.size();
                
				for(int sI=0; sI<senderCount; ++sI)
				{
                    const Json::Value& senderSerial = sendersSerializeData[sI];
                    
                    std::string name = senderSerial["name"].asString();
					std::string ip = senderSerial["ip"].asString();
                    unsigned int port = senderSerial["port"].asUInt();
                    bool extOscMode = senderSerial["extOscMode"].asBool();
                    
                    //std::cout << "sender " << sI << " name " << name << " ip " << ip << " port " << port << " extOscMode " << extOscMode << "\n";
					
					if( flockCom.checkSender( name ) == false || flockCom.senderIP( name ) != ip || flockCom.senderPort( name ) != port )
					{
						if( flockCom.checkSender( name ) == true )
						{
							flockCom.removeSender( name );
						}
                        
                        flockCom.createSender(name, ip, port, extOscMode);

					}
				}
			}
			
			if( comSerializeData.isMember( "Receivers" ) )
			{
				const Json::Value& receiversSerializeData = comSerializeData["Receivers"];
				
				int receiverCount = receiversSerializeData.size();
				
				for(int rI=0; rI<receiverCount; ++rI)
				{
					const Json::Value& receiverSerial = receiversSerializeData[rI];
					
					std::string name = receiverSerial["name"].asString();
					unsigned int port = receiverSerial["port"].asUInt();
					
					if( flockCom.checkReceiver( name ) == false || flockCom.receiverPort( name ) != port )
					{
						if( flockCom.checkReceiver( name ) == true )
						{
							flockCom.removeReceiver( name );
						}
						
						//std::cout << "receiver " << rI << " name " << name << " port " << port << "\n";
						
						flockCom.createReceiver( name, port );
					}
				}
			}
		}
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: Failed to deserialize communication", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeSpace( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		if( pSerializeData.isMember( "Spaces" ) == true )
		{
			space::SpaceManager& spaceManager = space::SpaceManager::get();
			
            const Json::Value& spacesSerializeData = pSerializeData["Spaces"];
			
			unsigned int spaceCount = spacesSerializeData.size();
			
			for(int sI=0; sI<spaceCount; ++sI)
			{
				const Json::Value& spaceSerial = spacesSerializeData[sI];

				std::string name = spaceSerial["name"].asString();
				
				if(spaceManager.checkSpace(name) == false)
				{
					std::string algName = spaceSerial["algorithm"].asString();

					bool fixedSize = spaceSerial.isMember( "minPos" );
					unsigned int dim = spaceSerial["dim"].asUInt();
                    
                    Eigen::VectorXf minPosValues;
                    Eigen::VectorXf maxPosValues;
                    
					if( fixedSize == true )
					{
                        getValues(spaceSerial, "minPos", minPosValues);
                        getValues(spaceSerial, "maxPos", maxPosValues);
					}
					
					if( algName == "RTree" )
					{
						std::string closestPointTypeName = spaceSerial["closestPointType"].asString();
						space::ClosestShapePointType closestShapePointType;
						
						if( closestPointTypeName == "ClosestPointAABB" ) closestShapePointType = space::ClosestPointAABB;
						else closestShapePointType = space::ClosestPointShape;
						
						if( fixedSize == false )
						{
                            throw Exception("FLOCK ERROR: RTree algorithm must belong to a space of fixed size", __FILE__, __FUNCTION__, __LINE__);
						}
						else
						{
							space::RTreeAlg* alg = new space::RTreeAlg(minPosValues, maxPosValues);
							alg->setClosestPointType(closestShapePointType);
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, alg )));
						}
					}
					else if( algName == "Manual" )
					{
						if( fixedSize == false )
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::PermanentNeighborsAlg( dim ))));
						}
						else
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::PermanentNeighborsAlg( minPosValues, maxPosValues ))));
						}
					}
					else if( algName == "NTree" )
					{
						if( fixedSize == false )
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::NTreeAlg( dim ))));
						}
						else
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::NTreeAlg( minPosValues, maxPosValues ))));
						}
					}
					else if( algName == "KDTree" )
					{
						if( fixedSize == false )
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::KDTreeAlg( dim ))));
						}
						else
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::KDTreeAlg( minPosValues, maxPosValues))));
						}
					}
					else if( algName == "ANN" )
					{
						if( fixedSize == false )
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::ANNAlg( dim ))));
						}
						else
						{
							spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::ANNAlg( minPosValues, maxPosValues))));
						}
					}
					else if( algName == "Grid" )
					{
						unsigned int valueDim = spaceSerial["valueDim"].asUInt();
                        dab::Array<unsigned int> subdivisionCount;
                        getValues<unsigned int>(spaceSerial, "subdivisionCount", subdivisionCount);
						std::string neighborModeName = spaceSerial["neighborMode"].asString();
						space::GridAlg::GridNeighborMode neighborMode;
						
						if( neighborModeName == "CellLocationMode" ) neighborMode = space::GridAlg::CellLocationMode;
						else if( neighborModeName == "GridLocationMode" ) neighborMode = space::GridAlg::GridLocationMode;
						else if( neighborModeName == "AvgLocationMode" ) neighborMode = space::GridAlg::AvgLocationMode;
						else if( neighborModeName == "AvgRegionMode" ) neighborMode = space::GridAlg::AvgRegionMode;
						
						std::string updateModeName = spaceSerial["updateMode"].asString();
						space::GridAlg::GridUpdateMode updateMode;
						
						if( updateModeName == "NoUpdateMode" ) updateMode = space::GridAlg::NoUpdateMode;
						else if( updateModeName == "NearestReplaceMode" ) updateMode = space::GridAlg::NearestReplaceMode;
						else if( updateModeName == "NearestAddMode" ) updateMode = space::GridAlg::NearestAddMode;
						else if( updateModeName == "AvgReplaceMode" ) updateMode = space::GridAlg::AvgReplaceMode;
						else if( updateModeName == "AvgAddMode" ) updateMode = space::GridAlg::AvgAddMode;
						
						spaceManager.addSpace( std::shared_ptr<space::Space>(new space::Space( name, new space::GridAlg( valueDim, subdivisionCount, minPosValues, maxPosValues, neighborMode, updateMode ))));

					}
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to deserialize space", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeSwarms( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		//std::cout << "SerializeTools::deSerializeSwarms( serialize::SerializeData& pSerializeData ) begin\n";
		
		Simulation& simulation = Simulation::get();
		FlockCom& flockCom = simulation.com();
		
		// **********
		// Parameters
		// **********
		
		if( pSerializeData.isMember("Envs") == true )
		{
			
			// ****
			// Envs
			// ****
			
			const Json::Value& envsSerializeData = pSerializeData["Envs"];
			unsigned int envCount = envsSerializeData.size();
			
			for( int eI =0; eI < envCount; ++eI )
			{
				const Json::Value& envSerial = envsSerializeData[eI];
				
				std::string envName = envSerial["name"].asString();
				unsigned int envDim = envSerial["dim"].asUInt();
				
				Env* env = new Env( envName, envDim );
				
				if( envSerial.isMember("Parameters") == true )
				{
					
					// ****************
					// Env Parameters
					// ****************
					
					const Json::Value& envParametersSerialData = envSerial["Parameters"];
					unsigned int envParameterCount = envParametersSerialData.size();
					
					for( int epI =0; epI < envParameterCount; ++epI )
					{
						const Json::Value& parameterSerial = envParametersSerialData[epI];
						
						std::string parameterName = parameterSerial["name"].asString();
						unsigned int parameterDim;
                        
						if( parameterSerial.isMember("gridSize") )
						{
							if( env->checkParameter( parameterName ) == false )
							{
								unsigned int valueDim = parameterSerial["valueDim"].asUInt();
								parameterDim = valueDim;
								unsigned int gridDim;
                                dab::Array<unsigned int> subDivisionCount;
                                Eigen::VectorXf minPos;
                                Eigen::VectorXf maxPos;
                                
                                getValues<unsigned int>(parameterSerial, "gridSize", subDivisionCount);
                                getValues(parameterSerial, "minPos", minPos);
                                getValues(parameterSerial, "maxPos", maxPos);

								env->addParameter( parameterName, valueDim, subDivisionCount, minPos, maxPos );

							}
						}
						else
						{
							parameterDim = parameterSerial["dim"].asUInt();
                            Eigen::VectorXf parameterValues;
                            
                            getValues(parameterSerial, "values", parameterValues);
							
							if( env->checkParameter( parameterName ) == false )
							{
								env->addParameter( parameterName, parameterValues );
							}
							else
							{
								env->set( parameterName, parameterValues );
							}
						}
						
						if( parameterSerial.isMember("Neighbors") == true )
						{
							
							// ************************
							// Env Parameters Neighbors
							// ************************
							
							const Json::Value& neighborSerialData = parameterSerial["Neighbors"];
							unsigned int envNeighborCount = neighborSerialData.size();
							
							for( int enI =0; enI < envNeighborCount; ++enI )
							{
								const Json::Value& neighborSerial = neighborSerialData[enI];
								
								std::string spaceName = neighborSerial["spaceName"].asString();
								
								if( env->parameter( parameterName )->checkNeighborGroup( spaceName ) == false )
								{
									bool visible = neighborSerial["visible"].asBool();
									space::NeighborGroupAlg* neighborAlg = nullptr;
									
									if( neighborSerial.isMember("radius") )
									{
										float radius = neighborSerial["radius"].asFloat();
										unsigned int maxNeighborCount = neighborSerial["maxNeighborCount"].asUInt();
										bool replaceNeighborMode = neighborSerial["replaceNeighborMode"].asBool();
										
										neighborAlg = new space::NeighborGroupAlg( radius, maxNeighborCount, replaceNeighborMode );
									}
									
									env->assignNeighbors( parameterName, spaceName, visible, neighborAlg );
								}
							}
						}
						
						if( parameterSerial.isMember("Communications") == true )
						{
							
							//****************************
							// Env Parameter Communication
							//****************************
							
							const Json::Value& communicationSerialData = parameterSerial["Communications"];
							unsigned int envCommunicationCount = communicationSerialData.size();
							
							for( int ecI =0; ecI < envCommunicationCount; ++ecI )
							{
								const Json::Value& communicationSerial = communicationSerialData[ecI];
								
								std::string senderName = communicationSerial["senderName"].asString();
								
								if( flockCom.checkParameter( senderName, envName, parameterName ) == false )
								{
                                    std::array<int, 2> agentRange;
                                    getValues(communicationSerial, "agentRange", agentRange);
                                    
									
									int sendInterval = 1;
									if( communicationSerial.isMember("sendInterval") )
									{
										sendInterval = communicationSerial["sendInterval"].asInt();
									}
									
									int agentGroupSize = 1;
									if( communicationSerial.isMember("agentGroupSize") == true )
									{
										agentGroupSize = communicationSerial["agentGroupSize"].asInt();
									}
									
									if( communicationSerial.isMember("minParValue") == true )
									{
                                        Eigen::VectorXf minParValue;
                                        Eigen::VectorXf maxParValue;
                                        
                                        getValues(communicationSerial, "minParValue", minParValue);
                                        getValues(communicationSerial, "maxParValue", maxParValue);
										
										flockCom.registerParameter(senderName, sendInterval, envName, parameterName, agentRange, agentGroupSize, minParValue, maxParValue );
									}
									else
									{
										flockCom.registerParameter(senderName, sendInterval, envName, parameterName, agentRange, agentGroupSize );
									}
								}
							}
						}
					}
				}
			}
		}
		
		if( pSerializeData.isMember("Swarms") == true )
		{
			// ******
			// Swarms
			// ******
			
			const Json::Value& swarmsSerialData = pSerializeData["Swarms"];
			
			unsigned int swarmCount = swarmsSerialData.size();
			
			for(int sI=0; sI<swarmCount; ++sI)
			{
				const Json::Value& swarmSerial = swarmsSerialData[sI];
				
				std::string swarmName = swarmSerial["name"].asString();
				unsigned int agentCount = swarmSerial["agentCount"].asUInt();
				
				Swarm* swarm;
				if( simulation.checkSwarm( swarmName ) == false ) swarm = new Swarm( swarmName );
				else swarm = simulation.swarm( swarmName );
				
				if( swarmSerial.isMember("SwarmParameters") == true )
				{
					// ****************
					// Swarm Parameters
					// ****************
					
					const Json::Value& swarmParametersSerialData = swarmSerial["SwarmParameters"];
					unsigned int swarmParameterCount = swarmParametersSerialData.size();
					
					for( int spI =0; spI < swarmParameterCount; ++spI )
					{
						const Json::Value& parameterSerial = swarmParametersSerialData[spI];
						
						std::string parameterName = parameterSerial["name"].asString();
                        unsigned int parameterDim = parameterSerial["dim"].asUInt();
                        
                        Eigen::VectorXf parameterValues;
                        getValues(parameterSerial, "values", parameterValues);
                        
						if( swarm->checkSwarmParameter( parameterName ) == false )
						{
							swarm->addSwarmParameter( parameterName, parameterValues );
						}
						else
						{
							swarm->set( parameterName, parameterValues );
						}
						
						if( parameterSerial.isMember("Neighbors") == true )
						{
							
							// **************************
							// Swarm Parameters Neighbors
							// **************************
							
							const Json::Value& neighborsSerialData = parameterSerial["Neighbors"];
							unsigned int neighborCount = neighborsSerialData.size();
							
							for( int nI =0; nI < neighborCount; ++nI )
							{
								const Json::Value& neighborSerial = neighborsSerialData[nI];
								
								std::string spaceName = neighborSerial["spaceName"].asString();
								
								if( swarm->swarmParameter( parameterName )->checkNeighborGroup( spaceName ) == false )
								{
									bool visible = neighborSerial["visible"].asBool();
									space::NeighborGroupAlg* neighborAlg = nullptr;
									
									if( neighborSerial.isMember("radius") )
									{
										float radius = neighborSerial["radius"].asFloat();
										unsigned int maxNeighborCount = neighborSerial["maxNeighborCount"].asUInt();
										bool replaceNeighborMode = neighborSerial["replaceNeighborMode"].asBool();
										
										neighborAlg = new space::NeighborGroupAlg( radius, maxNeighborCount, replaceNeighborMode );
									}
									
									swarm->assignNeighbors( parameterName, spaceName, visible, neighborAlg );
								}
							}
							
						}
						
						if( parameterSerial.isMember("Communications") == true )
						{
							
							//************************
							// Parameter Communication
							//************************
							
							const Json::Value communicationsSerialData = parameterSerial["Communications"];
							unsigned int communicationCount = communicationsSerialData.size();
							
							for( int cI =0; cI < communicationCount; ++cI )
							{
								const Json::Value& communicationSerial = communicationsSerialData[cI];
								
								std::string senderName = communicationSerial["senderName"].asString();
								
								if( flockCom.checkParameter( senderName, swarmName, parameterName ) == false )
								{
                                    std::array<int, 2> agentRange;
                                    getValues(communicationSerial, "agentRange", agentRange);
									
									int sendInterval = 1;
									if( communicationSerial.isMember("sendInterval") )
									{
										sendInterval = communicationSerial["sendInterval"].asInt();
									}
									
									int agentGroupSize = 1;
									
									if( communicationSerial.isMember("agentGroupSize") == true )
									{
										agentGroupSize = communicationSerial["agentGroupSize"].asInt();
									}
									
									if( communicationSerial.isMember("minParValue") == true )
									{
                                        Eigen::VectorXf minParValue;
                                        Eigen::VectorXf maxParValue;
                                        
                                        getValues(communicationSerial, "minParValue", minParValue);
                                        getValues(communicationSerial, "maxParValue", maxParValue);
										
										flockCom.registerParameter(senderName, sendInterval, swarmName, parameterName, agentRange, agentGroupSize, minParValue, maxParValue );
									}
									else
									{
										flockCom.registerParameter(senderName, sendInterval, swarmName, parameterName, agentRange, agentGroupSize );
									}
								}
							}
						}
					}
				}
				
				if( swarmSerial.isMember("AgentParameters") == true )
				{
					
					// ****************
					// Agent Parameters
					// ****************
					
					const Json::Value& agentParametersSerialData = swarmSerial["AgentParameters"];
					unsigned int agentParameterCount = agentParametersSerialData.size();
					
					for( int apI =0; apI < agentParameterCount; ++apI )
					{
						const Json::Value& parameterSerial = agentParametersSerialData[apI];
						
						std::string parameterName = parameterSerial["name"].asString();
						unsigned int parameterDim = parameterSerial["dim"].asInt();
						
						//std::cout << "agent parameter swarmName " << swarmName.toStdString() << " parameterName " << parameterName.toStdString() << " parameterDim " << parameterDim << "\n";
						
                        Eigen::VectorXf parameterValues;
                        getValues(parameterSerial, "values", parameterValues);
						
						if( swarm->checkParameter( parameterName ) == false )
						{
							swarm->addParameter( parameterName, parameterValues );
						}
						else
						{
							swarm->set( parameterName, parameterValues );
						}
						
						if( parameterSerial.isMember("Neighbors") == true )
						{
							
							// **************************
							// Agent Parameters Neighbors
							// **************************
							
							const Json::Value& neighborsSerialData = parameterSerial["Neighbors"];
							unsigned int neighborCount = neighborsSerialData.size();
							
							for( int nI =0; nI < neighborCount; ++nI )
							{
								const Json::Value& neighborSerial = neighborsSerialData[nI];
								
								std::string spaceName = neighborSerial["spaceName"].asString();
								
								if( static_cast<Agent*>( swarm )->parameter( parameterName )->checkNeighborGroup( spaceName ) == false )
								{
									bool visible = neighborSerial["visible"].asBool();
									space::NeighborGroupAlg* neighborAlg = nullptr;
									
									if( neighborSerial.isMember("radius") )
									{
										float radius = neighborSerial["radius"].asFloat();
										unsigned int maxNeighborCount = neighborSerial["maxNeighborCount"].asUInt();
										bool replaceNeighborMode = neighborSerial["replaceNeighborMode"].asBool();
										
										neighborAlg = new space::NeighborGroupAlg( radius, maxNeighborCount, replaceNeighborMode );
									}
									
									swarm->assignNeighbors( parameterName, spaceName, visible, neighborAlg );
								}
							}
						}
						
						if( parameterSerial.isMember("Communications") == true )
						{
							
							//******************************
							// Agent Parameter Communication
							//******************************
							
							const Json::Value& communicationSerialData = parameterSerial["Communications"];
							unsigned int communicationCount = communicationSerialData.size();
							
							for( int cI =0; cI < communicationCount; ++cI )
							{
								const Json::Value& communicationSerial = communicationSerialData[cI];
								
								std::string senderName = communicationSerial["senderName"].asString();
								
								if( flockCom.checkParameter( senderName, swarmName, parameterName ) == false )
								{
                                    std::array<int, 2> agentRange;
                                    getValues(communicationSerial, "agentRange", agentRange);
									
									int sendInterval = 1;
									if( communicationSerial.isMember("sendInterval") )
									{
										sendInterval = communicationSerial["sendInterval"].asInt();
									}
									
									int agentGroupSize = 1;
									
									if( communicationSerial.isMember("agentGroupSize") == true )
									{
										agentGroupSize = communicationSerial["agentGroupSize"].asInt();
									}
									
									if( communicationSerial.isMember("minParValue") == true )
									{
                                        Eigen::VectorXf minParValue;
                                        Eigen::VectorXf maxParValue;
                                        
                                        getValues(communicationSerial, "minParValue", minParValue);
                                        getValues(communicationSerial, "maxParValue", maxParValue);
                                        
										flockCom.registerParameter(senderName, sendInterval, swarmName, parameterName, agentRange, agentGroupSize, minParValue, maxParValue );
									}
									else
									{
										flockCom.registerParameter(senderName, sendInterval, swarmName, parameterName, agentRange, agentGroupSize );
									}
								}
							}
						}
					}
				}
				
				//swarm->addAgents( agentCount );
			}
		}
		
		//**********
		// Behaviors
		//**********
		
		if( pSerializeData.isMember("Envs") == true )
		{
			
			// ****
			// Envs
			// ****
			
			const Json::Value& envsSerializeData = pSerializeData["Envs"];
			unsigned int envCount = envsSerializeData.size();
            
			for( int eI =0; eI < envCount; ++eI )
			{
				const Json::Value& envSerial = envsSerializeData[eI];
				
				std::string envName = envSerial["name"].asString();
				unsigned int envDim = envSerial["dim"].asUInt();
				
				Env* env = simulation.env( envName );
				
				if( envSerial.isMember("Behaviors") == true )
				{
					
					// *************
					// Env behaviors
					// *************
					
					const Json::Value& envBehaviorsSerialData = envSerial["Behaviors"];
					unsigned int envBehaviorCount = envBehaviorsSerialData.size();
					
					for( int ebI =0; ebI < envBehaviorCount; ++ebI )
					{
						const Json::Value& behaviorSerial = envBehaviorsSerialData[ebI];
						
						std::string behaviorName = behaviorSerial["name"].asString();
						
						if( env->checkBehavior(behaviorName) == false )
						{
							std::string behaviorClass = behaviorSerial["className"].asString();
							std::string inputParameterString = behaviorSerial["inputParameters"].asString();
                            std::string outputParameterString = behaviorSerial["outputParameters"].asString();
							
                            if(mBehaviorMap.find(behaviorClass) == mBehaviorMap.end()) throw Exception("FLOCK ERROR: behavior class " + behaviorClass + " cannot be deserialized", __FILE__, __FUNCTION__, __LINE__);
                            
							Behavior* behavior = mBehaviorMap[behaviorClass]->create( inputParameterString, outputParameterString  );
							
							env->addBehavior( behaviorName, *behavior );
							
							delete behavior;
						}
					}
				}
			}
		}
		
		if( pSerializeData.isMember("Swarms") == true )
		{
			// ******
			// Swarms
			// ******
			
			const Json::Value& swarmsSerialData = pSerializeData["Swarms"];
			unsigned int swarmCount = swarmsSerialData.size();
			
			for(int sI=0; sI<swarmCount; ++sI)
			{
				const Json::Value& swarmSerial = swarmsSerialData[sI];
				
				std::string swarmName = swarmSerial["name"].asString();
				unsigned int agentCount = swarmSerial["agentCount"].asUInt();
				
				Swarm* swarm = simulation.swarm( swarmName );
				
				if( swarmSerial.isMember("SwarmBehaviors") == true )
				{
					
					// ***************
					// Swarm behaviors
					// ***************
					
					const Json::Value& swarmBehaviorsSerialData = swarmSerial["SwarmBehaviors"];
					unsigned int behaviorCount = swarmBehaviorsSerialData.size();
					
					for( int bI =0; bI < behaviorCount; ++bI )
					{
						const Json::Value& behaviorSerial = swarmBehaviorsSerialData[bI];
						
						std::string behaviorName = behaviorSerial["name"].asString();
						
						if( swarm->checkSwarmBehavior( behaviorName ) == false )
						{
							
							std::string behaviorClass = behaviorSerial["className"].asString();
							std::string behaviorType = behaviorSerial["configName"].asString();
							
							if( behaviorType == "SwarmBehavior" )
							{
								std::string inputAgentParameterString = behaviorSerial["inputAgentParameters"].asString();
								std::string outputAgentParameterString = behaviorSerial["outputAgentParameters"].asString();
								std::string inputSwarmParameterString = behaviorSerial["inputSwarmParameters"].asString();
								std::string outputSwarmParameterString = behaviorSerial["outputSwarmParameters"].asString();
								
								//std::cout << "swarmBehaviorMapSize: " << mSwarmBehaviorMap.size() << "\n";
								//std::cout << "behaviorMapSize: " << mBehaviorMap.size() << "\n";
								
								//std::cout << "behaviorClass " << behaviorClass.toStdString() << " behavior " << mSwarmBehaviorMap.value( behaviorClass ) << "\n";
								
								//std::cout << "behavior className " << mSwarmBehaviorMap.value( behaviorClass )->className().toStdString() << "\n";
								
                                if(mBehaviorMap.find(behaviorClass) == mBehaviorMap.end()) throw Exception("FLOCK ERROR: behavior class " + behaviorClass + " cannot be deserialized", __FILE__, __FUNCTION__, __LINE__);
                                
								Behavior* behavior = mSwarmBehaviorMap[behaviorClass]->create( inputSwarmParameterString, inputAgentParameterString, outputSwarmParameterString, outputAgentParameterString  );
								
								swarm->addSwarmBehavior( behaviorName, *behavior );
								
								delete behavior;
								
							}
							else
							{
								std::string inputParameterString = behaviorSerial["inputParameters"].asString();
								std::string outputParameterString = behaviorSerial["outputParameters"].asString();
								
                                if(mBehaviorMap.find(behaviorClass) == mBehaviorMap.end()) throw Exception("FLOCK ERROR: behavior class " + behaviorClass + " cannot be deserialized", __FILE__, __FUNCTION__, __LINE__);
                                
								Behavior* behavior = mBehaviorMap[behaviorClass]->create( inputParameterString, outputParameterString  );
								
								swarm->addSwarmBehavior( behaviorName, *behavior );
								
								delete behavior;
							}
						}
					}
				}
				
				if( swarmSerial.isMember("AgentBehaviors") == true )
				{
					
					// ***************
					// Agent behaviors
					// ***************
					
					const Json::Value& agentBehaviorsSerialData = swarmSerial["AgentBehaviors"];
					unsigned int agentBehaviorCount = agentBehaviorsSerialData.size();

					for( int abI =0; abI < agentBehaviorCount; ++abI )
					{
						const Json::Value& behaviorSerial = agentBehaviorsSerialData[abI];
						
						std::string behaviorName = behaviorSerial["name"].asString();
						
						if( static_cast<Agent*>(swarm)->checkBehavior(behaviorName) == false )
						{
							
							std::string behaviorClass = behaviorSerial["className"].asString();
							std::string inputParameterString = behaviorSerial["inputParameters"].asString();
							std::string outputParameterString = behaviorSerial["outputParameters"].asString();
                            
                            if(mBehaviorMap.find(behaviorClass) == mBehaviorMap.end()) throw Exception("FLOCK ERROR: behavior class " + behaviorClass + " cannot be deserialized", __FILE__, __FUNCTION__, __LINE__);
							
							Behavior* behavior = mBehaviorMap[behaviorClass]->create( inputParameterString, outputParameterString  );
							
							swarm->addBehavior( behaviorName, *behavior );
							
							delete behavior;
						}
					}
				}
				
				swarm->addAgents( agentCount );
			}
		}
		
		//std::cout << "SerializeTools::deSerializeSwarms( serialize::SerializeData& pSerializeData ) end\n";
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to deserialize swarms", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeVis( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		//std::cout << "SerializeTools::deSerializeVis( serialize::SerializeData& pSerializeData ) begin\n";
		
		if( pSerializeData.isMember("Visual") == true )
		{
			FlockVisuals& flockVisuals = FlockVisuals::get();
			
			const Json::Value& visSerializeData = pSerializeData["Visual"];
			
			//*****************
			// Display Settings
			//*****************
			
			//std::cout << "set display settings begin\n";
			
			unsigned int dim;
            
            glm::vec3 displayPosition;
            glm::quat displayOrientation;
            
            getValues(visSerializeData, "displayPosition", displayPosition);
            getValues(visSerializeData, "displayOrientation", displayOrientation);
			
			float displayZoom = visSerializeData["displayZoom"].asFloat();
            
            std::array<float, 4> displayColor;

            getValues(visSerializeData, "displayColor", displayColor);
            
			flockVisuals.setDisplayPosition( displayPosition );
			flockVisuals.setDisplayOrientation( displayOrientation );
			flockVisuals.setDisplayZoom( displayZoom );
			flockVisuals.setDisplayColor( displayColor );
			
			//std::cout << "set display settings end\n";
			
			if( visSerializeData.isMember("VisualSwarms") == true )
			{
				//**************
				// Visual Swarms
				//**************
				
				//std::cout << "show swarms begin\n";
				
				const Json::Value& visSwarmsSerializeData = visSerializeData["VisualSwarms"];
				unsigned int visSwarmCount = visSwarmsSerializeData.size();
				
				for( int vsI =0; vsI < visSwarmCount; ++vsI )
				{
					const Json::Value& visSwarmSerial = visSwarmsSerializeData[vsI];
					
					std::string swarmName = visSwarmSerial["swarmName"].asString();
					std::string posParName = visSwarmSerial["posParName"].asString();
					std::string velParName = visSwarmSerial["velParName"].asString();
                    
                    std::array<float, 4> agentColor;
                    glm::vec3 agentScale;
                    
                    getValues(visSwarmSerial, "agentColor", agentColor);
                    getValues(visSwarmSerial, "agentScale", agentScale);

                    float agentLineWidth = visSwarmSerial["agentLineWidth"].asFloat();
                    
                    std::array<float,4> trailColor;
                    
                    getValues(visSwarmSerial, "trailColor", trailColor);
                    
                    unsigned int trailLength = visSwarmSerial["trailLength"].asUInt();
					float trailWidth = visSwarmSerial["trailWidth"].asFloat();
					float trailDecay = visSwarmSerial["trailDecay"].asFloat();
					
					//std::cout << "show swarm " <<  swarmName.toStdString() << " begin\n";
					
                    //const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, unsigned int pMaxTrailLength
                    
					flockVisuals.showSwarm( swarmName, posParName, velParName );
                    
                    VisSwarm* visSwarm = flockVisuals.visualSwarm(swarmName);
                    
                    visSwarm->setAgentColor(agentColor);
                    visSwarm->setAgentScale(agentScale);
                    visSwarm->setAgentLineWidth(agentLineWidth);
                    visSwarm->setTrailColor(trailColor);
                    visSwarm->setTrailLength(trailLength);
                    visSwarm->setTrailWidth(trailWidth);
                    visSwarm->setTrailDecay(trailDecay);
                    
					//std::cout << "show swarm " <<  swarmName.toStdString() << " end\n";
				}
				
				//std::cout << "show swarms end\n";
			}
			
			if( visSerializeData.isMember("VisualNeighbors") == true )
			{
				//*****************
				// Visual Neighbors
				//*****************
				
				const Json::Value& visNeighborsSerializeData = visSerializeData["VisualNeighbors"];
				unsigned int visNeighborCount = visNeighborsSerializeData.size();
				
				for( int vnI =0; vnI < visNeighborCount; ++vnI )
				{
					const Json::Value& visNeighborSerial = visNeighborsSerializeData[vnI];
					
					std::string spaceName = visNeighborSerial["spaceName"].asString();
                    std::array<float, 4> spaceColor;
                    
                    getValues(visNeighborSerial, "spaceColor", spaceColor);
                    
                    float lineWidth = visNeighborSerial["lineWidth"].asFloat();
					
					flockVisuals.showSpace(spaceName);
                    flockVisuals.setSpaceColor(spaceName, spaceColor);
                    flockVisuals.setSpaceLineWidth(spaceName, lineWidth);
				}
			}
			
			if( visSerializeData.isMember("VisualGrids") == true )
			{
				//*************
				// Visual Grids
				//*************
				
				const Json::Value visGridsSerializeData = visSerializeData["VisualGrids"];
				unsigned int visGridCount = visGridsSerializeData.size();
				
				for( int vgI =0; vgI < visGridCount; ++vgI )
				{
					const Json::Value& visGridSerial = visGridsSerializeData[vgI];
					
					std::string spaceName = visGridSerial["spaceName"].asString();
                    std::array<float, 4> spaceColor;
                    getValues(visGridSerial, "spaceColor", spaceColor);
					float valueScale = visGridSerial["valueScale"].asFloat();
                    float lineWidth = visGridSerial["lineWidth"].asFloat();
					
					flockVisuals.showSpace(spaceName, spaceColor, valueScale);
                    flockVisuals.setSpaceLineWidth(spaceName, lineWidth);
				}
			}
		}
		
		//std::cout << "SerializeTools::deSerializeVis( serialize::SerializeData& pSerializeData ) end\n";
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to deserialize visualization", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
SerializeTools::deSerializeValues( const Json::Value& pSerializeData ) throw (Exception)
{
	try
	{
		//std::cout << "SerializeTools::deSerializeValues() begin\n";
		
		Simulation& simulation = Simulation::get();
        
		// ******
		// Swarms
		// ******
		
		if( pSerializeData.isMember("Swarms") == true )
		{
			const Json::Value& swarmsSerialData = pSerializeData["Swarms"];
			unsigned int swarmCount = swarmsSerialData.size();
			
            //			std::cout << "swarmCount " << swarmCount << "\n";
			
			for(int sI=0; sI<swarmCount; ++sI)
			{
				// *****
				// Swarm
				// *****
				
				const Json::Value& swarmSerialData = swarmsSerialData[sI];
				
				std::string swarmName = swarmSerialData["name"].asString();
				unsigned int agentCount = swarmSerialData["agentCount"].asUInt();
				
				Swarm* swarm = simulation.swarm( swarmName );
				
				//std::cout << "swarmName " << swarmName.toStdString() << " current agent count " << swarm->agentCount() << " values agentCount " << agentCount << "\n";
				
				if( swarm->agentCount() < agentCount ) swarm->addAgents( agentCount - swarm->agentCount() );
				else if( swarm->agentCount() > agentCount ) swarm->removeAgents( swarm->agentCount() - agentCount );
				
				//*****************
				// Swarm Parameters
				//*****************
				
				if( swarmSerialData.isMember("SwarmParameters") )
				{
					const Json::Value& swarmParametersSerialData = swarmSerialData["SwarmParameters"];
					int swarmParameterCount = swarmParametersSerialData.size();
					
					for(int spI=0; spI<swarmParameterCount; ++spI)
					{
						const Json::Value& swarmParameterSerialData = swarmParametersSerialData[spI];
						
						std::string swarmParameterName = swarmParameterSerialData["name"].asString();
						
                        //						std::cout << "swarmParameterName " << swarmParameterName.toStdString() << "\n";
						
						if( swarm->checkSwarmParameter(swarmParameterName) == true )
						{
                            Eigen::VectorXf swarmParameterValues;
                            getValues(swarmParameterSerialData, "values", swarmParameterValues);
							swarm->set( swarmParameterName, swarmParameterValues );
						}
					}
				}
				
				//*****************
				// Agent Parameters
				//*****************
				
				if( swarmSerialData.isMember("AgentParameters") )
				{
					const Json::Value& agentParametersSerialData = swarmSerialData["AgentParameters"];
					int agentParameterCount = agentParametersSerialData.size();
					
					for(int apI=0; apI<agentParameterCount; ++apI)
					{
						const Json::Value& agentParameterSerialData = agentParametersSerialData[apI];
						
						std::string agentParameterName = agentParameterSerialData["name"].asString();
						
                        //						std::cout << "agentParameterName " << agentParameterName.toStdString() << "\n";
						
						if( swarm->checkParameter(agentParameterName) == true )
						{
                            Eigen::VectorXf agentParameterValues;
                            getValues(agentParameterSerialData, "values", agentParameterValues);

							swarm->set( agentParameterName, agentParameterValues );
						}
						
						if( agentParameterSerialData.isMember("value0") )
						{
							for(unsigned int aI=0; aI < agentCount; ++aI)
							{
                                Eigen::VectorXf agentParameterValues;
                                getValues(agentParameterSerialData, "value" + std::to_string(aI), agentParameterValues);
                                
								swarm->set( aI, agentParameterName, agentParameterValues );
								
                                //								std::cout << "set agentNr " << aI << " values " << math::Vector<real>( agentParameterDim, agentParameterValues ) << "\n";
							}
                            
						}
					}
				}
			}
		}
        
		
		// ****
		// Envs
		// ****
		
		// TODO: untested!!
		
		if( pSerializeData.isMember("Envs") == true )
		{
			const Json::Value& envsSerialData = pSerializeData["Envs"].asString();
			unsigned int envCount = envsSerialData.size();
			
			//std::cout << "envCount " << envCount << "\n";
			
			for(int eI=0; eI<envCount; ++eI)
			{
				// ***
				// Env
				// ***
				
				const Json::Value& envSerialData = envsSerialData[eI];
				
				std::string envName = envSerialData["name"].asString();
				unsigned int envDim = envSerialData["dim"].asUInt();
				
				//std::cout << "envName " << envName.toStdString() << " envDim " << envDim << "\n";
				
				Env* env = simulation.env( envName );
				
				//***************
				// Env Parameters
				//***************
				
				if( envSerialData.isMember("Parameters") )
				{
					const Json::Value& envParametersSerialData = envSerialData["Parameters"];
					int envParameterCount = envParametersSerialData.size();
					
					//std::cout << "envParameterCount " << envParameterCount << "\n";
					
					for(int epI=0; epI<envParameterCount; ++epI)
					{
						const Json::Value& envParameterSerialData = envParametersSerialData[epI];
						
						std::string envParameterName = envParameterSerialData["name"].asString();
						std::string envParameterType = envParameterSerialData["className"].asString();
						
						//std::cout << "envParameterName " << envParameterName.toStdString() << " type " << envParameterType.toStdString() << "\n";
						
						if( env->checkParameter(envParameterName) == true )
						{
							if( envParameterType == "Parameter" )
							{
                                Eigen::VectorXf envParameterValues;
                                getValues(envParameterSerialData, "values", envParameterValues);
                                
								env->set( envParameterName, envParameterValues );
								
								//std::cout << "set values " << math::Vector<real>( envParameterDim, envParameterValues ) << "\n";
							}
							else if( envParameterType == "EnvParameter" )
							{
								unsigned int envParameterValueDim = envParameterSerialData["valueDim"].asUInt();
                                dab::Array<unsigned int> gridSize;
                                getValues(envParameterSerialData, "gridSize", gridSize);
                                
								math::VectorField<float> envParameterField( gridSize, Eigen::VectorXf(envParameterValueDim) );
								
								unsigned int envParameterVectorCount = envParameterField.vectorCount();
                                std::vector<Eigen::VectorXf>& envParameterVectors = envParameterField.vectors();
								
								for(unsigned int pvI=0; pvI<envParameterVectorCount; ++pvI)
								{
                                    Eigen::VectorXf envVectorValues;
                                    getValues(envParameterSerialData, "vec" + std::to_string(pvI), envVectorValues);
									
									envParameterVectors[pvI] = envVectorValues;
								}
								
								//std::cout << "envParameterField " << envParameterField << "\n";
								
								env->set( envParameterName, envParameterField );
							}
						}
					}
				}
			}
		}
		
		//std::cout << "SerializeTools::deSerializeValues() end\n";
	}
	catch (Exception& e)
	{
        e += Exception("FLOCK ERROR: failed to deserialize values", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}


const Json::Value&
SerializeTools::getValue( const Json::Value& pValue, const std::string& pValueName ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName];
}

int
SerializeTools::getInt( const Json::Value& pValue, const std::string& pValueName ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].isInt() == false) throw Exception("JSON ERROR: value name " + pValueName + " is not an integer", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName].asInt();
}

float
SerializeTools::getFloat( const Json::Value& pValue, const std::string& pValueName ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].isDouble() == false) throw Exception("JSON ERROR: value name " + pValueName + " is not a float", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName].asFloat();
}

std::string
SerializeTools::getString( const Json::Value& pValue, const std::string& pValueName ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].isString() == false) throw Exception("JSON ERROR: value name " + pValueName + " is not a float", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName].asString();
}

const Json::Value&
SerializeTools::getValue( const Json::Value& pValue, int pValueIndex ) throw (Exception)
{
    if(pValue.size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue.size()), __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueIndex];
}

int
SerializeTools::getInt( const Json::Value& pValue, unsigned int pValueIndex ) throw (Exception)
{
    if(pValue.size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue.size()), __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueIndex].isInt() == false) throw Exception("JSON ERROR: value at index " + std::to_string(pValueIndex) + " is not an integer", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueIndex].asInt();
}

float
SerializeTools::getFloat( const Json::Value& pValue, unsigned int pValueIndex  ) throw (Exception)
{
    if(pValue.size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue.size()), __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueIndex].isDouble() == false) throw Exception("JSON ERROR: value at index " + std::to_string(pValueIndex) + " is not a float", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueIndex].asFloat();
}

std::string
SerializeTools::getString( const Json::Value& pValue, unsigned int pValueIndex  ) throw (Exception)
{
    if(pValue.size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue.size()), __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueIndex].isString() == false) throw Exception("JSON ERROR: value at index " + std::to_string(pValueIndex) + " is not a string", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueIndex].asString();
}


const Json::Value&
SerializeTools::getValue( const Json::Value& pValue, const std::string& pValueName, int pValueIndex ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue[pValueName].size()) + " of value name: " + pValueName + " ", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName][pValueIndex];
}

int
SerializeTools::getInt( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue[pValueName].size()) + " of value name: " + pValueName + " ", __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName][pValueIndex].isInt() == false) throw Exception("JSON ERROR: value name " + pValueName + " at index " + std::to_string(pValueIndex) + " is not an integer", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName][pValueIndex].asInt();
}

float
SerializeTools::getFloat( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex  ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue[pValueName].size()) + " of value name: " + pValueName + " ", __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName][pValueIndex].isDouble() == false) throw Exception("JSON ERROR: value name " + pValueName + " at index " + std::to_string(pValueIndex) + " is not a float", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName][pValueIndex].asFloat();
}

std::string
SerializeTools::getString( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex  ) throw (Exception)
{
    if(pValue.isMember(pValueName) == false) throw Exception("JSON ERROR: Value does not contain value name: " + pValueName, __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName].size() <=  pValueIndex) throw Exception("JSON ERROR: value index " + std::to_string(pValueIndex) + " exceeds size " + std::to_string(pValue[pValueName].size()) + " of value name: " + pValueName + " ", __FILE__, __FUNCTION__, __LINE__ );
    if(pValue[pValueName][pValueIndex].isString() == false) throw Exception("JSON ERROR: value name " + pValueName + " at index " + std::to_string(pValueIndex) + " is not a string", __FILE__, __FUNCTION__, __LINE__ );
    
    return pValue[pValueName][pValueIndex].asString();
}

Json::Value
SerializeTools::addValues(const Eigen::VectorXf& pValues)
{
    Json::Value valueData;
    
    int valueDim = pValues.rows();
    for(int d=0; d<valueDim; ++d)
    {
        valueData.append(pValues[d]);
    }
    
    return valueData;
}

Json::Value
SerializeTools::addValues(const glm::vec3& pValues)
{
    Json::Value valueData;
    
    for(int d=0; d<3; ++d)
    {
        valueData.append(pValues[d]);
    }
    
    return valueData;
}

Json::Value
SerializeTools::addValues(const glm::quat& pValues)
{
    Json::Value valueData;
    
    for(int d=0; d<4; ++d)
    {
        valueData.append(pValues[d]);
    }
    
    return valueData;
}

void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, Eigen::VectorXf& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    pValues.resize(dataDim, 1);
    
    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asFloat();
    }
}

void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, std::array<int, 2>& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    if(dataDim != 2) throw Exception("FLOCK ERROR: data dimension " + std::to_string(dataDim) + " must equal 2", __FILE__, __FUNCTION__, __LINE__);

    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asInt();
    }
}

void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, std::array<float, 4>& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    if(dataDim != 4) throw Exception("FLOCK ERROR: data dimension " + std::to_string(dataDim) + " must equal 4", __FILE__, __FUNCTION__, __LINE__);
    
    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asFloat();
    }
}

void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, glm::vec3& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    if(dataDim != 3) throw Exception("FLOCK ERROR: data dimension " + std::to_string(dataDim) + " must equal 3", __FILE__, __FUNCTION__, __LINE__);
    
    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asFloat();
    }
}

void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, glm::quat& pValues ) throw (Exception)
{
    if( pData.isMember(pName) == false ) throw Exception("FLOCK ERROR: no data with name " + pName + " found", __FILE__, __FUNCTION__, __LINE__);
    
    const Json::Value& data = pData[pName];
    int dataDim = data.size();
    
    if(dataDim != 4) throw Exception("FLOCK ERROR: data dimension " + std::to_string(dataDim) + " must equal 4", __FILE__, __FUNCTION__, __LINE__);
    
    for(int d=0; d<dataDim; ++d)
    {
        pValues[d] = data[d].asFloat();
    }
}

