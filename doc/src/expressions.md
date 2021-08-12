# Expressions {#expressions}

\namespace PLEXIL
\addtogroup Expressions

The expression subsystem of the %PLEXIL Executive serves two key purposes:

- Determing when nodes can change state;

- General purpose computation.

## Expression representation

The Expression class serves as the basis for all expressions in the
%PLEXIL Executive.

## The change notification network

%Node state transitions in %PLEXIL are dictated by the values of each
node's conditions.  It would quickly become impractical to evaluate
every condition expression at every step of the Executive.  The change
notification network allows the Executive to only evaluate conditions
whose inputs have changed since the last step.  It also allows the
Executive to ignore conditions which are irrelevant in a particular
node state.

### Change notifications and listeners

Expressions can generate *change notifications* when their value is
updated.  Objects can *listen* for those notifications;
e.g. expressions listen for changes on their subexpressions.
Crucially, nodes can listen for changes on their conditions, and only
consider potential state transitions when a relevant condition has
changed.  The %PLEXIL Executive is therefore *event driven*.

### Active and inactive expressions

The change notification network also allows expressions to be *active*
when their changes are relevant to the node state transition
algorithm, and *inactive* when changes on those expressions would be
irrelevant.  Inactive expressions can neither generate nor propagate
change notifications.

The expression subsystem makes the additional distinction that an
inactive expression has an *unknown* value.  The rationale is that if
an expression cannot participate in a node state transition decision,
its value is irrelevant.  (This behavior may change in future %PLEXIL
releases.)

### Implementations

The base class for objects which monitor expressions for changes is
\ref ExpressionListener.  It defines one pure virtual method,
\ref ExpressionListener::notifyChanged.  Classes derived from
ExpressionListener can implement whatever behavior is appropriate.
Note that objects other than expressions can derive from
ExpressionListener.

Listeners can be considered *leaves* of the change notification graph.

Expressions can be divided into several categories with respect to
change notification and propagation behavior:

- Expressions whose values can never change; constants.  They do not
  participate in any notable way in the change notification graph.

- Expressions whose values can change independently of any other
  expressions.  Examples might include variables, random number
  generators, Lookups of a fixed state, etc.  These can be considered
  the *roots* of the change notification graph.
  
- Expressions whose values only change when a subexpression changes.
  E.g. arithmetic and logical operators, functions, etc.  These are
  *interior nodes* of the change notification graph.
  
- Expressions whose values change with subexpression changes, *and*
  can change independently even when the subexpressions do not change.
  E.g. a Lookup whose name or parameters are computed; a random
  generator function with computed parameters; etc.  These are also
  interior nodes.

The classes provided by the expression subsystem to handle these
categories are:

- \ref Listenable - an abstract base class which defines the
  API for expressions which an ExpressionListener can monitor for
  changes.  It defines the virtual member functions
  \ref Listenable::addListener,
  \ref Listenable::removeListener,
  \ref Listenable::activate,
  \ref Listenable::deactivate,
  and \ref Listenable::isActive.
  It does not implement any behavior of its own; all its
  methods are no-ops, except isActive, which always returns `true`.
  \ref Expression derives from Listenable.

- \ref Notifier - implements registration of listeners, and
  notifies those listeners via the member function 
  \ref Notifier::publishChange.

- \ref Propagator - derived from both Notifier and
  ExpressionListener, adds monitoring of its subexpressions and calls
  publishChange when a subexpression changes.  Propagator's
  notifyChanged method calls a protected member function, 
  \ref Propagator::handleChange, 
  which derived classes can specialize to perform appropriate actions
  when notified.

Constant expressions are derived from Listenable (indirectly, as a
superclass of Expression).  Expressions whose values change
independently should derive from Notifier.  Expressions whose values
are solely dependent on subexpressions should derive from Propagator.
Expressions whose values are both dependent on subexpressions and can
change independently should derive from Propagator, *and* should
ensure that they call the publishChange member function when their
values change independently.
