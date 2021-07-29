# The PLEXIL Executive Application Framework {#app-framework}

\namespace PLEXIL
\addtogroup App-Framework The PLEXIL Executive Application Framework

In order to do useful things with PLEXIL, there must be a way to
effect commands, and to inquire about the environment.  This is
the job of the Application Framework module.

## Key concepts

* The core functionality of a PLEXIL application is the responsibility
of the [ExecApplication] class.  It constructs the Executive and the
interfaces, serializes access to the Executive's internal data, and
runs the main loop.

* The Executive sends commands to, and receives lookup and command
return data from, the [InterfaceManager] class. The InterfaceManager,
in turn, delegates commands and lookups to

* instances of the [InterfaceAdapter] abstract base class;

* or to *command and lookup handlers* (new in PLEXIL 4.6).

* The API the InterfaceManager presents to interface adapters and
handlers is specified by the [AdapterExecInterface] abstract base
class.

* Node state changes in the Executive can be monitored and reported
through instances of the [ExecListener] abstract base class.

* The [AdapterConfiguration] class manages the construction of the
interface objects from a configuration file loaded at runtime.

## Standard interface library

Most every real-world application needs a sense of time.  The PLEXIL
Application Framework provides a TimeAdapter using standard POSIX
APIs to read the current time, and to schedule wakeups at a future
time.

Similarly, most every application will need to be debugged.  The
UtilityAdapter implements the `print()` and `pprint()` commands.

A PLEXIL plan can start, monitor, and terminate additional plans
through the Launcher adapter.

## Abstract factory class templates

The AdapterConfiguration class uses abstract factories to create the
interface objects.  [AdapterFactory], [ExecListenerFactory], and
[ExecListenerFilterFactory] are class templates which facilitate the
construction of user-developed interface objects.

## IMPORTANT NOTE

The interface APIs described here are transitional, an interim
step from the previous adapter-focused API to a handler-based API.
The InterfaceManager already delegates commands and lookups to
InterfaceAdapter instances via table lookups; the adapters would
then do their own internal delegation to the specific interface
functions.  The PLEXIL team decided this second-level delegation
was redundant.  We have refactored the interface API to allow
users to directly associate command and lookup names with handler
functions, so that requests can go directly from the
InterfaceManager to the external functions.

Applications based on the old InterfaceAdapter class API should
continue to work in this release with nothing more than
recompilation.  However, future releases of the PLEXIL Executive
will no longer offer the
[InterfaceAdapter::executeCommand()](@ref InterfaceAdapter::executeCommand)
and
[InterfaceAdapter::lookupNow()](@ref InterfaceAdapter::lookupNow)
member functions, among several
others.  These interfaces are redundant in the handler-centric
interfacing paradigm.  As such, they are marked as deprecated in
this release.

We suggest that developers of new applications use the
handler-based approach, and that maintainers of existing
applications transition to the handler-based approach as time
permits.

[ExecApplication]: @ref ExecApplication   "ExecApplication class"
[InterfaceManager]: @ref InterfaceManager "InterfaceManager class"
[InterfaceAdapter]: @ref InterfaceAdapter "InterfaceAdapter class"
[AdapterExecInterface]: @ref AdapterExecInterface "AdapterExecInterface class"
[ExecListener]: @ref ExecListener "ExecListener class"
[AdapterConfiguration]: @ref AdapterConfiguration "AdapterConfiguration class"
[AdapterFactory]: @ref AdapterFactory "AdapterFactory class"
[ExecListenerFactory]: @ref ExecListenerFactory "ExecListenerFactory class"
[ExecListenerFilterFactory]: @ref ExecListenerFilterFactory "ExecListenerFilterFactory class"
[InterfaceAdapter]: @ref InterfaceAdapter "InterfaceAdapter class"
