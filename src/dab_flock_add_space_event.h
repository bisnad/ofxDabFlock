/** \file dab_flock_add_space_event.h
 *
 *  Created by Daniel Bisig on 02/09/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 24/10/2017.
 */

#ifndef _dab_flock_add_space_event_h_
#define _dab_flock_add_space_event_h_

#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_space_types.h"
#include "dab_space_alg.h"
#include "dab_space_alg_grid.h"
#include "dab_event.h"

namespace dab
{

namespace flock
{

class AddSpaceEvent : public event::Event
{
public:
    AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pSpaceDim);
    AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pGridValueDim, const dab::Array<unsigned int>& pGridSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode);
    AddSpaceEvent(const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const std::string& pGridName, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode);
    AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pSpaceDim);
    AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, unsigned int pGridValueDim, const dab::Array<unsigned int>& pGridSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode);
    AddSpaceEvent(double pTime, const std::string& pSpaceName, space::SpaceAlgType pSpaceAlgType, const std::string& pGridName, space::GridAlg::GridNeighborMode pGridNeighborMode, space::GridAlg::GridUpdateMode pGridUpdateMode);
    AddSpaceEvent( const AddSpaceEvent& pEvent );
    AddSpaceEvent( double pTime, const AddSpaceEvent& pEvent );
    virtual ~AddSpaceEvent();
    
    event::Event* copy() const;
    event::Event* copy( double pTime ) const;
    void execute() throw (Exception);
    
protected:
    AddSpaceEvent();
    
    std::string mSpaceName;
    space::SpaceAlgType mSpaceAlgType;
    unsigned int mSpaceDim;
    Eigen::VectorXf mMinPos;
    Eigen::VectorXf mMaxPos;
    unsigned int mGridValueDim;
    dab::Array<unsigned int> mGridSubdivisionCount;
    space::GridAlg::GridNeighborMode mGridNeighborMode;
    space::GridAlg::GridUpdateMode mGridUpdateMode;
    std::string mGridName;
};

};

};

#endif