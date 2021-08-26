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

#ifndef PLEXIL_CACHED_VALUE_IMPL_HH
#define PLEXIL_CACHED_VALUE_IMPL_HH

#include "CachedValue.hh"

namespace PLEXIL
{

  //! \class VoidCachedValue
  //! \brief Placeholder object used by the state cache to represent
  //!        a Lookup value whose type is not yet known.
  //! \ingroup External-Interface
  class VoidCachedValue : public CachedValue
  {
  public:

    //! \brief Default constructor,
    VoidCachedValue();

    //! \brief Virtual destructor.
    virtual ~VoidCachedValue();

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    //! \note The VoidCachedValue method always returns false.
    virtual bool isKnown() const;

    //! \brief Return the value type.
    //! \return The value type.
    //! \note The VoidCachedValue method always returns UNKNOWN_TYPE.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \param other Const reference to another CachedValue instance.
    //! \return True if equal, false if not.
    //! \note The VoidCachedValue method returns true if other.isKnown() returns false.
    virtual bool operator==(CachedValue const &other) const;

    //! \brief Local macro to generate getValue() method definitions.
    //! \note The VoidCachedValue methods always return false.
#define DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(_rtype_) \
    virtual bool getValue(_rtype_ &) const \
    { return false; }

    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Boolean)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Integer)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(Real)
    DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD(String)
#undef DEFINE_VOID_CACHED_VALUE_GET_VALUE_METHOD

    //! \brief Local macro to generate getValuePointer() method definitions.
    //! \note The VoidCachedValue methods always return false.
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

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    //! \note The VoidCachedValue method always returns a default-constructed Value.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    //! \note The VoidCachedValue method always returns false.
    virtual bool setUnknown(unsigned int timestamp)
    { return false; }
      
    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note The VoidCachedValue methods always throw an exception.
    virtual bool update(unsigned int timestamp, Boolean const &val);
    virtual bool update(unsigned int timestamp, Integer const &val);
    virtual bool update(unsigned int timestamp, Real const &val);
    virtual bool update(unsigned int timestamp, String const &val);
      
    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param valPtr Const pointer to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note The VoidCachedValue methods always throw an exception.
    virtual bool updatePtr(unsigned int timestamp, String const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, BooleanArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, IntegerArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, RealArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, StringArray const *valPtr);

    //! \brief Update the entry to the given value with the given timestamp.
    //! \param timestamp The timestamp.
    //! \param val The new value
    //! \note The VoidCachedValue method throws an exception if the new value is known,
    //!       otherwise returns true.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented
    VoidCachedValue(VoidCachedValue const &);
  };

  //! \brief Template to implement CachedValue API for scalar-valued lookups.
  //! \ingroup External-Interface
  template <typename T>
  class CachedValueImpl : public CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValueImpl();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValueImpl(CachedValueImpl<T> const &);

    //! \brief Virtual destructor.
    ~CachedValueImpl() {}

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const;

    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &) const;

    //! \brief Retrieve the value of this object in its native type.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValue(T &result) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;

    //
    // API to external interface
    //
    
    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note If the type of the new value is inconsistent with the object's type,
    //!       the object's value is set to unknown.
    virtual bool update(unsigned int timestamp, T const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if changed, false otherwise.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented.
    CachedValueImpl<T> &operator=(CachedValueImpl<T> const &);

    //! \brief The current value.
    T m_value;

    //! \brief True if the value is known, false if unknown.
    bool m_known;
  };

  //! \brief Specialization of CachedValueImpl class template for Integer valued lookups.
  //! \note Specialized to support getValue(Real &) conversion method
  //! \ingroup External-Interface
  template <>
  class CachedValueImpl<Integer> : public CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValueImpl();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValueImpl(CachedValueImpl<Integer> const &);

    //! \brief Virtual destructor.
    ~CachedValueImpl() {}

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const;
    
    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &) const;

    //! \brief Retrieve the value of this object in its native type.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValue(Integer &result) const;

    //! \brief Retrieve the value of this object as a Real.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    //! \note Conversion method.
    virtual bool getValue(Real &result) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note If the type of the new value is inconsistent with the object's type,
    //!       the object's value is set to unknown.
    virtual bool update(unsigned int timestamp, Integer const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if changed, false otherwise.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented.
    CachedValueImpl<Integer> &operator=(CachedValueImpl<Integer> const &);

    //! \brief The current value.
    Integer m_value;

    //! \brief True if the value is known, false if unknown.
    bool m_known;
  };

  //! \brief Specialization of CachedValueImpl class template for Real valued lookups.
  //! \note Specialized to support update(unsigned int, Integer const &) conversion method
  //! \ingroup External-Interface
  template <>
  class CachedValueImpl<Real> : public CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValueImpl();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValueImpl(CachedValueImpl<Real> const &);

    //! \brief Virtual destructor.
    ~CachedValueImpl() {}

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const;
    
    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &) const;

    //! \brief Retrieve the value of this object in its native type.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValue(Real &result) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    virtual bool update(unsigned int timestamp, Real const &val); 

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note Conversion method.
    virtual bool update(unsigned int timestamp, Integer const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const pointer to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented.
    CachedValueImpl<Real> &operator=(CachedValueImpl<Real> const &);

    //! \brief The current value.
    Real m_value;

    //! \brief True if the value is known, false if unknown.
    bool m_known;
  };

  //! \brief Specialization of CachedValueImpl class template for String valued lookups.
  //! \ingroup External-Interface
  template <>
  class CachedValueImpl<String> : public CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValueImpl();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValueImpl(CachedValueImpl<String> const &);

    //! \brief Virtual destructor.
    ~CachedValueImpl() {}

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const;
    
    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &) const;

    //! \brief Retrieve the value of this object in its native type.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValue(String &result) const;

    //! \brief Retrieve a pointer to the (const) value of this object.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The pointer is not copied if the return value is false.
    virtual bool getValuePointer(String const *&) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    virtual bool update(unsigned int timestamp, String const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param valPtr Const pointer to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    virtual bool updatePtr(unsigned int timestamp, String const *valPtr);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if changed, false otherwise.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented.
    CachedValueImpl<String> &operator=(CachedValueImpl<String> const &);

    //! \brief The current value.
    String m_value;

    //! \brief True if the value is known, false if unknown.
    bool m_known;
  };

  //! \brief Specialization of CachedValueImpl class template for Array valued lookups.
  //! \ingroup External-Interface
  template <typename T>
  class CachedValueImpl<ArrayImpl<T> > : public CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValueImpl();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValueImpl(CachedValueImpl<ArrayImpl<T> > const &);

    //! \brief Virtual destructor.
    ~CachedValueImpl() {}

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const;
    
    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Test whether two CachedValue instances are equal.
    //! \param other Const reference to a CachedValue instance.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &other) const;

    //! \brief Retrieve the value of this object as a generic Array.
    //! \param ptr Reference to an appropriately typed pointer to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValuePointer(Array const *&ptr) const;

    //! \brief Retrieve the value of this object in its native type.
    //! \param ptr Reference to an appropriately typed pointer to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    virtual bool getValuePointer(ArrayImpl<T> const *&ptr) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream &s) const;
    

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param valPtr Const pointer to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    virtual bool updatePtr(unsigned int timestamp, ArrayImpl<T> const *valPtr);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if changed, false otherwise.
    virtual bool update(unsigned int timestamp, Value const &val);

  private:

    // Copy constructor not implemented.
    CachedValueImpl<ArrayImpl<T> > &operator=(CachedValueImpl<ArrayImpl<T> > const &);

    //! \brief The current value.
    ArrayImpl<T> m_value;

    //! \brief True if the value is known, false if unknown.
    bool m_known;
  };

} // namespace PLEXIL

#endif // PLEXIL_CACHED_VALUE_IMPL_HH
