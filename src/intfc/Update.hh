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

#ifndef PLEXIL_UPDATE_HH
#define PLEXIL_UPDATE_HH

#include "SimpleBooleanVariable.hh"
#include "Value.hh"

#include "SimpleMap.hh"

namespace PLEXIL
{
  // Forward declarations in PLEXIL namespace
  class NodeConnector;
  struct Pair;

  //! \class Update
  //! \brief Represents the information to be transmitted by a PlannerUpdate node.
  //! \ingroup External-Interface
  class Update 
  {
  public:

    //! \brief Shorthand for the name-value data structure type
    typedef SimpleMap<std::string, Value> PairValueMap;

    //! \brief Constructor.
    //! \param node Pointer to the owning UpdateNode.
    Update(NodeConnector *node);

    //! \brief Destructor.
    ~Update();

    //
    // Parser API
    //

    //! \brief Reserve space in the map for the given number of name-expression pairs.
    //! \param n The requested size.
    void reservePairs(size_t n);

    //! \brief Add a name-expression pair to be reported.
    //! \param name Const reference to the name.
    //! \param exp Pointer to the expression.
    //! \param expIsGarbage true if the expression should be deleted with the Update, false if not.
    void addPair(std::string const &name, Expression *exp, bool expIsGarbage);

    //! \brief Get the acknowledgement variable for this Update.
    //! \return Pointer to the variable, as an Expression.
    Expression *getAck() {return &m_ack;}

    //! \brief Get the map of name-value pairs.
    //! \return Const reference to the map.
    const PairValueMap& getPairs() const {return m_valuePairs;}

    //! \brief Get the owning node.
    //! \return Pointer to the node, as a NodeConnector.
    NodeConnector *getSource() {return m_source;}

    //! \brief Get the owning node.
    //! \return Const pointer to the node, as a NodeConnector.
    NodeConnector const *getSource() const {return m_source;}

    //! \brief Make the Update active.
    void activate();

    //! \brief Make the Update inactive.
    void deactivate();

    //! \brief Perform the planner update.
    void execute();

    //! \brief Return an acknowledgement value for the update.
    //! \param The value.
    void acknowledge(bool ack);

    //! \brief Delete all owned expressions, erase the pointers to the ones not owned.
    //! \note Public because all expressions owned by a Node must be unlinked
    //!       prior to deleting the Node.
    void cleanUp();

    //! \brief Evaluate all the expressions and fix their result values.
    //! \note Public for use by parser unit test.
    void fixValues();

    //
    // LinkedQueue item API
    //

    //! \brief Get the pointer to the next Update in the queue.
    //! \return Pointer to the Update.
    Update *next() const
    {
      return m_next;
    }

    //! \brief Get the pointer to the pointer to the next Update in the queue.
    //! \return Pointer to the next poitner.
    Update **nextPtr()
    {
      return &m_next;
    }

  private:

    // Deliberately unimplemented
    Update();
    Update(Update const &);
    Update& operator=(Update const &);

    Update *m_next;                //!< Pointer for LinkedQueue.
    NodeConnector *m_source;       //!< The owning Node.
    SimpleBooleanVariable m_ack;   //!< The acknowledgement variable.
    Pair *m_pairs;                 //!< Pointer to linked list of name-expression pairs.
    PairValueMap m_valuePairs;     //!< Map of name-value pairs.
  };

}

#endif // PLEXIL_UPDATE_HH
