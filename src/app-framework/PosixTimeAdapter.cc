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

//
// Time adapter implementation for platforms with the POSIX timer_create() function
//

#include "plexil-config.h"

#ifdef HAVE_TIMER_CREATE

#include "AdapterFactory.hh"
#include "AdapterExecInterface.hh"
#include "Debug.hh"
#include "InterfaceError.hh"
#include "TimeAdapterImpl.hh"
#include "timespec-utils.hh"

#include <iomanip>

#include "plexil-errno.h"
#include "plexil-time.h"

namespace PLEXIL
{

  //! \class PosixTimeAdapter
  //! \brief An interface adapter using standard POSIX time facilities
  //!        to implement LookupNow and LookupOnChange for the 'time' state.
  //! \ingroup interface-library
  class PosixTimeAdapter : public TimeAdapterImpl
  {
  public:

    //! \brief Constructor.
    //! \param execInterface Reference to the parent AdapterExecInterface object.
    PosixTimeAdapter(AdapterExecInterface& execInterface)
      : TimeAdapterImpl(execInterface)
    {
    }

    //! \brief Constructor from configuration XML.
    //! \param execInterface Reference to the parent AdapterExecInterface object.
    //! \param xml A const reference to the XML element describing this adapter
    PosixTimeAdapter(AdapterExecInterface& execInterface, 
                     pugi::xml_node const xml)
      : TimeAdapterImpl(execInterface, xml)
    {
    }

    //! \brief Virtual destructor.
    virtual ~PosixTimeAdapter()
    {
    }

  protected:

    //! \brief Initialize signal handling for the process.
    //! \return true if successful, false otherwise.
    virtual bool configureSignalHandling()
    {
      // Mask SIGUSR1 at the process level
      sigset_t mask;
      if (sigemptyset(&mask)) {
        warn("TimeAdapter: sigemptyset failed!");
        return false;
      }
    
      if (sigaddset(&mask, SIGUSR1)) {
        warn("TimeAdapter: sigaddset failed!");
        return false;
      }
    
      if (sigprocmask(SIG_BLOCK, &mask, NULL)) {
        warn ("TimeAdapter: sigprocmask failed, errno = " << errno);
        return false;
      }
      return true;
    }

    //! \brief Construct and initialize the timer as required.
    //! \return true if successful, false otherwise.
    virtual bool initializeTimer()
    {
      // Initialize sigevent
      m_sigevent.sigev_notify = SIGEV_SIGNAL;
      m_sigevent.sigev_signo = SIGUSR1; // was SIGALRM
      m_sigevent.sigev_value.sival_int = 0;
      m_sigevent.sigev_notify_function = NULL;
      m_sigevent.sigev_notify_attributes = NULL;

      // Create a timer
      if (timer_create(PLEXIL_CLOCK_GETTIME,
                       &m_sigevent,
                       &m_timer)) {
        warn("TimeAdapter: timer_create failed, errno = " << errno);
        return false;
      }
      return true;
    }

    //! \brief Set the timer in an implementation-dependent way.
    //! \param date The Unix-epoch wakeup time, as a double.
    //! \return true if the timer was set, false if clock time had already passed the wakeup time.
    virtual bool setTimer(double date)
    {
      // Get the wakeup time into the format timer_settime wants.
      itimerspec tymrSpec = {{0, 0}, {0, 0}};
      tymrSpec.it_value = doubleToTimespec(date);
      
      // Get the current time
      timespec now;
      checkInterfaceError(0 == clock_gettime(PLEXIL_CLOCK_GETTIME, &now), 
                          "TimeAdapter::setTimer: clock_gettime() failed, errno = "
                          << errno);

      // Have we missed the deadline already?
      if (tymrSpec.it_value < now) {
        // Already past the scheduled time
        debugMsg("TimeAdapter:setTimer",
                 " new value " << std::setprecision(15) << date << " is in past");
        return false;
      }

      checkInterfaceError(0 == timer_settime(m_timer,
                                             TIMER_ABSTIME, // flags
                                             &tymrSpec,
                                             NULL),
                          "TimeAdapter::setTimer: timer_settime failed, errno = "
                          << errno);

      debugMsg("TimeAdapter:setTimer",
               " timer set for "
               << std::setprecision(15) << timespecToDouble(tymrSpec.it_value));

      return true;
    }

    //! \brief Stop the timer in an implementation-dependent way.
    //! \return true if successful, false otherwise.
    virtual bool stopTimer()
    {
      static itimerspec sl_tymrDisable = {{0, 0}, {0, 0}};
      int status = timer_settime(m_timer,
                                 0,
                                 &sl_tymrDisable,
                                 NULL);
      condDebugMsg(status != 0,
                   "TimeAdapter:stopTimer",
                   " timer_settime() failed, errno = " << errno);
      condDebugMsg(status == 0,
                   "TimeAdapter:stopTimer", " succeeded");
      return status == 0;
    }

    //! \brief Shut down and delete the timer as required.
    //! \return true if successful, false otherwise.
    virtual bool deleteTimer()
    {
      int status = timer_delete(m_timer);
      if (status) {
        warn("TimeAdapter: timer_delete failed, errno = " << errno);
      }
      return status == 0;
    }

    //! \brief Initialize the wait thread signal mask.
    //! \param mask Pointer to the thread's mask.
    //! \return true if successful, false otherwise.
    virtual bool configureWaitThreadSigmask(sigset_t* mask)
    {
      if (sigemptyset(mask)) {
        warn("TimeAdapter: sigemptyset failed!");
        return false;
      }

      int errnum = sigaddset(mask, SIGALRM);
      errnum = errnum | sigaddset(mask, SIGINT);
      errnum = errnum | sigaddset(mask, SIGHUP);
      errnum = errnum | sigaddset(mask, SIGQUIT);
      errnum = errnum | sigaddset(mask, SIGTERM);
      errnum = errnum | sigaddset(mask, SIGUSR2);
      if (errnum) {
        warn("TimeAdapter: sigaddset failed!");
      }
      return errnum == 0;
    }

    //! \brief Initialize the sigwait mask.
    //! \param Pointer to the process's mask.
    //! \return True if successful, false otherwise.
    virtual bool initializeSigwaitMask(sigset_t* mask)
    {
      // listen only for SIGUSR1
      if (sigemptyset(mask)) {
        warn("TimeAdapter: sigemptyset failed!");
        return false;
      }
      if (sigaddset(mask, SIGUSR1)) {
        warn("TimeAdapter: sigaddset failed!");
        return false;
      }
      return true;
    }

  private:

    // Deliberately unimplemented
    PosixTimeAdapter();
    PosixTimeAdapter(const PosixTimeAdapter &);
    PosixTimeAdapter & operator=(const PosixTimeAdapter &);

    //
    // Member variables
    //

    sigevent m_sigevent; //!< The timer event.
    timer_t m_timer;     //!< The timer.
  };

  //! \brief Register the PosixTimeAdapter as the class implementing OSNativeTime.
  //! \ingroup interface-library
  void registerTimeAdapter()
  {
    REGISTER_ADAPTER(PosixTimeAdapter, "OSNativeTime");
  }

}

#endif // HAVE_TIMER_CREATE
