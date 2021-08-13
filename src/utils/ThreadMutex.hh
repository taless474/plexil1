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

#ifndef THREAD_MUTEX_HEADER_
#define THREAD_MUTEX_HEADER_

#include "plexil-config.h"

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

namespace PLEXIL
{
  //! \class ThreadMutex
  //! \brief Implements a simple mutex based on POSIX pthread_mutex_t with
  //!        modest convenience features.
  //! \note Users of this class should not depend on ThreadMutex detecting deadlocks,
  //!       multiple calls to lock() from the same thread, or unlock-while-unlocked.
  //! \deprecated Superseded by std::mutex in PLEXIL 6.
  //! \ingroup Utils
  class ThreadMutex
  {
  public:
    /**
     * \brief Default constructor.
     */
    ThreadMutex();

    /**
     * \brief Destructor.
     */
    ~ThreadMutex();

    /**
     * \brief Acquire the mutex.
     *
     *  If the mutex is held by another thread, the current thread is
     *  blocked until the other thread releases the mutex.
     */
    void lock();

    /**
     * \brief Try to acquire the mutex for the current thread.
     * \return true if successful, false if the mutex is already held by any thread.
     */
    bool trylock();

    /**
     * \brief Release the mutex.
     */
    void unlock();
    
  private:

    // deliberately unimplemented
    ThreadMutex( const ThreadMutex& );
    const ThreadMutex& operator=( const ThreadMutex& );

    pthread_mutex_t m_mutex; //!< The platform's native mutex.
  };

  //! \class ThreadMutexGuard
  //! \brief A guard object for use with ThreadMutex.  Locks the mutex
  //!        when constructed; unlocks the mutex when destroyed.
  //! \note Meant to be used as a stack-allocated local variable.  The
  //!       destructor will be called automatically when the variable
  //!       context is exited.
  //! \see ThreadMutex
  //! \deprecated Replaced by std::lock_guard in PLEXIL 6.
  //! \ingroup Utils
  class ThreadMutexGuard
  {
  public:

    //! \brief Constructor from a mutex reference.  Locks the mutex.
    //! \param Reference to a ThreadMutex instance.
    ThreadMutexGuard(ThreadMutex& mutex);

    //! \brief Destructor. Unlocks the mutex.
    ~ThreadMutexGuard();

  private:

    // Default and copy constructors deliberately unimplemented.
    ThreadMutexGuard();
    ThreadMutexGuard(const ThreadMutexGuard &);

    // Copy assignment deliberately unimplemented.
    const ThreadMutexGuard& operator=(const ThreadMutexGuard &);

    ThreadMutex& m_mutex; //!< The ThreadMutex instance being guarded.
  };
}

#endif //THREAD_MUTEX_HEADER_
