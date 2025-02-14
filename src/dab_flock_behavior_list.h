/** \file dab_flock_behavior_list.h
 *
 *  Created by Daniel Bisig on 4/14/07.
 *  Ported to OpenFrameworks by Daniel Bisig on 3/08/17.
 */

#ifndef _dab_flock_behavior_list_h_
#define _dab_flock_behavior_list_h_

#include "dab_exception.h"
#include "dab_flock_behavior.h"
#include "dab_index_map.h"

namespace dab
{

namespace flock
{

class Agent;

class BehaviorList
{
public:
    /**
     \brief default constructor
     */
    BehaviorList();
    
    /**
     \brief destructor
     */
    ~BehaviorList();
    
    /**
     \brief return number of behaviors this list contains
     \return number of behaviors this list contains
     */
    unsigned int behaviorCount() const;
    
    /**
     \brief checks if behavior list contains behavior
     \param pName behavior name
     \return true, if behavior name exists
     */
    bool contains(const std::string& pName) const;
    
    /**
     \brief get behavior
     \param pIndex behavior index
     \return behavior
     \exception Exception behavior not found
     */
    Behavior* behavior(unsigned int pIndex) throw (Exception);
    
    /**
     \brief get behavior
     \param pIndex behavior index
     \return behavior
     \exception Exception behavior not found
     */
    const Behavior* behavior(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief get behavior
     \param pName behavior name
     \return behavior
     \exception Exception behavior not found
     */
    Behavior* behavior(const std::string& pName) throw (Exception);
    
    /**
     \brief get behavior
     \param pName behavior name
     \return behavior
     \exception Exception behavior not found
     */
    const Behavior* behavior(const std::string& pName) const throw (Exception);
    
    /**
     \brief get behavior index
     \param pName behavior name
     \return behavior index
     \exception Exception behavior not found
     */
    unsigned int behaviorIndex(const std::string& pName) throw (Exception);
    
    /**
     \brief add behavior
     \param pBehavior behavior
     \exception Exception behavior already exists
     */
    void addBehavior(Behavior* pBehavior) throw (Exception);
    
    /**
     \brief add behavior
     \param pBehavior behavior
     \param pBehaviorPosition behavior position
     \exception Exception behavior already exists or Illegal Behavior Parameters
     */
    void addBehavior(Behavior* pBehavior, unsigned int pBehaviorPosition) throw (Exception);
    
    /**
     \brief move behavior
     \param pOldBehaviorPosition old behavior position
     \param pOldBehaviorPosition new behavior position
     \exception FlockException behavior not found
     */
    void moveBehavior(unsigned int pOldBehaviorPosition, unsigned int pNewBehaviorPosition) throw (Exception);
    
    /**
     \brief remove behavior
     \param pName behavior name
     \exception FlockException behavior not found
     */
    void removeBehavior(const std::string& pName) throw (Exception);
    
    /**
     \brief perform behaviors
     */
    void act();
    
    /**
     \brief print behavior list information
     */
    virtual operator std::string() const;
    
    /**
     \brief print behavior list information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief print behavior list information
     */	
    friend std::ostream& operator<< (std::ostream& pOstream, const BehaviorList& pBehaviorList)
    {
        pOstream << pBehaviorList.info();
        
        return pOstream;
    };

    
protected:
    IndexMap<std::string, Behavior*> mBehaviors; ///\brief behaviors
};

};

};

#endif