\page cleanup-on-exit Cleanup on exit

\ingroup Utils

Programs frequently have a need to perform a laundry list of cleanups
at exit.  These C functions implement a facility to register functions
to be run at exit.

The header file lifecycle-utils.h contains these definitions:

+ Typedef `lc_operator`

  Shorthand for a pointer to a function of no arguments, returning void.

+ Function `plexilAddFinalizer`

  Register the given finalizer function.

+ Function `plexilRunFinalizers`

  Run all the registered finalizers, in last-in, first-out order, and
  clear the registry.  The application should explicitly call this
  function just before exiting.
