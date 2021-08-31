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

#ifndef PLEXIL_VARIABLE_CONFLICT_SET_HH
#define PLEXIL_VARIABLE_CONFLICT_SET_HH

#include <vector>

#include "plexil-stddef.h" // size_t

namespace PLEXIL
{
  // Forward references
  class Assignable;
  class Node;

  //! \class VariableConflictSet
  //! \brief A hybrid of std::multiset and std::priority_queue, used
  //!        to maintain a set of Assignment nodes assigning to the
  //!        same variable in priority order.
  //! \ingroup Exec-Core
  class VariableConflictSet
  {
  public:

    typedef std::vector<Node *>::const_iterator const_iterator;

    typedef std::vector<Node *>::iterator iterator;

    //! \brief Default constructor.
    VariableConflictSet();

    //! \brief Destructor.
    ~VariableConflictSet();

    //! \brief Get the variable
    //! \return Const pointer to the variable, as an Assignable.
    Assignable const *getVariable() const;

    //! \brief Set the variable for this conflict set.
    //! \param a Pointer to the variable, as an Assignable.
    void setVariable(Assignable *a);

    //
    //  API used by PlexilExec
    //

    //! \brief Get the pointer to the next conflict set in the list.
    //! \return The pointer.
    VariableConflictSet *next() const;

    //! \brief Set the pointer to the next conflict set in the list.
    //! \param nxt The pointer.
    void setNext(VariableConflictSet *nxt);

    //! \brief Return the number of Nodes in the set.
    //! \return The count.
    size_t size() const;

    //! \brief Is this conflict set empty?
    //! \return true if empty, false if not.
    bool empty() const;

    //! \brief Insert a unique Node in (weakly) sorted order, behind
    //!        any other Nodes at the same priority.
    //! \param node Pointer to the Node.
    void push(Node *node);

    //! \brief Get the node with lowest priority, in first-in-first-out order.
    Node *front();

    //! \brief Remove a Node from the conflict set.
    //! \param node Pointer to the Node.
    void remove(Node *node);

    //! \brief Get the count of Nodes with the same priority as the front Node.
    //! \return The count.
    size_t front_count() const;

    //! \brief Get an iterator to the first Node in the set.
    //! \return The iterator.
    iterator begin();

    //! \brief Get a const_iterator to the first Node in the set.
    //! \return The iterator.
    const_iterator begin() const;

    //! \brief Get an iterator past the last Node in the set.
    //! \return The iterator.
    iterator end();

    //! \brief Get a const_iterator past the last Node in the set.
    //! \return The iterator.
    const_iterator end() const;

    //
    // Managing pool of instances
    //

    //! \brief Allocate an instance.  Draw from the free list if
    //!        available; construct one if none are available.
    //! \return Pointer to a VariableConflictSet.
    static VariableConflictSet *allocate();

    //! \brief Return a VariableConflictSet to the free list.
    //! \param v Pointer to the VariableConflictSet.
    static void release(VariableConflictSet *v);

  private:

    // Not implemented
#if __cplusplus >= 201103L
    VariableConflictSet(VariableConflictSet const &) = delete;
    VariableConflictSet(VariableConflictSet &&) = delete;
    VariableConflictSet &operator=(VariableConflictSet const &) = delete;;
    VariableConflictSet &operator=(VariableConflictSet &&) = delete;;
#else
    VariableConflictSet(VariableConflictSet const &);
    VariableConflictSet &operator=(VariableConflictSet const &);
#endif

    VariableConflictSet *m_next; //!< Pointer to the next set in the list.
    Assignable *m_variable;      //!< Pointer to the conflicted variable.
    std::vector<Node *> m_nodes; //!< List of pointers to nodes assigning to the variable, sorted by priority, in order within each priority by time of insertion.
  };

} // namespace PLEXIL

#endif // PLEXIL_VARIABLE_CONFLICT_SET_HH
