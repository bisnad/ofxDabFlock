/** \file dab_flock_behavior_list.cpp
 */

#include "dab_flock_behavior_list.h"
#include "dab_flock_agent.h"

using namespace dab;
using namespace dab::flock;

BehaviorList::BehaviorList()
{
	assert("illegal constructor");
}

BehaviorList::~BehaviorList()
{
	unsigned int behaviorCount = mBehaviors.size();
	
	for(unsigned int i=0; i<behaviorCount; ++i)
	{
		Behavior* behavior = mBehaviors[i];
		delete behavior;
	}
	
	mBehaviors.clear();
}

unsigned int
BehaviorList::behaviorCount() const
{
	return mBehaviors.size();
}

bool
BehaviorList::contains(const std::string& pName) const
{
	return mBehaviors.contains(pName);
}

Behavior*
BehaviorList::behavior(unsigned int pIndex) throw (Exception)
{
	if( pIndex >= mBehaviors.size() ) throw Exception( "FLOCK ERROR: behavior index " + std::to_string(pIndex) + " out of bounds", __FILE__, __FUNCTION__, __LINE__ );
	
	return mBehaviors[pIndex];
}

const Behavior*
BehaviorList::behavior(unsigned int pIndex) const throw (Exception)
{
	if( pIndex >= mBehaviors.size() ) throw Exception( "FLOCK ERROR: behavior index " + std::to_string(pIndex) + " out of bounds", __FILE__, __FUNCTION__, __LINE__ );
	
	return mBehaviors[pIndex];
}

Behavior*
BehaviorList::behavior(const std::string& pName) throw (Exception)
{

	if(mBehaviors.contains(pName) == false) throw Exception( "FLOCK ERROR: behavior name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );

	return mBehaviors[pName];
}

const Behavior*
BehaviorList::behavior(const std::string& pName) const throw (Exception)
{
	if(mBehaviors.contains(pName) == false) throw Exception( "FLOCK ERROR: behavior name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
    
	return mBehaviors[pName];
}

unsigned int
BehaviorList::behaviorIndex(const std::string& pName) throw (Exception)
{
    if(mBehaviors.contains(pName) == false) throw Exception( "FLOCK ERROR: behavior name " + pName + " not found", __FILE__, __FUNCTION__, __LINE__ );
	
	return mBehaviors.index(pName);
}

void
BehaviorList::addBehavior(Behavior* pBehavior) throw (Exception)
{
	if(mBehaviors.contains(pBehavior->name()) == true) throw Exception( "FLOCK ERROR: behavior name " + pBehavior->name() + " already exists", __FILE__, __FUNCTION__, __LINE__ );
	
	try
	{
		mBehaviors.add( pBehavior->name(), pBehavior );
	}
	catch (Exception& e)
	{
        e += Exception( "FLOCK ERROR: failed to add behavior " + pBehavior->name(), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
BehaviorList::addBehavior(Behavior* pBehavior, unsigned int pBehaviorPosition) throw (Exception)
{
	if(mBehaviors.contains(pBehavior->name()) == true) throw Exception( "FLOCK ERROR: behavior name " + pBehavior->name() + " already exists", __FILE__, __FUNCTION__, __LINE__ );

	if( pBehaviorPosition > mBehaviors.size() ) pBehaviorPosition = mBehaviors.size();
	
	try
	{
		mBehaviors.insert( pBehavior->name(), pBehavior, pBehaviorPosition );
	}
	catch (Exception& e)
	{
        e += Exception( "FLOCK ERROR: failed to add behavior " + pBehavior->name(), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
BehaviorList::moveBehavior(unsigned int pOldBehaviorPosition, unsigned int pNewBehaviorPosition) throw (Exception)
{
	if( pOldBehaviorPosition > mBehaviors.size() ) pOldBehaviorPosition = mBehaviors.size();
	if( pNewBehaviorPosition > mBehaviors.size() ) pNewBehaviorPosition = mBehaviors.size();
	if( pOldBehaviorPosition == pNewBehaviorPosition ) return;
    
    
	try
	{
		mBehaviors.move( pOldBehaviorPosition, pNewBehaviorPosition );
	}
	catch (Exception& e)
	{
        e += Exception( "FLOCK ERROR: failed to move behavior at position " + std::to_string(pOldBehaviorPosition) + " to position " + std::to_string(pNewBehaviorPosition), __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
BehaviorList::removeBehavior(const std::string& pName) throw (Exception)
{
	if(mBehaviors.contains(pName) == false) throw Exception( "FLOCK ERROR: behavior name " + pName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );

	mBehaviors.remove(pName);
}

void
BehaviorList::act()
{
	//std::cout << "BehaviorList::act() begin\n";
    
	unsigned int behaviorCount = mBehaviors.size();
	for(unsigned int i=0; i<behaviorCount; ++i)
	{
		//std::cout << "behavior " << mBehaviors[i].name().toStdString() << " act begin\n";
        
		mBehaviors[i]->act();
		
		//std::cout << "behavior " << mBehaviors[i].name().toStdString() << " act end\n";
	}
	
	//std::cout << "BehaviorList::act() end\n";
}

BehaviorList::operator std::string() const
{
    return info();
}

std::string
BehaviorList::info(int pPropagationLevel) const
{
	std::stringstream ss;
	
	unsigned int behaviorCount = mBehaviors.size();
    
	for(unsigned int i=0; i<behaviorCount; ++i)
	{
		const Behavior* behavior = mBehaviors.value(i);
		const std::string& behaviorName = mBehaviors.key(i);
		
		if(pPropagationLevel == 0)
		{
            ss << behaviorName << " ";
		}
		else if(pPropagationLevel < 0 || pPropagationLevel >= 1)
		{
            ss << behavior->info(pPropagationLevel - 1) << "\n";
		}
	}
    
	return ss.str();
}