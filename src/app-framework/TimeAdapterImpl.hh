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

#ifndef PLEXIL_TIME_ADAPTER_IMPL_HH
#define PLEXIL_TIME_ADAPTER_IMPL_HH

#include "plexil-config.h"

#include "InterfaceAdapter.hh"
#include "InterfaceError.hh"

#ifdef PLEXIL_WITH_THREADS
#include "ThreadMutex.hh"
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif // HAVE_PTHREAD_H
#endif // PLEXIL_WITH_THREADS

// Which clock_gettime setting to use

#if defined(HAVE_CLOCK_GETTIME)
#if defined(CLOCK_REALTIME)
#define PLEXIL_CLOCK_GETTIME CLOCK_REALTIME
#elif defined(CLOCK_MONOTONIC)
#define PLEXIL_CLOCK_GETTIME CLOCK_MONOTONIC
#else
#error "clock_gettime() is defined, but not CLOCK_MONOTONIC or CLOCK_REALTIME"
#endif // defined(CLOCK_REALTIME)_
#endif // defined(HAVE_CLOCK_GETTIME)

#include "plexil-signal.h" // sigset_t

namespace PLEXIL
{
  // Forward reference
  class AdapterConfiguration;
  
  //! \class TimeAdapterImpl
  //! \brief Abstract base class for implementing Lookups of 'time' state,
  //!        including scheduling Exec wakeups.
  //!
  //! TimeAdapterImpl implements the (mostly) platform-independent
  //! functions required for a time interface in PLEXIL.  It presumes
  //! a somewhat POSIX-compliant platform.  Derived classes implement
  //! functionality which is platform dependent.
  //! \note This functionality is reimplemented in a completely different
  //!       fashion in PLEXIL 6.
  //! \see DarwinTimeAdapter
  //! \see PosixTimeAdapter
  //! \ingroup interface-library
  class TimeAdapterImpl : public InterfaceAdapter
  {
    // The lookup handler needs access to some protected member functions
    friend class TimeLookupHandler;

  public:

    //! \brief Constructor.
    //! \param mgr Reference to parent AdapterExecInterface.
    TimeAdapterImpl(AdapterExecInterface &mgr);

    //! \brief Constructor with configuration XML.
    //! \param mgr Reference to parent AdapterExecInterface.
    //! \param config Const handle to configuration XML.
    TimeAdapterImpl(AdapterExecInterface &mgr,
                    pugi::xml_node const config);

    //! \brief Virtual destructor.
    virtual ~TimeAdapterImpl();

    //
    // Public InterfaceAdapter API
    //

    //
    // API to ExecApplication
    //

    //! \brief Initializes the adapter, possibly using its configuration data.
    //! \return true if successful, false otherwise.
    virtual bool initialize(AdapterConfiguration *config);

    //! \brief Starts the adapter, possibly using its configuration data.  
    //! \return true if successful, false otherwise.
    virtual bool start();

    //! \brief Stops the adapter.  
    //! \return true if successful, false otherwise.
    virtual bool stop();

    //! \brief Shuts down the adapter, releasing any of its resources.
    //! \return true if successful, false otherwise.
    virtual bool shutdown();

  protected:

    //
    // Generic internal functions which should not need to be overridden.
    // (Famous last words.)
    //

    //! \brief Set a wakeup at the given time.
    //! \param date The Unix-epoch wakeup time, as a double.
    void setNextWakeup(double date);

    //
    // Internal functions
    //

    //! \brief Get the current time from the operating system.
    //! \return A double representing the current time.
    //! \note Default method uses clock_gettime() or gettimeofday() as available.
    //!       Derived classes may override this method.
    virtual double getCurrentTime();

    //! \brief Initialize signal handling for the process.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool configureSignalHandling() = 0;

    //! \brief Construct and initialize the timer as required.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool initializeTimer() = 0;

    //! \brief Set the timer in an implementation-dependent way.
    //! \param date The Unix-epoch wakeup time, as a double.
    //! \return true if the timer was set, false if clock time had already passed the wakeup time.
    //! \note Derived classes must implement this method.
    virtual bool setTimer(double date) = 0;

    //! \brief Stop the timer in an implementation-dependent way.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool stopTimer() = 0;

    //! \brief Shut down and delete the timer as required.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool deleteTimer() = 0;

    //! \brief Initialize the wait thread signal mask.
    //! \param mask Pointer to the thread's mask.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool configureWaitThreadSigmask(sigset_t* mask) = 0;

    //! \brief Initialize the sigwait mask.
    //! \param mask Pointer to the process's mask.
    //! \return true if successful, false otherwise.
    //! \note Derived classes must implement this method.
    virtual bool initializeSigwaitMask(sigset_t* mask) = 0;

  private:

    // Not implemented
    TimeAdapterImpl();
    TimeAdapterImpl(TimeAdapterImpl const &);
    TimeAdapterImpl &operator=(TimeAdapterImpl const &);
    
    //! \brief Report the current time to the Exec as an asynchronous lookup value.
    void timerTimeout();

#ifdef PLEXIL_WITH_THREADS
    //! \brief Static member function which waits for timer wakeups.
    //! \param this_as_void_ptr Pointer to the TimeAdapterImpl instance, as a void *.
    //! \return Ignored.
    static void* timerWaitThread(void* this_as_void_ptr);

    //! \brief Internal function for the above.
    //! \return Ignored.
    virtual void* timerWaitThreadImpl();

    //
    // Member variables
    //

    pthread_t m_waitThread;   //!< The timer wait thread.
    ThreadMutex m_timerMutex; //!< Mutex to serialize access to timer resources.
#endif

    double m_nextWakeup;      //!< Time of the next scheduled wakeup.

    bool m_stopping;          //!< Flag to command timer wait thread to exit.a

  }; // class TimeAdapterImpl

} // namespace PLEXIL

#endif // PLEXIL_TIME_ADAPTER_IMPL_HH
