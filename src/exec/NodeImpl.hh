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

#ifndef NODE_IMPL_HH
#define NODE_IMPL_HH

#include "Node.hh"
#include "NodeVariables.hh"
#include "Notifier.hh"

namespace PLEXIL
{

  // Forward references
  class NodeTimepointValue;
  class NodeVariableMap;

  //! \class NodeImpl
  //! \brief The innards shared between node implementation classes,
  //!        the XML parser, and external interfaces; also the 
  //!        implementation class for Empty nodes.
  //! \ingroup Exec-Core
  class NodeImpl :
    public Node,
    public Notifier
  {
  public:

    //! \brief Constant; the worst node priority representable in %PLEXIL.
    static const int32_t WORST_PRIORITY;

    //! \brief Constant list of name strings for all valid %PLEXIL node conditions.
    //! \note Order must be consistent with the ConditionIndex enumeration.
    //! \see ConditionIndex
    static char const * const ALL_CONDITIONS[];

    //! \brief Enumeration of indices associated with each valid %PLEXIL node condition.
    //! \note Order must be consistent with ALL_CONDITIONS.
    //! \see ALL_CONDITIONS
    enum ConditionIndex {
                         // Conditions on parent
                         // N.B. Ancestor end/exit/invariant MUST come before
                         // end/exit/invariant, respectively, because the former depend
                         // on the latter and must be cleaned up first.
                         ancestorExitIdx = 0,
                         ancestorInvariantIdx,
                         ancestorEndIdx,
                         // User specified conditions
                         skipIdx,
                         startIdx,
                         preIdx,
                         exitIdx,
                         invariantIdx,
                         endIdx,
                         postIdx,
                         repeatIdx,
                         // For all but Empty nodes
                         actionCompleteIdx,
                         // For all but Empty and Update nodes
                         abortCompleteIdx,

                         conditionIndexMax
    };
    
    //! \brief Get the condition name associated with the given index.
    //! \param idx The index.
    //! \note Public only for plan analyzer
    static char const *getConditionName(size_t idx);

    //! \brief Primary constructor.
    //! \param nodeId The name of this node, as a pointer to const character string.
    //! \param parent (Optional) Pointer to the parent of this node; used for the ancestor conditions and variable lookup.
    NodeImpl(char const *nodeId, NodeImpl *parent = NULL);

    //! \brief Alternate constructor.  Used only by Exec test module.
    //! \param type Const reference to a node type name.
    //! \param name Const reference to the name to give this instance.
    //! \param state The state to assign this node.
    //! \param parent (Optional) The parent of this node; may be null.
    NodeImpl(const std::string& type,
             const std::string& name,
             NodeState state,
             NodeImpl *parent = NULL);

    //! \brief Virtual destructor.  Deletes the node and all of its descendants, if any.
    virtual ~NodeImpl();

    //
    // Listenable API
    //
    
    //! \brief Query whether an object can generate change notifications.
    //! \return True if so, false if not.
    virtual bool isPropagationSource() const
    {
      return true;
    }

    //! \brief Query whether this object is active in the notification network.
    //! \return true if active, false if not.
    //! \note This method overrides the Notifier method.
    virtual bool isActive() const
    {
      return true;
    }

    //! \brief Make this object active in the notification network.
    //! \note This method does nothing.
    virtual void activate()
    {
    }

    //! \brief Make this object inactive in the notification network.
    //! \note This method does nothing.
    virtual void deactivate()
    {
    }

    //
    // Support for LinkedQueue<Node>
    //

    //! \brief Return a pointer to the next item in the queue.
    //! \return Pointer to a Node; may be null.
    Node *next() const
    {
      return (Node *) m_next;
    }

    //! \brief Return a pointer to the place to store a pointer to the next item in the queue.
    //! \return A pointer to a pointer to Node. Must not be null.
    Node **nextPtr()
    {
      return (Node **) &m_next;
    }

    //! \brief Return the queue status of this node.
    //! \return The status.
    //! \see QueueStatus
    uint8_t getQueueStatus() const
    {
      return m_queueStatus;
    }

    //! \brief Update the queue status of this node.
    //! \param newval The new status value to set.
    //! \see QueueStatus
    void setQueueStatus(uint8_t newval)
    {
      m_queueStatus = newval;
    }

    //! \brief Get a printed representation of this node as a string.
    //! \param indent (Optional) Number of spaces to indent.
    //! \return The string.
    std::string toString(const unsigned int indent = 0);

    //! \brief Print this node to a stream.
    //! \param stream Reference to an output stream.
    //! \param indent (Optional) Number of spaces to indent.
    void print(std::ostream& stream, const unsigned int indent = 0) const;

    //! \brief Make the node active.
    virtual void activateNode();

    //! \brief Return the ID (name) of this node.
    //! \return The node's ID as a const reference to string.
    std::string const &getNodeId() const { return m_nodeId; }

    //! \brief Get the parent of this node,
    //! \return Pointer to this node's parent. May be null.
    NodeImpl *getParentNode() {return m_parent;}

    //! \brief Get the parent of this node,
    //! \return Const pointer to this node's parent. May be null.
    NodeImpl const *getParentNode() const {return m_parent; }

    //! \brief Calculate the destination state of this node, were it to transition,
    //!        based on the values of various conditions.
    //! \return True if the new destination state is different from the last check, false otherwise.
    //! \note Sets m_nextState, m_nextOutcome, m_nextFailureType as a side effect.
    bool getDestState();

    //! \brief Get the previously calculated destination state of this node.
    //! \return The destination state.
    NodeState getNextState() const 
    {
      return (NodeState) m_nextState;
    }

    //! \brief Commit a pending state transition as determined by getDestState.
    //! \param time The time of the transition.
    void transition(double time = 0.0);

    //! \brief Get this node's priority.
    //! \return The priority of this node as an integer.
    //! \note Default method.
    //! \note In PLEXIL 4.6 and earlier, priority only matters for Assignment nodes.
    virtual int32_t getPriority() const {return WORST_PRIORITY;}

    //
    // ExpressionListener API
    //

    //! \brief Notify this object that another object it listens to has changed.
    virtual void notifyChanged();

    //! \brief Query the current state of this node.
    //! \return The current node state.
    NodeState getState() const;

    //! \brief Query the outcome of this node.
    //! \return The current outcome.
    NodeOutcome getOutcome() const;

    //! \brief Query the failure type of this node.
    //! \return The current failure type.
    FailureType getFailureType() const;

    //! \brief Set the node's state.
    //! \param newValue The new node state.
    //! \param tym The timestamp to record for this state transition.
    //! \note Virtual so it can be overridden by ListNode wrapper method.
    //! \note Only used by node implementation classes and unit tests.
    virtual void setState(NodeState newValue, double tym);

    //! \brief Set the node's failure type,
    //! \param f The new value.
    //! \note Only used by unit tests.
    void setNodeFailureType(FailureType f);

    //! \brief Query the time at which this node entered its current state.
    //! \return Time value as a double (Real).
    //! \note Used by GanttListener and PlanDebugListener.
    double getCurrentStateStartTime() const;

    //! \brief Query the time at which this node entered a particular state.
    //! \param state The state.
    //! \return Time value as a double. If the node has never been in the state,
    //!         or has not been the state since the last reset, returns -DBL_MAX.
    //! \note Used by GanttListener and PlanDebugListener.
    //! \see reset
    double getStateStartTime(NodeState state) const;

    //! \brief Find a variable by name.
    //! \param name Name of the variable, as a pointer to const character string.
    //! \return Pointer to the variable; will be null if no variable with
    //!         the given name is found in the node or its direct ancestors.
    //! \note Used only by XML parser.
    Expression *findVariable(char const *name);

    //! \brief Find the named variable in this node, ignoring its ancestors.
    //! \param name Name of the variable, as a pointer to const character string..
    //! \return Pointer to the variable; will be null if the node has no
    //!         variable with that name.
    //! \note Used only by XML parser.
    Expression *findLocalVariable(char const *name);

    //! \brief Get the variable being assigned by this node.
    //! \return Pointer to the variable as an Assignable.
    //! \note Only Assignment nodes will return a non-null value.
    //! \note This default method for all other node types always returns null.
    virtual Assignable *getAssignmentVariable() const 
    {
      return NULL;
    }

    //! \brief Get the type of this node.
    //! \return The type of this node.
    //! \note This method only applies to Empty nodes.
    virtual PlexilNodeType getType() const
    {
      return NodeType_Empty;
    }

    //! \brief Get the parent of this node.
    //! \return Const pointer to the parent node; may be null.
    Node const *getParent() const
    {
      return dynamic_cast<Node const *>(m_parent);
    }
  
    //! \brief Get the children of this node.
    //! \return Reference to a vector of pointers to the children.
    //! \note Only ListNode and LibraryCallNode have chidren.
    //! \note The default method returns an empty vector.
    virtual std::vector<NodeImpl *>& getChildren();

    //! \brief Get the children of this node.
    //! \return Const reference to a vector of pointers to the children.
    //! \note Only ListNode and LibraryCallNode have chidren.
    //! \note The default method returns an empty vector.
    virtual const std::vector<NodeImpl *>& getChildren() const;

    //! \brief Find a child node of this node with the given name.
    //! \param childName The name, as a pointer to const character string.
    //! \return Const pointer to the child node; null if no child with that name exists.
    //! \note Only ListNode and LibraryCallNode have chidren.
    //! \note The default method always returns null.
    virtual NodeImpl const *findChild(char const *childName) const;

    //! \brief Find a child node of this node with the given name.
    //! \param childName The name, as a pointer to const character string.
    //! \return Pointer to the child; null if no child with that name exists.
    //! \note Only ListNode and LibraryCallNode have chidren.
    //! \note The default method always returns null.
    virtual NodeImpl *findChild(char const *childName);

    //
    // Utilities for plan parser and analyzer
    //

    //! \brief Reserve space in the node's variable vector and variable map.
    //! \param n The number of entries to reserve.
    void allocateVariables(size_t n);

    //! \brief Add a named "variable" to the node, to be deleted with the node.
    //! \param name The name as a pointer to const character string.
    //! \param var Pointer to the expression to associate with the name.
    //!            It will be deleted when the node is deleted.
    //! \return true if successful, false if name is a duplicate
    bool addLocalVariable(char const *name, Expression *var);

    //! \brief Get the node's name -> variable map.
    //! \return Const pointer to the map; may be null.
    //! \note Used by GanttListener.
    NodeVariableMap const *getVariableMap() const { return m_variablesByName; }

    //! \brief Add a condition expression to the node.
    //! \param cname The name of the condition as a pointer to const character string.
    //! \param cond Pointer to the expression.
    //! \param isGarbage True if the expression should be deleted with the node, false otherwise.
    void addUserCondition(char const *cname, Expression *cond, bool isGarbage);

    //! \brief Construct any internal conditions now that the node is complete.
    void finalizeConditions();

    //! \brief Get the variable representing this node's state.
    //! \return Pointer to the state variable. Will never be null.
    Expression *getStateVariable() { return &m_stateVariable; }

    //! \brief Get the variable representing this node's outcome.
    //! \return Pointer to the outcome variable. Will never be null.
    Expression *getOutcomeVariable() { return &m_outcomeVariable; }

    //! \brief Get the variable representing this node's failure type.
    //! \return Pointer to the failure type variable. Will never be null.
    Expression *getFailureTypeVariable() { return &m_failureTypeVariable; }

    //! \brief Get or create the timepoint variable corresponding to the given state's start or end.
    //! \param st The node state.
    //! \param isEnd True to get the end timepoint; false to get the start timepoint.
    //! \note Used by plan parser.
    Expression *ensureTimepoint(NodeState st, bool isEnd);

    //! \brief Get the vector of this node's local variables.
    //! \return Pointer to the vector; may return NULL.
    //! \note Used by plan analyzer and plan parser module test only.
    const std::vector<Expression *> *getLocalVariables() const { return m_localVariables; }

    //
    // Condition accessors
    // These are public only to appease the module test
    //

    //! \brief Get the ancestor-end condition used by this node.
    //! \return Pointer to the ancestor-end condition expression;
    //!         will be null if the node has no parent.
    //! \note This condition is owned by the parent node.
    Expression *getAncestorEndCondition()               { return getCondition(ancestorEndIdx); }

    //! \brief Get the ancestor-exit condition used by this node.
    //! \return Pointer to the ancestor-exit condition expression;
    //!         will be null if the node has no parent.
    //! \note This condition is owned by the parent node.
    Expression *getAncestorExitCondition()              { return getCondition(ancestorExitIdx); }

    //! \brief Get the ancestor-invariant condition used by this node.
    //! \return Pointer to the ancestor-invariant condition expression;
    //!         will be null if the node has no parent.
    //! \note This condition is owned by the parent node.
    Expression *getAncestorInvariantCondition()         { return getCondition(ancestorInvariantIdx); }

    //
    // User conditions
    //

    //! \brief Get this node's skip condition.
    //! \return Pointer to the skip condition expression; may be null.
    Expression *getSkipCondition()                      { return m_conditions[skipIdx]; }

    //! \brief Get this node's start condition.
    //! \return Pointer to the start condition expression; may be null.
    Expression *getStartCondition()                     { return m_conditions[startIdx]; }

    //! \brief Get this node's end condition.
    //! \return Pointer to the end condition expression; may be null.
    Expression *getEndCondition()                       { return m_conditions[endIdx]; }

    //! \brief Get this node's exit condition.
    //! \return Pointer to the exit condition expression; may be null.
    Expression *getExitCondition()                      { return m_conditions[exitIdx]; }

    //! \brief Get this node's invariant condition.
    //! \return Pointer to the invariant condition expression; may be null.
    Expression *getInvariantCondition()                 { return m_conditions[invariantIdx]; }

    //! \brief Get this node's precondition.
    //! \return Pointer to the precondition expression; may be null.
    Expression *getPreCondition()                       { return m_conditions[preIdx]; }

    //! \brief Get this node's postcondition.
    //! \return Pointer to the postcondition expression; may be null.
    Expression *getPostCondition()                      { return m_conditions[postIdx]; }

    //! \brief Get this node's repeat condition.
    //! \return Pointer to the repeat condition expression; may be null.
    Expression *getRepeatCondition()                    { return m_conditions[repeatIdx]; }

    //
    // The conditions below are only populated for specialized node types.
    //

    //! \brief Get this node's action-complete condition.
    //! \return Pointer to the action-complete condition expression; may be null.
    Expression *getActionCompleteCondition()            { return m_conditions[actionCompleteIdx]; }

    //! \brief Get this node's abort-complete condition.
    //! \return Pointer to the abort-complete condition expression; may be null.
    Expression *getAbortCompleteCondition()             { return m_conditions[abortCompleteIdx]; }

    //! \brief Get the condition corresponding to the given index.
    //! \return Const pointer to the condition expression; may be null.
    //! \note Used internally, also by LuvListener.
    //! \note Non-const variant is protected.
    Expression const *getCondition(size_t idx) const;

  protected:

    friend class ListNode;

    //! \brief Get the condition corresponding to the given index.
    //! \return Pointer to the condition expression; may be null.
    Expression *getCondition(size_t idx);

    //! \brief Get the name -> variable mapping that children of this node should reference.
    //! \return Const pointer to a variable map; may be null.
    //! \note Only used by Node, ListNode, LibraryCallNode.
    //! \note The default method returns null.
    virtual NodeVariableMap const *getChildVariableMap() const;

    //! \brief Perform common initialization for all node types.
    void commonInit();

    //
    // Member functions called from the transition handler
    //
    
    //! \brief Perform the actions specified for EXECUTING state.
    //! \note Delegates to virtual member function specialized by node type.
    //! \see specializedHandleExecution
    void execute();

    //! \brief Reset the node's outcome and failure type.
    void reset();

    //! \brief Deactivate the expressions relevant to the node's execution.
    //! \note Delegates to virtual member function specialized by node type.
    //! \see specializedDeactivateExecutable
    void deactivateExecutable();

    //
    // Activation and deactivation of variables
    //

    //! \brief Activate this node's local variables.
    void activateLocalVariables();

    //! \brief Deactivate this node's local variables.
    void deactivateLocalVariables();

    //
    // Activate conditions
    //

    //! \brief Activate the ancestor-end condition used by this node, if there is one.
    //! \note The ancestor-end condition is owned by the parent node, if there is one.
    void activateAncestorEndCondition();

    //! \brief Activate the ancestor-exit and -invariant conditions used by this node,
    //!        if they exist.
    //! \note These conditions are owned by the parent node, if there is one.
    void activateAncestorExitInvariantConditions();

    // User conditions
    //

    //! \brief Activate the node's precondition, skip condition, and start condition expressions.
    void activatePreSkipStartConditions();

    //! \brief Activate the node's end condition expression. 
    void activateEndCondition();
    
    //! \brief Activate the node's exit condition expression. 
    void activateExitCondition();

    //! \brief Activate the node's invariant condition expression.
    void activateInvariantCondition();

    //! \brief Activate the node's postcondition expression.
    void activatePostCondition();

    //! \brief Activate the node's repeat condition expression.
    void activateRepeatCondition();

    // These are for specialized node types

    //! \brief Activate the node's action-complete condition expression.
    void activateActionCompleteCondition();

    //! \brief Activate the node's abort-complete condition expression.
    void activateAbortCompleteCondition();

    //
    // Deactivate a condition
    //

    //! \brief Deactivate the ancestor-end condition used by this node, if there is one.
    //! \note The ancestor-end condition is owned by the parent node, if there is one.
    void deactivateAncestorEndCondition();

    //! \brief Deactivate the ancestor-exit and -invariant conditions used by this node,
    //!        if they exist.
    //! \note These conditions are owned by the parent node, if there is one.
    void deactivateAncestorExitInvariantConditions();

    // User conditions
    //

    //! \brief Deactivate the node's precondition, skip condition, and start condition expressions.
    void deactivatePreSkipStartConditions();

    //! \brief Deactivate the node's end condition expression. 
    void deactivateEndCondition();

    //! \brief Deactivate the node's exit condition expression. 
    void deactivateExitCondition();

    //! \brief Deactivate the node's invariantcondition expression. 
    void deactivateInvariantCondition();

    //! \brief Deactivate the node's postcondition expression. 
    void deactivatePostCondition();

    //! \brief Deactivate the node's repeat condition expression. 
    void deactivateRepeatCondition();

    // These are for specialized node types

    //! \brief Deactivate the node's action-complete condition expression. 
    void deactivateActionCompleteCondition();

    //! \brief Deactivate the node's abort-complete condition expression. 
    void deactivateAbortCompleteCondition();

    //
    // Specialised behaviors for derived classes
    //

    //! \brief Create any condition wrapper expressions appropriate to the node type.
    //! \note The default method does nothing.
    virtual void specializedCreateConditionWrappers();

    //! \brief Perform activations appropriate to the node type.
    //! \note The default method does nothing.
    virtual void specializedActivate();

    //! \brief Perform the execution operations appropriate to the node type.
    //! \note The default method does nothing.
    virtual void specializedHandleExecution();

    //! \brief Perform deactivations appropriate to the node type.
    //! \note The default method does nothing.
    virtual void specializedDeactivateExecutable();

    //
    // State transition implementation methods
    //
    // Non-virtual member functions are common to all node types.
    // Virtual members are specialized by node type.
    //

    // getDestStateFrom...
    // Return true if the new destination state is different from the last check, false otherwise.
    // Set m_nextState, m_nextOutcome, m_nextFailureType as a side effect.

    //! \brief Determine the destination state from INACTIVE.
    //! \return True if the new destination state differs from last check; false otherwise.
    bool getDestStateFromInactive();

    //! \brief Determine the destination state from WAITING.
    //! \return True if the new destination state differs from last check; false otherwise.
    bool getDestStateFromWaiting();

    //! \brief Determine the destination state from EXECUTING.
    //! \return True if the new destination state differs from last check; false otherwise.
    //! \note Default method.
    virtual bool getDestStateFromExecuting();

    //! \brief Determine the destination state from FINISHING.
    //! \return True if the new destination state differs from last check; false otherwise.
    //! \note The default method throws an exception.
    virtual bool getDestStateFromFinishing();

    //! \brief Determine the destination state from FINISHED.
    //! \return True if the new destination state differs from last check; false otherwise.
    bool getDestStateFromFinished();

    //! \brief Determine the destination state from FAILING.
    //! \return True if the new destination state differs from last check; false otherwise.
    //! \note The default method throws an exception.
    virtual bool getDestStateFromFailing();

    //! \brief Determine the destination state from ITERATION_ENDED.
    //! \return True if the new destination state differs from last check; false otherwise.
    bool getDestStateFromIterationEnded();

    //
    // Transitions out of the named current state
    //
    
    //! \brief Transition out of INACTIVE state.
    void transitionFromInactive();
    
    //! \brief Transition out of WAITING state.
    void transitionFromWaiting();
    
    //! \brief Transition out of EXECUTING state.
    //! \note The base class method is specific to Empty nodes.
    virtual void transitionFromExecuting();
    
    //! \brief Transition out of FINISHING state.
    //! \note The default method throws an exception.
    virtual void transitionFromFinishing();
    
    //! \brief Transition out of FINISHED state.
    void transitionFromFinished();
    
    //! \brief Transition out of FAILING state.
    //! \note The default method throws an exception.
    virtual void transitionFromFailing();
    
    //! \brief Transition out of ITERATION_ENDED state.
    void transitionFromIterationEnded();

    //
    // Transitions into the named next state
    //

    //! \brief Transition into INACTIVE state.
    void transitionToInactive();

    //! \brief Transition into WAITING state.
    void transitionToWaiting();

    //! \brief Transition into EXECUTING state.
    //! \note Default method.
    virtual void transitionToExecuting();

    //! \brief Transition into FINISHING state.
    //! \note The default method throws an exception.
    virtual void transitionToFinishing();

    //! \brief Transition into FINISHED state.
    void transitionToFinished();

    //! \brief Transition into FAILING state.
    //! \note The default method throws an exception.
    virtual void transitionToFailing();

    //! \brief Transition into ITERATION_ENDED state.
    void transitionToIterationEnded(); 

    //
    // Helper member functions for destructors
    //
    // Note that virtual member functions called from the base class destructor
    // will only invoke the base class method.

    //! \brief Delete condition expressions as applicable for the node type.
    //! \note Sets m_cleanedConditions when finished, so as to avoid repeating work already done.
    //! \note This member function may be called multiple times during deletion.
    //! \note This base class **method** may be called multiple times during deletion.
    //! \note Derived class methods must delete any condition expressions
    //!       specific to that node type.
    //! \note Derived class methods **must** explicitly call this base class method **last**.
    virtual void cleanUpConditions();

    //! \brief Delete any additional objects as applicable for the node type.
    //! \note It is not useful to call this member function from the base class destructor.
    virtual void cleanUpNodeBody();

    //! \brief Delete all the variable expressions in this node.
    //! \note Must not be called until the conditions and the node body have been deleted.
    void cleanUpVars();

    //! \brief Print this node's command handle value to an output stream.
    //! \param stream Reference to the output stream.
    //! \param indent Number of spaces to indent.
    //! \note Called from the print method.
    //! \note The default method does nothing.
    virtual void printCommandHandle(std::ostream& stream, const unsigned int indent) const;

    //
    // Common state for all node types
    //

    Node   *m_next;                /*!< For LinkedQueue<Node> */
    uint8_t m_queueStatus;         /*!< Which exec queue the node is in, if any. */
    uint8_t m_state;               /*!< The current state of the node. */
    uint8_t m_outcome;             /*!< The current outcome. */
    uint8_t m_failureType;         /*!< The current failure. */

    bool m_pad; // to ensure 8 byte alignment
    uint8_t m_nextState;           /*!< The state returned by getDestState() the last time checkConditions() was called. */
    uint8_t m_nextOutcome;         /*!< The pending outcome. */
    uint8_t m_nextFailureType;     /*!< The pending failure. */

    NodeImpl *m_parent;                          /*!< The parent of this node.*/
    Expression *m_conditions[conditionIndexMax]; /*!< The condition expressions. */
 
    std::vector<Expression *> *m_localVariables; /*!< Variables created in this node. */
    StateVariable m_stateVariable;
    OutcomeVariable m_outcomeVariable;
    FailureVariable m_failureTypeVariable;
    NodeVariableMap *m_variablesByName; /*!< Locally declared variables or references to variables gotten through an interface. */
    std::string m_nodeId;  /*!< the NodeId from the xml.*/

  private:
    
    // Node transition history trace
    double m_currentStateStartTime;
    NodeTimepointValue *m_timepoints;

  protected:

    // Housekeeping details
    bool m_garbageConditions[conditionIndexMax]; /*!< Flags for conditions to delete. */
    bool m_cleanedConditions, m_cleanedVars, m_cleanedBody;

  private:

    //! \brief Create any necessary condition wrapper expressions.
    //! \note Only called from finalizeConditions.
    //! \note Delegates to specializedCreateConditionWrappers.
    void createConditionWrappers();

    //! \brief Set the node's outcome to the given value.
    //! \param o The new value.
    //! \note Should only be called from NodeImpl::transition.
    void setNodeOutcome(NodeOutcome o);

    //! \brief Transition out of the current state.
    //! \note Should only be called from NodeImpl::transition.
    void transitionFrom();

    //! \brief Transition into the next state as specified by m_nextState.
    //! \param typ The timestamp for the transition.
    //! \note Should only be called from NodeImpl::transition.
    void transitionTo(double tym);

    //! \brief Log the time of this transition for the node's timepoint variables.
    //! \param time The timestamp for the transition.
    //! \param newState The state to which the node is transitioning.
    //! \note Called only from NodeImpl::commonInit and NodeImpl::setState.
    void logTransition(double time, NodeState newState);

    //! \brief Print the node's variables and their values to an stream.
    //! \param stream Reference to the output stream.
    //! \param indent (Optional) Number of spaces to indent.
    //! \note Called only from NodeImpl::print.
    void printVariables(std::ostream& stream, const unsigned int indent = 0) const;

  };

}

#endif // NODE_IMPL_HH
