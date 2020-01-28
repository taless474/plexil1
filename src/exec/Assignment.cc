/* Copyright (c) 2006-2018, Universities Space Research Association (USRA).
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

#include "Assignment.hh"

#include "Assignable.hh"
#include "Debug.hh"
#include "Error.hh"
#include "ExecConnector.hh" // getExecListener()
#include "ExecListenerBase.hh"

namespace PLEXIL
{

  Assignment::Assignment()
    : m_ack("ack"),
      m_abortComplete("abortComplete"),
      m_value(),
      m_next(nullptr),
      m_rhs(nullptr),
      m_dest(nullptr),
      m_deleteLhs(false),
      m_deleteRhs(false)
  {
  }

  Assignment::~Assignment() 
  {
    if (m_deleteLhs)
      delete m_dest;
    if (m_deleteRhs)
      delete m_rhs;
  }

  Assignment *Assignment::next() const
  {
    return m_next;
  }

  Assignment **Assignment::nextPtr()
  {
    return &m_next;
  }

  //
  // Both const and non-const flavors for accessors
  //

  Expression *Assignment::getDest()
  {
    return m_dest;
  }

  Expression const *Assignment::getDest() const
  {
    return m_dest;
  }
  
  Expression *Assignment::getAck()
  {
    return &m_ack;
  }
  
  Expression const *Assignment::getAck() const
  {
    return &m_ack;
  }

  Expression *Assignment::getAbortComplete()
  {
    return &m_abortComplete;
  }

  Expression const *Assignment::getAbortComplete() const
  {
    return &m_abortComplete;
  }

  void Assignment::setVariable(Expression *lhs, bool garbage)
  {
    m_dest = lhs;
    m_deleteLhs = garbage;
  }

  void Assignment::setExpression(Expression *rhs, bool garbage)
  {
    m_rhs = rhs;
    m_deleteRhs = garbage;
  }

  void Assignment::fixValue() 
  {
    m_dest->asAssignable()->saveCurrentValue();
    m_value = m_rhs->toValue();
  }

  void Assignment::activate() 
  {
    assertTrue_2(m_dest && m_rhs,
                 "Attempt to activate uninitialized Assignment");
    m_rhs->activate();  // resets to unknown
    m_dest->activate(); // ""     "" ""
  }

  void Assignment::deactivate() 
  {
    m_rhs->deactivate();
    m_dest->deactivate();
  }

  void Assignment::execute()
  {
    debugMsg("Test:testOutput", "Assigning " << m_dest->toString() << " to " << m_value);
    m_dest->asAssignable()->setValue(m_value);
    m_ack.setValue(true);
    ExecListenerBase *listener = g_exec->getExecListener();
    if (listener)
      listener->notifyOfAssignment(m_dest, m_dest->getName(), m_value);
  }

  void Assignment::retract()
  {
    debugMsg("Test:testOutput",
             "Restoring previous value of " << m_dest->toString());
    m_dest->asAssignable()->restoreSavedValue();
    m_abortComplete.setValue(true);
    ExecListenerBase *listener = g_exec->getExecListener();
    if (listener)
      listener->notifyOfAssignment(m_dest,
                                   m_dest->getName(),
                                   m_dest->asAssignable()->getSavedValue());
  }

}
