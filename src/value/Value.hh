/* Copyright (c) 2006-2021, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_VALUE_HH
#define PLEXIL_VALUE_HH

#include "ValueType.hh"

#include <vector>

#if __cplusplus >= 201103L
#include "Array.hh"
#include <memory> // std::unique_ptr
// Explicit instantiation
template class std::unique_ptr<PLEXIL::Array>;
#endif

namespace PLEXIL
{

  //! \class Value
  //! \brief An encapsulation representing any possible value in the PLEXIL language.
  //! \note Implemented as a tagged (discriminated) union.
  //! \note Of use when there is no way of knowing the PLEXIL type of a value at C++ compile time.
  //! \ingroup Values
  class Value
  {
  public:

    //! \brief Default constructor.
    Value();

    //! \brief Copy constructor.
    Value(Value const &);

#if __cplusplus >= 201103L
    //! \brief Move constructor.
    //! \note Only available if the compiler supports C++11 or later.
    Value(Value &&);
#endif

    //! \brief Constructor from a Boolean.
    Value(Boolean val);

    //! \brief Constructor for internal values or typed UNKNOWN.
    //! \param enumVal Value from the internal type enumeration, or 0 for UNKNOWN.
    //! \param typ The desired ValueType of the result.
    Value(uint16_t enumVal, ValueType typ); //  internal values, typed UNKNOWN

    //! \brief Constructor from an Integer.
    Value(Integer val);

    //! \brief Constructor from a Real.
    Value(Real val);

    //! \brief Constructor from a reference to const std::string.
    Value(String const &val);

    //! \brief Constructor from a const pointer to char.
    Value(char const *val); // for convenience

    //! \brief Constructor from a const reference to a (generic) Array.
    Value(Array const &val);

    //! \brief Constructor from a const reference to a BooleanArray.
    Value(BooleanArray const &val);

    //! \brief Constructor from a const reference to an IntegerArray.
    Value(IntegerArray const &val);

    //! \brief Constructor from a const reference to a RealArray.
    Value(RealArray const &val);

    //! \brief Constructor from a const reference to a StringArray.
    Value(StringArray const &val);

    //! \brief Constructor from a const reference to a std::vector of Value.
    //! \note The Value instances must all be of the same ValueType.
    Value(std::vector<Value> const &vals);
    
    //! \brief Destructor.
    ~Value();
    
    //! \brief Copy assignment from const reference to another Value.
    Value &operator=(Value const &);

#if __cplusplus >= 201103L
    //! \brief Move assignment from lvalue reference to another Value.
    //! \note Only available if the compiler supports C++11 or later.
    Value &operator=(Value &&);
#endif

    Value &operator=(Boolean val);
    Value &operator=(uint16_t enumVal);
    Value &operator=(Integer val);
    Value &operator=(Real val);
    Value &operator=(String const &val);
    Value &operator=(char const *val);
    Value &operator=(BooleanArray const &val);
    Value &operator=(IntegerArray const &val);
    Value &operator=(RealArray const &val);
    Value &operator=(StringArray const &val);

    //! \brief Set the value to UNKNOWN.
    void setUnknown();

    //! \brief Return the ValueType of this Value.
    ValueType valueType() const;

    //! \brief Returns false if the value is unknown, true otherwise.
    bool isKnown() const;

    bool getValue(Boolean &result) const;
    bool getValue(uint16_t &result) const;
    bool getValue(Integer &result) const;
    bool getValue(Real &result) const;
    bool getValue(String &result) const;

    bool getValuePointer(String const *&ptr) const;
    bool getValuePointer(Array const *&ptr) const;
    bool getValuePointer(BooleanArray const *&ptr) const;
    bool getValuePointer(IntegerArray const *&ptr) const;
    bool getValuePointer(RealArray const *&ptr) const;
    bool getValuePointer(StringArray const *&ptr) const;

    bool equals(Value const &) const;
    bool lessThan(Value const &) const; // for (e.g.) std::map

    void print(std::ostream &s) const;
    std::string valueToString() const;

    char *serialize(char *b) const; 
    char const *deserialize(char const *b);
    size_t serialSize() const; 

  private:

    //! \brief Prepare to be assigned a new value.
    void cleanup();
    
    //! \brief Prepare to be assigned a new value if the previous value was a String.
    void cleanupForString();

    //! \brief Prepare to be assigned a new value if the previous value was an Array.
    void cleanupForArray();

    union {
      Boolean  booleanValue;
      uint16_t enumValue;
      Integer  integerValue;
      Real     realValue;
#if __cplusplus >= 201103L
      std::unique_ptr<String>  stringValue;
      std::unique_ptr<Array>   arrayValue;
#else
      String  *stringValue;
      Array   *arrayValue;
#endif
    };
    ValueType m_type;
    bool m_known;
  };

  //! \brief Overloaded formatted output operator for Value.
  //! \ingroup Values
  std::ostream &operator<<(std::ostream &, Value const &);

  //! \brief Overloaded equality operator for Value.
  //! \ingroup Values
  inline bool operator==(Value const &a, Value const &b)
  {
    return a.equals(b);
  }

  //! \brief Overloaded inequality operator for Value.
  //! \ingroup Values
  inline bool operator!=(Value const &a, Value const &b)
  {
    return !a.equals(b);
  }

  //! \brief Overloaded less-than operator for Value.
  //! \ingroup Values
  inline bool operator<(Value const &a, Value const &b)
  {
    return a.lessThan(b);
  }

  //! \brief Overloaded less-than-or-equal operator for Value.
  //! \ingroup Values
  inline bool operator<=(Value const &a, Value const &b)
  {
    return !b.lessThan(a);
  }

  //! \brief Overloaded greater-than operator for Value.
  //! \ingroup Values
  inline bool operator>(Value const &a, Value const &b)
  {
    return b.lessThan(a);
  }

  //! \brief Overloaded greater-than-or-equal operator for Value.
  //! \ingroup Values
  inline bool operator>=(Value const &a, Value const &b)
  {
    return !a.lessThan(b);
  }

} // namespace PLEXIL

#endif // PLEXIL_VALUE_HH
