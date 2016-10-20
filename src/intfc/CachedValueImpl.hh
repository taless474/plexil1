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

#ifndef PLEXIL_CACHED_VALUE_IMPL_HH
#define PLEXIL_CACHED_VALUE_IMPL_HH

#include "CachedValue.hh"
#include "GetValueImpl.hh"

namespace PLEXIL
{

  // Placeholder object
  class VoidCachedValue final : public CachedValue
  {
  public:
    VoidCachedValue();
    VoidCachedValue(VoidCachedValue const &) = delete;
    VoidCachedValue(VoidCachedValue &&) = delete;

    ~VoidCachedValue();

    ValueType valueType() const;
    bool isKnown() const;
    CachedValue &operator=(CachedValue const &other);
    CachedValue &operator=(CachedValue &&other);
    bool operator==(CachedValue const &) const;
    CachedValue *clone() const;
    void printValue(std::ostream &) const;

    /**
     * @brief Retrieve the cached value.
     * @param The appropriately typed place to put the result.
     * @return True if known, false if unknown or invalid.
     * @note The expression value is not copied if the return value is false.
     * @note Derived classes should implement only the appropriate methods.
     */

    // Local macro
#define DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(_rtype_) \
    virtual bool getValue(_rtype_ &) const \
    { return false; }

    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Boolean)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Integer)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Real)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(NodeState)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(NodeOutcome)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(FailureType)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(CommandHandleValue)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(String)
#undef DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD

    /**
     * @brief Retrieve a pointer to the (const) cached value.
     * @param ptr Reference to the pointer variable to receive the result.
     * @return True if known, false if unknown or invalid.
     * @note The pointer is not copied if the return value is false.
     * @note Derived classes should implement only the appropriate method.
     * @note Default methods return an error in every case.
     */

    // Local macro
#define DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(_rtype_) \
    virtual bool getValuePointer(_rtype_ const *&) const \
    { return false; }

    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(String)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(Array)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(BooleanArray)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(IntegerArray)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(RealArray)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD(StringArray)

#undef DEFINE_VOID_CACHED_VALUE_GET_VALUE_POINTER_METHOD

    virtual Value toValue() const;

    /**
     * @brief Set the state to unknown.

     */
    virtual bool setUnknown(unsigned int timestamp)
    { return false; }

    /**
     * @brief Update the cache entry with the given new value.
     * @param val The new value.
     * @return True if correct type for lookup, false otherwise.
     * @note Notifies all lookups of the new value.
     * @note The caller is responsible for deleting the object pointed to upon return.
     */

    virtual bool update(unsigned int timestamp, Boolean const &val);
    virtual bool update(unsigned int timestamp, Integer const &val);
    virtual bool update(unsigned int timestamp, Real const &val);
    virtual bool update(unsigned int timestamp, NodeState const &val);
    virtual bool update(unsigned int timestamp, NodeOutcome const &val);
    virtual bool update(unsigned int timestamp, FailureType const &val);
    virtual bool update(unsigned int timestamp, CommandHandleValue const &val);
    virtual bool update(unsigned int timestamp, String const &val);
    virtual bool updatePtr(unsigned int timestamp, String const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, BooleanArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, IntegerArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, RealArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, StringArray const *valPtr);

    // For convenience of TestExternalInterface, others
    virtual bool update(unsigned int timestamp, Value const &val);
  };
  
  // Another invocation of CRTP.
  // This class should never be seen in public.
  template <class IMPL>
  class CachedValueShim : public CachedValue
  {
  public:
    CachedValueShim() : CachedValue() {}
    CachedValueShim(CachedValueShim<IMPL> const &orig) : CachedValue(orig) {}
    CachedValueShim(CachedValueShim<IMPL> &&orig) : CachedValue(orig) {}

    ~CachedValueShim() {}

    inline CachedValue *clone() const
    {
      return static_cast<IMPL const *>(this)->cloneImpl();
    }

    // Local macro
#define DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(_type_) \
    bool update(unsigned int timestamp, _type_ const &val) \
    {return static_cast<IMPL *>(this)->updateImpl(timestamp, val);}

    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(Boolean)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(Integer)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(Real)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(NodeState)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(NodeOutcome)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(FailureType)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(CommandHandleValue)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(String)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD(Value)

#undef DEFINE_CACHED_VALUE_SHIM_UPDATE_METHOD

    // Local macro
#define DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(_type_) \
    bool updatePtr(unsigned int timestamp, _type_ const *valPtr) \
    {return static_cast<IMPL *>(this)->updatePtrImpl(timestamp, valPtr);}

    DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(String)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(BooleanArray)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(IntegerArray)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(RealArray)
    DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD(StringArray)
#undef DEFINE_CACHED_VALUE_SHIM_UPDATE_PTR_METHOD

  };

  // Scalar types
  template <typename T>
  class CachedValueImpl final :
    public GetValueImpl<T>,
    public CachedValueShim<CachedValueImpl<T> >
  {
  public:
    CachedValueImpl();
    CachedValueImpl(CachedValueImpl<T> const &);
    CachedValueImpl(CachedValueImpl<T> &&);

    ~CachedValueImpl();

    CachedValue &operator=(CachedValue const &);
    CachedValue &operator=(CachedValue &&);
    CachedValueImpl<T> &operator=(CachedValueImpl<T> const &);
    CachedValueImpl<T> &operator=(CachedValueImpl<T> &&);

    CachedValue *cloneImpl() const;

    bool operator==(CachedValue const &) const;

    bool isKnown() const;

    bool getValueImpl(T &result) const;

    // API to external interface
    bool setUnknown(unsigned int timestamp);

    virtual bool updateImpl(unsigned int timestamp, T const &val);

    // Type conversion or invalid type
    template <typename U>
    bool updateImpl(unsigned int timestamp, U const &val);

    bool updateImpl(unsigned int timestamp, Value const &val);

    // Type error
    template <typename U>
    bool updatePtrImpl(unsigned int timestamp, U const *valPtr);

  private:
    T m_value;
    bool m_known;
  };

  // String is special
  template <>
  class CachedValueImpl<String> :
    public GetValueImpl<String>,
    public CachedValueShim<CachedValueImpl<String> >
  {
  public:
    CachedValueImpl();
    CachedValueImpl(CachedValueImpl<String> const &);
    CachedValueImpl(CachedValueImpl<String> &&);

    ~CachedValueImpl();

    CachedValue &operator=(CachedValue const &);
    CachedValue &operator=(CachedValue &&);
    CachedValueImpl<String> &operator=(CachedValueImpl<String> const &);
    CachedValueImpl<String> &operator=(CachedValueImpl<String> &&);

    bool isKnown() const;

    CachedValue *cloneImpl() const;

    bool operator==(CachedValue const &) const;

    bool getValueImpl(String &result) const;
    bool getValuePointerImpl(String const *&ptr) const;

    // API to external interface
    bool setUnknown(unsigned int timestamp);

    virtual bool updateImpl(unsigned int timestamp, String const &val);

    // Type error
    template <typename U>
    bool updateImpl(unsigned int timestamp, U const &val);

    bool updateImpl(unsigned int timestamp, Value const &val);

    bool updatePtrImpl(unsigned int timestamp, String const *valPtr);

    // Type error
    template <typename U>
    bool updatePtrImpl(unsigned int timestamp, U const *valPtr);

  private:
    String m_value;
    bool m_known;
  };

  // Specialized for arrays
  template <typename T>
  class CachedValueImpl<ArrayImpl<T> > :
    public GetValueImpl<ArrayImpl<T> >,
    public CachedValueShim<CachedValueImpl<ArrayImpl<T> > >
  {
  public:
    CachedValueImpl();
    CachedValueImpl(CachedValueImpl<ArrayImpl<T> > const &);
    CachedValueImpl(CachedValueImpl<ArrayImpl<T> > &&);

    ~CachedValueImpl();

    CachedValue &operator=(CachedValue const &);
    CachedValue &operator=(CachedValue &&);
    CachedValueImpl<ArrayImpl<T> > &operator=(CachedValueImpl<ArrayImpl<T> > const &);
    CachedValueImpl<ArrayImpl<T> > &operator=(CachedValueImpl<ArrayImpl<T> > &&);

    bool isKnown() const;

    CachedValue *cloneImpl() const;

    bool operator==(CachedValue const &) const;

    bool getValuePointerImpl(ArrayImpl<T> const *&ptr) const;

    // Conversion to array base
    bool getValuePointerImpl(Array const *&ptr) const;

    // API to external interface
    bool setUnknown(unsigned int timestamp);

    template <typename U>
    bool updateImpl(unsigned int timestamp, U const &val);

    bool updateImpl(unsigned int timestamp, Value const &val);

    bool updatePtrImpl(unsigned int timestamp, ArrayImpl<T> const *valPtr);

    // Type conversion or invalid type
    template <typename U>
    bool updatePtrImpl(unsigned int timestamp, U const *valPtr);

  private:
    ArrayImpl<T> m_value;
    bool m_known;
  };

} // namespace PLEXIL

#endif // PLEXIL_CACHED_VALUE_IMPL_HH
