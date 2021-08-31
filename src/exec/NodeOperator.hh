// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     // Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     // Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     // Neither the name of the Universities Space Research Association nor the
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

#ifndef PLEXIL_NODE_OPERATOR_HH
#define PLEXIL_NODE_OPERATOR_HH

#include "ValueType.hh"

namespace PLEXIL
{
  // Forward references
  class ListenableUnaryOperator;
  class NodeImpl;
  class Value;

  // TODO:
  // - Support printing

  //! \class NodeOperator
  //! \brief Abstract base class representing a computation process to
  //!        be performed on a Node or its nearest neighbors,
  //!        returning a value.  Analogous to the Operator class.
  //! \see Operator
  //! \see NodeFunction
  //! \ingroup Exec-Core
  class NodeOperator
  {
  public:

    //! \brief Virtual destructor.
    virtual ~NodeOperator()
    {
    }

    //! \brief Get the name of this operator.
    //! \return Const reference to the name string.
    std::string const &getName() const
    {
      return m_name;
    }


    //! \brief Return the value type of this Operation's result.
    //! \return The ValueType.
    //! \note Delegated to NodeOperatorImpl by default.
    virtual ValueType valueType() const = 0;

    //! \brief Allocate a cache for the result of this Operation.
    //! \return Pointer to the cache.  May be NULL.
    //! \note Not needed for Boolean, Integer, Real, internal values
    //! \note Delegated to NodeOperatorImpl by default.
    virtual void *allocateCache() const = 0;

    //! \brief Delete a cache for the result of this Operation.
    //! \param ptr Pointer to the cache.
    //! \note Not needed for Boolean, Integer, Real, internal values
    //! \note Delegated to NodeOperatorImpl by default.
    virtual void deleteCache(void *Ptr) const = 0;

    //! \brief Execute the operation on the Node, and store the result.
    //! \param result Reference to the result variable.
    //! \param node Const pointer to the Node.
    //! \return true if the result is known, false if unknown.
    //! \note The base class method throws an exception.
    virtual bool operator()(Boolean &result, NodeImpl const *node) const;

    // Only Boolean operators implemented to date
    // virtual bool operator()(uint16_t &result, NodeImpl const *arg) const;
    // virtual bool operator()(Integer &result, NodeImpl const *node) const;
    // virtual bool operator()(Real &result, NodeImpl const *node) const;
    // virtual bool operator()(String &result, NodeImpl const *node) const;

    // Not needed yet
    // virtual bool operator()(Array &result, NodeImpl const *node) const;
    // virtual bool operator()(BooleanArray &result, NodeImpl const *node) const;
    // virtual bool operator()(IntegerArray &result, NodeImpl const *node) const;
    // virtual bool operator()(RealArray &result, NodeImpl const *node) const;
    // virtual bool operator()(StringArray &result, NodeImpl const *node) const;

    //! \brief Is the result of this operation on this Node known?
    //! \param node Const pointer to the node.
    //! \return true if known, false if unknown.
    virtual bool isKnown(NodeImpl const *node) const = 0;

    //! \brief Print the result of this Operation on this Node to an output stream.
    //! \param s The stream.
    //! \param node Const pointer to the node.
    virtual void printValue(std::ostream &s, NodeImpl const *node) const = 0;

    //! \brief Return the result of this operation on this Node as a Value instance.
    //! \param node Const pointer to the Node.
    //! \return The Value.
    virtual Value toValue(NodeImpl const *node) const = 0;

    //! \brief Map the given function over all propagation sources for this operator and Node.
    //! \param node Pointer to the Node.
    //! \param oper Const reference to the Operator.
    //! \note Helper for setup and teardown of the notification network.
    virtual void doPropagationSources(NodeImpl *node,
                                      ListenableUnaryOperator const &oper) const = 0;

  protected:

    //! \brief Protected constructor.  Only accessible to derived classes.
    //! \param name Const reference to the operator's name string.
    NodeOperator(std::string const &name)
      : m_name(name)
    {
    }

    std::string const m_name; //!< The name of this NodeOperator.

  private:

    // Explicitly unimplemented.
#if __cplusplus >= 201103L
    NodeOperator() = delete;
    NodeOperator(NodeOperator const &) = delete;
    NodeOperator(NodeOperator &&) = delete;
    NodeOperator &operator=(NodeOperator const &) = delete;
    NodeOperator &operator=(NodeOperator &&) = delete;
#else
    NodeOperator();
    NodeOperator(NodeOperator const &);
    NodeOperator &operator=(NodeOperator const &);
#endif
  };

} // namespace PLEXIL

#endif // PLEXIL_NODE_OPERATOR_HH
