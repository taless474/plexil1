// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
//  All rights reserved.
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

#include "plexil-config.h"

#include "UdpAdapter.h"

#include "AdapterConfiguration.hh"
#include "AdapterExecInterface.hh"
#include "AdapterFactory.hh"    // REGISTER_ADAPTER
#include "ArrayImpl.hh"
#include "Command.hh"
#include "Debug.hh"             // debugMsg
#include "Error.hh"
#include "InterfaceAdapter.hh"
#include "InterfaceError.hh"
#include "MessageQueueMap.hh"
#include "NodeConnector.hh"
#include "ThreadSpawn.hh"
#include "udp-utils.hh"
#include "Update.hh"

#include <iostream>         // cout

#include "plexil-errno.h"
#include "plexil-float.h"
#include "plexil-stdio.h"
#include "plexil-string.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h> // close()
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

namespace PLEXIL
{
  // Forward reference
  class UdpAdapter;

  namespace {

    // Local Constants
    std::string const COMMAND_PREFIX = "__COMMAND__";
    std::string const PARAM_PREFIX = "__PARAMETER__";

    std::string const SEND_MESSAGE_COMMAND = "SendMessage";
    std::string const RECEIVE_COMMAND_COMMAND = "ReceiveCommand";
    std::string const GET_PARAMETER_COMMAND = "GetParameter";
    std::string const SEND_RETURN_VALUE_COMMAND = "SendReturnValue";

    //
    // Helper functions
    //

    //! \brief Convert a message name into the proper format,
    //!        given the command type and a user-defined id.
    //! \param name Const reference to the message name.
    //! \param command Const reference to the command name.
    //! \param id The id.
    //! \return The formatted message name string.
    //! \ingroup interface-library
    static std::string formatMessageName(const std::string &name,
                                         const std::string &command,
                                         int id = 0)
    {
      std::ostringstream ss;
      if (command == RECEIVE_COMMAND_COMMAND)
        ss << COMMAND_PREFIX;
      else if (command == GET_PARAMETER_COMMAND)
        ss << PARAM_PREFIX;
      ss << name << '_' << id;
      debugMsg("UdpAdapter:formatMessageName", " returning " << ss.str());
      return ss.str();
    }

    //! \brief Print the parsed content of a message for debugging.
    //! \param name The message name.
    //! \param args Const reference to vector of Value instances.
    static void printMessageContent(const std::string& name,
                                    const std::vector<Value>& args)
    {
      std::cout << "Message: " << name << ", Params:";
      for (std::vector<Value>::const_iterator it = args.begin();
           it != args.end();
           it++) {
        // Real, Integer, Boolean, String (and Array, maybe...)
        std::cout << " ";
        if (it->valueType() == STRING_TYPE) { 
          // Extract strings
          if (it->isKnown())
            std::cout << '"' << *it << '"';
          else
            std::cout << "UNKNOWN";
        }
        else { // Extract numbers (bool, float, int)
          std::cout << *it;
        }
      }
      std::cout << std::endl;
    }

  } // namespace

  //! \brief Structure describing one parameter of a command or return value message.
  //! \ingroup interface-library
  struct UdpParameter
  {
    std::string desc;           //!< optional parameter description
    std::string type;           //!< int|float|bool|string|int-array|float-array|string-array|bool-array
    unsigned int len;           //!< number of bytes for type (or array element)
    unsigned int elements;      //!< number of elements in the array (non-array types are 0 or 1?)
  };

  //! \brief Specifications for a message to be sent by UdpAdapter.
  //! \ingroup interface-library
  struct UdpMessage
  {
    std::string name;                     //!< the Plexil Command name
    std::string peer;                     //!< peer to which to send
    std::vector<UdpParameter> parameters; //!< message value parameters
    UdpAdapter *self;                     //!< reference to the UdpAdapter for use in message decoding
    unsigned int len;                     //!< the length of the message in bytes
    unsigned int local_port;              //!< local port on which to receive
    unsigned int peer_port;               //!< port to which to send
    int sock;                             //!< socket to use -- only meaningful in call to waitForUdpMessage

    //! \brief Default constructor.
    UdpMessage()
      : name(),
        peer(),
        parameters(),
        self(NULL),
        len(0),
        local_port(0),
        peer_port(0),
        sock(0)
    {
    }

  };

  typedef std::map<std::string, UdpMessage> MessageMap;
  typedef std::map<std::string, pthread_t> ThreadMap;
  typedef std::map<std::string, int> SocketMap;

  //! \class UdpAdapter
  //! \brief Interface adapter to send and receive commands, messages,
  //!        and return values via UDP datagrams.
  //! \ingroup interface-library
  class UdpAdapter : public InterfaceAdapter
  {
  public:

    //! \brief Constructor.
    //! \param execInterface Parent AdapterExecInterface instance.
    UdpAdapter(AdapterExecInterface& execInterface)
      : InterfaceAdapter(execInterface),
        m_default_local_port(0),
        m_default_peer_port(0),
        m_messageQueues(execInterface),
        m_debug(true)
    {
      debugMsg("UdpAdapter:UdpAdapter(execInterface)", " called");
    }

    //! \brief Constructor with configuration XML.
    //! \param execInterface Parent AdapterExecInterface instance.
    //! \param xml Const handle to the configuration XML.
    UdpAdapter(AdapterExecInterface& execInterface, pugi::xml_node const xml)
      : InterfaceAdapter(execInterface, xml),
        m_default_local_port(0),
        m_default_peer_port(0),
        m_messageQueues(execInterface),
        m_debug(false)
    {
      checkInterfaceError(xml,
                          "UdpAdapter constructor: Configuration information missing");
      debugMsg("UdpAdapter:UdpAdapter",
               " Using " << xml.attribute("AdapterType").value());
    }

    //! \brief Destructor
    virtual ~UdpAdapter()
    {
      debugMsg("UdpAdapter:~UdpAdapter", " called");
    }

    //
    // InterfaceAdapter API
    //

    //! \brief Initialize the adapter and register it for the
    //! specified commands, lookups, and planner update.
    //! \param config Pointer to the AdapterConfiguration interface registry.
    //! \return true if successful, false otherwise.
    virtual bool initialize(AdapterConfiguration *config)
    {
      debugMsg("UdpAdapter:initialize", " called");
      // Parse the message definitions in the XML configuration
      pugi::xml_node const xml = this->getXml();
      // parse the XML message definitions
      parseXmlMessageDefinitions(xml); // also calls registerCommandInterface for each message
      if (m_debug)
        printMessageDefinitions();
      config->registerCommandInterface(std::string(SEND_MESSAGE_COMMAND), this);
      config->registerCommandInterface(std::string(RECEIVE_COMMAND_COMMAND), this);
      config->registerCommandInterface(std::string(GET_PARAMETER_COMMAND), this);
      config->registerCommandInterface(std::string(SEND_RETURN_VALUE_COMMAND), this);
      debugMsg("UdpAdapter:initialize", " done");
      return true;
    }

    //! \brief Start the adapter.
    //! \return true if successful, false otherwise.
    virtual bool start()
    {
      debugMsg("UdpAdapter:start()", " called");
      // Start the UDP listener thread
      return true;
    }

    //! \brief Stop the adapter.
    //! \return true if successful, false otherwise.
    virtual bool stop()
    {
      debugMsg("UdpAdapter:stop", " called");
      // Stop the UDP listener thread
      return true;
    }

    //! \brief Shut down the adapter.
    //! \return true if successful, false otherwise.
    virtual bool shutdown()
    {
      debugMsg("UdpAdapter:shutdown", " called");
      return true;
    }

    //! \brief Send the name of the supplied node, and the supplied value pairs, to the planner.
    //! \param update Pointer to the Update instance containing the data to report to the planner.
    virtual void sendPlannerUpdate(Update *update)
    {
      debugMsg("UdpAdapter:sendPlannerUpdate", " called");
      debugMsg("ExternalInterface:udp", " sendPlannerUpdate called");
      // acknowledge updates
      debugMsg("ExternalInterface:udp",
               " faking acknowledgment of update node \""
               << update->getSource()->getNodeId() << '"');
      m_execInterface.handleUpdateAck(update, true);
      m_execInterface.notifyOfExternalEvent();
    }

    //! \brief Execute a command with the requested arguments.
    //! \param cmd The Command instance.
    virtual void executeCommand(Command *cmd)
    {
      std::string const &name = cmd->getName();
      debugMsg("UdpAdapter:executeCommand", " " << name);
      if (name == SEND_MESSAGE_COMMAND)
        executeSendMessageCommand(cmd);
      //else if (name == SEND_UDP_MESSAGE_COMMAND)
      //  executeSendUdpMessageCommand(cmd);
      //else if (name == RECEIVE_UDP_MESSAGE_COMMAND) // SendUdpCommand("cmd_name", arg1, ...); XXXX
      //  executeReceiveUdpCommand(cmd);
      else if (name == RECEIVE_COMMAND_COMMAND)
        executeReceiveCommandCommand(cmd); // OnCommand cmd_name (arg1, ...); XXXX
      else if (name == GET_PARAMETER_COMMAND)
        executeGetParameterCommand(cmd);
      else if (name == SEND_RETURN_VALUE_COMMAND)
        executeSendReturnValueCommand(cmd);
      else
        executeDefaultCommand(cmd); // Command cmd_name (arg1, ...); XXXX
      //debugMsg("UdpAdapter:executeCommand", " " << name << " done.");
    }

    //! \brief Abort the pending command.
    //! \param cmd Pointer to the command being aborted.
    virtual void invokeAbort(Command *cmd)
    {
      std::string const &cmdName = cmd->getName();
      if (cmdName != RECEIVE_COMMAND_COMMAND) {
        m_execInterface.handleCommandAbortAck(cmd, true);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      std::vector<Value> const &cmdArgs = cmd->getArgValues();
      if (cmdArgs.size() != 1) {
        warn("UdpAdapter: Aborting ReceiveCommand requires exactly one argument");
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      if (cmdArgs.front().valueType() != STRING_TYPE) {
        warn("UdpAdapter: The argument to the ReceiveCommand abort request, "
             << cmdArgs.front() << ", is not a string");
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      std::string msgName;
      if (!cmdArgs.front().getValue(msgName)) { // The defined message name, needed for looking up the thread and socket
        warn("UdpAdapter: The argument to the ReceiveCommand abort request, "
             << cmdArgs.front() << ", is unknown");
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      debugMsg("UdpAdapter:invokeAbort", " called for " << cmdName);
      int status;                        // The return status of the calls to pthread_cancel() and close()
      // First, find the active thread for this message, cancel and erase it
      ThreadMap::iterator thread;
      thread = m_activeThreads.find(msgName); // recorded by startUdpMessageReceiver
      if (thread == m_activeThreads.end()) {
        warn("UdpAdapter:invokeAbort: no thread found for " << msgName);
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      if ((status = pthread_cancel(thread->second))) {
        warn("UdpAdapter:invokeAbort: pthread_cancel(" << thread->second
             << ") returned " << status << ", errno " << errno);
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      // Wait for cancelled thread to finish
      if ((status = pthread_join(thread->second, NULL))) {
        warn("UdpAdapter:invokeAbort: pthread_join(" << thread->second
             << ") returned " << status << ", errno " << errno);
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      debugMsg("UdpAdapter:invokeAbort", " " << msgName
               << " listener thread (" << thread->second << ") cancelled");
      m_activeThreads.erase(thread); // erase the cancelled thread
      // Second, find the open socket for this message and close it
      SocketMap::iterator socket;
      socket=m_activeSockets.find(msgName); // recorded by startUdpMessageReceiver
      if (socket == m_activeSockets.end()) {
        warn("UdpAdapter:invokeAbort: no socket found for " << msgName);
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      if ((status = close(socket->second))) {
        warn("UdpAdapter:invokeAbort: close(" << socket->second
             << ") returned " << status
             << ", errno " << errno);
        m_activeSockets.erase(socket); // erase the closed socket
        m_execInterface.handleCommandAbortAck(cmd, false);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      debugMsg("UdpAdapter:invokeAbort", " " << msgName
               << " socket (" << socket->second << ") closed");
      m_activeSockets.erase(socket); // erase the closed socket
      // Let the exec know that we believe things are cleaned up
      m_messageQueues.removeRecipient(formatMessageName(msgName,
                                                        RECEIVE_COMMAND_COMMAND),
                                      cmd);
      m_execInterface.handleCommandAbortAck(cmd, true);
      m_execInterface.notifyOfExternalEvent();
    }

  private:

    // Deliberately unimplemented
    UdpAdapter();
    UdpAdapter(const UdpAdapter &);
    UdpAdapter& operator=(const UdpAdapter &);

    //
    // Implementation methods
    //

    //! \brief Send a generic command via UDP.  Use the first parameter as the message name.
    //! \param cmd Pointer to the command to be sent.
    void executeDefaultCommand(Command *cmd)
    {
      std::vector<Value> const &args = cmd->getArgValues();
      if (args.size() == 0) {
        warn("UdpAdapter: command \"" << cmd->getName() << "\" requires at least one argument");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      if (args[0].valueType() != STRING_TYPE) {
        warn("UdpAdapter: command \"" << cmd->getName()
             << "\": message name must be a string");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      std::string const &msgName = cmd->getName();
      debugMsg("UdpAdapter:executeDefaultCommand",
               " called for \"" << msgName << "\" with " << args.size() << " args");
      ThreadMutexGuard guard(m_cmdMutex);
      MessageMap::iterator msg;
      msg = m_messages.find(msgName);
      // Check for an obviously bogus port
      if (msg->second.peer_port == 0) {
        warn("executeDefaultCommand: bad peer port (0) given for " << msgName << " message");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      // Set up the outgoing UDP buffer to be sent
      int length = msg->second.len;
      unsigned char* udp_buffer = new unsigned char[length]; // fixed length to start with
      memset((char*)udp_buffer, 0, length); // zero out the buffer
      // Walk the parameters and encode them in the buffer to be sent out
      if (0 > buildUdpBuffer(udp_buffer, msg->second, args, false, m_debug)) {
        warn("executeDefaultCommand: error formatting buffer");
        delete[] udp_buffer;
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      // Send the buffer to the given host:port
      int status = sendUdpMessage(udp_buffer, msg->second, m_debug);
      debugMsg("UdpAdapter:executeDefaultCommand",
               " sendUdpMessage returned " << status << " (bytes sent)");
      // Clean up some (one hopes)
      delete[] udp_buffer;
      // Do the internal Plexil Boiler Plate (as per example in IpcAdapter.cc)
      m_execInterface.handleCommandAck(cmd, COMMAND_SUCCESS);
      m_execInterface.notifyOfExternalEvent();
    }
    
    //! \brief Implement the ReceiveCommand command.
    //! \param cmd Pointer to the command instance.
    void executeReceiveCommandCommand(Command *cmd)
    {
      std::vector<Value> const &args = cmd->getArgValues();
      if (args.size() != 1) {
        warn("UdpAdapter: The " << RECEIVE_COMMAND_COMMAND
             << " command requires exactly one argument");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      if (args.front().valueType() != STRING_TYPE) {
        warn("UdpAdapter: The argument to the " << RECEIVE_COMMAND_COMMAND
             << " command, " << args.front() << ", is not a string");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      std::string msgName;
      if (!args.front().getValue(msgName)) {
        warn("UdpAdapter:executeDefaultCommand: message name is unknown");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      debugMsg("UdpAdapter:executeReceiveCommandCommand", " called for " << msgName);
      std::string command = formatMessageName(msgName, RECEIVE_COMMAND_COMMAND);
      m_messageQueues.addRecipient(command, cmd);
      // Set up the thread on which the message may/will eventually be received
      int status = startUdpMessageReceiver(msgName, cmd);
      if (status) {
        warn("UdpAdapter::executeReceiveCommandCommand: startUdpMessageReceiver failed");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
      }
      else {
        debugMsg("UdpAdapter:executeReceiveCommandCommand",
                 " message handler for \"" << command << "\" registered");
      }
    }

    //! \brief Implement the GetParameter command.
    //! \param cmd Pointer to the command instance.
    void executeGetParameterCommand(Command *cmd)
    {
      std::vector<Value> const &args = cmd->getArgValues();
      if (args.size() < 1 || args.size() > 2) {
        warn("UdpAdapter: The " << GET_PARAMETER_COMMAND
             << " command requires either one or two arguments");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      if (args.front().valueType() != STRING_TYPE) {
        warn("UdpAdapter: The first argument to the " << GET_PARAMETER_COMMAND
             << " command, " << args.front() << ", is not a string");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      // Extract the message name and try to verify the number of parameters defined vs the number of args used in the plan
      std::string msgName;
      if (!args.front().getValue(msgName)) {
        warn("UdpAdapter:executeGetParameterCommand: message name is unknown");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      debugMsg("UdpAdapter:executeGetParameterCommand", " " << msgName);
      size_t pos;
      pos = msgName.find(":");
      std::string const baseName = msgName.substr(0, pos);
      MessageMap::iterator msg;
      msg = m_messages.find(baseName);
      if (msg == m_messages.end()) {
        warn("UdpAdapter:executeGetParameterCommand: no message definition found for "
             << baseName);
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      int params = msg->second.parameters.size();
      debugMsg("UdpAdapter:executeGetParameterCommand",
               " msgName==" << msgName << ", params==" << params);
      std::vector<Value>::const_iterator it = ++args.begin();
      int32_t id = 0;
      if (it != args.end()) {
        if (it->valueType() != INTEGER_TYPE) {
          warn("UdpAdapter: The second argument to the " << GET_PARAMETER_COMMAND
               << " command, " << *it << ", is not an integer");
          m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
          m_execInterface.notifyOfExternalEvent();
          return;
        }
        
        if (!it->getValue(id)) {
          warn("UdpAdapter: The second argument to the " << GET_PARAMETER_COMMAND
               << " command is unknown");
          m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
          m_execInterface.notifyOfExternalEvent();
          return;
        }

        if (id < 0) {
          warn("UdpAdapter: The second argument to the " << GET_PARAMETER_COMMAND
               << " command, " << *it << ", is not a valid index");
          warn("UdpAdapter: The second argument to the " << GET_PARAMETER_COMMAND
               << " command is unknown");
          m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
          m_execInterface.notifyOfExternalEvent();
          return;
        }

        // Brute strength error check for the plan using a message/command with to many arguments.
        // The intent is that this might be discovered during development.
        if (id >= params) {
          warn("UdpAdapter: the message \"" << msgName << "\" is defined to have " << params
               << " parameters in the XML configuration file, but is being used in the plan with "
               << id+1 << " arguments");
          m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
          m_execInterface.notifyOfExternalEvent();
          return;
        }
      }
      std::string command = formatMessageName(msgName, GET_PARAMETER_COMMAND, id);
      m_messageQueues.addRecipient(command, cmd);
      debugMsg("UdpAdapter:executeGetParameterCommand", " message handler for \"" << cmd->getName() << "\" registered");
    }    


    //! \brief Implement the SendReturnValue command.
    //! \param cmd Pointer to the command instance.
    //! \note No-op for UdpAdapter.
    void executeSendReturnValueCommand(Command *cmd)
    {
      m_execInterface.handleCommandAck(cmd, COMMAND_SUCCESS);
      m_execInterface.notifyOfExternalEvent();
    }

    //! \brief Implement the SendMessage command.
    //! \param cmd Pointer to the command instance.
    void executeSendMessageCommand(Command *cmd)
    {
      std::vector<Value> const &args = cmd->getArgValues();
      // Check for one argument, the message
      if (args.size() != 1) {
        warn("UdpAdapter: The SendMessage command requires exactly one argument");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      if (args.front().valueType() != STRING_TYPE) {
        warn("UdpAdapter: The argument to the SendMessage command, "
             << args.front() << ", is not a string");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }

      std::string theMessage;
      if (!args.front().getValue(theMessage)) {
        warn("UdpAdapter: The message name argument to the SendMessage command is unknown");
        m_execInterface.handleCommandAck(cmd, COMMAND_FAILED);
        m_execInterface.notifyOfExternalEvent();
        return;
      }
      
      debugMsg("UdpAdapter:executeSendMessageCommand", " SendMessage(\"" << theMessage << "\")");
      debugMsg("UdpAdapter:executeSendMessageCommand", " message \"" << theMessage << "\" sent.");
      // store ack
      m_execInterface.handleCommandAck(cmd, COMMAND_SUCCESS);
      m_execInterface.notifyOfExternalEvent();
    }

    //
    // XML Support
    //

    //! \brief Parse and verify the adapter configuration XML.
    //! \param xml Const handle to the XML.
    void parseXmlMessageDefinitions(pugi::xml_node const xml)
    {
      m_messages.clear();         // clear the old messages (if any)
      // First, set up the internal debugging output
      pugi::xml_attribute const debug = xml.attribute("debug");
      if (debug && debug.as_bool())
        m_debug = true;
      // Now, do the real work of parsing the XML UDP Configuration
      const pugi::xml_attribute default_local_port = xml.attribute("default_local_port");
      const pugi::xml_attribute default_peer_port = xml.attribute("default_peer_port");
      const char* default_peer = xml.attribute("default_peer").value();
      if (default_local_port)
        m_default_local_port = default_local_port.as_uint();
      if (default_peer_port)
        m_default_peer_port = default_peer_port.as_uint();
      if (*default_peer)
        m_default_peer = default_peer;
      // Walk the messages
      for (pugi::xml_node child = xml.first_child(); child; child = child.next_sibling()) {
        UdpMessage msg;
        const char* name = child.attribute("name").value(); // name is required, hence...
        checkInterfaceError(name && *name,
                            "parseXmlMessageDefinitions: no name given in <Message/>");
        msg.name = name;
        const char* peer = child.attribute("peer").value(); // needed for bool test below (i.e., it is optional)

        pugi::xml_attribute local_port = child.attribute("local_port");
        pugi::xml_attribute peer_port = child.attribute("peer_port");
        // Use either the given peer, the default_peer, or "localhost"
        msg.peer = *peer ? peer : (*default_peer ? m_default_peer : "localhost");
        // Warn about possible run time errors (planners may simply not use a message I suppose)
        if (!(default_local_port || local_port))
          std::cout << "Warning: no default or message specific local port given for <Message name=\""
                    << name << "\"/>\n         this will cause a run time error if "
                    << name << "it is called to send an outgoing command/message\n";
        if (!(default_peer_port || peer_port))
          std::cout << "Warning: no default or message specific peer port given for <Message name=\""
                    << name << "\"/>\n         this will cause a run time error if "
                    << name << "it is called to receive an incoming command/message\n";
        msg.local_port = local_port ? local_port.as_uint() : m_default_local_port;
        msg.peer_port = peer_port ? peer_port.as_uint() : m_default_peer_port;
        // Walk the <Parameter/> elements of this <Message/>
        for (pugi::xml_node param = child.first_child();
             !param.empty();
             param = param.next_sibling())
          {
            UdpParameter arg;
            // Get the description (if any)
            pugi::xml_attribute param_desc = param.attribute("desc");
            if (param_desc)
              arg.desc = param_desc.value(); // only assign it if it exists
            // Get the (required) type
            const char* param_type = param.attribute("type").value();
            checkInterfaceError(param_type && *param_type,
                                "parseXmlMessageDefinitions: no type for parameter given in <Message name=\""
                                << name << "\"/>");
            arg.type = param_type;
            // Get the length, which is required
            pugi::xml_attribute len = param.attribute("bytes");
            checkInterfaceError(len,
                                "parseXmlMessageDefinitions: no parameter length (in bytes) given in <Message name=\""
                                << name << "\"/>");
            arg.len = len.as_uint();
            checkInterfaceError((arg.len > 0),
                                "parseXmlMessageDefinitions: zero length (in bytes) parameter given in <Message name=\""
                                << name << "\"/>");
            // Get the number of elements for the array types
            int size = 1;
            pugi::xml_attribute param_elements = param.attribute("elements");
            if (param_elements)
              size = param_elements.as_uint();
            arg.elements = size;
            // Do some error checking for reasonable/usable encoding/decoding byte lengths
            if (arg.type.find("array") != std::string::npos) {
              //printf("\nsize==%d, find==%d\n", size, arg.type.find("array"));
              checkInterfaceError(param_elements,
                                  "parseXmlMessageDefinitions: arrays must have a size element, "
                                  << "<Message=\"" << name << "\"/> does not");
            }
            if ((arg.type.compare("int") == 0) || (arg.type.compare("int-array") == 0)) {
              checkInterfaceError((arg.len==2 || arg.len==4),
                                  "parseXmlMessageDefinitions: integers must be 2 or 4 bytes, not " << arg.len
                                  << " (in <Message name=\"" << name << "\"/>)");
            }
            else if ((arg.type.compare("float") == 0) || (arg.type.compare("float-array") == 0)) {
              checkInterfaceError((arg.len==2 || arg.len==4),
                                  "parseXmlMessageDefinitions: floats must be 2 or 4 bytes, not " << arg.len
                                  << " (in <Message name=\"" << name << "\"/>)");
            }
            else if ((arg.type.compare("bool") == 0) || (arg.type.compare("bool-array") == 0)) {
              checkInterfaceError((arg.len==1 || arg.len==2 || arg.len==4),
                                  "parseXmlMessageDefinitions: booleans must be 1, 2 or 4 bytes, not " << arg.len
                                  << " (in <Message name=\"" << name << "\"/>)");
            }
            // what about strings? -- fixed length to start with I suppose...
            else if ((arg.type.compare("string") == 0) || (arg.type.compare("string-array") == 0)) {
              checkInterfaceError(arg.len>=1,
                                  "parseXmlMessageDefinitions: strings must be 1 byte or longer (in <Message name=\""
                                  << name << "\"/>)");
            }
            else {
              checkInterfaceError(0, "parseXmlMessageDefinitions: unknown parameter type \"" << arg.type
                                  << "\" (in <Message name=\"" << name << "\"/>)");
            }
            msg.len += arg.len * size; // only arrays are not of size 1
            msg.parameters.push_back(arg);
          }
        m_messages[child.attribute("name").value()] = msg; // record the message with the name as the key
        g_configuration->registerCommandInterface(std::string(name), this); // register name with executeCommand
      }
    }

    //! \brief Print all the registered message definitions to std::cout.
    void printMessageDefinitions()
    {
      // print all of the stuff in m_message for debugging
      MessageMap::iterator msg;
      std::string indent = "             ";
      int i = 0;
      for (msg=m_messages.begin(); msg != m_messages.end(); msg++, i++)
        {
          std::cout << "UDP Message: " << msg->first;
          std::vector<UdpParameter>::iterator param;
          for (param=msg->second.parameters.begin(); param != msg->second.parameters.end(); param++)
            {
              std::string temp = param->desc.empty() ? " (no description)" : " (" + param->desc + ")";
              if (param->elements == 1)
                {
                  std::cout << "\n" << indent << param->len << " byte " << param->type << temp;
                }
              else
                {
                  size_t pos = param->type.find("-");
                  std::cout << "\n" << indent << param->elements << " element array of " << param->len << " byte "
                            << param->type.substr(0, pos) << "s" << temp;
                }
            }
          std::cout << std::endl << indent << "length: " << msg->second.len << " (bytes)";
          std::cout << ", peer: " << msg->second.peer << ", peer_port: " << msg->second.peer_port;
          std::cout << ", local_port: " << msg->second.local_port;
          std::cout << std::endl;
        }
    }

    //! \brief Start a thread to receive the named message.
    //! \param name Const reference to the message name.
    //! \param cmd Pointer to the receive command.
    int startUdpMessageReceiver(const std::string& name, Command *cmd)
    {
      //ThreadMutexGuard guard(m_cmdMutex);
      debugMsg("UdpAdapter:startUdpMessageReceiver",
               " entered for " << name);
      // Find the message definition to get the message port and size
      MessageMap::iterator msg;
      msg=m_messages.find(name);
      if (msg == m_messages.end()) {
        warn("UdpAdapter:startUdpMessageReceiver: no message found for " << name);
        return -1;
      }
    
      // Check for a bogus local port
      if (msg->second.local_port == 0) {
        warn("startUdpMessageReceiver: bad local port (0) given for " << name << " message");
        return -1;
      }

      msg->second.name = name;
      msg->second.self = this; // pass a reference to "this" UdpAdapter for later use
      // Try to set up the socket so that we can close it later if the thread is cancelled
      int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (sock < 0) {
        warn("UdpAdapter:startUdpMessageReceiver: call to socket() failed");
        return -1;
      }
    
      debugMsg("UdpAdapter:startUdpMessageReceiver",
               " " << name << " socket (" << sock << ") opened");
      msg->second.sock = sock; // pass the socket descriptor to waitForUdpMessage, which will then reset it
      pthread_t thread_handle;
      // Spawn the listener thread
      threadSpawn((THREAD_FUNC_PTR) waitForUdpMessage, &msg->second, thread_handle);
      // Check to see if the thread got started correctly
      if (thread_handle == 0) {
        warn("UdpAdapter:startUdpMessageReceiver: threadSpawn returned NULL");
        return -1;
      }

      debugMsg("UdpAdapter:startUdpMessageReceiver",
               " " << name << " listener thread (" << thread_handle << ") spawned");
      // Record the thread and socket in case they have to be cancelled and closed later (in invokeAbort)
      m_activeThreads[name] = thread_handle;
      m_activeSockets[name] = sock;
      return 0;
    }

    //! \brief Block until the specified message arrives.
    //! \param msg Pointer to a description of the message.
    //! \return 0.
    static void* waitForUdpMessage(UdpMessage* msg)
    {
      debugMsg("UdpAdapter:waitForUdpMessage", " called for " << msg->name);
      // A pointer to the adapter
      UdpAdapter* udpAdapter = msg->self;
      //int local_port = msg->local_port;
      size_t size = msg->len;
      udp_thread_params params;
      params.local_port = msg->local_port;
      params.buffer = new unsigned char[size];
      params.size = size;
      params.debug = udpAdapter->m_debug; // see if debugging is enabled
      params.sock = msg->sock; // The socket opened in startUdpMessageReceiver
      msg->sock = 0;           // Reset the temporary socket descriptor in the UdpMessage
      int status = wait_for_input_on_thread(&params);
      if (status) {
        warn("waitForUdpMessage: call to wait_for_input_on_thread returned " << status);
        delete[] params.buffer;     // release the input buffer
        return (void *) 0;
      }

      // When the message has been received, tell the UdpAdapter about it and its contents
      status = udpAdapter->handleUdpMessage(msg, params.buffer, params.debug);
      delete[] params.buffer;     // release the input buffer
      if (status) {
        warn("waitForUdpMessage call to handleUdpMessage returned " << status);
      }

      return (void *) 0;
    }

    //! \brief Process a UDP message after receipt.
    //! \param msgDef Description of the message to be processed.
    //! \param buffer Pointer to the receive buffer.
    //! \param debug If true, print debugging messages. Default is false.
    //! \return 0 if successful, -1 on error.
    int handleUdpMessage(const UdpMessage* msgDef,
                         const unsigned char* buffer,
                         bool debug = false)
    {
      // Handle a UDP message once it has indeed arrived.
      // msgDef is passed in, therefore, we will assume it is good.
      debugMsg("UdpAdapter:handleUdpMessage", " called for " << msgDef->name);
      if (debug) {
        std::cout << "  handleUdpMessage: buffer: ";
        print_buffer(buffer, msgDef->len);
      }
      // (1) addMessage for expected message
      static int counter = 1;     // gensym counter
      std::ostringstream unique_id;
      unique_id << msgDef->name << ":msg_parameter:" << counter++;
      std::string msg_label(unique_id.str());
      debugMsg("UdpAdapter:handleUdpMessage", " adding \"" << msgDef->name << "\" to the command queue");
      const std::string msg_name = formatMessageName(msgDef->name, RECEIVE_COMMAND_COMMAND);
      m_messageQueues.addMessage(msg_name, msg_label);
      // (2) walk the parameters, and for each, call addMessage(label, <value-or-key>), which
      //     (somehow) arranges for executeCommand(GetParameter) to be called, and which in turn
      //     calls addRecipient and updateQueue
      int i = 0;
      int offset = 0;
      for (std::vector<UdpParameter>::const_iterator param = msgDef->parameters.begin();
           param != msgDef->parameters.end();
           param++, i++) {
        const std::string param_label = formatMessageName(msg_label, GET_PARAMETER_COMMAND, i);
        int len = param->len;   // number of bytes to read
        int size = param->elements; // size of the array, or 1 for scalars
        std::string type = param->type; // type to decode
        if (debug) {
          if (size == 1) {
            std::cout << "  handleUdpMessage: decoding " << len << " byte " << type
                      << " starting at buffer[" << offset << "]: ";
          }
          else {
            size_t pos = type.find("-"); // remove the "-array" from the type
            std::cout << "  handleUdpMessage: decoding " << size << " element array of " << len
                      << " byte " << type.substr(0, pos) << "s starting at buffer[" << offset
                      << "]: ";
          }
        }
        if (type.compare("int") == 0) {
          if (len != 2 && len != 4){
            warn("handleUdpMessage: Integers must be 2 or 4 bytes, not " << len);
            return -1;
          }
          int num;
          num = (len == 2) ? decode_short_int(buffer, offset) : decode_int32_t(buffer, offset);
          if (debug)
            std::cout << num << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing numeric (integer) parameter " << num);
          m_messageQueues.addMessage(param_label, Value(num));
          offset += len;
        }
        else if (type.compare("int-array") == 0) {
          if (len != 2 && len != 4) {
            warn("handleUdpMessage: Integers must be 2 or 4 bytes, not " << len);
            return -1;
          }
          IntegerArray array(size);
          for (int i = 0 ; i < size ; i++) {
            array.setElement(i, (int32_t) ((len == 2) ? decode_short_int(buffer, offset) : decode_int32_t(buffer, offset)));
            offset += len;
          }
          if (debug)
            std::cout << array.toString() << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing numeric (integer) array " << array.toString());
          m_messageQueues.addMessage(param_label, array);
        }
        else if (type.compare("float") == 0) {
          if (len != 4) {
            warn("handleUdpMessage: Reals must be 4 bytes, not " << len);
            return -1;
          }
          float num = decode_float(buffer, offset);
          if (debug)
            std::cout << num << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing numeric (real) parameter " << num);
          m_messageQueues.addMessage(param_label, Value((double) num));
          offset += len;
        }
        else if (type.compare("float-array") == 0) {
          if (len != 4) {
            warn("handleUdpMessage: Reals must be 4 bytes, not " << len);
            return -1;
          }
          RealArray array(size);
          for (int i = 0 ; i < size ; i++) {
            array.setElement(i, (double) decode_float(buffer, offset));
            offset += len;
          }
          if (debug)
            std::cout << array.toString() << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing numeric (real) array " << array.toString());
          m_messageQueues.addMessage(param_label, Value(array));
        }
        else if (type.compare("bool") == 0) {
          int num;
          switch (len) {
          case 1:
            num = buffer[offset];
            break;

          case 2:
            num = decode_short_int(buffer, offset); break;

          case 4:
            num = decode_int32_t(buffer, offset); break;

          default:
            warn("handleUdpMessage: Booleans must be 1, 2 or 4 bytes, not " << len);
            return -1;
          }
          if (debug)
            std::cout << num << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing numeric (boolean) parameter " << num);
          m_messageQueues.addMessage(param_label, Value(num != 0));
          offset += len;
        }
        else if (type.compare("bool-array") == 0) {
          BooleanArray array(size);
          for (int i = 0 ; i < size ; i++) {
            switch (len) {
            case 1: 
              array.setElement(i, 0 != buffer[offset]); break;
            case 2:
              array.setElement(i, 0 != decode_short_int(buffer, offset)); break;
            case 4:
              array.setElement(i, 0 != decode_int32_t(buffer, offset)); break;
            default:
              warn("handleUdpMessage: Booleans must be 1, 2 or 4 bytes, not " << len);
              return -1;
            }
            offset += len;
          }
          if (debug)
            std::cout << array.toString() << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queueing boolean array " << array.toString());
          m_messageQueues.addMessage(param_label, Value(array));
        }
        else if (type.compare("string-array") == 0) {
          // XXXX For unknown reasons, OnCommand(... String arg); is unable to receive this (inlike int and float arrays)
          StringArray array(size);
          for (int i = 0 ; i < size ; i++) {
            array.setElement(i, decode_string(buffer, offset, len));
            offset += len;
          }
          if (debug)
            std::cout << array.toString() << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queuing string array " << array.toString());
          m_messageQueues.addMessage(param_label, Value(array));
        }
        else { // string or die
          if (type.compare("string")) {
            warn("handleUdpMessage: unknown parameter type " << type);
            return -1;
          }
          std::string str = decode_string(buffer, offset, len);
          if (debug)
            std::cout << str << std::endl;
          debugMsg("UdpAdapter:handleUdpMessage", " queuing string parameter \"" << str << "\"");
          m_messageQueues.addMessage(param_label, Value(str));
          offset += len;
        }
      }
      debugMsg("UdpAdapter:handleUdpMessage", " for " << msgDef->name << " complete");
      return 0;
    }
      
    //! \brief Send the contents of the buffer as described by the message spec.
    //! \param buffer Pointer to the send buffer.
    //! \param msg Const reference to the message specification.
    //! \param debug If true, print debugging messages. Default is false.
    int sendUdpMessage(const unsigned char* buffer,
                       const UdpMessage& msg,
                       bool debug = false)
    {
      int status = 0; // return status
      debugMsg("UdpAdapter:sendUdpMessage", " sending " << msg.len << " bytes to " << msg.peer << ":" << msg.peer_port);
      status = send_message_connect(msg.peer.c_str(), msg.peer_port, (const char*) buffer, msg.len, debug);
      return status;
    }

    //! \brief Format the vector of argument values into the buffer, as specified
    //!        by the message definition.
    //! \param buffer Pointer to the send buffer.
    //! \param msg Const reference to the message definition.
    //! \param args Const reference to the vector of values.
    //! \param skip_arg If true, do not format the first value. Default is false.
    //! \param debug If true, print debugging messages. Default is false.
    int buildUdpBuffer(unsigned char* buffer,
                       const UdpMessage& msg,
                       const std::vector<Value>& args,
                       bool skip_arg=false,
                       bool debug=false)
    {
      std::vector<Value>::const_iterator it;
      std::vector<UdpParameter>::const_iterator param;
      int start_index = 0; // where in the buffer to write

      // Do what error checking we can, since we absolutely know that planners foul this up.
      debugMsg("UdpAdapter:buildUdpBuffer",
               " args.size()==" << args.size()
               << ", parameters.size()==" << msg.parameters.size());
      size_t param_count = msg.parameters.size();
      if (skip_arg)
        param_count++;

      if (args.size() != param_count) {
        warn("the " << param_count
             << " parameters defined in the XML configuration file do not match the "
             << args.size() << " parameters used in the plan for <Message name=\""
             << msg.name << "\"/>");
        return -1;
      }

      // Iterate over the given args (it) and the message definition (param)
      // in lock step to encode the outgoing buffer.
      for (param = msg.parameters.begin(), it=args.begin();
           param != msg.parameters.end();
           param++, it++) {
        if (skip_arg) { // only skip the first arg
          it++; 
          skip_arg = false; 
        }
        unsigned int len = param->len;
        std::string type = param->type;
        Value const &plexil_val = *it;
        if (!plexil_val.isKnown()) {
          warn("buildUdpBuffer: Value to be sent is unknown");
          return -1;
        }

        ValueType valType = plexil_val.valueType();
      
        if (debug)
          std::cout << "  buildUdpBuffer: encoding ";
        if (type.compare("bool") == 0) {
          if (valType != BOOLEAN_TYPE) {
            warn("buildUdpBuffer: Format requires Boolean, but supplied value is not");
            return -1;
          }
          bool temp;
          plexil_val.getValue(temp);
          if (debug)
            std::cout << len << " byte bool starting at buffer[" << start_index << "]: " << temp;
          switch (len) {
          case 1: 
            buffer[start_index] = (unsigned char) temp;
            break;
          case 2:
            encode_short_int(temp, buffer, start_index);
            break;
          case 4:
            encode_int32_t(temp, buffer, start_index);
            break;
          default:
            warn("buildUdpBuffer: Booleans must be 1, 2 or 4 bytes, not " << len);
            return -1;
          }
          start_index += len;
        }
        else if (type.compare("int") == 0) {
          if (valType != INTEGER_TYPE) {
            warn("buildUdpBuffer: Format requires Integer, but supplied value is not");
            return -1;
          }
          if (len != 2 && len != 4) {
            warn("buildUdpBuffer: Integers must be 2 or 4 bytes, not " << len);
            return -1;
          }
          if (debug)
            std::cout << len << " byte int starting at buffer[" << start_index << "]: " << plexil_val;
          Integer temp;
          plexil_val.getValue(temp);
          if (len == 2) {
            if (INT16_MIN > temp || temp > INT16_MAX) {
              warn("buildUdpBuffer: 2 byte integers must be between "
                   << INT16_MIN << " and " << INT16_MAX
                   << ", " << temp << " is not");
              return -1;
            }
            encode_short_int(temp, buffer, start_index);
          }
          else
            encode_int32_t(temp, buffer, start_index);
          start_index += len;
        }
        else if (type.compare("float") == 0) {
          if (valType != REAL_TYPE) {
            warn("buildUdpBuffer: Format requires Real, but supplied value is not");
            return -1;
          }
          if (len != 4) {
            warn("buildUdpBuffer: Reals must be 4 bytes, not " << len);
            return -1;
          }
          double temp;
          plexil_val.getValue(temp);
          // Catch really big floats
          if ((-FLT_MAX) > temp || temp > FLT_MAX) {
            warn("buildUdpBuffer: Reals (floats) must be between "
                 << (-FLT_MAX) << " and " << FLT_MAX <<
                 ", not " << plexil_val);
            return -1;
          }
          if (debug)
            std::cout << len << " byte float starting at buffer[" << start_index << "]: " << temp;
          encode_float((float) temp, buffer, start_index);
          start_index += len;
        }
        else if (type.compare("string") == 0) {
          if (valType != STRING_TYPE) {
            warn("buildUdpBuffer: Format requires String, but supplied value is not");
            return -1;
          }
          std::string const *str = NULL;
          plexil_val.getValuePointer(str);
          if (str->length() > len) {
            warn("buildUdpBuffer: declared string length (" << len <<
                 ") and actual length (" << str->length() << ", " << *str <<
                 ") used in the plan are not compatible");
            return -1;
          }
          if (debug) 
            std::cout << len << " byte string starting at buffer[" << start_index << "]: " << str;
          encode_string(*str, buffer, start_index);
          start_index += len;
        }
        else if (type.compare("bool-array") == 0) {
          if (valType != BOOLEAN_ARRAY_TYPE) {
            warn("buildUdpBuffer: Format requires BooleanArray, but supplied value is not");
            return -1;
          }
          if (len != 1 && len != 2 && len != 4) {
            warn("buildUdpBuffer: Booleans must be 1, 2 or 4 bytes, not " << len);
            return -1;
          }
          unsigned int size = param->elements;
          BooleanArray const *array = NULL;
          plexil_val.getValuePointer(array);
          if (debug)
            std::cout << size << " element array of "
                      << len << " byte booleans starting at buffer["
                      << start_index << "]: " << array->toString();
          if (size != array->size()) {
            warn("buildUdpBuffer: declared and actual (boolean) array sizes differ: "
                 << size << " was declared, but "
                 << array->size() << " is being used in the plan");
            return -1;
          }
          for (unsigned int i = 0 ; i < size ; i++) {
            bool temp;
            if (!array->getElement(i, temp)) {
              warn("buildUdpBuffer: Array element at index " << i << " is unknown");
              return -1;
            }
            switch (len) {
            case 1:
              buffer[start_index] = (unsigned char) temp;
              break;
            case 2:
              encode_short_int(temp, buffer, start_index);
              break;
            default:
              encode_int32_t(temp, buffer, start_index);
              break;
            }
            start_index += len;
          }
        }
        else if (type.compare("int-array") == 0) {
          if (plexil_val.valueType() != INTEGER_ARRAY_TYPE) {
            warn("buildUdpBuffer: Format requires IntegerArray, supplied value is a "
                 << valueTypeName(plexil_val.valueType()));
            return -1;
          }
          if (len != 2 && len != 4) {
            warn("buildUdpBuffer: Integers must be 2 or 4 bytes, not " << len);
            return -1;
          }
          unsigned int size = param->elements;
          IntegerArray const *array = NULL;
          plexil_val.getValuePointer(array);
          if (debug)
            std::cout << size << " element array of " << len << " byte ints starting at ["
                      << start_index << "]: " << array->toString();
          if (size != array->size()) {
            warn("buildUdpBuffer: declared and actual array sizes differ: "
                 << size << " was declared, but "
                 << array->size() << " is being used in the plan");
            return -1;
          }
          for (unsigned int i = 0 ; i < size ; i++) {
            Integer temp;
            if (!array->getElement(i, temp)) {
              warn("buildUdpBuffer: Array element at index " << i << " is unknown");
              return -1;
            }
            if (len == 2) {
              if (INT16_MIN > temp || temp > INT16_MAX) {
                warn("buildUdpBuffer: 2 bytes integers must be between "
                     << INT16_MIN << " and " << INT16_MAX
                     << ", " << temp << " is not");
                return -1;
              }
              encode_short_int(temp, buffer, start_index);
            }
            else
              encode_int32_t(temp, buffer, start_index);
            start_index += len;
          }
        }
        else if (type.compare("float-array") == 0) {
          if (plexil_val.valueType() != REAL_ARRAY_TYPE) {
            warn("buildUdpBuffer: Format requires RealArray, supplied value is a "
                 << valueTypeName(plexil_val.valueType()));
            return -1;
          }
          if (len != 4) {
            warn("buildUdpBuffer: Reals must be 4 bytes, not " << len);
            return -1;
          }
          unsigned int size = param->elements;
          if (size < 1) {
            warn("buildUdpBuffer: all scalars and arrays must be of at least size 1, not " << size);
            return -1;
          }
          RealArray const *array = NULL;
          plexil_val.getValuePointer(array);
          if (debug)
            std::cout << size << " element array of " << len << " byte floats starting at buffer["
                      << start_index << "]: " << array->toString();
          if (size != array->size()) {
            warn("buildUdpBuffer: declared and actual (float) array sizes differ: "
                 << size << " was declared, but "
                 << array->size() << " is being used in the plan");
            return -1;
          }
          for (unsigned int i = 0 ; i < size ; i++) {
            double temp;
            if (!array->getElement(i, temp)) {
              warn("buildUdpBuffer: Array element at index " << i << " is unknown");
              return -1;
            }
            if ((-FLT_MAX) > temp || temp > FLT_MAX) {
              warn("buildUdpBuffer: Reals (floats) must be between "
                   << (-FLT_MAX) << " and " << FLT_MAX <<
                   ", " << temp << " is not");
              return -1;
            }
            encode_float((float) temp, buffer, start_index);
            start_index += len;
          }
        }
        else if (type.compare("string-array") == 0) {
          if (plexil_val.valueType() != STRING_ARRAY_TYPE) {
            warn("buildUdpBuffer: Format requires StringArray, supplied value is a "
                 << valueTypeName(plexil_val.valueType()));
            return -1;
          }
          unsigned int size = param->elements;
          StringArray const *array = NULL;
          plexil_val.getValuePointer(array);
          if (debug)
            std::cout << size << " element array of " << len << " byte strings starting at buffer["
                      << start_index << "]: " << array->toString();
          if (size != array->size()) {
            warn("buildUdpBuffer: declared and actual (string) array sizes differ: "
                 << size << " was declared, but " << array->size() << " is being used in the plan");
            return -1;
          }
          for (unsigned int i = 0 ; i < size ; i++) {
            std::string const *temp = NULL;
            if (!array->getElementPointer(i, temp)) {
              warn("buildUdpBuffer: Array element at index " << i << " is unknown");
              return -1;
            }
            if (temp->length() > len) {
              warn("buildUdpBuffer: declared string length (" << len <<
                   ") and actual length (" << temp->length() <<
                   ") used in the plan are not compatible");
              return -1;
            }
            encode_string(*temp, buffer, start_index);
            start_index += len;
          }
        }
        else {
          warn("buildUdpBuffer: unknown parameter type " << type);
          return -1;
        }

        if (debug)
          std::cout << std::endl;
      }
      if (debug) {
        std::cout << "  buildUdpBuffer: buffer: ";
        print_buffer(buffer, msg.len);
      }
      return start_index;
    }

    //
    // Member variables
    //

    ThreadMutex m_cmdMutex;

    // Somewhere to hang the messages, default ports and peers, threads and sockets
    unsigned int m_default_local_port;
    unsigned int m_default_peer_port;
    std::string m_default_peer;
    MessageMap m_messages;
    MessageQueueMap m_messageQueues;
    ThreadMap m_activeThreads;
    SocketMap m_activeSockets;
    bool m_debug; // Show debugging output
  };

}

//! \brief Register the UdpAdapter factory with the PLEXIL Application Framework.
//! \ingroup interface-library
extern "C"
void initUdpAdapter()
{
  REGISTER_ADAPTER(PLEXIL::UdpAdapter, "UdpAdapter");
}
