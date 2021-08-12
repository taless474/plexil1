\page exec-algorithm The %PLEXIL Executive Algorithm

This is an overview of the execution algorithm implemented in the
PLEXIL Executive Application Framework.

## The components of a PLEXIL Executive application

Broadly speaking, a %PLEXIL Executive application consists of these
objects:

- The *plans*, a forest of *nodes*;
- The *Executive engine*, which implements the node state transition and
  conflict resolution rules;
- An *input queue*, for data received from the outside world;
- An *output queue* for variable assignments, commands, and updates to
  be performed as a result of node state transitions;
- *Interfaces* to perform the commands and updates, and to send data
  to the input queue;
- A *main program* to set up, drive, and tear down the other objects.

Additional details will be exposed on subsequent pages, but this
overview should suffice for the explanation below.

## The PLEXIL execution algorithm

In broad strokes, this is how a %PLEXIL Executive application works.

1. Data arriving from the outside world, via user-defined interfaces,
   is placed on the input queue as it arrives.  This may include the
   state of the external world and the controlled system, feedback
   from command and update execution, or new plans to execute.
   
   This process goes on continuously, in parallel with the application
   main loop, for as long as the application is running.

2. (Start of the application main loop)

   At an appropriate time for the particular application, the current
   time is queried and cached, and the input queue is processed.  This
   step updates the values of *lookups* (including the `time` lookup),
   *command acknowledgement variables* (command handles), *command
   return variables*, and *update acknowledgement flags*.  This step
   may also load new plans into the Executive.

3. The updates from step 2 may trigger *change notifications*, which
   propagate throughout the forest of nodes.  Nodes that receive
   change notifications become candidates for a *node state
   transition*, and are registered with the Executive engine.  Root
   nodes of new plans are also registered as candidates.
   
   If there are no candidate nodes, go to step 8.
   
4. Start of the %PLEXIL Executive *quiescence loop*.

   The Executive engine evaluates the conditions on each of the
   candidate nodes, resolves any resource conflicts, and decides which
   nodes will transition in conformance with the node state transition
   rules.
   
5. The Executive engine performs all the selected node state
   transitions (pseudo-)simultaneously.  The effects of those
   transitions may trigger new change notifications, which in turn may
   make some nodes candidates for transition.  Nodes entering the
   `EXECUTING` state may trigger side effects, specifically variable
   assignments, commands, and updates.  Those side effects are placed
   on the output queue.

   This completes a *micro step* in the quiescence loop.

   If the output queue is non-empty at the completion of this step,
   proceed to step 6.

   If there are candidate nodes, go back to step 4.

   If there are no candidate nodes, go to step 7.
   
6. Any side effects in the output queue are processed.  Variable
   assignments are performed by the Executive engine itself; commands
   and updates are delegated to the appropriate user-defined
   interfaces.

   Variable assignments may trigger change notifications, which in
   turn may cause nodes to become candidates for transition.
   
   Feedback from commands and updates is sent to the input queue and
   processed in a subsequent cycle.

7. If a micro step completes and there are no further candidates for
   state transition (i.e. *quiescence* is achieved), or if side
   effects were performed in step 6, the Executive has completed a
   *macro step* and has reached the end of the *quiescence loop*.
   
   If there are candidate nodes, go back to step 4.
   
   If the input queue is not empty, go back to step 2 and start the
   cycle again.

8. When a macro step is complete, no nodes are candidates for
   transition, and the input queue is empty, the application sleeps
   until the next time it is awakened (e.g. by a timer or external
   event), at which time the process repeats from step 2.

   (End of the main loop)

[Previous](\ref interfacing-basics) | [Up](\ref interfacing-tutorial) | [Next](\ref interface-components-roles)

[Home](index.html)
