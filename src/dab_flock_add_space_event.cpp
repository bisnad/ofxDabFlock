/** \file dab_flock_add_space_event.cpp
 */

#include "dab_flock_add_space_event.h"
#include "dab_space.h"
#include "dab_space_manager.h"
#include "dab_space_alg_permanent_neighbors.h"
#include "dab_space_alg_ntree.h"
#include "dab_space_alg_kdtree.h"
#include "dab_space_alg_rtree.h"
#include "dab_space_alg_ann.h"
#include "dab_space_alg_grid.h"

using namespace dab;
using namespace dab::flock;

AddSpaceEvent::AddSpaceEvent()
: event::Event()
{}

AddSpaceEvent::AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pSpaceDim)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pSpaceDim)
{}

AddSpaceEvent::AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pMinPos.rows())
, mMinPos(pMinPos)
, mMaxPos(pMaxPos)
{}

AddSpaceEvent::AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pGridValueDim, const dab::Array<unsigned int>& pGridSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pMinPos.rows())
, mMinPos(pMinPos)
, mMaxPos(pMaxPos)
, mGridValueDim(pGridValueDim)
, mGridSubdivisionCount(pGridSubdivisionCount)
, mGridNeighborMode(pGridNeighborMode)
, mGridUpdateMode(pGridUpdateMode)
{}

AddSpaceEvent::AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const std::string& pGridName, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode)
: event::Event(0.0, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mGridName(pGridName)
, mGridNeighborMode(pGridNeighborMode)
, mGridUpdateMode(pGridUpdateMode)
{}

AddSpaceEvent::AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pSpaceDim)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pSpaceDim)
{}

AddSpaceEvent::AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pMinPos.rows())
, mMinPos(pMinPos)
, mMaxPos(pMaxPos)
{}

AddSpaceEvent::AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pGridValueDim, const dab::Array<unsigned int>& pGridSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mSpaceDim(pMinPos.rows())
, mMinPos(pMinPos)
, mMaxPos(pMaxPos)
, mGridValueDim(pGridValueDim)
, mGridSubdivisionCount(pGridSubdivisionCount)
, mGridNeighborMode(pGridNeighborMode)
, mGridUpdateMode(pGridUpdateMode)
{}

AddSpaceEvent::AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const std::string& pGridName, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode)
: event::Event(pTime, -1.0, event::RelativeTime)
, mSpaceName(pSpaceName)
, mSpaceAlgType(pSpaceAlgType)
, mGridName(pGridName)
, mGridNeighborMode(pGridNeighborMode)
, mGridUpdateMode(pGridUpdateMode)
{}

AddSpaceEvent::AddSpaceEvent( const AddSpaceEvent& pEvent )
: event::Event( pEvent )
, mSpaceName( pEvent.mSpaceName )
, mSpaceAlgType( pEvent.mSpaceAlgType )
, mSpaceDim( pEvent.mSpaceDim )
, mMinPos( pEvent.mMinPos )
, mMaxPos( pEvent.mMaxPos )
, mGridName( pEvent.mGridName )
, mGridValueDim( pEvent.mGridValueDim )
, mGridSubdivisionCount( pEvent.mGridSubdivisionCount )
, mGridNeighborMode( pEvent.mGridNeighborMode )
, mGridUpdateMode( pEvent.mGridUpdateMode )
{}

AddSpaceEvent::AddSpaceEvent( double pTime, const AddSpaceEvent& pEvent )
: event::Event( pTime, pEvent )
, mSpaceName( pEvent.mSpaceName )
, mSpaceAlgType( pEvent.mSpaceAlgType )
, mSpaceDim( pEvent.mSpaceDim )
, mMinPos( pEvent.mMinPos )
, mMaxPos( pEvent.mMaxPos )
, mGridName( pEvent.mGridName )
, mGridValueDim( pEvent.mGridValueDim )
, mGridSubdivisionCount( pEvent.mGridSubdivisionCount )
, mGridNeighborMode( pEvent.mGridNeighborMode )
, mGridUpdateMode( pEvent.mGridUpdateMode ){}

AddSpaceEvent::~AddSpaceEvent()
{}

event::Event*
AddSpaceEvent::copy() const
{
	return new AddSpaceEvent( *this );
}

event::Event*
AddSpaceEvent::copy( double pTime ) const
{
	return new AddSpaceEvent( pTime, *this );
}

void
AddSpaceEvent::execute() throw (Exception)
{
	mStarted = true;
	mFinished = false;
    
	try
	{
		space::SpaceAlg* spaceAlg = nullptr;
		
		if( mSpaceAlgType ==  space::PermanentNeighborsAlgType )
		{
			if( mMinPos == mMaxPos ) spaceAlg = new space::PermanentNeighborsAlg( mSpaceDim );
			else spaceAlg = new space::PermanentNeighborsAlg( mMinPos, mMaxPos );
		}
		else if( mSpaceAlgType == space::NTreeAlgType )
		{
			if( mMinPos == mMaxPos ) spaceAlg = new space::NTreeAlg( mSpaceDim );
			else spaceAlg = new space::NTreeAlg( mMinPos, mMaxPos );
		}
		else if( mSpaceAlgType == space::KDTreeAlgType )
		{
			if( mMinPos == mMaxPos ) spaceAlg = new space::KDTreeAlg( mSpaceDim );
			else spaceAlg = new space::KDTreeAlg( mMinPos, mMaxPos );
		}
		else if( mSpaceAlgType == space::ANNAlgType )
		{
			if( mMinPos == mMaxPos ) spaceAlg = new space::ANNAlg( mSpaceDim );
			else spaceAlg = new space::ANNAlg( mMinPos, mMaxPos );
		}
		else if( mSpaceAlgType == space::RTreeAlgType )
		{
			spaceAlg = new space::RTreeAlg( mMinPos, mMaxPos );
			
			static_cast<space::RTreeAlg*>( spaceAlg )->setClosestPointType(space::ClosestPointShape);
		}
		else if( mSpaceAlgType == space::GridAlgType )
		{
			if(mGridName.empty()) spaceAlg = new space::GridAlg( mGridValueDim, mGridSubdivisionCount, mMinPos, mMaxPos, mGridNeighborMode, mGridUpdateMode );
			else
			{
                //				/*
                //				// TODO
                //				Simulation::get().space().spaceGrid( mSpaceGridName )
                //				Simulation::get().space()(new space::GridSpace(mSpaceName, Simulation::get().space().spaceGrid( mSpaceGridName ), mNeighborMode, mUpdateMode));
                //				*/
			}
		}
		
		if( spaceAlg == nullptr ) throw Exception( "FLOCK ERROR: failed to create algorithm for space " + mSpaceName, __FILE__, __FUNCTION__, __LINE__ );
		else
		{
			Simulation::get().space().addSpace( std::shared_ptr< space::Space>(new space::Space( mSpaceName, spaceAlg )));
		}
	}
	catch(Exception& e)
	{
		Simulation::get().exceptionReport( e );
	}
    
	mStarted = false;
	mFinished = true;
}