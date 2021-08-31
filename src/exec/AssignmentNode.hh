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

#ifndef ASSIGNMENT_NODE_HH
#define ASSIGNMENT_NODE_HH

#include "NodeImpl.hh"

namespace PLEXIL
{
  // Forward references
  class Assignable;
  class Assignment;

  //! \class AssignmentNode
  //! \brief Implements the PLEXIL Assignment Node.
  //! \see Assignment
  //! \ingroup Exec-Core
  class AssignmentNode : public NodeImpl
  {
  public:

    //! \brief Primary constructor.
    //! \param nodeId The name of this node, as a pointer to const character string.
    //! \param parent (Optional) Pointer to the parent of this node; used for the ancestor conditions and variable lookup.
    AssignmentNode(char const *nodeId, NodeImpl *parent = NULL);

    //! \brief Alternate constructor.  Used only by Exec test module.
    //! \param type Const reference to a node type name.
    //! \param name Const reference to the name to give this instance.
    //! \param state The state to assign this node.
    //! \param parent (Optional) The parent of this node; may be null.
    AssignmentNode(const std::string &type,
                   const std::string &name,
                   NodeState state,
                   NodeImpl *parent = NULL);

    //! \brief Virtual destructor.
    virtual ~AssignmentNode();

    //! \brief Get the type of this node.
    //! \return The type of this node.
    virtual PlexilNodeType getType() const
    {
      return NodeType_Assignment;
    }

    //! \brief Get the assignment's variable.
    //! \return Pointer to the variable, as an Assignable.
    virtual Assignable *getAssignmentVariable() const;

    //! \brief Accessor for the priority of a node.  The priority is used to resolve resource conflicts.
    //! \return the priority of this node.
    int32_t getPriority() const {return m_priority;}

    //! \brief Get this node's priority.
    //! \return The priority of this node as an integer.
    //! \note Should only be used by plan parser and unit tests.
    void setPriority(int32_t prio)
    {
      m_priority = prio;
    }

    //! \brief Get the Assignment object.
    //! \return Pointer to the Assignment.
    //! \note Only used by the plan parser and its unit tests.
    Assignment *getAssignment() { return m_assignment; }

    //! \brief Set the assignment.
    //! \param assn The assignment object.
    //! \note Only used by the plan parser.
    void setAssignment(Assignment *assn);
    
  protected:

    // Specific behaviors for derived classes

    //! \brief Perform the execution operations appropriate to the node type.
    virtual void specializedHandleExecution();

    //! \brief Perform deactivations appropriate to the node type.
    virtual void specializedDeactivateExecutable();

    //! \brief Determine the destination state from EXECUTING.
    //! \return True if the new destination state differs from last check; false otherwise.
    virtual bool getDestStateFromExecuting();

    //! \brief Determine the destination state from FAILING.
    //! \return True if the new destination state differs from last check; false otherwise.
    virtual bool getDestStateFromFailing();

    //! \brief Transition out of EXECUTING state.
    virtual void transitionFromExecuting();

    //! \brief Transition out of FAILING state.
    virtual void transitionFromFailing();

    //! \brief Transition into EXECUTING state.
    virtual void transitionToExecuting();

    //! \brief Transition into FAILING state.
    virtual void transitionToFailing();

  private:

    //! \brief Abort the assignment.
    void abort();

    //! \brief Construct a dummy Assignment.
    //! \note Only used in the unit test variant constructor.
    void initDummyAssignment();

    Assignment *m_assignment;   //!< Pointer to the Assignment object.
    int32_t m_priority;         //!< The priority of this node.
  };

}

#endif // ASSIGNMENT_NODE_HH
