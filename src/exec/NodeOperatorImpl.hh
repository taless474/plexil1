/* Copyright (c) 2006-2016, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_NODE_OPERATOR_IMPL_HH
#define PLEXIL_NODE_OPERATOR_IMPL_HH

#include "NodeOperator.hh"

#include "allocateCache.hh"
#include "PlanError.hh"
#include "PlexilTypeTraits.hh"
#include "Value.hh"

namespace PLEXIL
{
  class Node;

  template <class IMPL>
  class NodeOperatorShim : public NodeOperator
  {
  protected:

    // Only accessible to derived classes
    NodeOperatorShim(std::string const &name)
      : NodeOperator(name)
    {
    }

  private:

    // unimplemented
    NodeOperatorShim() = delete;
    NodeOperatorShim(NodeOperatorShim const &) = delete;
    NodeOperatorShim(NodeOperatorShim &&) = delete;
    NodeOperatorShim &operator=(NodeOperatorShim const &) = delete;
    NodeOperatorShim &operator=(NodeOperatorShim &&) = delete;

  public:

    virtual ~NodeOperatorShim()
    {
    }

#define DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(_rtype_) \
    virtual bool operator()(_rtype_ &result, Node const *node) const \
    {return static_cast<IMPL const *>(this)->calc(result, node);}

    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(Boolean)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(NodeState)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(NodeOutcome)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(FailureType)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(CommandHandleValue)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(Integer)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(Real)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(String)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(Array)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(BooleanArray)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(IntegerArray)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(RealArray)
    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(StringArray)

#undef DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD

  };

  template <typename R>
  class NodeOperatorImpl : public NodeOperatorShim<NodeOperatorImpl<R> >
  {
  public:
    virtual ~NodeOperatorImpl()
    {
    }

    // Default methods, based on R
    ValueType valueType() const;

    void *allocateCache() const
    {
      return static_cast<void *>(PLEXIL::allocateCache<R>());
    }

    void deleteCache(void *ptr) const
    {
      PLEXIL::deallocateCache<R>(static_cast<R *>(ptr));
    }

    bool calcNative(void *cache, Node const *node) const;
    void printValue(std::ostream &s, void *cache, Node const *node) const;
    Value toValue(void *cache, Node const *node) const;

    // Delegated to derived classes
    virtual bool calc(R &result, Node const *node) const;

    // Conversion or type error
    // *** OS X 10.9.x requires these to be here, instead of the .cc file ***
    template <typename U>
    bool calc(U & /* result */, Node const */* arg */) const
    {
      checkPlanError(ALWAYS_FAIL,
                     "Operator " << this->getName() << " not implemented for return type "
                     << valueTypeName(PlexilValueType<U>::value));
      return false;
    }

  protected:

    // Base class shouldn't be instantiated by itself
    NodeOperatorImpl(std::string const &name)
      : NodeOperatorShim<NodeOperatorImpl<R> >(name)
    {
    }

  private:

    // Unimplemented
    NodeOperatorImpl() = delete;
    NodeOperatorImpl(NodeOperatorImpl const &) = delete;
    NodeOperatorImpl(NodeOperatorImpl &&) = delete;
    NodeOperatorImpl &operator=(NodeOperatorImpl const &) = delete;
    NodeOperatorImpl &operator=(NodeOperatorImpl &&) = delete;

  };

  // Not currently used
  // Specialized conversions for Integer operator to Real
  // *** Must be declared here for OS X 10.9 ***
  // template <>
  // template <>
  // bool NodeOperatorImpl<Integer>::calc(Real &result, Node const *node) const;

  // Maybe later
  // template <typename R>
  // class NodeOperatorImpl<ArrayImpl<R> >
  //   : public NodeOperatorShim<NodeOperatorImpl<ArrayImpl<R> > >
  // {
  // public:
  //   virtual ~NodeOperatorImpl() {}

  //   // Default methods, based on R
  //   ValueType valueType() const;
  //   void *allocateCache() const { return static_cast<void *>(new ArrayImpl<R>); }
  //   void deleteCache(void *ptr) const { delete static_cast<ArrayImpl<R> *>(ptr); }

  //   bool calcNative(void *cache, Node const *node) const;
  //   void printValue(std::ostream &s, void *cache, Node const *node) const;
  //   Value toValue(void *cache, Node const *node) const;

  //   // Delegated to derived classes
  //   virtual bool calc(ArrayImpl<R> &result, Node const *node) const = 0;

  //   // Downcast to Array base
  //   virtual bool calc(Array &result, Node const *node) const = 0;

  //   // Conversion or type error
  //   // *** OS X 10.9.x requires these to be here, instead of the .cc file ***
  //   template <typename U>
  //   bool calc(U & /* result */, Node const */* arg */) const
  //   {
  //     checkPlanError(ALWAYS_FAIL,
  //                    "Operator " << this->getName() << " not implemented for return type "
  //                    << valueTypeName(PlexilValueType<U>::arrayValue));
  //     return false;
  //   }

  // protected:
  //   // Base class shouldn't be instantiated by itself
  //   NodeOperatorImpl(std::string const &name)
  //     : NodeOperatorShim<NodeOperatorImpl<R> >(name)
  //   {
  //   }

  // private:
  //   // Unimplemented
  //   NodeOperatorImpl() = delete;
  //   NodeOperatorImpl(NodeOperatorImpl const &) = delete;
  //   NodeOperatorImpl(NodeOperatorImpl &&) = delete;
  //   NodeOperatorImpl &operator=(NodeOperatorImpl const &) = delete;
  //   NodeOperatorImpl &operator=(NodeOperatorImpl &&) = delete;
  // };

} // namespace PLEXIL

/**
 * @brief Helper macro, intended to implement "boilerplate" singleton accessors
 *        for classes derived from NodeOperatorImpl<R>.
 */
#define DECLARE_NODE_OPERATOR_STATIC_INSTANCE(CLASS, RETURNS) \
  static PLEXIL::NodeOperator const *instance() \
  { \
    static CLASS const sl_instance; \
    return static_cast<PLEXIL::NodeOperator const *>(&sl_instance); \
  }

#endif // PLEXIL_NODE_OPERATOR_IMPL_HH
