/* Copyright (c) 2006-2021, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_INTERFACE_MANAGER_HH
#define PLEXIL_INTERFACE_MANAGER_HH

#include "plexil-config.h"

#include "ExternalInterface.hh"
#include "AdapterExecInterface.hh"

#include <map>

// Forward reference
namespace pugi
{
  class xml_node;
  class xml_document;
}

namespace PLEXIL 
{
  // Forward references
  class ExecApplication;
  class InputQueue;

  //! \class InterfaceManager
  //! \brief A concrete class implementing the APIs of the ExternalInterface and AdapterExecInterface classes.

  //! The InterfaceManager class is responsible for keeping track of
  //! all the external interface adapters used by the PlexilExec.  It
  //! maintains a queue of messages for the Exec to process.
  //! Interface instantiation, initialization, startup, stopping,
  //! shutdown, and deallocation are all handled by the
  //! InterfaceManager instance.

  //! \note Supersedes the old ThreadedExternalInterface class.

  //! \see AdapterExecInterface
  //! \see ExecApplication
  //! \see ExternalInterface
  //! \ingroup App-Framework

  class InterfaceManager :
    public ExternalInterface,
    public AdapterExecInterface
  {
  public:

    //! \brief Constructor.
    //! \param app The ExecApplication instance to which this object belongs.
    InterfaceManager(ExecApplication & app);

    //! \brief Virtual destructor.
    virtual ~InterfaceManager();

    //! \brief Get parent application.
    //! \return Reference to the ExecApplication.
    inline ExecApplication& getApplication() const
    {
      return m_application;
    }

    //
    // ExternalInterface public API
    //
    
    //! \brief Perform an immediate lookup on an existing state.
    //! \param state Const reference to the state.
    //! \param cacheEntry Reference to the StateCacheEntry which will accept the new value.
    virtual void lookupNow(State const &state, StateCacheEntry &cacheEntry);

    //! \brief Inform the interface that it should report changes in value of this state.
    //! \param state Const reference to the state.
    virtual void subscribe(const State& state);

    //! \brief Inform the interface that a lookup should no longer receive updates.
    //! \param state Const reference to the state.
    virtual void unsubscribe(const State& state);

    //! \brief Advise the interface of the current thresholds to use when reporting this state.
    //! \param state Const reference to the state.
    //! \param hi The upper threshold, at or above which to report changes.
    //! \param lo The lower threshold, at or below which to report changes.
    virtual void setThresholds(const State& state, double hi, double lo);

    //! \brief Advise the interface of the current thresholds to use when reporting this state.
    //! \param state Const reference to the state.
    //! \param hi The upper threshold, at or above which to report changes.
    //! \param lo The lower threshold, at or below which to report changes.
    virtual void setThresholds(const State& state, int32_t hi, int32_t lo);

    //! \brief Return the most recently cached value of the current time.
    //! \return The cached time, as a double float.
    virtual double currentTime();

    //
    // API for all related objects
    //

    //
    // AdapterExecInterface public API
    //

    //! \brief Notify of the availability of a new value for a lookup.
    //! \param state Const reference to the state for the new value.
    //! \param value Const reference to the new value.
    virtual void handleValueChange(const State& state, const Value& value);

    //! \brief Notify of the availability of a command handle value for a command.
    //! \param cmd Pointer to the Command instance.
    //! \param value The new CommandHandleValue.
    virtual void handleCommandAck(Command * cmd, CommandHandleValue value);

    //! \brief Notify of the availability of a return value for a command.
    //! \param cmd Pointer to the Command instance.
    //! \param value Const reference to the new value.
    virtual void handleCommandReturn(Command * cmd, Value const& value);

    //! \brief Notify of the availability of a command abort acknowledgment.
    //! \param cmd Pointer to the Command instance.
    //! \param ack The acknowledgment value.
    virtual void handleCommandAbortAck(Command * cmd, bool ack);

    //! \brief Notify of the availability of a planner update acknowledgment.
    //! \param upd Pointer to the Update instance.
    //! \param ack The acknowledgment value.
    virtual void handleUpdateAck(Update * upd, bool ack);

    //! \brief Notify the executive of a new plan.
    //! \param planXml The pugixml representation of the new plan.
    virtual void handleAddPlan(pugi::xml_node const planXml);

    //! \brief Notify the executive of a new library node.
    //! \param doc Pointer to the XML document containing the new library node.
    //! \return True if successful, false otherwise.
    //! \note The Exec takes ownership of the xml_document.
    virtual bool handleAddLibrary(pugi::xml_document *doc);

    //! \brief Notify the executive that it should run one cycle.
    void notifyOfExternalEvent();

#ifdef PLEXIL_WITH_THREADS
    //! \brief Run the exec. Return only after all events in the queue have been processed.
    void notifyAndWaitForCompletion();
#endif

    //! \brief Query the appropriate interface to get the current time.
    //! \return Seconds since the epoch as a double float.
    double queryTime();

    //! \brief Associate an arbitrary object with a string.
    //! \param name Const reference to the string naming the property.
    //! \param thing The property value as an untyped pointer.
    virtual void setProperty(const std::string& name, void * thing);

    //! \brief Fetch the named property.
    //! \param name Const reference to the string naming the property.
    //! \return The property value as an untyped pointer.
    virtual void* getProperty(const std::string& name);

    //
    // InterfaceManager public API for ExecApplication
    //

    //! \brief Performs basic initialization of the interface and all adapters.
    //! \return true if all initializations were successful, false otherwise.
    virtual bool initialize();

    //! \brief Prepares the interface and adapters for execution.
    //! \return true if successful, false otherwise.
    virtual bool start();

    //! \brief Halts all interfaces.
    //! \return true if successful, false otherwise.
    virtual bool stop();

    //! \brief Resets the interface prior to restarting.
    //! \return true if successful, false otherwise.
    virtual bool reset();

    //! \brief Shuts down the interface.
    //! \return true if successful, false otherwise.
    virtual bool shutdown();
    
    //! \brief Updates the state cache from the items in the queue.
    //! \return True if the exec needs to be stepped, false otherwise.
    //! \note Should only be called with exec locked by the current thread.
    bool processQueue();

    //! \brief Insert a mark in the value queue.
    //! \return The sequence number of the mark.
    unsigned int markQueue();

    //! \brief Get the sequence number of the most recently processed mark.
    //! \return The sequence number, 0 if no marks have yet been processed.
    unsigned int getLastMark() const
    {
      return m_lastMark;
    }

    //! \brief Load the named library from the library path.
    //! \param libName Const reference to string naming the library.
    //! \return true if successful, false otherwise.
    bool handleLoadLibrary(std::string const &libName);

    //! \brief Determine whether the named library is loaded.
    //! \param libName Const reference to string naming the library.
    //! \return True if loaded, false otherwise.
    bool isLibraryLoaded(const std::string& libName) const;

  protected:

    //
    // Protected Implementation API for ExternalInterface
    //

    //! \brief Report a command arbitration failure in the appropriate way for the application.
    //! \param cmd Pointer to the command.
    virtual void reportCommandArbitrationFailure(Command *cmd);

    //! \brief Schedule this command for execution.
    //! \param cmd Pointer to the Command instance.
    virtual void executeCommand(Command *cmd);

    //! \brief Abort an executing command.
    //! \param cmd Pointer to the Command instance.
    void invokeAbort(Command *cmd);

    //! \brief Schedule this update for execution.
    //! \param update Pointer to the Update instance.
    void executeUpdate(Update *update);

    //
    // Internal functionality
    //

    //! \brief Reject a command due to non-availability of resources.
    //! \param cmd Pointer to the Command instance.
    void rejectCommand(Command *cmd);

    friend class AdapterConfiguration;

  private:

    // Deliberately unimplemented
    InterfaceManager();
    InterfaceManager(const InterfaceManager &);
    InterfaceManager & operator=(const InterfaceManager &);

    //
    // Internal types and classes
    //

    //
    // Private member variables
    //

    //* Parent object
    ExecApplication& m_application;

    // Properties
    typedef std::map<const std::string, void*> PropertyMap;
    PropertyMap m_propertyMap;

    //* The queue
    InputQueue *m_inputQueue;

    //* Holds the most recent idea of the current time
    double m_currentTime;

    //* Most recent mark processed.
    unsigned int m_lastMark;

    //* Last mark enqueued.
    unsigned int m_markCount;
  };

}

#endif // PLEXIL_INTERFACE_MANAGER_HH
