# The PLEXIL Executive Application Framework {#app-framework}

\namespace PLEXIL
\addtogroup App-Framework The PLEXIL Executive Application Framework

In order to do useful things with %PLEXIL, the Executive must be able
to inquire about the environment, effect commands, and report
execution status.  The Application Framework module provides hooks to
implement those capabilities.

### Key concepts

* The core functionality of a %PLEXIL application is the responsibility
of the ExecApplication class.  It constructs the Executive and the
interfaces, serializes access to the Executive's internal data, and
runs the main loop.

* The Executive sends commands to, and receives lookup and command
return data from, the InterfaceManager class. The InterfaceManager,
in turn, delegates commands and lookups to

* *adapters*, objects derived from the InterfaceAdapter abstract base
  class;

* or to *command and lookup handlers* (new in %PLEXIL 4.6).

* The API the InterfaceManager presents to interface adapters and
handlers is specified by the AdapterExecInterface abstract base
class.

* Node state changes in the Executive can be monitored and reported
through *listeners*, instances of the ExecListener abstract base
class.

* The AdapterConfiguration class manages the construction of the
interface objects from a configuration file loaded at runtime.

### Standard interface library

A standard set of adapters and listeners, the [%PLEXIL Standard
Interface Library](#interface-library), is part of the %PLEXIL
distribution.  It includes these modules:

+ The TimeAdapter uses standard POSIX APIs to read the current
  time, and to schedule wakeups at a future time.

+ The UtilityAdapter implements printing commands.

+ The Launcher allows %PLEXIL plans to start, monitor, and terminate
  other plans.

+ The IpcAdapter uses a cross-platform, publish/subscribe
  inter-process communication (IPC) package to command and monitor
  external systems, and to publish planner updates.
  
+ The UdpAdapter uses UDP datagrams to command and monitor external
  systems.
  
+ LuvListener allows monitoring %PLEXIL plan execution in real time
  via the %PLEXIL Viewer Java application.

Users can take advantage of these capabilities directly, or use the
source code of these interfaces as templates for implementing their
own custom interfaces.

### AbstractFactory classes for interface objects

The AdapterConfiguration class uses abstract factories to create the
interface objects; AdapterFactory, ExecListenerFactory, and
ExecListenerFilterFactory, and template classes derived from them,
implement the *AbstractFactory* design pattern.  Macros create these
abstract factory classes.

### IMPORTANT NOTE

The interface APIs described here are transitional, an interim
step from the previous adapter-focused API to a handler-based API.
The InterfaceManager already delegates commands and lookups to
InterfaceAdapter instances via table lookups; the adapters would
then do their own internal delegation to the specific interface
functions.  The %PLEXIL team decided this second-level delegation
was redundant.  We have refactored the interface API to allow
users to directly associate command and lookup names with handler
functions, so that requests can go directly from the
InterfaceManager to the external functions.

Applications based on the old InterfaceAdapter class API should
continue to work in this release with nothing more than recompilation.
However, future releases of the %PLEXIL Executive will no longer offer
the InterfaceAdapter::executeCommand and InterfaceAdapter::lookupNow
member functions, among several others.  These member functions are
redundant in the handler-centric interfacing paradigm.  As such, they
are marked as deprecated in this release.

We suggest that developers of new applications use the
handler-based approach, and that maintainers of existing
applications transition to the handler-based approach as time
permits.
