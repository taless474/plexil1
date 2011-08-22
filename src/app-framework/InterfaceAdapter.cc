/* Copyright (c) 2006-2008, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "InterfaceAdapter.hh"
#include "AdapterExecInterface.hh"
#include "Debug.hh"
#include "Error.hh"
#include "LabelStr.hh"
#include "StateCache.hh"
#include "Node.hh"
#include "Variable.hh"
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include "tinyxml.h"

namespace PLEXIL
{

  //
  // Constructors
  //

  InterfaceAdapter::InterfaceAdapter(AdapterExecInterface& execInterface)
    : m_execInterface(execInterface),
      m_xml(NULL),
      m_id(this)
  {
  }

  InterfaceAdapter::InterfaceAdapter(AdapterExecInterface& execInterface, 
				     const TiXmlElement* xml)
    : m_execInterface(execInterface),
      m_xml(xml),
      m_id(this)
  {
  }

  //
  // Destructor
  //

  InterfaceAdapter::~InterfaceAdapter()
  {
    m_id.remove();
  }


  //
  // Default methods for InterfaceManager API
  //

  void InterfaceAdapter::registerChangeLookup(const LookupKey& /* uniqueId */,
					      const StateKey& /* stateKey */,
					      const std::vector<double>& /* tolerances */)
  {
    assertTrue(ALWAYS_FAIL,
	       "InterfaceAdapter::registerChangeLookup: default method called!");
  }

  void InterfaceAdapter::unregisterChangeLookup(const LookupKey& /* uniqueId */)
  {
    assertTrue(ALWAYS_FAIL,
	       "InterfaceAdapter::unregisterChangeLookup: default method called!");
  }

  void InterfaceAdapter::lookupNow(const StateKey& /* key */,
				   std::vector<double>& /* dest */)
  {
    assertTrue(ALWAYS_FAIL,
	       "InterfaceAdapter::lookupNow: default method called!");
  }

  void InterfaceAdapter::sendPlannerUpdate(const NodeId& /* node */,
					   const std::map<double, double>& /* valuePairs */,
					   const ExpressionId /* ack */)
  {
    assertTrue(ALWAYS_FAIL,
	       "InterfaceAdapter::updatePlanner: default method called!");
  }

  // This default method is a wrapper for backward compatibility.
  void InterfaceAdapter::executeCommand(CommandId cmd)
  {
	this->executeCommand(cmd->getName(), cmd->getArgValues(), cmd->getDest(), cmd->getAck());
  }

  // executes a command with the given arguments
  void InterfaceAdapter::executeCommand(const LabelStr& /* name */,
					const std::list<double>& /* args */,
					ExpressionId /* dest */,
					ExpressionId /* ack */)
  {
    assertTrue(ALWAYS_FAIL,
			   "InterfaceAdapter::executeCommand: default method called!");
  }


  //abort the given command with the given arguments that had acknowledgment old_ack.  store the abort-complete into ack
  void InterfaceAdapter::invokeAbort(const LabelStr& /* name */, 
				     const std::list<double>& /* args */, 
				     ExpressionId /* cmd_ack */,
				     ExpressionId /* ack */)
  {
    assertTrue(ALWAYS_FAIL,
	       "InterfaceAdapter::invokeAbort: default method called!");
  }

  //
  // Methods to facilitate implementations
  //

  void
  InterfaceAdapter::registerAsynchLookup(const LookupKey& uniqueId,
					 const StateKey& key)
  {
	// Get debugging info, and sanity check while we're at it
	State state;
	checkError(m_execInterface.stateForKey(key, state),
			   "InterfaceAdapter::registerAsynchLookup: no state found for state key " << key);
    StateToLookupMap::iterator it = m_asynchLookups.find(key);
    if (it == m_asynchLookups.end()) {
	  debugMsg("InterfaceAdapter:registerAsynchLookup",
			   " for state " << LabelStr(state.first).toString()
			   << "; new lookup for unique ID " << uniqueId);
	  std::set<LookupKey> theSet;
	  theSet.insert(uniqueId);
	  m_asynchLookups.insert(std::pair<StateKey, std::set<LookupKey> >(key, theSet));
	}
    else {
	  debugMsg("InterfaceAdapter:registerAsynchLookup",
			   " for state " << LabelStr(state.first).toString()
			   << "; adding unique ID " << uniqueId << " to existing lookup");
	  (*it).second.insert(uniqueId);
	}
  }

  void
  InterfaceAdapter::unregisterAsynchLookup(const LookupKey& uniqueId)
  {
    debugMsg("InterfaceAdapter:unregisterAsynchLookup",
	     " for unique ID " << uniqueId);
    StateToLookupMap::iterator tableIt =
      m_asynchLookups.begin();
    for (; tableIt != m_asynchLookups.end(); tableIt++)
      {
	std::set<LookupKey> & theSet = tableIt->second;
	std::set<LookupKey>::iterator setIt = theSet.find(uniqueId);
	if (setIt != theSet.end())
	  {
	    // Found it -- is it the only member?
	    if (theSet.size() == 1)
	      {
		// delete entry from table
		debugMsg("InterfaceAdapter:unregisterAsynchLookup",
			 " deleting last lookup for state " << tableIt->first);
		m_asynchLookups.erase(tableIt);
	      }
	    else
	      {
		// delete unique ID from entry
		debugMsg("InterfaceAdapter:unregisterAsynchLookup",
			 " deleting lookup for state with remaining lookups");
		theSet.erase(setIt);
	      }
	    return; // done in either case
	  }
      }
    // Warn, don't barf.
    debugMsg("InterfaceAdapter:unregisterAsynchLookup",
	     " Unique ID " << uniqueId << " not found.");
  }
    
  InterfaceAdapter::StateToLookupMap::const_iterator 
  InterfaceAdapter::getAsynchLookupsBegin()
  {
    return m_asynchLookups.begin();
  }
  
  InterfaceAdapter::StateToLookupMap::const_iterator
  InterfaceAdapter::getAsynchLookupsEnd()
  {
    return m_asynchLookups.end();
  }

  InterfaceAdapter::StateToLookupMap::const_iterator
  InterfaceAdapter::findStateKey(const StateKey& key)
  {
    return m_asynchLookups.find(key);
  }

  bool
  InterfaceAdapter::isStateKeySubscribed(const StateKey& key) const
  {
    return m_asynchLookups.find(key) != m_asynchLookups.end();
  }

  InterfaceAdapter::StateToLookupMap::const_iterator
  InterfaceAdapter::findLookupKey(const LookupKey& key)
  {
    StateToLookupMap::const_iterator it =
      m_asynchLookups.begin();
    while (it != m_asynchLookups.end())
      {
	const std::set<LookupKey>& keys = it->second;
	if (keys.find(key) != keys.end())
	  return it;
	it++;
      }
    return m_asynchLookups.end();
  }

  bool 
  InterfaceAdapter::getState(const StateKey& key, State& state)
  {
    return m_execInterface.getStateCache()->stateForKey(key, state);
  }

  bool
  InterfaceAdapter::getStateKey(const State& state, StateKey& key)
  {
    return !m_execInterface.getStateCache()->keyForState(state, key);
  }

  /**
   * @brief Register this adapter based on its XML configuration data.
   * @note The adapter is presumed to be fully initialized and working at the time of this call.
   * @note This is a default method; adapters are free to override it.
   */
  void InterfaceAdapter::registerAdapter()
  {
    m_execInterface.defaultRegisterAdapter(m_id);
  }

}
