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

#ifndef PLEXIL_CACHED_VALUE_HH
#define PLEXIL_CACHED_VALUE_HH

#include "ArrayFwd.hh"
#include "ValueType.hh"

namespace PLEXIL
{
  class Value;

  //! \class CachedValue
  //! \brief Abstract base class for storing state cache values
  //! \ingroup External-Interface
  class CachedValue
  {
  public:

    //! \brief Default constructor.
    CachedValue();

    //! \brief Copy constructor.
    //! \param orig Const reference to a CachedValue instance.
    CachedValue(CachedValue const &orig);

    //! \brief Virtual destructor.
    virtual ~CachedValue();

    //! \brief Assignment operator.
    //! \param other Const reference to another CachedValue instance.
    //! \return Reference to *this.
    //! \note Derived classes should wrap this method.
    virtual CachedValue &operator=(CachedValue const &other);

    //! \brief Get the timestamp of this cache entry.
    //! \return The timestamp.
    unsigned int getTimestamp() const;

    //
    // Delegated to derived classes
    //

    //! \brief Create an identical copy of this object.
    //! \return Pointer to the copy.
    virtual CachedValue *clone() const = 0;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    virtual bool isKnown() const = 0;
    
    //! \brief Return the value type.
    //! \return The value type.
    virtual ValueType valueType() const = 0;

    //! \brief Test whether two CachedValue instances are equal.
    //! \return True if equal, false if not.
    virtual bool operator==(CachedValue const &) const = 0;

    //! \brief Retrieve the value of this object.
    //! \param result Reference to an appropriately typed variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The value is not copied if the return value is false.
    //! \note Derived classes should implement only the appropriate methods.
    //! \note Default methods report a type error.
    virtual bool getValue(Boolean &result) const;
    virtual bool getValue(Integer &result) const;
    virtual bool getValue(Real &result) const;
    virtual bool getValue(String &result) const;

    //! \brief Retrieve a pointer to the (const) value of this object.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if the value is known, false if unknown or invalid for the result type.
    //! \note The pointer is not copied if the return value is false.
    //! \note Derived classes should implement only the appropriate methods.
    //! \note Default methods report a type error.
    virtual bool getValuePointer(String const *&ptr) const;
    virtual bool getValuePointer(Array const *&ptr) const;
    virtual bool getValuePointer(BooleanArray const *&ptr) const;
    virtual bool getValuePointer(IntegerArray const *&ptr) const;
    virtual bool getValuePointer(RealArray const *&ptr) const;
    virtual bool getValuePointer(StringArray const *&ptr) const;

    //! \brief Get the value of this object as a Value instance.
    //! \return The Value instance.
    virtual Value toValue() const = 0;

    //! \brief Print the object's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream& s) const = 0;

    //! \brief Set the value to unknown.
    //! \param timestamp Sequence number, e.g. the Exec macro step count.
    //! \return True if the value was previously known, false otherwise.
    virtual bool setUnknown(unsigned int timestamp) = 0;

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note If the type of the new value is inconsistent with the object's type,
    //!       the object's value is set to unknown.
    //! \note Default methods report a type error.
    virtual bool update(unsigned int timestamp, Boolean const &val);
    virtual bool update(unsigned int timestamp, Integer const &val);
    virtual bool update(unsigned int timestamp, Real const &val);
    virtual bool update(unsigned int timestamp, String const &val);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param valPtr Const pointer to the new value.
    //! \return True if the new value differs from the old, false otherwise.
    //! \note The caller is responsible for deleting the object pointed to upon return.
    //! \note Default methods report a type error.
    virtual bool updatePtr(unsigned int timestamp, String const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, BooleanArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, IntegerArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, RealArray const *valPtr);
    virtual bool updatePtr(unsigned int timestamp, StringArray const *valPtr);

    //! \brief Update the cache entry with the given new value.
    //! \param timestamp Sequence number.
    //! \param val Const reference to the new value.
    //! \return True if changed, false otherwise.
    //! \note Convenience member function for (e.g) TestExternalInterface, others
    virtual bool update(unsigned int timestamp, Value const &val) = 0;

  protected:

    //! \brief The sequence number at last modification. Initialized to 0.
    unsigned int m_timestamp;
  };

  // Factory function
  // See CachedValueImpl.cc

  //! \brief Construct an instance of a CachedValue for the value type.
  //! \param vtype The ValueType.
  //! \return Pointer to the new CachedValue instance.
  //! \ingroup External-Interface
  extern CachedValue *CachedValueFactory(ValueType vtype);

}

#endif // PLEXIL_CACHED_VALUE_HH
