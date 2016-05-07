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

#include "Error.hh"
#include "Value.hh"

namespace PLEXIL
{
  class Node;

  template <class IMPL>
  class NodeOperatorShim : public NodeOperator
  {
  protected:
    NodeOperatorShim(std::string const &name) : NodeOperator(name) {}

  public:
    virtual ~NodeOperatorShim() {}

#define DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(_rtype_) \
    virtual bool operator()(_rtype_ &result, Node const *node) const \
    {return static_cast<IMPL const *>(this)->calc(result, node);}

    DEFINE_NODE_OPERATOR_SHIM_OPERATOR_METHOD(Boolean)
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
    virtual ~NodeOperatorImpl() {}

    // Default methods, based on R
    ValueType valueType() const;
    void *allocateCache() const { return static_cast<void *>(new R); }
    void deleteCache(void *ptr) const { delete static_cast<R *>(ptr); }

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
      assertTrueMsg(ALWAYS_FAIL, "Type error for " << this->getName());
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
    NodeOperatorImpl();
    NodeOperatorImpl(NodeOperatorImpl const &);
    NodeOperatorImpl &operator=(NodeOperatorImpl const &);
  };

  // Specialized conversions for Integer operator to Real
  // *** Must be declared here for OS X 10.9 ***
  template <>
  template <>
  bool NodeOperatorImpl<int32_t>::calc(double &result, Node const *node) const;

  template <typename R>
  class NodeOperatorImpl<ArrayImpl<R> >
    : public NodeOperatorShim<NodeOperatorImpl<ArrayImpl<R> > >
  {
  public:
    virtual ~NodeOperatorImpl() {}

    // Default methods, based on R
    ValueType valueType() const;
    void *allocateCache() const { return static_cast<void *>(new ArrayImpl<R>); }
    void deleteCache(void *ptr) const { delete static_cast<ArrayImpl<R> *>(ptr); }

    bool calcNative(void *cache, Node const *node) const;
    void printValue(std::ostream &s, void *cache, Node const *node) const;
    Value toValue(void *cache, Node const *node) const;

    // Delegated to derived classes
    virtual bool calc(ArrayImpl<R> &result, Node const *node) const = 0;

    // Downcast to Array base
    virtual bool calc(Array &result, Node const *node) const = 0;

    // Conversion or type error
    // *** OS X 10.9.x requires these to be here, instead of the .cc file ***
    template <typename U>
    bool calc(U & /* result */, Node const */* arg */) const
    {
      assertTrueMsg(ALWAYS_FAIL, "Type error for " << this->getName());
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
    NodeOperatorImpl();
    NodeOperatorImpl(NodeOperatorImpl const &);
    NodeOperatorImpl &operator=(NodeOperatorImpl const &);
  };

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
