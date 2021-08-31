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
  //! \brief A base class for notifying external agents about execution events
  //!        in the Exec.  Provides event filtering hooks.
  //! \see ExecListenerHub
  //! \see ExecListenerFilter
  //! \see ExecListenerFactory
  //! \ingroup App-Framework
  //!
  //! ExecListener serves as the base class for all
  //! application-specific Exec listeners.  It receives event
  //! notification calls from the ExecListenerHub, and provides
  //! optional filtering capabilities for selecting the events to be
  //! reported.
  //!
  //! Derived classes may extract information from the configuration
  //! XML element for their own purposes.  Derived classes may
  //! implement methods for implementNotifyNodeTransitions(),
  //! implementNotifyNodeTransition(), implementNotifyAssignment(),
  //! implementNotifyAddPlan(), and/or implementNotifyAddLibrary();
  //! the default methods for these member functions do nothing.

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
    // API to ExecListenerBase
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

    //! \brief Notify that a plan has been received by the Exec.
    //! \param plan Const handle to the XML representation of the plan.
    void notifyOfAddPlan(pugi::xml_node const plan) const;

    //! \brief Notify that a library node has been received by the Exec.
    //! \param libNode Const handle to the XML representation of the library node.
    void notifyOfAddLibrary(pugi::xml_node const libNode) const;

    //
    // API to be extended by derived classes
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

  protected:

    //
    // API to be implemented by derived classes
    //

    //! \brief Notify that nodes have changed state.
    //! \param transitions Const reference to vector of node state transition data.
    //! \note The default method calls the
    //!       implementNotifyNodeTransition member function for each
    //!       NodeTransition instance in the vector.
    //! \note Derived classes should implement either this member
    //!       function or implementNotifyNodeTransition(), not both.
    virtual void implementNotifyNodeTransitions(std::vector<NodeTransition> const &transitions) const;

    //! \brief Notify that a node has changed state.
    //! \param prevState The old state.
    //! \param newState The new state.
    //! \param node Pointer to the node which has transitioned.
    //! \note The default method does nothing.
    //! \note Derived classes should implement either this member
    //!       function or implementNotifyNodeTransitions(), not both.
    virtual void implementNotifyNodeTransition(NodeState prevState,
                                               NodeState newState,
                                               Node* node) const;

    //! \brief Notify that a variable assignment has been performed.
    //! \param dest Const pointer to the Expression (variable) being assigned.
    //! \param destName Const reference to a string naming the variable.
    //! \param value Const reference to the Value being assigned to the variable.
    //! \note The default method does nothing.
    virtual void implementNotifyAssignment(Expression const *dest,
                                           std::string const &destName,
                                           Value const &value) const;

    //! \brief Notify that a plan has been received by the Exec.
    //! \param plan Const handle to the XML representation of the plan.
    //! \note The default method does nothing.
    virtual void implementNotifyAddPlan(pugi::xml_node const plan) const;

    //! \brief Notify that a library node has been received by the Exec.
    //! \param libNode Const handle to the XML representation of the plan.
    //! \note The default method does nothing.
    virtual void implementNotifyAddLibrary(pugi::xml_node const libNode) const;

    //
    // Shared API made available to derived classes
    //

    //! \brief Get the configuration XML for this object.
    //! \return Const handle to the configuration XML element.
    pugi::xml_node const getXml() const
    {
      return m_xml;
    }

  private:

    // Copy, move, assignment operators deliberately not implemented.
#if __cplusplus >= 201103L
    ExecListener(ExecListener const &) = delete;
    ExecListener(ExecListener &&) = delete;
    ExecListener &operator=(ExecListener const &) = delete;
    ExecListener &operator=(ExecListener &&) = delete;
#else
    ExecListener(ExecListener const &);
    ExecListener &operator=(ExecListener const &);
#endif

    //! \brief Construct the ExecListenerFilter as specified by the configuration XML.
    //! \return True if successful, false otherwise.
    virtual bool constructFilter();

    //! \brief Const handle to the configuration XML for this object.
    pugi::xml_node const m_xml;

    //! \brief Pointer to the filter. May be NULL.
    ExecListenerFilter *m_filter;
  };
}

#endif // PLEXIL_EXEC_LISTENER_HH
