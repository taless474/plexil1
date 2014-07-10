/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
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

//
// *** Ignore this file on systems that implement POSIX timers
//

#include "plexil-config.h"
#include <unistd.h>

#if defined(HAVE_SETITIMER) && (!defined(_POSIX_TIMERS) || (((_POSIX_TIMERS - 200112L) < 0L) && !defined(PLEXIL_ANDROID)))

#include "DarwinTimeAdapter.hh"
#include "AdapterExecInterface.hh"
#include "Debug.hh"
#include "Error.hh"
#include "TimeAdapter.hh"
#include "timeval-utils.hh"
#include <cerrno>
#include <cmath> // for modf
#include <mach/kern_return.h> // for KERN_ABORTED
#include <sys/time.h> // for gettimeofday, itimerval

namespace PLEXIL
{
  /**
   * @brief Constructor.
   * @param execInterface Reference to the parent AdapterExecInterface object.
   */
  DarwinTimeAdapter::DarwinTimeAdapter(AdapterExecInterface& execInterface)
    : TimeAdapter(execInterface)
  {
  }

  /**
   * @brief Constructor from configuration XML.
   * @param execInterface Reference to the parent AdapterExecInterface object.
   * @param xml A const reference to the XML element describing this adapter
   * @note The instance maintains a shared pointer to the XML.
   */
  DarwinTimeAdapter::DarwinTimeAdapter(AdapterExecInterface& execInterface, 
                                       const pugi::xml_node& xml)
    : TimeAdapter(execInterface, xml)
  {
  }

  /**
   * @brief Destructor.
   */
  DarwinTimeAdapter::~DarwinTimeAdapter()
  {
  }

  /**
   * @brief Get the current time from the operating system.
   * @return A double representing the current time.
   */
  double DarwinTimeAdapter::getCurrentTime()
  {
    timeval tv;
    int status = gettimeofday(&tv, NULL);
    assertTrueMsg(status == 0,
                  "TimeAdapter:getCurrentTime: gettimeofday() failed, errno = " << errno);
    double tym = timevalToDouble(tv);
    debugMsg("TimeAdapter:getCurrentTime", " returning " << tym);
    return tym;
  }

  /**
   * @brief Initialize signal handling for the process.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::configureSignalHandling()
  {
    // block SIGALRM and SIGUSR1 for the process as a whole
    sigset_t processSigset, originalSigset;
    assertTrueMsg(0 == sigemptyset(&processSigset),
                  "DarwinTimeAdapter::configureSignalHandling: sigemptyset failed!");
    int errnum = sigaddset(&processSigset, SIGALRM);
    errnum = errnum | sigaddset(&processSigset, SIGUSR1);
    assertTrueMsg(errnum == 0,
                  "DarwinTimeAdapter::configureSignalHandling: sigaddset failed!");
    assertTrueMsg(0 == sigprocmask(SIG_BLOCK, &processSigset, &originalSigset),
                  "DarwinTimeAdapter::configureSignalHandling: sigprocmask failed, errno = " << errno);
    return true;
  }

  /**
   * @brief Construct and initialize the timer as required.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::initializeTimer()
  {
    return true; // nothing to do
  }

  /**
   * @brief Set the timer.
   * @param date The Unix-epoch wakeup time, as a double.
   * @return True if the timer was set, false if clock time had already passed the wakeup time.
   */
  bool DarwinTimeAdapter::setTimer(double date)
  {
    // Convert to timeval
    timeval dateval = doubleToTimeval(date);

    // Get the current time
    timeval now;
    int status = gettimeofday(&now, NULL);
    assertTrueMsg(status == 0,
                  "TimeAdapter:setTimer: gettimeofday() failed, errno = " << errno);

    // Compute the interval
    itimerval myItimerval = {{0, 0}, {0, 0}};
    myItimerval.it_value = dateval - now;
    if (myItimerval.it_value.tv_usec < 0 || myItimerval.it_value.tv_sec < 0) {
      // Already past the scheduled time, submit wakeup
      debugMsg("TimeAdapter:setTimer",
               " new value " << date << " is in past");
      return false;
    }

    // Set the timer 
    assertTrueMsg(0 == setitimer(ITIMER_REAL, &myItimerval, NULL),
                  "TimeAdapter:setTimer: setitimer failed, errno = " << errno);
    debugMsg("TimeAdapter:setTimer",
             " timer set for " << date);
    return true;
  }

  /**
   * @brief Stop the timer.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::stopTimer()
  {
    static itimerval const sl_disableItimerval = {{0, 0}, {0, 0}};
    int status = setitimer(ITIMER_REAL, & sl_disableItimerval, NULL);
    condDebugMsg(status != 0,
                 "TimeAdapter:stopTimer",
                 " setitimer() failed, errno = " << errno);
    return status == 0;
  }

  /**
   * @brief Shut down and delete the timer as required.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::deleteTimer()
  {
    return true; // nothing to do
  }

  /**
   * @brief Initialize the wait thread signal mask.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::configureWaitThreadSigmask(sigset_t* mask)
  {
    assertTrue_2(0 == sigemptyset(mask),
                 "DarwinTimeAdapter::configureWaitThreadSigmask: sigemptyset failed!");
    int errnum = sigaddset(mask, SIGINT);
    errnum = errnum | sigaddset(mask, SIGHUP);
    errnum = errnum | sigaddset(mask, SIGQUIT);
    errnum = errnum | sigaddset(mask, SIGTERM);
    errnum = errnum | sigaddset(mask, SIGUSR2);
    assertTrue_2(errnum == 0,
                 "DarwinTimeAdapter::configureWaitThreadSigmask: sigaddset failed!");
    return errnum == 0;
  }

  /**
   * @brief Initialize the sigwait mask.
   * @param Pointer to the mask.
   * @return True if successful, false otherwise.
   */
  bool DarwinTimeAdapter::initializeSigwaitMask(sigset_t* mask)
  {
    // listen for SIGALRM and SIGUSR1
    assertTrue_2(0 == sigemptyset(mask),
                 "DarwinTimeAdapter::initializeSigwaitMask: sigemptyset failed!");
    int status = sigaddset(mask, SIGUSR1);
    status = status | sigaddset(mask, SIGALRM);
    assertTrue_2(0 == status,
                 "DarwinTimeAdapter::initializeSigwaitMask: sigaddset failed!");
    return true;
  }


}

#endif // defined(HAVE_SETITIMER) && (!defined(_POSIX_TIMERS) || (((_POSIX_TIMERS - 200112L) < 0L) && !defined(PLEXIL_ANDROID)))
