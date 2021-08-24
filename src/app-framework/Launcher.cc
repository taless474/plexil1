// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
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

#include "Launcher.hh"

#include "AdapterConfiguration.hh"
#include "AdapterExecInterface.hh"
#include "AdapterFactory.hh"
#include "Array.hh"
#include "Command.hh"
#include "commandHandlerDefs.hh"
#include "Error.hh"
#include "ExecListener.hh"
#include "InterfaceAdapter.hh"
#include "lookupHandlerDefs.hh"
#include "Node.hh"
#include "ParserException.hh"
#include "parser-utils.hh"
#include "PlexilExec.hh" // g_exec
#include "State.hh"

#include <algorithm>

namespace PLEXIL
{

  //! \class LauncherListener
  //! \brief Helper class to allow plans to monitor plans executed by the Launcher adapter.
  //! \ingroup interface-library
  class LauncherListener : public ExecListener
  {
  public:

    //! \brief Default constructor.
    LauncherListener()
      : ExecListener()
    {
    }

    //! \brief Virtual destructor.
    virtual ~LauncherListener()
    {
    }

    //! \brief Notify that a node has changed state.
    //! \param prevState The old state.
    //! \param newState The new state.
    //! \param node The node that has transitioned.
    virtual void implementNotifyNodeTransition(NodeState prevState,
                                               NodeState newState,
                                               Node *node) const
    {
      assertTrue_2(node,
                   "LauncherListener:implementNotifyNodeTransition received null Node pointer");

      // We only care about root nodes
      // Cheaper to implement here than as an ExecListenerFilter
      if (node->getParent())
        return;

      Value const nodeIdValue(node->getNodeId());
      g_execInterface->handleValueChange(State(PLAN_STATE_STATE, nodeIdValue),
                                         Value(nodeStateName(newState)));
      NodeOutcome o = node->getOutcome();
      if (o != NO_OUTCOME) {
        g_execInterface->handleValueChange(State(PLAN_OUTCOME_STATE, nodeIdValue),
                                           Value(outcomeName(o)));
        FailureType f = node->getFailureType();
        if (f != NO_FAILURE)
          g_execInterface->handleValueChange(State(PLAN_FAILURE_TYPE_STATE, nodeIdValue),
                                             Value(failureTypeName(f)));
      }
    }
  };

  //! \brief Generate the XML DOM representation of a valid Core PLEXIL literal
  //!        for the given Value, and add it as a child to the given parent element.
  //! \param parent The XML element receiving the XML literal value.
  //! \param v Const reference to the value being represented.
  static void valueToExprXml(pugi::xml_node parent, Value const &v)
  {
    ValueType vt = v.valueType();
    if (isArrayType(vt)) {
      pugi::xml_node aryxml = parent.append_child("ArrayValue");
      char const *eltType = typeNameAsValue(arrayElementType(vt));
      aryxml.append_attribute("Type").set_value(eltType);
      Array const *ary;
      v.getValuePointer(ary); // better succeed!
      for (size_t i = 0; i < ary->size(); ++i) {
        // quick & dirty
        aryxml.append_child(eltType).append_child(pugi::node_pcdata)
          .set_value(ary->getElementValue(i).valueToString().c_str());
      }
    }
    else {
      // Scalar value
      parent.append_child(typeNameAsValue(vt)).append_child(pugi::node_pcdata)
        .set_value(v.valueToString().c_str());
    }
  }

  //! \brief Generate a unique serial number on each call, starting from 1.
  //! \return The serial number.
  //! \ingroup interface-library
  static unsigned int nextSerialNumber()
  {
    static unsigned int sl_next = 0;
    return ++sl_next;
  }

  //
  // Helper function
  //

  //! \brief Check the plan name argument of a Launcher command.
  //! \param cmd Pointer to the command being checked.
  //! \return true if the command has a plan name argument and that argument
  //!         is of the appropriate type and known, false otherwise.
  //! \ingroup interface-library
  static bool checkPlanNameArgument(Command *cmd)
  {
    std::vector<Value> const &args = cmd->getArgValues();
    size_t nargs = args.size();
    if (nargs < 1) {
      warn("Not enough parameters to " << cmd->getName() << " command");
      return false;
    }
    if (args[0].valueType() != STRING_TYPE) {
      warn("First argument to " << cmd->getName() << " command is not a string");
      return false;
    }

    std::string const *nodeName = NULL;
    if (!args[0].getValuePointer(nodeName)) {
      warn("Node name parameter value to " << cmd->getName() << " command is UNKNOWN");
      return false;
    }

    return true;
  }

  //! \brief Implement the Launcher StartPlan command.
  //! \param cmd Pointer to the command being executed
  //! \param intf Pointer to the AdapterExecInterface instance.
  //! \see ExecuteCommandHandler
  //! \ingroup interface-library
  static void executeStartPlanCommand(Command *cmd, AdapterExecInterface *intf)
  {
    if (!checkPlanNameArgument(cmd)) {
      intf->handleCommandAck(cmd, COMMAND_FAILED);
      intf->notifyOfExternalEvent();
      return;
    }

    std::vector<Value> const &args = cmd->getArgValues();
    size_t nargs = args.size();

    std::vector<std::string> formals;
    std::vector<Value> actuals;

    for (size_t i = 1; i < nargs; i += 2) {
      if (i + 1 >= nargs) {
        warn("Arguments to " << cmd->getName() << " command not in name-value pairs");
        intf->handleCommandAck(cmd, COMMAND_FAILED);
        intf->notifyOfExternalEvent();
        return;
      }

      if (args[i].valueType() != STRING_TYPE) {
        warn("StartPlan command argument " << i << " is not a String");
        intf->handleCommandAck(cmd, COMMAND_FAILED);
        intf->notifyOfExternalEvent();
        return;
      }
      std::string const *formal = NULL;
      if (!args[i].getValuePointer(formal)) {
        warn("StartPlan command argument " << i << " is UNKNOWN");
        intf->handleCommandAck(cmd, COMMAND_FAILED);
        intf->notifyOfExternalEvent();
        return;
      }
      formals.push_back(*formal);

      if (!args[i + 1].isKnown()) {
        warn("StartPlan command argument " << i + 1 << " is UNKNOWN");
        intf->handleCommandAck(cmd, COMMAND_FAILED);
        intf->notifyOfExternalEvent();
        return;
      }
      actuals.push_back(args[i + 1]);
    }
    
    // Construct XML for wrapper plan (a LibraryNodeCall node)
    std::string const *nodeName = NULL;
    args[0].getValuePointer(nodeName); // for effect, value is known

    std::ostringstream s;
    s << *nodeName << '_' << nextSerialNumber();
    std::string callerId = s.str();
    
    pugi::xml_document doc;
    pugi::xml_node plan = doc.append_child("PlexilPlan");
    pugi::xml_node rootNode = plan.append_child("Node");
    rootNode.append_attribute("NodeType").set_value("LibraryNodeCall");
    rootNode.append_child("NodeId").append_child(pugi::node_pcdata)
      .set_value(callerId.c_str());

    // Construct ExitCondition
    pugi::xml_node exitLookup = rootNode.append_child("ExitCondition")
      .append_child("LookupNow");
    exitLookup.append_child("Name").append_child("StringValue")
      .append_child(pugi::node_pcdata).set_value(EXIT_PLAN_CMD);
    exitLookup.append_child("Arguments").append_child("StringValue")
      .append_child(pugi::node_pcdata).set_value(callerId.c_str());

    pugi::xml_node call = rootNode.append_child("NodeBody")
      .append_child("LibraryNodeCall");
    call.append_child("NodeId").append_child(pugi::node_pcdata).set_value(nodeName->c_str());
    for (size_t i = 0; i < formals.size(); ++i) {
      pugi::xml_node alias = call.append_child("Alias");
      alias.append_child("NodeParameter").append_child(pugi::node_pcdata)
        .set_value(formals[i].c_str());
      valueToExprXml(alias, actuals[i]);
    }
    
    try {
      intf->handleAddPlan(plan);
    }
    catch (ParserException &e) {
      warn("Launching plan " << nodeName << " failed:\n"
           << e.what());
      intf->handleCommandAck(cmd, COMMAND_FAILED);
      intf->notifyOfExternalEvent();
      return;
    }
    intf->handleCommandReturn(cmd, Value(callerId));
    intf->handleCommandAck(cmd, COMMAND_SUCCESS);
    intf->notifyOfExternalEvent();
  }

  //! \class NodeNameEquals
  //! \brief Functor helper class for comparing a node name to a constant string.
  //! \ingroup interface-library
  class NodeNameEquals
  {
  public:

    //! \brief Constructor.
    //! \param s Const reference to the string to be compared.
    NodeNameEquals(std::string const &s)
      : m_string(s)
    {
    }

    //! \brief Copy constructor.
    //! \param other Const reference to another NodeNameEquals instance.
    NodeNameEquals(NodeNameEquals const &other)
      : m_string(other.m_string)
    {
    }

    //! \brief Destructor.
    ~NodeNameEquals()
    {
    }

    //! \brief Copy assignment operator.
    //! \param other Const reference to the object being copied.
    //! \return Reference to *this.
    NodeNameEquals &operator=(NodeNameEquals const &other)
    {
      m_string = other.m_string;
      return *this;
    }

    //! \brief Compare a Node's nodeId to the saved string.
    //! \param n Const pointer to the Node.
    //! \return true if the Node's nodeId is equal to the saved string,
    //!         false otherwise.
    bool operator()(Node const *n)
    {
      return n->getNodeId() == m_string;
    }

  private:
    std::string m_string; //!< The comparison string.
  };

  //! \brief Find the root Node with the given nodeId.
  //! \param nodeName Const reference to the nodeId to search for.
  //! \return If one root Node has the given nodeId, the pointer to that Node;
  //          NULL if not found or multiple root Nodes have the given nodeId.
  //! \ingroup interface-library
  static Node *findNode(std::string const &nodeName)
  {
    // Find the named node
    NodeNameEquals pred(nodeName);
    std::list<Node *> const &allNodes = g_exec->getPlans();
    std::list<Node *>::const_iterator it =
      std::find_if(allNodes.begin(), allNodes.end(), pred);
    if (it == allNodes.end()) {
      warn("No such node"); // FIXME
      return NULL;
    }
    Node *result = *it;
    if (allNodes.end() !=
        std::find_if(++it, allNodes.end(), pred)) {
      warn("Multiple nodes named " << nodeName); // FIXME
      return NULL;
    }
    return result;
  }

  //! \brief Implement the Launcher ExitPlan command.
  //! \param cmd Pointer to the Command.
  //! \param intf Pointer to the AdapterExecInterface.
  //! \see ExecuteCommandHandler
  //! \ingroup interface-library
  static void executeExitPlanCommand(Command *cmd, AdapterExecInterface *intf)
  {
    if (!checkPlanNameArgument(cmd)) {
      intf->handleCommandAck(cmd, COMMAND_FAILED);
      intf->notifyOfExternalEvent();
      return;
    }

    std::vector<Value> const &args = cmd->getArgValues();
    if (args.size() > 1) {
      warn("Too many parameters to " << cmd->getName() << " command");
      intf->handleCommandAck(cmd, COMMAND_FAILED);
      intf->notifyOfExternalEvent();
      return;
    }

    std::string const *nodeName = NULL;
    args[0].getValuePointer(nodeName); // for effect, value is known
    Node *node = findNode(*nodeName);
    if (!node) {
      // Not found or multiples with same name
      intf->handleCommandAck(cmd, COMMAND_FAILED);
      intf->notifyOfExternalEvent();
      return;
    }
    intf->handleValueChange(State(EXIT_PLAN_CMD, args[0]), Value(true));
    intf->handleCommandAck(cmd, COMMAND_SUCCESS);
    intf->notifyOfExternalEvent();
  }

  //! \class Launcher
  //! \brief An interface adapter allowing plans to start, stop, and
  //!        check the status of other plans.
  //! \ingroup interface-library
  class Launcher : public InterfaceAdapter
  {
  public:

    //! \brief Constructor with reference to parent interface.
    //! \param execInterface Reference to the parent AdapterExecInterface.
    Launcher(AdapterExecInterface& execInterface)
      : InterfaceAdapter(execInterface)
    {
    }
  
    //! \brief Constructor with reference to parent interface and configuration XML.
    //! \param execInterface Reference to the parent AdapterExecInterface.
    //! \param xml Const handle to the configuration data.
    Launcher(AdapterExecInterface& execInterface, 
             pugi::xml_node const xml)
      : InterfaceAdapter(execInterface, xml)
    {
    }

    //! \brief Virtual destructor.
    virtual ~Launcher()
    {
    }

    //! \brief Initialize and register the adapter.
    //! \param config Pointer to the AdapterConfiguration instance.
    //! \return true if initialization was successful, false if not.
    virtual bool initialize(AdapterConfiguration *config)
    {
      // Register command implementations
      config->registerCommandHandler(START_PLAN_CMD,
                                     (ExecuteCommandHandler) &executeStartPlanCommand);
      config->registerCommandHandler(EXIT_PLAN_CMD,
                                     (ExecuteCommandHandler) &executeExitPlanCommand);

      // Register our special ExecListener
      config->addExecListener(new LauncherListener());

      // Register these lookups as telemetry-only
      LookupHandler *handler = new LookupHandler();
      config->registerLookupHandler(EXIT_PLAN_CMD, handler);
      config->registerLookupHandler(PLAN_STATE_STATE, handler);
      config->registerLookupHandler(PLAN_OUTCOME_STATE, handler);
      config->registerLookupHandler(PLAN_FAILURE_TYPE_STATE, handler);
      return true;
    }
  };
  
}

//! \brief Module initialization function for the Launcher adapter.
//! \ingroup interface-library
extern "C"
void initLauncher()
{
  REGISTER_ADAPTER(PLEXIL::Launcher, "Launcher");
}
