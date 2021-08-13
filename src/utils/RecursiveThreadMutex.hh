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

// -*- Mode: C++ -*-
//      CVS: $Id: RecursiveThreadMutexs.hh,v 1.2 2006/10/27 22:21:22 miatauro Exp $
//      Tag: $name$
//     Info: 

#ifndef RECURSIVE_THREAD_MUTEX_HEADER_
#define RECURSIVE_THREAD_MUTEX_HEADER_

#include "plexil-config.h"

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

namespace PLEXIL
{
  //! \class RecursiveThreadMutex
  //! \brief Implements a mutex based on POSIX pthread_mutex_t which
  //!        supports multiple symmetrical lock/unlock calls by the
  //!        same thread.
  //! \deprecated Superseded by std::recursive_mutex in PLEXIL 6.
  //! \ingroup Utils
  class RecursiveThreadMutex
  {
  public:
    /**
     * \brief Default constructor.
     */
    RecursiveThreadMutex();

    /**
     * \brief Destructor.
     */
    ~RecursiveThreadMutex();

    /**
     * \brief Ensure the mutex is acquired.
     *
     * If the mutex is available, acquires it and sets m_lockCount to
     * 1.  If already held by this thread, increments m_lockCount.  If
     * held by another thread, blocks the current thread until the
     * mutex is released.
     */
    void lock();

    /**
     * \brief "Release" the mutex.  Undo one call to lock().
     *
     * Decrements m_lockCount.  When the count reaches 0, the mutex is
     * released.
     *
     * \note If the mutex is held by another thread, signals an error.
     */
    void unlock();

    /**
     * \brief Is this mutex currently held by any thread?
     * \return true if some thread currently holds the mutex, false otherwise.
     * \note Slight chance of race condition between check of lock status and locking thread.
     */
    inline bool isLocked()
    {
      return !pthread_equal(m_lockingThread, (pthread_t) 0);
    }

    /**
     * \brief Is this mutex held by the current thread?
     * \return true if the current thread holds the mutex, false if not.
     */
    inline bool isLockedByCurrentThread()
    {
      return pthread_equal(m_lockingThread, pthread_self());
    }
    
  private:

    // deliberately unimplemented
    RecursiveThreadMutex( const RecursiveThreadMutex& );
    const RecursiveThreadMutex& operator=( const RecursiveThreadMutex& );

    pthread_mutex_t m_mutex; //!< The platform's native mutex.
    pthread_t m_lockingThread; //!< The thread currently holding the mutex.
    int m_lockCount; //!< The number of times the current thread has called lock().
  };

  //! \class RTMutexGuard
  //! \brief A guard object for use with RecursiveThreadMutex.  Locks
  //!        the mutex when constructed; unlocks the mutex when destroyed.
  //! \note Meant to be used as a stack-allocated local variable.  The
  //!       destructor will be called automatically when the variable
  //!       context is exited.
  //! \see RecursiveThreadMutex
  //! \deprecated Replaced by std::lock_guard in PLEXIL 6.
  //! \ingroup Utils
  class RTMutexGuard
  {
  public:

    //! \brief Constructor from a mutex reference.  Locks the mutex.
    //! \param Reference to a RecursiveThreadMutex instance.
    RTMutexGuard(RecursiveThreadMutex& mutex):
      m_mutex( mutex )
    {
      m_mutex.lock();
    }

    //! \brief Destructor. Unlocks the mutex.
    ~RTMutexGuard( )
    {
      m_mutex.unlock();
    }

  private:

    // Default and copy constructors deliberately unimplemented.
    RTMutexGuard();
    RTMutexGuard(const RTMutexGuard &);

    // Copy assignment deliberately unimplemented.
    const RTMutexGuard& operator=(const RTMutexGuard &);

    RecursiveThreadMutex& m_mutex; //!< The RecursiveThreadMutex being guarded.
  };
}

#endif //RECURSIVE_THREAD_MUTEX_HEADER_
