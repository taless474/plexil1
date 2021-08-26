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

#ifndef PLEXIL_QUEUE_ENTRY_HH
#define PLEXIL_QUEUE_ENTRY_HH

#include "Value.hh"

namespace PLEXIL
{
  // Forward declarations
  class Command;
  class NodeImpl;
  class State;
  class Update;

  //! \brief Enumeration representing the purpose of an item in the queue.
  //! \ingroup External-Interface
  enum QueueEntryType {
    Q_UNINITED = 0,     //!< Not a valid enum value.
    Q_LOOKUP,           //!< A lookup return value.
    Q_COMMAND_ACK,      //!< A command handle value.
    Q_COMMAND_RETURN,   //!< A command return value.
    Q_COMMAND_ABORT,    //!< A command abort acknowledgement value.
    Q_UPDATE_ACK,       //!< A planner update acknowledgement.
    Q_ADD_PLAN,         //!< A plan to be executed.
    Q_MARK,             //!< A marker.

    Q_INVALID           //!< Values equal to or greater than this are invalid.
  };

  //! \struct QueueEntry
  //! \brief Discriminated union representing external state to send to the Exec.
  //! \see InputQueue
  //! \ingroup External-Interface
  struct QueueEntry
  {
    QueueEntry *next;         //!< Pointer to the next item in the queue.
    union {
      Command *command;       //!< Only valid if type is one of Q_COMMAND_ACK, Q_COMMAND_RETURN, Q_COMMAND_ABORT.
      Update *update;         //!< Only valid if type is Q_UPDATE_ACK.
      State *state;           //!< Only valid if type is Q_LOOKUP.
      NodeImpl *plan;         //!< Only valid if type is Q_ADD_PLAN.
      unsigned int sequence;  //!< Only valid if type is Q_MARK.
    };

    //! \brief The value associated with the command, update, or state.
    //!        Not valid if type is Q_ADD_PLAN or Q_MARK.
    Value value;

    QueueEntryType type;      //!< The purpose of this queue entry.

    //
    // Member functions.
    //

    //! \brief Reset the entry to a blank state.
    //!        Type is set to Q_UNINITED, value to unknown, the union to NULL.
    void reset();

    //! \brief Prepare the entry for a lookup value return.
    //! \param st The State whose value is being returned.
    //! \param val The return value.
    void initForLookup(State const &st, Value const &val);

    //! \brief Prepare the entry for a command handle return.
    //! \param cmd Pointer to the Command whose handle is being returned.
    //! \param val The CommandHandleValue being returned.
    void initForCommandAck(Command *cmd, uint16_t val);

    //! \brief Prepare the entry for a command return value.
    //! \param cmd Pointer to the Command returning a value.
    //! \param val The value being returned.
    void initForCommandReturn(Command *cmd, Value const &val);

    //! \brief Prepare the entry for a command abort acknowledgement.
    //! \param cmd Pointer to the Command whose abort is being acknowledged.
    //! \param ack The acknowledgement value.
    void initForCommandAbort(Command *cmd, bool ack);

    //! \brief Prepare the entry for a planner update acknowledgement.
    //! \param upd Pointer to the Update being acknowledged.
    //! \param ack The acknowledgement value.
    void initForUpdateAck(Update *upd, bool ack);

    //! \brief Prepare the entry for a new plan.
    //! \param p Pointer to the root Node of the plan.
    void initForAddPlan(NodeImpl *p);

    //! \brief Prepare the entry for a mark.
    //! \param seq The serial number of the mark.
    void initForMark(unsigned int seq);

  };

} // namespace PLEXIL

#endif // PLEXIL_QUEUE_ENTRY_HH
