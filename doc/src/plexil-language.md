# The PLEXIL Language {#plexil-language}

The name *PLEXIL* is short for PLan EXecution and Interchange
Language.  A *plan*, in this usage, is a script for performing some
task.  Plans can be human-generated, or constructed by automated
planning software.

PLEXIL was originally conceived as a language for representing
*contingent* plans.

## Why do we need contingent plans?

Let's suppose we are writing plans for a robotic rover on the surface
of another planet.  We may have a particular sequence of tasks to
perform, e.g. contact Earth, drive to a location, take detailed
pictures of the target, collect rock samples, analyze the samples.

Each of these tasks may have *preconditions*, *invariants*, and
*postconditions*.  Examples of preconditions:

* A rover requires a certain amount of battery charge before it can
  start driving.
* Sample analysis requires samples.

Examples of invariants:

* Contacting Earth requires that the antenna stay aimed at the Earth,
  or the contact will fail.
* Driving a rover requires the battery voltage to stay above a minimum
  threshold.

Examples of postconditions:

* When the rover has finished contacting Earth, it should have a plan
  for that day's activities.
* When the rover is finished driving, we expect it to be in a certain
  location.

%PLEXIL can represent plans which not only detect failure to meet pre-
or post-conditions, and failure to maintain invariants, but which can
*recover* from these failures in a pre-planned fashion.  %PLEXIL can
also represent plans which perform *opportunistic* tasks, e.g. if
something is observed moving across the rover's path, the rover should
immediately take pictures of the moving object.

## What is a PLEXIL plan?

A plan is a tree of *Nodes*.  A plan has one *root node*, and any
number of descendant nodes, or none.

## What is a PLEXIL Node?

See [What is a PLEXIL Node?](@ref what-is-a-node).

## Data Types and Variables in PLEXIL
   
See [Data Types and Variables in PLEXIL](@ref data-types).

## Expressions in PLEXIL

See [Expressions in PLEXIL](@ref plexil-expressions).

## Lookups, Commands, and Planner Updates

See [Lookups, Commands, and Planner Updates](@ref lookups-commands-updates).

## Core PLEXIL

See [Core PLEXIL](@ref core-plexil).

## Extended PLEXIL

See [Extended PLEXIL](@ref extended-plexil).

## Standard PLEXIL

See [Standard PLEXIL](@ref standard-plexil).
