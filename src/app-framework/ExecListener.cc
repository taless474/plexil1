// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
//  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Universities Space Research Association nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ExecListener.hh"

#include "Debug.hh"
#include "Error.hh"
#include "ExecListenerFilter.hh"
#include "ExecListenerFilterFactory.hh"
#include "InterfaceSchema.hh"
#include "NodeTransition.hh"

namespace PLEXIL
{

  ExecListener::ExecListener()
    : m_filter(NULL)
  {
  }

  ExecListener::ExecListener(pugi::xml_node const xml)
    : m_xml(xml),
      m_filter(NULL)
  {
  }

  ExecListener::~ExecListener() 
  { 
    delete m_filter;
  }

  void ExecListener::notifyOfTransitions(const std::vector<NodeTransition>& transitions) const
  {
    this->implementNotifyNodeTransitions(transitions);
  }

  void
  ExecListener::notifyOfAddPlan(pugi::xml_node const plan) const
  {
    if (!m_filter
        || m_filter->reportAddPlan(plan))
      this->implementNotifyAddPlan(plan);
  }

  void 
  ExecListener::notifyOfAddLibrary(pugi::xml_node const libNode) const
  {
    if (!m_filter
        || m_filter->reportAddLibrary(libNode))
      this->implementNotifyAddLibrary(libNode);
  }

  void
  ExecListener::notifyOfAssignment(Expression const *dest,
                                   const std::string& destName,
                                   const Value& value) const
  {
    if (!m_filter
        || m_filter->reportAssignment(dest, destName, value))
      this->implementNotifyAssignment(dest, destName, value);
  }

  bool ExecListener::constructFilter()
  {
    if (m_xml.empty())
      return true; // nothing to do
    if (m_filter)
      return true; // already initialized

    pugi::xml_node filterSpec = m_xml.child(InterfaceSchema::FILTER_TAG());
    if (filterSpec.empty())
      return true;
              
    // Construct specified event filter
    pugi::xml_attribute filterTypeAttr = filterSpec.attribute(InterfaceSchema::FILTER_TYPE_ATTR());
    if (filterTypeAttr.empty()) {
      warn("ExecListener:constructFilter: invalid XML: <"
           << InterfaceSchema::FILTER_TAG()
           << "> element missing a "
           << InterfaceSchema::FILTER_TYPE_ATTR()
           << " attribute");
      return false;
    }

    const char* filterType = filterTypeAttr.value();
    if (!*filterType) {
      warn("ExecListener:constructFilter: invalid XML: <"
           << InterfaceSchema::FILTER_TAG()
           << "> element's "
           << InterfaceSchema::FILTER_TYPE_ATTR()
           << " attribute is empty");
      return false;
    }

    ExecListenerFilter *f = 
      ExecListenerFilterFactory::createInstance(std::string(filterType),
                                                filterSpec);
    if (!f) {
      warn("ExecListener:constructFilter: failed to construct exec listener filter "
           << filterType);
      return false;
    }

    if (!f->initialize()) {
      warn("ExecListener:constructFilter: error initializing listener filter " << filterType);
      delete f;
      return false;
    }
    m_filter = f;
    return true;
  }

  bool ExecListener::initialize()
  {
    if (!constructFilter())
      return false;
    return true; 
  }

  bool ExecListener::start() 
  { 
    return true; 
  }

  bool ExecListener::stop() 
  {
    return true; 
  }

  bool ExecListener::reset() 
  {
    return true; 
  }

  bool ExecListener::shutdown() 
  { 
    return true; 
  }

  //
  // Default methods to be overridden by derived classes
  //

  void ExecListener::implementNotifyNodeTransitions(const std::vector<NodeTransition>& transitions) const
  {
    debugMsg("ExecListener:implementNotifyNodeTransitions", " default method called");
    if (!m_filter) {
      for (std::vector<NodeTransition>::const_iterator it = transitions.begin();
           it != transitions.end();
           ++it) 
        this->implementNotifyNodeTransition(it->oldState, it->newState, it->node);
    }
    else {
      for (std::vector<NodeTransition>::const_iterator it = transitions.begin();
           it != transitions.end();
           ++it)
        if (m_filter->reportNodeTransition(it->oldState, it->newState, it->node))
          this->implementNotifyNodeTransition(it->oldState, it->newState, it->node);
    }
  }

  void ExecListener::implementNotifyNodeTransition(NodeState /* prevState */,
                                                   NodeState /* newState */,
                                                   Node * /* node */) const
  {
    debugMsg("ExecListener:implementNotifyNodeTransition", " default method called");
  }

  void ExecListener::implementNotifyAddPlan(pugi::xml_node const /* plan */) const
  {
    debugMsg("ExecListener:implementNotifyAddPlan", " default method called");
  }

  void ExecListener::implementNotifyAddLibrary(pugi::xml_node const /* libNode */) const
  {
    debugMsg("ExecListener:implementNotifyAddLibrary", " default method called");
  }

  void ExecListener::implementNotifyAssignment(Expression const * /* dest */,
                                               const std::string& /* destName */,
                                               const Value& /* value */) const
  {
    debugMsg("ExecListener:implementNotifyAssignment", " default method called");
  }

}
