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

#ifndef PLEXIL_NODE_HH
#define PLEXIL_NODE_HH

#include "plexil-stdint.h" // int32_t, also includes plexil-config.h

#include "ExpressionListener.hh"
#include "NodeConnector.hh"
#include "NodeConstants.hh"
#include "PlexilNodeType.hh"

// Take care of annoying VxWorks macro
#undef UPDATE

namespace PLEXIL {

  // Forward references
  class Assignable;

  //! \brief An enumeration which identifies which queue(s) a Node is in.
  //! \ingroup Exec-Core
  enum QueueStatus {
                    QUEUE_NONE = 0,          //!< not in any queue
                    QUEUE_CHECK,             //!< in check-conditions queue
                    QUEUE_TRANSITION,        //!< in state transition queue
                    QUEUE_TRANSITION_CHECK,  //!< in state transition queue AND check-conditions requested
                    QUEUE_DELETE             //!< no longer eligible to transition
  };

  //! \class Node
  //! \brief A stateless virtual base class defining the interface for a Node in a PLEXIL plan.
  //! \see NodeImpl
  //! \ingroup Exec-Core
  class Node :
    public NodeConnector,
    public ExpressionListener
  {
  public:

    //
    // LinkedQueue API used by PlexilExec
    //

    //! \brief Get the pointer to the next item in the queue.
    //! \return The pointer.
    //! \see LinkedQueue
    virtual Node *next() const = 0;

    //! \brief Get the pointer to the pointer to the next item in the queue.
    //! \return The pointer.
    //! \see LinkedQueue
    virtual Node **nextPtr() = 0;

    //
    // Node API
    //

    //! \brief Make the node active.
    virtual void activateNode() = 0;

    //! \brief Calculate the destination state of this node, were it to transition,
    //!        based on the values of its conditions.
    //! \return True if the new destination state is different from the last check, false otherwise.
    virtual bool getDestState() = 0;

    //! \brief Get the previously calculated destination state of this node.
    //! \return The destination state.
    //! \note Should only be called by PlexilExec::resolveVariableConflicts and unit tests.
    virtual NodeState getNextState() const = 0;

    //! \brief Commit a pending node state transition.
    //! \param time The time of the transition.
    virtual void transition(double time = 0.0) = 0; // FIXME - need a better representation

    //! \brief Get the Node's priority.  The priority is used to resolve resource conflicts.
    //! \return The priority.
    //! \note Default method; only assignment nodes care about priority.
    virtual int32_t getPriority() const = 0;

    //! \brief Get the current state of this node.
    //! \return The current state.
    virtual NodeState getState() const = 0;

    //! \brief Get the outcome of this node.
    //! \return The outcome.
    virtual NodeOutcome getOutcome() const = 0;

    //! \brief Get the failure type of this node.
    //! \return The failure type.
    virtual FailureType getFailureType() const = 0;

    //! \brief Get the variable from an Assignment node.
    //! \return Pointer to the variable.
    //! \note Will return NULL for all but Assignment nodes.
    virtual Assignable *getAssignmentVariable() const = 0;

    //! \brief Get the type of this node.
    //! \return The node type.
    virtual PlexilNodeType getType() const = 0;

    //! \brief Get the parent of this node.
    //! \return Pointer to the parent Node.  May be NULL.
    virtual Node const *getParent() const = 0;

    //
    // For convenience of PlexilExec queue management
    //

    //! \brief Get this Node's queue status.
    //! \return The status.
    //! \see QueueStatus
    virtual uint8_t getQueueStatus() const = 0;

    //! \brief Set this Node's queue status.
    //! \param newval The new status.
    //! \see QueueStatus
    virtual void setQueueStatus(uint8_t newval) = 0;

    //! \brief Print this Node to an output stream, with an optional indent.
    //! \param stream Reference to the stream.
    //! \param indent Number of spaces to indent before printing.
    virtual void print(std::ostream& stream, const unsigned int indent = 0) const = 0;

    //! \brief Get a printed representation of the Node as a string.
    //! \param indent Number of spaces to indent before printing.
    //! \return The string.
    virtual std::string toString(const unsigned int indent = 0) = 0;

  };

  // See implementation in NodeImpl.cc

  //! \brief Overloaded formatted output operator.
  //! \param stream Reference to the output stream.
  //! \param node Const reference to the Node to be printed.
  //! \ingroup Exec-Core
  std::ostream& operator<<(std::ostream &stream, Node const &node);

}

#endif
