# What is a PLEXIL Node? {#what-is-a-node}

A Node in PLEXIL is a unit of contingent action.  A Node has a *node
state*, an *outcome*, and optionally a *failure type*; the node state
evolves as a function of the Node's *conditions*.  There are several
Node *types* specialized for different purposes.

## Node types

Nodes are distinguished by what action they perform when executed.

Available *node types* are:

* `Empty` - The Node performs no action when executed.
* `Assignment` - The Node assigns a value to a *variable* when executed.
* `Command` - The Node executes a *command* when executed.
* `PlannerUpdate` - The Node executes a *planner update*, i.e. sends
  information as a list of name-value pairs, to an external system
  when executed.
* `NodeList` - The Node has *child Nodes*, which become eligible to
  execute when the `NodeList` node is executed.
* `LibraryNodeCall` - The Node has one child, a *library Node*, which
  becomes eligible to execute when the `LibraryNodeCall` node is
  executed.  It can have *aliases*, which pass data to the library
  Node.  The called Node cannot access any of the ancestors' state if
  it is not passed as an alias.

## Node states

A Node can take on a number of different *node states*:

* `INACTIVE` - the Node is not yet eligible to execute.  This is the
  initial state of all Nodes.  The Node's conditions are ignored.
* `WAITING` - the Node is eligible to execute, but neither its *start
  condition* nor its *skip condition* has yet become true.
* `EXECUTING` - the Node has started and is performing its designated
  action.
* `FAILING` - either the Node's or an ancestor Node's *invariant
  condition* has become false, or the *exit condition* has become
  true, and the action the Node was performing is being aborted.
  Applicable to all but `Empty` Nodes.
* `FINISHING` - the Node's *end condition* has become true, but some
  action it or its children were performing has not yet completed.
  Only `Command`, `NodeList`, and `LibraryNodeCall` nodes implement
  this state. 
* `ITERATION_ENDED` - the Node's action is complete, but it may be
  eligible to reset to `INACTIVE` if its *repeat condition* is true.
* `FINISHED` - the Node is no longer eligible to execute.  This is the
  terminal state of all nodes.

## Node outcomes

A Node's *outcome* tells what kind of transition brought the Node to
the `ITERATION_ENDED` or `FINISHED` state.  The outcome is only
relevant when the node is in those states.  It is *unknown* when the
Node is in any other state.

The possible Node outcomes are:

* `SUCCESS` - the Node transitioned from `EXECUTING` to
  `ITERATION_ENDED` (possibly via `FINISHING`) normally.
* `FAILURE` - the Node or one of its ancestors had an *invariant
  condition* become false.
* `INTERRUPTED` - the Node or one of its ancestors had an *exit
  condition* become true.
* `SKIPPED` - the Node was never executed; it transitioned directly
  from `WAITING` to `FINISHED`.

## Failure types

In the event a Node's outcome is either `FAILURE` or `INTERRUPTED`,
the node's *failure type* tells how it got that way.  As with
outcomes, a Node's failure type is only valid if the Node's state is
`ITERATION_ENDED` or `FINISHED`, and is *unknown* in any other node
state.

Possible failure types are:

* `PRE_CONDITION_FAILED` - the Node's *precondition* was false or
  *unknown* when its *start condition* became true.
* `POST_CONDITION_FAILED` - the Node executed normally, but its
  *postcondition* evaluated to false or *unknown* as it transitioned
  to `ITERATION_ENDED`.
* `INVARIANT_CONDITION_FAILED` - the Node's *invariant condition*
  became false while the Node was in `EXECUTING` state. 
* `PARENT_FAILED` - The invariant condition of an *ancestor node*
  became false while the Node was in `EXECUTING` state.
* `EXITED` - The Node's *exit condition* became true while the Node
  was in `EXECUTING` state.
* `PARENT_EXITED` - The exit condition of an *ancestor node*
  became true while the Node was in `EXECUTING` state.

## Node conditions

A *condition* is a Boolean expression which participates in the *node
state transition* algorithm.  (PLEXIL implements a 3-state "Boolean"
logic, in which the states are true, false, and *unknown*.  More about
this later.)

Which conditions are *active* depends on the Node's current state, and
whether it has *ancestors*.

Nodes have the following *explicit conditions,* in alphabetical order:

* `EndCondition` - when true, tells the Node to exit normally.
* `ExitCondition` - when true, terminates the Node early, possibly
  without executing at all.
* `InvariantCondition` - when false or *unknown*, tells the Node to
  *fail*.
* `PreCondition` - when a Node's `StartCondition` becomes true, if the
  `PreCondition` is true, the Node *executes*; when false or unknown,
  the Node *fails*.
* `RepeatCondition` - evaluated when the Node is in `ITERATION_ENDED`
  state.  When true, causes the Node to *repeat*, i.e. transition back
  to `WAITING` state; when false, causes the Node to transition to
  `FINISHED`; when *unknown*, the Node remains in `ITERATION_ENDED`
  state.
* `SkipCondition` - evaluated when the node is in `WAITING` state.
  When true, *skips* the Node without executing it.
* `StartCondition` - when true, the Node can *execute* if the
  `PreCondition` is also true, otherwise the Node *fails*.

Nodes with *parent Nodes*, i.e. child Nodes of `NodeList` Nodes and
library Nodes called by `LibraryNodeCall` Nodes, also *inherit*
conditions from those parents and direct *ancestors*
(i.e. grandparents, great-grandparents, etc., but not their siblings).
These conditions are not explicitly accessible in the PLEXIL language,
but they drive state transitions, and are documented in the node state
transition rules:

* *AncestorEndCondition* - True when any direct ancestor node's
  `EndCondition` becomes true.
* *AncestorExitCondition* - True when any direct ancestor node's
  `ExitCondition` becomes true.
* *AncestorInvariantCondition* - False or *unknown* when any direct
  ancestor node's `InvariantCondition` becomes false or *unknown*,
  respectively.
  
`Command`, `Update`, `NodeList`, and `LibraryNodeCall` Nodes have
additional implicit conditions specific to their functions.  See the
node state transition rules for specifics.

## Node variables

Any Node type can declare *variables*.  These variables are accessible
to the declaring Node and all descendants.  If a child Node declares a
variable with the same name as an ancestor Node's variable, the
child's variable *shadows* the ancestor's, and the ancestor's becomes
inaccessible in the child and any of its descendants.

Variables are *strongly typed*. Variables may be declared with
`Boolean`, `Integer`, `Real`, or `String` types.  One-dimensional,
homogenous *array variables* with elements of those types can also be
declared, using a distinct syntax.  See
[Data Types and Variables in PLEXIL](@ref data-types)
for more information on the available data types.

Variables can be declared to take an *initial value* of a compatible
type.  Currently variable initial value expressions are restricted to
literal values and references to other variables.  This restriction
may be removed in future PLEXIL releases.

Variables are initially *inactive*.  While inactive, their value
cannot be known.  They become *active* when the declaring Node
transitions to `EXECUTING`.  When activated, the declared initial
value is evaluated and assigned to the variable; if no initial value
has been declared, the variable is initialized to *unknown*.
Variables become inactive again when the Node transitions to
`ITERATION_ENDED` or `FINISHED`, whichever comes first.

Variables can be assigned a new value by `Assignment` Nodes.  They can
also receive return values from `Command` Nodes.
