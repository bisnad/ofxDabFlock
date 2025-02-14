/** \file dab_flock_line_follow_behavior.cpp
 */

#include "dab_flock_line_follow_behavior.h"
#include "dab_flock_agent.h"
#include "dab_geom_line.h"
#include "dab_geom_spline.h"
#include "dab_geom_spline_tools.h"

using namespace dab;
using namespace dab::flock;

LineFollowBehavior::LineFollowBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{}

LineFollowBehavior::LineFollowBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
, mPreviousClosestGeomGroup( nullptr )
, mPreviousClosestLineIndex( 0 )
, mPreviousSpaceShape( nullptr )
{
	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputParameters[0]->dim() != mOutputParameters[0]->dim() )  throw Exception( "FLOCK ERROR: input parameter " + mInputParameters[0]->name() + " dim " + std::to_string(mInputParameters[0]->dim()) + " must match output parameter " + mOutputParameters[0]->name() + " dim " + std::to_string(mOutputParameters[0]->dim()), __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
    
	// input parameter
	mPositionPar = mInputParameters[0];
	
	// output parameter
	mForcePar = mOutputParameters[0];
	
	// create internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f });
	mMaxDistPar = createInternalParameter("maxDist", { 0.5f });
	mContourMaintainDistPar = createInternalParameter("contourMaintainDist",{ 10.0f });
	mOrtAmountPar = createInternalParameter("ortAmount", { 1.0f });
	mTanAmountPar = createInternalParameter("tanAmount", { 1.0f });
	mAmountPar = createInternalParameter("amount", { 0.1f });
	
	// input neighbor groups
	mPositionNeighbors = mInputNeighborGroups[0];
}

LineFollowBehavior::~LineFollowBehavior()
{}

Behavior*
LineFollowBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new LineFollowBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new LineFollowBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
LineFollowBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new LineFollowBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
LineFollowBehavior::act()
{
	if(mActivePar->value() <= 0.0) return;
    
    Eigen::VectorXf& position = mPositionPar->values();
	space::NeighborGroup& positionNeighbors = *mPositionNeighbors;
	Eigen::VectorXf& force = mForcePar->backupValues();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& contourMaintainDist = mContourMaintainDistPar->value();
	float& ortAmount = mOrtAmountPar->value();
	float& tanAmount = mTanAmountPar->value();
	float& amount = mAmountPar->value();
	
	unsigned int totalNeighborCount = positionNeighbors.neighborCount();
	if(totalNeighborCount == 0)
	{
		mPreviousClosestGeomGroup = NULL;
		return;
	}
	
	// get neighbor space shape
	space::SpaceShape* spaceShape = dynamic_cast<space::SpaceShape*>( positionNeighbors.neighbor(0) );
    if(spaceShape == nullptr)
    {
        mPreviousClosestGeomGroup = NULL;
		return;
    }
	
	if( spaceShape != mPreviousSpaceShape )
	{
		mPreviousClosestGeomGroup = NULL;
		mPreviousSpaceShape = spaceShape;
	}
    
    int swarmDim = position.rows();
    int swarmDimLim = std::min(swarmDim, 3);
	
	// determine agent position in object coordinate space of space shape
    for(int d=0; d<swarmDimLim; ++d)
    {
        mWC_AgentPosition[d] = position[d];
    }
    mOC_AgentPosition = spaceShape->world2object(mWC_AgentPosition);
    
    //std::cout << "wc_agentPos " << mWC_AgentPosition << " oc_agentPos " << mOC_AgentPosition << "\n";
    
    geom::GeometryGroup* geomGroup;
	
	// check if previous geom group is part of current space shape and if it is sufficiently close
	geomGroup = mPreviousClosestGeomGroup;
	float closestGroupDist;

    //std::cout << "geomGroup " << geomGroup << "\n";
    
	if( geomGroup != nullptr )
	{
        //std::cout << "geomGroup min " << geomGroup->minPos() << " max " << geomGroup->maxPos() << "\n";
        
		mBoundingBox.set( mPreviousClosestGeomGroup->minPos(), mPreviousClosestGeomGroup->maxPos() );
		mBoundingBox.closestPoint( mOC_AgentPosition, mOC_ClosestPosition );
		float geomDist = glm::length( mOC_ClosestPosition - mOC_AgentPosition );
        
        //std::cout << "geomDist " << geomDist << "\n";
        
		if( geomDist > contourMaintainDist )
		{
			mPreviousClosestGeomGroup = nullptr;
			geomGroup = mPreviousClosestGeomGroup;
		}
	}
	
	// look for new innermost closest geometry group
	if( geomGroup == nullptr )
	{
		geomGroup = dynamic_cast< geom::GeometryGroup* >( spaceShape->geometry().get() ); //top geom group
		
		if( geomGroup == nullptr ) return;
		closestGroupDist = FLT_MAX;
		
		getClosestGeomGroup( &geomGroup, mOC_AgentPosition, closestGroupDist );
        
        //std::cout << "new closest geom group " << geomGroup << " ap " << mOC_AgentPosition  << " cgd " << closestGroupDist  << "\n";
	}
	
    //std::cout << "closest geom group " << geomGroup << " : dist " << closestGroupDist << "\n";
	
    std::vector< geom::Geometry* >& lines = geomGroup->geometries();
	int lineCount = lines.size();
    
    //std::cout << "lineCount " << lineCount << "\n";
    
	int searchLineStart1 = 0;
	int searchLineEnd1 = 0;
	int searchLineStart2 = 0;
	int searchLineEnd2 = 0;
	
	if( mPreviousClosestGeomGroup != geomGroup ) // start a new full search
	{
		mPreviousClosestGeomGroup = geomGroup;
		searchLineStart1 = 0;
		searchLineEnd1 = lineCount;
		searchLineStart2 = 0;
		searchLineEnd2 = 0;
	}
	else
	{
		searchLineStart1 = mPreviousClosestLineIndex - 3;
		searchLineEnd1 = mPreviousClosestLineIndex + 3;
		
		if( searchLineStart1 < 0 )
		{
			searchLineStart2 = lineCount + searchLineStart1;
			searchLineStart1 = 0;
			searchLineEnd2 = lineCount;
		}
		else if( searchLineEnd1 > lineCount )
		{
			searchLineEnd2 = searchLineEnd1 - lineCount;
			searchLineStart2 = 0;
			searchLineEnd1 = lineCount;
		}
		
		if( searchLineStart2 >= lineCount ) searchLineStart2 = lineCount - 1;
		else if( searchLineStart2 < 0 ) searchLineStart2 = 0;
	}
    
    if(searchLineStart1 < 0) searchLineStart1 = 0;
    if(searchLineStart1 > lineCount) searchLineStart1 = lineCount;
    if(searchLineStart2 < 0) searchLineStart2 = 0;
    if(searchLineStart2 > lineCount) searchLineStart2 = lineCount;
    if(searchLineEnd1 < 0) searchLineEnd1 = 0;
    if(searchLineEnd1 > lineCount) searchLineEnd1 = lineCount;
    if(searchLineEnd2 < 0) searchLineEnd2 = 0;
    if(searchLineEnd2 > lineCount) searchLineEnd2 = lineCount;
    
	//std::cout << "line search range start1 " << searchLineStart1 << " end1 " << searchLineEnd1 << " start2 " << searchLineStart2 << " end2 " << searchLineEnd2 << "\n";
	
	geom::Line* line;
	float lineDist;
	
	geom::Geometry* closestLine;
	float closestLineDist = FLT_MAX;
	
	for( int lI = searchLineStart1; lI < searchLineEnd1; ++lI )
	{
		line = static_cast<geom::Line*>( lines[ lI ] );
        
		line->closestPoint(mOC_AgentPosition, mOC_ClosestPosition);
		lineDist = glm::length( mOC_ClosestPosition - mOC_AgentPosition );
		
		if( lineDist <= closestLineDist )
		{
			closestLineDist = lineDist;
			closestLine = line;
			mPreviousClosestLineIndex = lI;
		}
		
		//std::cout << lI << " v0 " << line->v0() << " v1 " << line->v1() << " dist " << lineDist << " closest " << closestLineDist << "\n";
	}
	for( int lI = searchLineStart2; lI < searchLineEnd2; ++lI )
	{
		line = static_cast<geom::Line*>( lines[ lI ] );
		
		line->closestPoint(mOC_AgentPosition, mOC_ClosestPosition);
		lineDist = glm::length( mOC_ClosestPosition - mOC_AgentPosition );
		
		if( lineDist <= closestLineDist )
		{
			closestLineDist = lineDist;
			closestLine = line;
			mPreviousClosestLineIndex = lI;
		}
		
		//std::cout << lI << " v0 " << line->v0() << " v1 " << line->v1() << " dist " << lineDist << " closest " << closestLineDist << "\n";
	}
	
	closestLine->closestPoint(mOC_AgentPosition, mOC_ClosestPosition);
	
	// calculate orthogonal direction to closest point in world coordinates
    
    mWC_ClosestPosition = spaceShape->object2world(mOC_ClosestPosition);
	mOrtDirection = mWC_ClosestPosition - mWC_AgentPosition;
    
    //std::cout << "closest line index " << mPreviousClosestLineIndex << " v0 " << line->v0() << " v1 " << line->v1() << "\n";
    
    //std::cout << "agent wc " << mWC_AgentPosition << " oc " << mOC_AgentPosition << " line closest wc " << mWC_ClosestPosition << " oc " << mOC_ClosestPosition << "\n";
	
	int tangIndex = mPreviousClosestLineIndex;
	//mTangDirection = OC2WC_Matrix.multiply( lines[tangIndex]->maxPos() - lines[tangIndex]->minPos(), 1);
	mTangDirection = static_cast< geom::Line* >( lines[tangIndex] )->v1() - static_cast< geom::Line* >( lines[tangIndex] )->v0();
    
	// normalise orthogonal direction and tangent direction
	mOrtDirection = glm::normalize(mOrtDirection);
	mTangDirection = glm::normalize(mTangDirection);
	
	float scale = (closestLineDist - minDist) / (maxDist - minDist);
	
	//if( closestLineDist > maxDist) scale = 1.0;
    
    // new version
    if( closestLineDist > maxDist)
    {
        mPreviousClosestLineIndex = 0;
        mPreviousSpaceShape = nullptr;
        return;
    }
    
    for(int d=0; d<swarmDimLim; ++d)
    {
      force[d] += ( mOrtDirection[d] * scale * ortAmount + mTangDirection[d] * (1.0 - scale) * tanAmount ) * amount;
    }
}

void
LineFollowBehavior::getClosestGeomGroup( geom::GeometryGroup** pGeomGroup, const glm::vec3& pOC_AgentPosition, float& pGroupDist )
{
	//std::cout << "LineFollowBehavior::getClosestGeomGroup : geomGroup " << *pGeomGroup << " dist " << pGroupDist << " begin\n";
    
	// determine closest geometry group
    std::vector< geom::Geometry* >& geometries = (*pGeomGroup)->geometries();
	int geometryCount = geometries.size();
	geom::GeometryGroup* geomGroup;
	float groupDist;
	bool closerGroupFound = false;
    
    //std::cout << "geometryCount " << geometryCount << "\n";
    
	for( int i=0; i<geometryCount; ++i )
	{
		geomGroup = dynamic_cast< geom::GeometryGroup* >( geometries[i] );
        
        //std::cout << "geometry " << i << " geomGroup " << geomGroup << "\n";
        
		if( geomGroup != NULL )
		{
			mBoundingBox.set( geomGroup->minPos(), geomGroup->maxPos() );
			mBoundingBox.closestPoint( mOC_AgentPosition, mOC_ClosestPosition );
			groupDist = glm::length( mOC_ClosestPosition - mOC_AgentPosition );
            
            //std::cout << "groupDist " << groupDist << " former GroupDist " << pGroupDist << "\n";
			
			if( groupDist <= pGroupDist )
			{
				(*pGeomGroup) = geomGroup;
				pGroupDist = groupDist;
				closerGroupFound = true;
			}
			
			//std::cout << "check group " << i << " : " << geomGroup << " bounding box min " << geomGroup->minPos() << " max " << geomGroup->maxPos() << " dist " << groupDist <<"\n";
		}
	}
	
	//std::cout << "LineFollowBehavior::getClosestGeomGroup : geomGroup " << *pGeomGroup << " dist " << pGroupDist << " end\n";
	
	if( closerGroupFound == true ) getClosestGeomGroup( pGeomGroup, pOC_AgentPosition, pGroupDist );
	else return;
}