/* Copyright (c) 2006-2013, Universities Space Research Association (USRA).
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

#include "ListenerFilters.hh"
#include "CoreExpressions.hh"
#include "InterfaceSchema.hh"
#include "Node.hh"

#define STATES_TAG "States"
#define IGNORED_STATES_TAG "IgnoredStates"

namespace PLEXIL
{
  static NodeState parseStateName(const std::string& name)
  {
    const std::vector<Value>& allStates = StateVariable::ALL_STATES();
    for (size_t i = 0; i < NODE_STATE_MAX; ++i) {
      if (allStates[i].getStringValue() == name)
        return (NodeState) i;
    }
    assertTrueMsg(ALWAYS_FAIL,
                  "NodeStateFilter constructor: configuration error: \""
                  << name << "\" is not a valid state name");
    return NO_NODE_STATE;
  }

  NodeStateFilter::NodeStateFilter(const pugi::xml_node& xml)
    : ExecListenerFilter(xml)
  {
    bool hasStates = false;
    bool hasIgnoredStates = false;

    const char* states = xml.child_value(STATES_TAG);
    if (*states) {
      hasStates = true;
      // Default is don't report
      for (size_t i = 0; i < NODE_STATE_MAX; ++i)
        m_stateEnabled[i] = false;
      std::vector<std::string>* stateNames = InterfaceSchema::parseCommaSeparatedArgs(states);
      for (std::vector<std::string>::const_iterator it = stateNames->begin();
           it != stateNames->end();
           ++it)
        m_stateEnabled[parseStateName(*it)] = true;
      delete stateNames;
    }
    states = xml.child_value(IGNORED_STATES_TAG);
    if (*states) {
      assertTrue(!hasStates,
                 "NodeStateFilter constructor: configuration error: both <States> and <IgnoredStates> provided");
      // Default is report
      for (size_t i = 0; i < NODE_STATE_MAX; ++i)
        m_stateEnabled[i] = true;
      std::vector<std::string>* stateNames = InterfaceSchema::parseCommaSeparatedArgs(states);
      for (std::vector<std::string>::const_iterator it = stateNames->begin();
           it != stateNames->end();
           ++it)
        m_stateEnabled[parseStateName(*it)] = false;
      delete stateNames;
    }
    if (!hasStates && !hasIgnoredStates) {
      warn("NodeStateFilter constructor: neither <States> nor <IgnoredStates> provided; no node transitions will be reported");
    }
  }
  
  NodeStateFilter::~NodeStateFilter()
  {
  }

  // Return true if either the previous or new state is in the filter.

  bool NodeStateFilter::reportNodeTransition(NodeState prevState, const NodeId& node)
  {
    return m_stateEnabled[prevState] || m_stateEnabled[node->getState()];
  }

}