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

#ifndef PLEXIL_PLAN_DEBUG_LISTENER_HH
#define PLEXIL_PLAN_DEBUG_LISTENER_HH

#include "ExecListener.hh"

namespace PLEXIL
{

  //! \class PlanDebugListener
  //! \brief Provides optional time-stamped messages tracing execution of a PLEXIL plan.
  //! \ingroup interface-library

  //! The PlanDebugListener uses the debug message facility to print a
  //! message when a node transitions into either the `EXECUTING` or
  //! `FINISHED` state.  The message includes the time (in seconds
  //! after the epoch) at which the state transition occurred.
  //!
  //! The debug messages are keyed with the tag `Node:clock`.  To
  //! enable them, add the line:
  //!       :Node:clock
  //! to the debug configuration file (default Debug.cfg in the
  //! current directory).

  class PlanDebugListener : public ExecListener
  {
  public:

    //! \brief Default constructor.
    PlanDebugListener();

    //! \brief Constructor from configuration XML.
    //! \param xml Const handle to the XML.
    PlanDebugListener(pugi::xml_node const xml);

    //! \brief Virtual destructor.
    virtual ~PlanDebugListener();

    //! \brief Notify that a node has changed state.
    //! \param prevState The old state.
    //! \param newState The new state.
    //! \param node Pointer to the node which has transitioned.
    void implementNotifyNodeTransition(NodeState prevState,
                                       NodeState newState,
                                       Node *node) const;

  private:
    // Disallow copy and assignment
    PlanDebugListener(const PlanDebugListener&);
    PlanDebugListener& operator= (const PlanDebugListener&);
  };
}

extern "C"
void initPlanDebugListener();

#endif // PLEXIL_PLAN_DEBUG_LISTENER_HH
