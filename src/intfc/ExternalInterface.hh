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

#ifndef PLEXIL_EXTERNAL_INTERFACE_HH
#define PLEXIL_EXTERNAL_INTERFACE_HH

#include "CommandHandle.hh"
#include "Expression.hh"
#include "LinkedQueue.hh"
#include "State.hh"

namespace PLEXIL {
  // Forward declarations
  class Command;
  class StateCacheEntry;
  class ResourceArbiterInterface;
  class Update;

  //! \class ExternalInterface
  //! \brief Abstract base class for anything that interfaces the Exec to the outside world.
  //! \addtogroup External-Interface The %PLEXIL Executive core external interface
  class ExternalInterface {
  public:

    //! \brief Virtual destructor.
    virtual ~ExternalInterface();

    //
    // API to Lookup and StateCacheEntry
    //

    //
    // The cycle counter is used by the Lookup interface to check whether a value is stale.
    // It is incremented by the PlexilExec.
    //

    //! \brief Return the number of "macro steps" since this instance was constructed.
    //! \return The macro step count.
    unsigned int getCycleCount() const;

    //! \brief Perform an immediate lookup on an existing state.
    //! \param state Const reference to the state.
    //! \param cacheEntry Reference to the corresponding entry in the state cache.
    //! \note Value is returned via callback on StateCacheEntry.
    virtual void lookupNow(State const &state, StateCacheEntry &cacheEntry) = 0;

    //! \brief Inform the interface that it should report changes in value of this state.
    //! \param state Const reference to the state.
    virtual void subscribe(const State& state) = 0;

    //! \brief Inform the interface that a lookup should no longer receive updates.
    //! \param state Const reference to the state.
    virtual void unsubscribe(const State& state) = 0;

    //! \brief Advise the interface of the current thresholds to use when reporting this state.
    //! \param state The state.
    //! \param hi The upper threshold, at or above which to report changes.
    //! \param lo The lower threshold, at or below which to report changes.
    virtual void setThresholds(const State& state, double hi, double lo) = 0;

    //! \brief Advise the interface of the current thresholds to use when reporting this state.
    //! \param state The state.
    //! \param hi The upper threshold, at or above which to report changes.
    //! \param lo The lower threshold, at or below which to report changes.
    virtual void setThresholds(const State& state, int32_t hi, int32_t lo) = 0;

    //
    // API to Node classes
    //

    // Made virtual for convenience of module tests

    //! \brief Schedule this command for execution.
    //! \param cmd Pointer to the command.
    virtual void enqueueCommand(Command *cmd);

    //! \brief Abort the pending command.
    //! \param cmd Pointer to the command.
    virtual void abortCommand(Command *cmd);

    //! \brief Schedule an update for execution.
    //! \param update Pointer to the update.
    virtual void enqueueUpdate(Update *update);

    //
    // API to Command
    //
    
    //! \brief Release resources in use by the command.
    //! \param cmd Pointer to the command.
    void releaseResourcesForCommand(Command *cmd);

    //
    // API to Exec
    //

    //! \brief Send all pending commands and updates to the external system(s).
    virtual void executeOutboundQueue();

    //! \brief Query if the command and update queues are empty.
    //! \return True if both empty, false otherwise.
    bool outboundQueueEmpty() const;

    //! \brief Get the most recent value of the current time.
    //! \return The current time, as a double (Real).
    virtual double currentTime() = 0;

    //! \brief Increment the macro step count and return the new value.
    //! \return The updated macro step count.
    unsigned int incrementCycleCount();

    //
    // Interface from outside world to plan state
    //

    //! \brief Return a new value from a lookup.
    //! \param state Const reference to the state.
    //! \param value Const reference to the value.
    void lookupReturn(State const &state, Value const &value);

    //! \brief Return a value from a command
    //! \param cmd Pointer to the Command.
    //! \param value Const reference to the value.
    void commandReturn(Command *cmd, Value const &value);

    //! \brief Return a command handle value for a command.
    //! \param cmd Pointer to the Command.
    //! \param val The command handle value.
    void commandHandleReturn(Command *cmd, CommandHandleValue val);

    //! \brief Return an abort-acknowledge value for a command.
    //! \param cmd Pointer to the Command.
    //! \param ack The acknowledgement value.
    void commandAbortAcknowledge(Command *cmd, bool ack);

    //! \brief Return an update acknowledgment value
    //! \param upd Pointer to the update.
    //! \param val The ack value.
    void acknowledgeUpdate(Update *upd, bool val);

    //
    // API to application
    //
    
    //! \brief Read command resource hierarchy from the named file.
    //! \param fname File name.
    //! \return True if successful, false otherwise.
    bool readResourceFile(std::string const &fname);

  protected:

    //! \brief Default constructor.
    //! \note Protected; may only be called from derived classes.
    ExternalInterface();

    //
    // Member functions to be implemented by derived classes
    //

    //! \brief Report a command arbitration failure in the appropriate way for the application.
    //! \param cmd Pointer to the command.
    virtual void reportCommandArbitrationFailure(Command *cmd) = 0;

    //! \brief Execute a command.
    //! \param cmd Pointer to the command.
    virtual void executeCommand(Command *cmd) = 0;

    //! \brief Abort a command in execution.
    //! \param cmd Pointer to the command.
    virtual void invokeAbort(Command *cmd) = 0;

    //! \brief Execute this update.
    //! \param update Pointer to the update.
    virtual void executeUpdate(Update *update) = 0;

  private:

    // Copy, assign disallowed
    ExternalInterface(ExternalInterface const &);
    ExternalInterface &operator=(ExternalInterface const &);

    //! \brief The outgoing update queue.
    LinkedQueue<Update> m_updatesToExecute;

    //! \brief The outgoing command queue.
    LinkedQueue<Command> m_commandsToExecute;

    //! \brief Pointer to the resource arbiter.
    ResourceArbiterInterface *m_raInterface;

    //! \brief Macro step counter for the Exec.
    unsigned int m_cycleCount;
  };

  //! \brief Global variable pointing to the ExternalInterface instance.
  //! \ingroup External-Interface
  extern ExternalInterface *g_interface;
}

#endif
