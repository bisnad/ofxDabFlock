/** \file iso_flock_stats.cpp
 */

#include "dab_flock_stats.h"

using namespace dab;
using namespace dab::flock;

FlockStats::FlockStats()
{}

FlockStats::~FlockStats()
{}

void
FlockStats::registerParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const Parameter& pParameter) throw (Exception)
{
	if( checkAnalyzer(pAnalyzerName) == false) throw Exception("FLOCK ERROR: analyzer " + pAnalyzerName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	
	space::SpaceObjectsAnalyzer* analyzer = mAnalyzers[pAnalyzerName];
	
	if( analyzer->checkObjectGroup(pGroupName) == false && analyzer->modifiable() == false ) throw Exception( "FLOCK ERROR: anaylzer " + pAnalyzerName + " does not contain data group " + pGroupName + " and cannot be extended", __FILE__, __FUNCTION__, __LINE__ );
	
	if( analyzer->checkObjectGroup(pGroupName) == false ) analyzer->addObjectGroup( pGroupName );
	analyzer->addObject( pGroupName, &pParameter );
}

void
FlockStats::registerParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const space::NeighborGroup& pNeighborGroup ) throw (Exception)
{
	if( checkAnalyzer(pAnalyzerName) == false) throw Exception( "FLOCK ERROR: analyzer " + pAnalyzerName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	space::SpaceObjectsAnalyzer* analyzer = mAnalyzers[pAnalyzerName];
	
	if( analyzer->checkObjectGroup(pGroupName) == false && analyzer->modifiable() == false ) throw Exception( "FLOCK EXCEPTION: anaylzer " + pAnalyzerName + " does not contain data group " + pGroupName + " and cannot be extended", __FILE__, __FUNCTION__, __LINE__ );
	
	if( analyzer->checkObjectGroup(pGroupName) == false ) analyzer->addObjectGroup( pGroupName );
	
	try
	{
		analyzer->addObject( pGroupName, &pNeighborGroup );
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
FlockStats::deregisterParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const Parameter& pParameter) throw (Exception)
{
	if( checkAnalyzer(pAnalyzerName) == false) throw Exception( "FLOCK ERROR: analyzer " + pAnalyzerName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	space::SpaceObjectsAnalyzer* analyzer = mAnalyzers[pAnalyzerName];
	
	if( analyzer->checkObjectGroup(pAnalyzerName) == false ) throw Exception( "FLOCK ERROR: anaylzer " + pAnalyzerName + " does not contain object group " + pGroupName, __FILE__, __FUNCTION__, __LINE__ );
	
	analyzer->removeObject( pGroupName, &pParameter );
}

void
FlockStats::deregisterParameter(const std::string& pAnalyzerName, const std::string& pGroupName, const space::NeighborGroup& pNeighborGroup) throw (Exception)
{

	if( checkAnalyzer(pAnalyzerName) == false) throw Exception( "FLOCK ERROR: analyzer " + pAnalyzerName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	space::SpaceObjectsAnalyzer* analyzer = mAnalyzers[pAnalyzerName];
	
	if( analyzer->checkObjectGroup(pGroupName) == false ) throw Exception( "FLOCK ERROR: anaylzer " + pAnalyzerName + " does not contain object group " + pGroupName, __FILE__, __FUNCTION__, __LINE__ );
    
	analyzer->removeObject( pGroupName, &pNeighborGroup );
}