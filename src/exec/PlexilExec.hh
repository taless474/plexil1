/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
*  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Universities Space Research Association nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _H_PlexilExec
#define _H_PlexilExec

#include "ExecConnector.hh"
#include "generic_hash_map.hh"
#include "PlexilPlan.hh"

#include <list>
#include <queue>
#include <set>

namespace PLEXIL 
{
  // Forward references
  class Assignable;
  class ExecListenerBase;
  class ExecListenerHub;

  struct NodeConflictComparator;

  /**
   * @brief The core PLEXIL executive.
   */
  class PlexilExec : public ExecConnector
  {
  public:
    /**
     * @brief Default constructor.
     */
    PlexilExec();

    /**
     * @brief Destructor.
     */
    ~PlexilExec();

    /**
     * @brief Queries whether the named library node is loaded.
     * @param nodeName The name of the library node.
     * @return True if the node is already defined, false otherwise.
     */
    bool hasLibrary(const std::string& nodeName) const;

    /**
     * @brief Retrieves the named library node if it is present.
     * @param nodeName The name of the library node.
     * @return The library node, or NULL if not found.
     */
    PlexilNode const *getLibrary(const std::string& nodeName) const;

    //
    // API to ExternalInterface
    //

    /**
     * @brief Prepare the given plan for execution.
     * @param Intermediate representation of the plan's root node.
     * @return True if succesful, false otherwise.
     */
    // *** TO BE DELETED ***
    bool addPlan(PlexilNode *plan);

    /**
     * @brief Prepare the given plan for execution.
     * @param The plan's root node.
     * @return True if succesful, false otherwise.
     */
    bool addPlan(Node *root);

    /**
     * @brief Add the given plan as a library node.
     * @param Intermediate representation of the plan's root node.
     */
    // *** TO BE DELETED ***
    void addLibraryNode(PlexilNode *plan);

    /**
     * @brief Begins a single "macro step" i.e. the entire quiescence cycle.
     */
    void step(double startTime); // *** FIXME ***

    /**
     * @brief Returns true if the Exec needs to be stepped.
     */
    bool needsStep() const;

    /**
     * @brief Set the ExecListenerHub instance.
     */
    void setExecListenerHub(ExecListenerHub *hub)
    {
      m_listener = hub;
    }

    /**
     * @brief Adds an ExecListener for publication of node transition events.
     * @note Convenience method for backward compatibility.
     */
    void addListener(ExecListenerBase *listener);

    /**
     * @brief Removes an ExecListener.
     * @note Convenience method for backward compatibility.
     */
    void removeListener(ExecListenerBase *listener);

    /**
     * @brief Queries whether all plans are finished.
     * @return true if all finished, false otherwise.
     */
    bool allPlansFinished() const;

    /**
     * @brief Deletes any finished root nodes.
     */
    void deleteFinishedPlans();

    //
    // API to Node classes
    //

    /**
     * @brief Schedule this assignment for execution.
     */
    void enqueueAssignment(Assignment *assign);

    /**
     * @brief Schedule this assignment for retraction.
     */
    void enqueueAssignmentForRetraction(Assignment *assign);

    /**
     * @brief Mark node as finished and no longer eligible for execution.
     */
    void markRootNodeFinished(Node *node);

    /**
     * @brief Handle the fact that a node's conditions may have changed (it is eligible for state change).
     * @param node The node which is eligible for state change.
     */
    void notifyNodeConditionChanged(Node *node);

    /**
     * @brief Handle the fact that a node's relevant conditions have changed (it is eligible for state change).
     * @param node The node which is eligible for state change.
     * @param newState The state the node will transition to.
     */
    void handleConditionsChanged(Node *node, NodeState newState);

  private:
    // Not implemented
    PlexilExec(PlexilExec const &);
    PlexilExec &operator=(PlexilExec const &);

    // Private types
    typedef std::multiset<Node *, NodeConflictComparator> VariableConflictSet;
    typedef std::map<Assignable const *, VariableConflictSet> VariableConflictMap;

    /**
     * @brief Resolve conflicts among potentially executing assignment variables.
     */
    void resolveResourceConflicts();

    /**
     * @brief Resolve conflicts for this variable.
     */
    void resolveVariableConflicts(Assignable const *var,
                                  const VariableConflictSet& conflictSet);

    /**
     * @brief Adds a node to consideration for resource contention.  The node must be an assignment node and it must be eligible to transition to EXECUTING.
     * @param node The assignment node.
     */
    void addToResourceContention(Node *node);

    /**
     * @brief Removes a node from consideration for resource contention.  This is usually because some condition has changed that makes the node no longer
     * eligible for execution.
     * @param node The assignment node.
     */
    void removeFromResourceContention(Node *node);

    /**
     * @brief Gets a stringified version of the current state change queue.
     */
    std::string stateChangeQueueStr();

    /**
     * @brief Batch-perform internal assignments queued up from a quiescence step.
     */
    void performAssignments();

    ExecListenerHub *m_listener;
    std::list<Node *> m_plan; /*<! The root of the plan.*/
    std::vector<Node *> m_finishedRootNodes; /*<! Root nodes which are no longer eligible to execute. */
    std::queue<Node *> m_nodesToConsider; /*<! Nodes whose conditions have changed and may be eligible to transition. */
    std::vector<Node *> m_stateChangeQueue; /*<! Nodes that are eligible for state transition.*/
    std::vector<Assignment *> m_assignmentsToExecute;
    std::vector<Assignment *> m_assignmentsToRetract;
    std::vector<Assignable *> m_variablesToRetract; /*<! Set of variables with assignments to be retracted due to node failures */
    VariableConflictMap m_resourceConflicts; /*<! A map from variables to sets of nodes which is used to resolve resource contention.
                                               The nodes in the sets are assignment nodes which can assign values to the variable.
                                               The sets are ordered by priority, but the order is dominated by FAILING nodes.
                                               Essentially, at each quiescence cycle, the first node in each set that isn't already
                                               in state FAILING gets added to the end of the queue. */
    std::map<std::string, PlexilNode *> m_libraries;
    unsigned int m_queuePos;
    bool m_finishedRootNodesDeleted; /*<! True if at least one finished plan has been deleted */
  };

}

#endif
