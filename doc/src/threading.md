\page threading Threading utilities

\namespace PLEXIL
\ingroup Utils

The following multithreading utilities are provided in %PLEXIL 4.6.

\note All but ThreadSemaphore.hh are replaced in %PLEXIL 6 by C++
Standard Library facilities.

+ File ThreadSpawn.hh

  - Function `threadSpawn`

    Uses the POSIX pthreads API to create a thread with an initial
    function of one argument.  If successful, stores the thread ID in
    the given variable.

    This functionality is replaced by the `std::thread` class in %PLEXIL 6.

+ File ThreadMutex.hh

  - Class `ThreadMutex`
  
    A simple Boolean mutex.

    This class is replaced by `std::mutex` in %PLEXIL 6.
    
  - Class `ThreadMutexGuard`
  
    A guard object for `ThreadMutex`.  Its constructor locks the given
    mutex; its destructor unlocks the mutex.
    
    This class is replaced by `std::lock_guard<std::mutex>` in %PLEXIL 6.

+ File RecursiveThreadMutex.hh

  - Class `RecursiveThreadMutex`
  
    A simple Boolean mutex which may be locked repeatedly by the same
    thread.  It must then be unlocked the same number of times by that
    thread before it is released.

    This class is replaced by `std::recursive_mutex` in %PLEXIL 6.

  - Class `RecursiveThreadMutexGuard`
  
    A guard object for `RecursiveThreadMutex`.  Its constructor locks
    the given mutex; its destructor unlocks the mutex.
    
    This class is replaced by `std::lock_guard<std::recursive_mutex>` in %PLEXIL 6.

+ File ThreadSemaphore.hh

  - Class `ThreadSemaphore`
  
    A simple binary semaphore.  Uses POSIX pthread semaphores where
    supported.  Uses Mach semaphores on macOS, and any other platforms
    supporting the Mach semaphore API.
