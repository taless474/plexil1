# Extended %PLEXIL {#extended-plexil}

*Extended %PLEXIL* is a macro layer, implemented in XSLT, which adds
some "syntactic sugar" to simplify writing %PLEXIL plans.  It provides
some common higher-level control structures; `Date` and `Duration`
data types; a simplified `Lookup` expression; and various predicates
on node state, outcome, and failure type.

## Control structures

[Core PLEXIL](@ref core-plexil) has no higher-level control
structures.  `NodeList` Nodes are inherently concurrent; any
sequencing between sibling Nodes must be explicitly expressed as
conditions.

Extended %PLEXIL remedies this by adding control structures for
parallel execution, sequential execution, conditional execution,
iteration, and delay.

### Parallel execution: Concurrence

- `Concurrence` executes its children in parallel.  When a
  `Concurrence` transitions to `EXECUTING`, all its child Nodes are
  eligible to start, and by default all start at once.  It is
  straightforwardly expanded to a `NodeList` node.

### Sequential execution: Sequence, CheckedSequence, UncheckedSequence, and Try

In most programming languages, sequential execution of statements is a
given.  But in %PLEXIL, *parallel* execution of sibling Nodes is the
default.  Explicit sequencing of more than a couple of nodes in Core
%PLEXIL gets rather complex, rather quickly.

Extended %PLEXIL provides several sequencing constructs.

- `CheckedSequence` executes its children in the order in which they
  appear in the plan.  If all of the children succeed, the
  `CheckedSequence` also succeeds.  If any of the children fail, the
  `CheckedSequence` also fails with a failure type of
  `INVARIANT_FAILED`.


- `UncheckedSequence` executes its children in the order they appear
  in the plan, irrespective of the outcome of the children.  In the
  absence of a user-specified `InvariantCondition` or `ExitCondition`,
  an `UncheckedSequence` will always succeed.

- `Sequence` is an alias for `CheckedSequence`.  A future release of
  Extended %PLEXIL may alias it to `UncheckedSequence` instead.

- `Try` executes each of its child Nodes in order.  If any of the
  children finishes with a `SUCCESS` outcome, the rest of the children
  are skipped, and the `Try` also finishes with outcome `SUCCESS`.  If
  the last child of a `Try` finishes with some other outcome, the
  outcome of the `Try` is `FAILED` with a failure type of
  `POST_CONDITION_FAILED`.

### Conditional execution: If

- `If` implements the common if-then-elseif-else conditional
  construct.  When the `Condition` is true, the `Then` action is
  executed.  Any `ElseIf` clauses are similarly processed in the
  order in which they appear.  Finally, if there is an `Else` action,
  it is executed if all of the `Condition` clauses evaluate to false.
  Any actions which are not executed are skipped.

### Iteration: Do, While, and For

Extended %PLEXIL provides several familiar iteration constructs.

- `Do` implements a classic 'do Action while Condition' loop.  It
  expands rather straightforwardly to a node with a `RepeatCondition`.

- `While` implements a classic 'while Condition do Action' loop.  The
  expansion of a `While` element may be a single Node in simple
  cases.

- `For` implements a restricted variant of the common 'for' loop.  It
  has a `LoopVariable` clause declaring a Real or Integer variable
  with optional `InitialValue`, a `Condition` expression , a
  `LoopVariableUpdate` expression which is assigned to the loop
  variable at the end of every iteration, and an `Action`.

(An enhanced `For` construct, with semantics more closely
approximating a simple C 'for' statement, may be available in a future
release of Extended %PLEXIL.)

### Delay: Wait

- `Wait` element does just what its name implies: It waits for the
  interval specified in its `Units` parameter.  If `Units` is a
  numeric value, it is interpreted as seconds.  (The `Tolerance`
  parameter is deprecated and will be removed in a future release.)
  `Wait` is implemented as an `Empty` Node with an `EndCondition`
  which becomes true `Units` seconds after the Node starts.

### Inter-executive communication: OnCommand and OnMessage

`OnCommand` and `OnMessage` are meant to be used with the %PLEXIL
Executive's IpcAdapter and UdpAdapter.

More documentation to be supplied.

## Date and Duration types

Many plans need to reason about time.  Core %PLEXIL has no native
representation for dates and durations.  Extended %PLEXIL adds `Date`
and `Duration` types.  Their interpretations are taken from ISO
standard 8601:

- The `Date` type represents a point in time.  Its values are
  interpreted as the number of seconds since the beginning of the Unix
  epoch, 1970-01-01 00:00:00+0000.  The contents of `DateValue`
  elements are parsed as ISO 8601 Dates.  The variable form is
  `DateVariable`.

- The `Duration` type represents an interval between two time points.
  Its values are interpreted as seconds.  The contents of
  `DurationValue` values are parsed as ISO 8601 Durations.  The
  variable form is `DateVariable`.

Both `Date` and `Duration` types are mapped to the Core %PLEXIL `Real`
type.

## Genereralized Lookup

Core %PLEXIL provides two kinds of lookups: `LookupNow` and
`LookupOnChange`.  Which one is appropriate depends on the context;
`LookupNow` is best used when an instantaneous sample is required,
while `LookupOnChange` is appropriate for waiting for an external
event.

The `Lookup` element in Extended %PLEXIL subsumes both `LookupNow` and
`LookupOnChange`.  It expands into the appropriate lookup form based
on the context.  If `Lookup` is in a context where it is expanded to
`LookupNow`, any `Tolerance` element is ignored.

## Node predicates

Extended %PLEXIL augments the node state, outcome, and failure type
predicates provided in Core %PLEXIL with frequently needed
combinations:

- `IterationSucceeded` is true when the referenced Node is in
  `ITERATION_ENDED` state and has an outcome of `SUCCESS`, false
  otherwise.

- `Interrupted` is true when the referenced Node is in `FINISHED`
  state and has an outcome of `INTERRUPTED`, false otherwise.

- `IterationFailed` is true when the referenced Node is in
  `ITERATION_ENDED` state and has an outcome of `FAILURE`, false
  otherwise.

- `InvariantFailed` is true when the referenced Node is in `FINISHED`
  state, has an outcome of `FAILURE`, and a failure type of
  `INVARIANT_FAILED`, false otherwise.

- `PreconditionFailed` is true when the referenced Node is in `FINISHED`
  state, has an outcome of `FAILURE`, and a failure type of
  `PRE_CONDITION_FAILED`, false otherwise.

- `ParentFailed` is true when the referenced Node is in `FINISHED`
  state, has an outcome of `FAILURE`, and a failure type of
  `PARENT_FAILED`, false otherwise.

Note that some of these predicates may be implemented in Core %PLEXIL
in a future release.
