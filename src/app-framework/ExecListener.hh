// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
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

#ifndef PLEXIL_EXEC_LISTENER_HH
#define PLEXIL_EXEC_LISTENER_HH

#include "NodeConstants.hh" // enum NodeState

#include "pugixml.hpp"

#include <vector>

namespace PLEXIL
{
  // Forward references
  class ExecListenerFilter;
  class Expression;
  class Node;
  class Value;

  struct NodeTransition;

  //! \class ExecListener
  //! \brief A base class for notifying external agents about state changes in the Exec.
  //! \note Provides event filtering hooks.
  //! \see ExecListenerFilter
  //! \ingroup App-Framework
  class ExecListener
  {
  public:

    //! \brief Default constructor.
    ExecListener();

    //! \brief Constructor from configuration XML.
    //! \param xml Const handle to the configuration XML.
    ExecListener(pugi::xml_node const xml);

    //! \brief Virtual destructor.
    virtual ~ExecListener();

    //
    // API to Exec
    // See ExecListenerBase.hh
    //

    //! \brief Notify that nodes have changed state.
    //! \param transitions Const reference to vector of node state transition data.
    void notifyOfTransitions(std::vector<NodeTransition> const &transitions) const;

    //! \brief Notify that a variable assignment has been performed.
    //! \param dest Const pointer to the Expression (variable) being assigned.
    //! \param destName Const reference to a string naming the variable.
    //! \param value Const reference to the new Value.
    void notifyOfAssignment(Expression const *dest,
                            std::string const &destName,
                            Value const &value) const;

    //
    // API to application
    //

    //! \brief Notify that a plan has been received by the Exec.
    //! \param plan Const handle to the XML representation of the plan.
    void notifyOfAddPlan(pugi::xml_node const plan) const;

    //! \brief Notify that a library node has been received by the Exec.
    //! \param libNode Const handle to the XML representation of the library node.
    void notifyOfAddLibrary(pugi::xml_node const libNode) const;

    //
    // API to be implemented by derived classes
    //

    //! \brief Perform listener-specific initialization.
    //! \return true if successful, false otherwise.
    //! \note The default method does nothing and returns true.
    virtual bool initialize();

    //! \brief Perform listener-specific startup.
    //! \return true if successful, false otherwise.
    //! \note The default method does nothing and returns true.
    virtual bool start();

    //! \brief Perform listener-specific actions to stop.
    //! \return true if successful, false otherwise.
    //! \note The default method does nothing and returns true.
    virtual bool stop();

    //! \brief Perform listener-specific actions to reset to initialized state.
    //! \return true if successful, false otherwise.
    //! \note The default method does nothing and returns true.
    //! \deprecated This member function is not present in the PLEXIL 6 release.
    virtual bool reset();

    //! \brief Perform listener-specific actions to shut down.
    //! \return true if successful, false otherwise.
    //! \note The default method does nothing and returns true.
    virtual bool shutdown();

    //! \brief Set the filter of this instance.
    //! \param fltr Pointer to the filter.
    //! \note The ExecListener takes ownership of the filter,
    //!       and deletes it at application shutdown.
    void setFilter(ExecListenerFilter *fltr);

  protected:

    //
    // API to be implemented by derived classes
    //

    //! \brief Notify that nodes have changed state.
    //! \param transitions Const reference to vector of node state transition data.
    //! \note ExecListener provides a default method, which calls the
    //!       implementNotifyNodeTransition member function for each
    //!       NodeTransition instance.
    //! \note Derived classes may override this member function.
    virtual void implementNotifyNodeTransitions(std::vector<NodeTransition> const &transitions) const;

    //! \brief Notify that a node has changed state.
    //! \param prevState The old state.
    //! \param newState The new state.
    //! \param node Pointer to the node which has transitioned.
    //! \note The default method does nothing.
    //! \note Derived classes should implement this member function.
    virtual void implementNotifyNodeTransition(NodeState prevState,
                                               NodeState newState,
                                               Node* node) const;

    //! \brief Notify that a plan has been received by the Exec.
    //! \param plan Const handle to the XML representation of the plan.
    //! \note The default method does nothing.
    virtual void implementNotifyAddPlan(pugi::xml_node const plan) const;

    //! \brief Notify that a library node has been received by the Exec.
    //! \param libNode Const handle to the XML representation of the plan.
    //! \note The default method does nothing.
    virtual void implementNotifyAddLibrary(pugi::xml_node const libNode) const;

    //! \brief Notify that a variable assignment has been performed.
    //! \param dest Const pointer to the Expression (variable) being assigned.
    //! \param destName Const reference to a string naming the variable.
    //! \param value Const reference to the Value being assigned to the variable.
    virtual void implementNotifyAssignment(Expression const *dest,
                                           std::string const &destName,
                                           Value const &value) const;

    //
    // Shared API made available to derived classes
    //

    //! \brief Construct the ExecListenerFilter specified by this listener's configuration XML.
    //! \return True if successful, false otherwise.
    virtual bool constructFilter();

    //! \brief Get the configuration XML for this object.
    //! \return Const handle to the configuration XML.
    pugi::xml_node const getXml() const
    {
      return m_xml;
    }

    //! \brief Pointer to the filter. May be null.
    ExecListenerFilter *m_filter;

  private:

    //! \brief Shared handle to the configuration XML for this object.
    pugi::xml_node const m_xml;
  };
}

#endif // PLEXIL_EXEC_LISTENER_HH
