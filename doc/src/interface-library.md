# The PLEXIL Standard Interface Library {#interface-library}

The following interface adapters are built by default, and are
available to all PLEXIL applications:

* TimeAdapter

The *TimeAdapter* gives PLEXIL access to standard POSIX time APIs for
reading wall clock time and scheduling wakeups.

* UtilityAdapter

The *UtilityAdapter* implements printing commands in PLEXIL.

* Launcher

The *Launcher* allows one PLEXIL plan to start, monitor, and stop
other plans.

The interface adapters below are optional.  They provide the
interfacing commands used by the PLEXIL `OnCommand` and `OnMessage`
compound nodes.

* IpcAdapter

The *IpcAdapter* implements inter-process communication via Carnegie
Mellon University's cross-platform IPC package, allowing a PLEXIL
Executive to interoperate with other Executives, simulators, or
external systems.

* UdpAdapter

The *UdpAdapter* implements inter-process communication via UDP
datagrams.
