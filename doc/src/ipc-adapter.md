# IpcAdapter {#ipc-adapter}

The *IpcAdapter* implements inter-process communication via Carnegie
Mellon University's cross-platform 
[IPC](http://www.cs.cmu.edu/afs/cs/project/TCA/www/ipc/) 
package, allowing a %PLEXIL Executive to interoperate with other
Executives, simulators, or external systems.

By default, the IpcAdapter provides the interfacing commands used by
the Extended %PLEXIL `OnCommand` and `OnMessage` compound nodes.

## Building IpcAdapter

IpcAdapter is built when either the `--enable-ipc` or `--enable-sas`
(for StandAlone Simulator) option is passed to `configure`.

## IpcAdapter configuration

IpcAdapter's interface XML element allows several options to be
specified.  Any or all of these options may be omitted.

 + The `Server` attribute is the hostname, or IP address, and port
   number, where IPC's `central` program is listening.  It defaults to
   "localhost:1381".  If a host name is specified, but no port number,
   the port number defaults to 1381.
   
 + The `TaskName` attribute is a string to identify this particular
   IPC client.  The name is registered with `central`, and also used
   in debug output.  It defaults to a randomly generated UUID string.
   
 + The `AllowDuplicateMessages` attribute is a Boolean value which
   controls how the IpcAdapter responds when duplicate IPC messages
   are received.  It defalts to `false`.

 * The optional `ExternalLookups` element, if present, specifies the
   Lookups to be published by the IpcAdapter.  The contents of this
   element are a list of `Lookup` elements describing the Lookups to
   be published.  Each `Lookup` element must have the following
   attributes:
   
   - The `name` attribute, naming the State;
   - The `type` attribute, specifying the %PLEXIL data type returned
     by the Lookup, and 
   - The `value` attribute, specifying the default value to be used if
     no value is received from the external system.

   See [External lookups](#external-lookups) and the 
   [UpdateLookup command](#update-lookup) below for more information.

 * The optional `DefaultLookupAdapter` element, if present, tells the
   Application Framework to direct all Lookups not registered to other
   adapters to the IpcAdapter.
 
 * The optional `DefaultCommandAdapter` element, if present, tells the
   Application Framework to direct all Commands not registered to
   other adapters to the IpcAdapter.
   
 * The optional `PlannerUpdate` element, if present, tells the
   Application Framework to direct planner updates to the IpcAdapter.
  
 * The optional `DefaultAdapter` element, if present, tells the
   Application Framework to direct all Lookups and Commands not
   registered to other adapters, and planner updates, to the
   IpcAdapter.

A typical configuration XML fragment for the IpcAdapter might look like:

    <Adapter AdapterType="IpcAdapter"
             Server="someserver:1381"
             TaskName="PlexilExecutive"
             AllowDuplicateMessages="true">
     <ExternalLookups>
      <Lookup name="foo" type="Integer" value="0"/>
     </ExternalLookups>
     <DefaultCommandAdapter/>
     <PlannerUpdate/>
    </Adapter>

## Standard Commands provided by IpcAdapter

In addition to any user-defined commands, IpcAdapter provides standard
commands for use with the `OnCommand` and `OnMessage` Extended %PLEXIL
macros, and for publishing data for external lookups.

### OnCommand

bla bla description bla bla

#### Command message formats in IPC

#### Sending commands via the IpcAdapter

#### Receiving commands via the IpcAdapter

These commands are used in conjunction with the `OnCommand` Extended
%PLEXIL macro:

+ `String Command ReceiveCommand(String cmdName);`

  Awaits the named command.  When the command is received, returns a
  unique ID string used by the `GetParameter` command to retrieve any
  parameters sent with the command, and returns a CommandHandle value
  of `COMMAND_SUCCESS`.
   
+ `Any Command GetParameter(String uniqueId, Integer index);`

  Awaits the requested parameter of the requested command message
  sequence.  When the parameter is received (frequently it will
  already be in the queue), the parameter value is returned from the
  command, along with a CommandHandle value of `COMMAND_SUCCESS`.

+ `Command SendReturnValue(String uniqueId, Any returnVal);`

  Sends `returnVal` to the originator of the command message sequence
  with the `uniqueId`.
  
### OnMessage

The IpcAdapter implements sending broadcast *messages* between agents.
These messages are text strings, treated as tokens; the only data
conveyed is the name of the message.

#### Sending messages

The IpcAdapter implements the `SendMessage` command:

+ `Command SendMessage(String msg);`

  Broadcasts `msg`.  Returns a CommandHandle value of
  `COMMAND_SUCCESS` when complete.
  
#### Receiving messages

The IpcAdapter implements the `ReceiveMessage` command:

+ `String Command ReceiveMessage(String msg);`

  Awaits the receipt of `msg` from another agent.  When the message is
  received, the message itself is returned as a String (which may be
  ignored), and the CommandHandle is set to `COMMAND_SUCCESS`.

### External lookups {#external-lookups}

The IpcAdapter implements sending and receiving Lookup values between
agents.
