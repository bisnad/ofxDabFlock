/** \file dab_flock_grid_avg_behavior.cpp
 */

#include "dab_flock_grid_avg_behavior.h"

using namespace dab;
using namespace dab::flock;

GridAvgBehavior::GridAvgBehavior(const std::string& pInputParameterString, const std::string& pOutputParameterString)
: Behavior(pInputParameterString, pOutputParameterString)
{
	mClassName = "GridAvgBehavior";
}

GridAvgBehavior::GridAvgBehavior(Agent* pAgent, const std::string& pBehaviorName, const std::string& pInputParameterString, const std::string& pOutputParameterString) throw (Exception)
: Behavior(pAgent, pBehaviorName, pInputParameterString, pOutputParameterString)
{
	mClassName = "GridAvgBehavior";

	if( mInputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mInputParameters.size()) + " input parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mOutputParameters.size() < 1 ) throw Exception( "FLOCK ERROR: " + std::to_string(mOutputParameters.size()) + " output parameters supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	if( mInputNeighborGroups.size() < 1) throw Exception( "FLOCK ERROR: " + std::to_string(mInputNeighborGroups.size()) + " neighbor groups supplied, " + std::to_string(1) + " needed", __FILE__, __FUNCTION__, __LINE__ );
	
	// input parameter
	mParIn = mInputParameters[0];
	
	// output parameter
	mParOut = mOutputParameters[0];
	
	// create internal parameters
	mMinDistPar = createInternalParameter("minDist", { 0.0f } );
	mMaxDistPar = createInternalParameter("maxDist", { 1.0f } );
	mAmountPar = createInternalParameter("amount", { 0.1f });
    
	// input neighbor groups
	mGridValues = mInputNeighborGroups[0];
	
	// other stuff
    int dim = mParOut->dim();
	mTmpValueOut.resize(dim, 1);
}

GridAvgBehavior::~GridAvgBehavior()
{}

Behavior*
GridAvgBehavior::create(const std::string& pBehaviorName, Agent* pAgent) const throw (Exception)
{
	try
	{
		if(pAgent != NULL) return new GridAvgBehavior(pAgent, pBehaviorName, mInputParameterString, mOutputParameterString);
		else return new GridAvgBehavior(mInputParameterString, mOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

Behavior*
GridAvgBehavior::create(const std::string& pInputParameterString, const std::string& pOutputParameterString) const
{
	try
	{
		return new GridAvgBehavior(pInputParameterString, pOutputParameterString);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
GridAvgBehavior::act()
{
	//std::cout << "GridAvgBehavior " << mName.toStdString() << " act() begin\n";
    
	if(mActivePar->value() <= 0.0) return;
    
    Eigen::VectorXf& valueIn = mParIn->values();
	space::NeighborGroup& valueInNeighbors = *mGridValues;
	Eigen::VectorXf& valueOut = mParOut->backupValues();
	float& minDist = mMinDistPar->value();
	float& maxDist = mMaxDistPar->value();
	float& amount = mAmountPar->value();
	
	Eigen::VectorXf& tmpValueOut = mTmpValueOut;
	unsigned int totalNeighborCount = valueInNeighbors.neighborCount();
	unsigned int neighborCount = 0;
	
	//std::cout << "totalNeighborCount " << totalNeighborCount << "\n";
	
	if(totalNeighborCount == 0) return;
	
	tmpValueOut.setConstant(0.0);
	float distance;
	float scale;
	
	for(unsigned int i=0; i<totalNeighborCount; ++i)
	{
		//const math::Vector<real>& gridValue = valueInNeighbors.neighbor(i)->position();
		const Eigen::VectorXf& gridValue = valueInNeighbors.value(i);
		
		assert(gridValue.rows() == valueOut.rows() && "wrong output value dimension");
        
		distance = valueInNeighbors.distance(i);
        
		//std::cout << "valueIn " << valueIn << " gridValue " << gridValue << " dist " << distance << "\n";
        
		if(minDist > 0.0 && distance < minDist) continue;
		if(maxDist > 0.0 && distance > maxDist) continue;
		
		scale = (maxDist - minDist) / (1.0 + distance - minDist);
        
		tmpValueOut += gridValue * scale;
		
		//std::cout << "scale " << scale << " tmpValueOut " << tmpValueOut << "\n";
		
		neighborCount++;
	}
	
	if(neighborCount == 0) return;
	
	tmpValueOut /= static_cast<float>(neighborCount);
	tmpValueOut *= amount;
	
	valueOut += tmpValueOut;
	
	//std::cout << "tmpValueOut " << tmpValueOut << "\n";
    
	//std::cout << "GridAvgBehavior " << mName.toStdString() << " act() end\n";
}