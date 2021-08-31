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

#ifndef UPDATE_NODE_HH
#define UPDATE_NODE_HH

#include "NodeImpl.hh"

namespace PLEXIL
{
  class Update;

  //! \class UpdateNode
  //! \brief Implementation class for the PlannerUpdate node.
  //! \ingroup Exec-Core
  class UpdateNode : public NodeImpl
  {
  public:

    //! \brief Primary constructor.
    //! \param nodeId The name of this node, as a pointer to const character string.
    //! \param parent (Optional) Pointer to the parent of this node; used for the ancestor conditions and variable lookup.
    UpdateNode(char const *nodeId, NodeImpl *parent = NULL);

    //! \brief Alternate constructor.  Used only by Exec test module.
    //! \param type Const reference to a node type name.
    //! \param name Const reference to the name to give this instance.
    //! \param state The state to assign this node.
    //! \param parent (Optional) The parent of this node; may be null.
    UpdateNode(const std::string& type,
               const std::string& name,
               NodeState state,
               NodeImpl *parent = NULL);

    //! \brief Virtual destructor.
    virtual ~UpdateNode();

    //! \brief Get the type of this node.
    //! \return The type of this node.
    virtual PlexilNodeType getType() const
    {
      return NodeType_Update;
    }

    //! \brief Get the node's update structure.
    //! \return Pointer to the Update.
    //! \see Update
    Update *getUpdate()
    {
      return m_update; 
    }

    //! \brief Set the Node's update.
    //! \param upd Pointer to the update.
    //! \note Should only be used by plan parser and unit tests.
    void setUpdate(Update *upd);

  protected:

    //
    // Specific behaviors for UpdateNode
    //

    //! \brief Create any condition wrapper expressions appropriate to the node type.
    virtual void specializedCreateConditionWrappers();

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

    //! \brief Transition into FAILING state.
    virtual void transitionToFailing();

    //! \brief Delete any additional objects as applicable for the node type.
    virtual void cleanUpNodeBody();

  private:

    //! \brief Abort the Update.
    void abort();

    //! \brief Construct a dummy update for unit testing.
    void createDummyUpdate();

    Update *m_update; //!< Pointer to the Update object.
  };

}

#endif // UPDATE_NODE_HH
