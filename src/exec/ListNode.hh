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

#ifndef LIST_NODE_HH
#define LIST_NODE_HH

#include "NodeImpl.hh"

#include "NodeFunction.hh"

namespace PLEXIL
{

  //! \class ListNode
  //! \brief Specialization of NodeImpl for NodeList nodes.
  //! \addtogroup Exec-Core The %PLEXIL Executive core
  class ListNode : public NodeImpl
  {
  public:

    //! \brief Constructor.
    //! \param nodeId The name of this node, as a pointer to const character string.
    //! \param parent (Optional) Pointer to the parent of this node; used for the ancestor conditions and variable lookup.
    ListNode(char const *nodeId, NodeImpl *parent = NULL);

    //! \brief Alternate constructor.  Used only by Exec test module.
    //! \param type Const reference to a node type name.
    //! \param name Const reference to the name to give this instance.
    //! \param state The state to assign this node.
    //! \param parent (Optional) The parent of this node; may be null.
    ListNode(const std::string& type,
             const std::string& name,
             NodeState state,
             NodeImpl *parent = NULL);

    //! \brief Virtual destructor.
    virtual ~ListNode();

    //! \brief Get the type of this node.
    //! \return The type of this node.
    virtual PlexilNodeType getType() const
    {
      return NodeType_NodeList;
    }

    //! \brief Get the children of this node.
    //! \return Reference to a vector of pointers to the children.
    virtual std::vector<NodeImpl *>& getChildren()
    {
      return m_children;
    }

    //! \brief Get the children of this node.
    //! \return Const reference to a vector of pointers to the children.
    virtual const std::vector<NodeImpl *>& getChildren() const
    {
      return m_children; 
    }

    //! \brief Add a child to this node.
    //! \param node Pointer to the new child node.
    //! \note Intended for use by the plan parser. Not used by unit tests.
    void addChild(NodeImpl *node)
    {
      m_children.push_back(node);
    }

    //! \brief Find a child node of this node with the given name.
    //! \param childName The name, as a pointer to const character string.
    //! \return Pointer to the child node; null if no child with that name exists.
    virtual NodeImpl *findChild(char const *childName);

    //! \brief Find a child node of this node with the given name.
    //! \param childName The name, as a pointer to const character string.
    //! \return Const pointer to the child node; null if no child with that name exists.
    virtual NodeImpl const *findChild(char const *childName) const;

    //! \brief Reserve storage for children of this node.
    //! \param n The number of children
    //! \note For use by parsers. An optional optimization.
    void reserveChildren(size_t n);

    //! \brief Get the name -> variable mapping that children of this node should reference.
    //! \return Const pointer to a variable map; may be null.
    virtual NodeVariableMap const *getChildVariableMap() const;

    //! \brief Set the node's state.
    //! \param newValue The new node state.
    //! \param tym The timestamp to record for this state transition.
    //! \note This method wraps NodeImpl::setState, and notifies child nodes
    //!       of a change in the parent node's state.
    virtual void setState(NodeState newValue, double tym);

  protected:

    //! \brief Create any condition wrapper expressions appropriate to the node type.
    virtual void specializedCreateConditionWrappers();

    //! \brief Perform activations appropriate to the node type.
    virtual void specializedActivate();

    //! \brief Delete condition expressions as applicable for the node type.
    virtual void cleanUpConditions();

    //! \brief Delete conditions of the children of this node.
    virtual void cleanUpChildConditions();

    //! \brief Delete any additional objects as applicable for the node type.
    virtual void cleanUpNodeBody();

    //
    // getDestState helpers
    //

    //! \brief Determine the destination state from EXECUTING.
    //! \return True if the new destination state differs from last check; false otherwise.
    virtual bool getDestStateFromExecuting();

    //! \brief Determine the destination state from FAILING.
    //! \return True if the new destination state differs from last check; false otherwise.
    virtual bool getDestStateFromFailing();

    //! \brief Determine the destination state from FINISHING.
    //! \return True if the new destination state differs from last check; false otherwise.
    virtual bool getDestStateFromFinishing();

    //
    // transition helpers
    //

    //! \brief Transition out of EXECUTING state.
    virtual void transitionFromExecuting();

    //! \brief Transition out of FINISHING state.
    virtual void transitionFromFinishing();

    //! \brief Transition out of FAILING state.
    virtual void transitionFromFailing();

    //! \brief Transition into EXECUTING state.
    virtual void transitionToExecuting();

    //! \brief Transition into FINISHING state.
    virtual void transitionToFinishing();

    //! \brief Transition into FAILING state.
    virtual void transitionToFailing();

    //! \brief This node's action-complete condition expression.
    NodeFunction m_actionCompleteFn;

    //! \brief This node's default end condition expression.
    //! \see ListNode::specializedCreateConditionWrappers
    NodeFunction m_allFinishedFn;

    //! \brief The vector of child nodes.
    //! \note Shared with derived class LibraryCallNode
    std::vector<NodeImpl *> m_children;
  };

}

#endif // LIST_NODE_HH
