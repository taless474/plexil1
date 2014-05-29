/* Copyright (c) 2006-2014, Universities Space Research Association (USRA).
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

#include "Value.hh"

#include "ArrayImpl.hh"
#include "Error.hh"

namespace PLEXIL
{

  Value::Value()
    : m_type(UNKNOWN_TYPE),
      m_known(false)
  {}

  Value::Value(Value const &other)
    : m_type(other.m_type),
      m_known(other.m_known)
  {
    switch (m_type) {
      // Unknown - do nothing
    case UNKNOWN_TYPE:
      break;

      // Immediate data - copy the union
    case BOOLEAN_TYPE:
    case INTEGER_TYPE:
    case REAL_TYPE:
    case NODE_STATE_TYPE:
    case OUTCOME_TYPE:
    case FAILURE_TYPE:
    case COMMAND_HANDLE_TYPE:
      m_value = other.m_value;
      break;

      // Copy the actual value
    case STRING_TYPE:
      m_value.stringValue = new std::string(*other.m_value.stringValue);
      break;

      // Copy the entire array
    case BOOLEAN_ARRAY_TYPE:
      m_value.booleanArrayValue = new BooleanArray(*other.m_value.booleanArrayValue);
      break;

    case INTEGER_ARRAY_TYPE:
      m_value.integerArrayValue = new IntegerArray(*other.m_value.integerArrayValue);
      break;

    case REAL_ARRAY_TYPE:
      m_value.realArrayValue = new RealArray(*other.m_value.realArrayValue);
      break;

    case STRING_ARRAY_TYPE:
      m_value.stringArrayValue = new StringArray(*other.m_value.stringArrayValue);
      break;

    default:
      assertTrue_2(ALWAYS_FAIL, "Value copy constructor: unknown type");
      break;
    }
  }

  Value::Value(bool val)
    : m_type(BOOLEAN_TYPE),
      m_known(true)
  {
    m_value.booleanValue = val;
  }

  Value::Value(uint16_t enumVal, ValueType typ)
    : m_type(typ)
  {
    switch (m_type) {
      // Internal enumerations
    case NODE_STATE_TYPE:
    case OUTCOME_TYPE:
    case FAILURE_TYPE:
    case COMMAND_HANDLE_TYPE:
      m_value.enumValue = enumVal;
      m_known = true;
      break;

    default:
      m_known = false;
    }
  }
      
  Value::Value(int32_t val)
    : m_type(INTEGER_TYPE),
      m_known(true)
  {
    m_value.integerValue = val;
  }

  Value::Value(double val)
    : m_type(REAL_TYPE),
      m_known(true)
  {
    m_value.realValue = val;
  }

  Value::Value(std::string const &val)
    : m_type(STRING_TYPE),
      m_known(true)
  {
    m_value.stringValue = new std::string(val);
  }

  Value::Value(char const *val)
    : m_type(STRING_TYPE),
      m_known(true)
  {
    m_value.stringValue = new std::string(val);
  }

  Value::Value(BooleanArray const &val)
    : m_type(BOOLEAN_ARRAY_TYPE),
      m_known(true)
  {
    m_value.booleanArrayValue = new BooleanArray(val);
  }

  Value::Value(IntegerArray const &val)
    : m_type(INTEGER_ARRAY_TYPE),
      m_known(true)
  {
    m_value.integerArrayValue = new IntegerArray(val);
  }

  Value::Value(RealArray const &val)
    : m_type(REAL_ARRAY_TYPE),
      m_known(true)
  {
    m_value.realArrayValue = new RealArray(val);
  }

  Value::Value(StringArray const &val)
    : m_type(STRING_ARRAY_TYPE),
      m_known(true)
  {
    m_value.stringArrayValue = new StringArray(val);
  }
    
  Value::~Value()
  {
    cleanup();
  }
    
  Value &Value::operator=(Value const &other)
  {
    cleanup();
    m_type = other.m_type;
    m_known = other.m_known;
    if (m_known) {
      switch (m_type) {
        // Unknown - do nothing
      case UNKNOWN_TYPE:
        break;

        // Immediate data - just copy the union
      case BOOLEAN_TYPE:
      case INTEGER_TYPE:
      case REAL_TYPE:
      case NODE_STATE_TYPE:
      case OUTCOME_TYPE:
      case FAILURE_TYPE:
      case COMMAND_HANDLE_TYPE:
        m_value = other.m_value;
        break;

        // Copy the actual value
      case STRING_TYPE:
        m_value.stringValue = new std::string(*other.m_value.stringValue);
        break;

      case BOOLEAN_ARRAY_TYPE:
        m_value.booleanArrayValue = new BooleanArray(*other.m_value.booleanArrayValue);
        break;

      case INTEGER_ARRAY_TYPE:
        m_value.integerArrayValue = new IntegerArray(*other.m_value.integerArrayValue);
        break;

      case REAL_ARRAY_TYPE:
        m_value.realArrayValue = new RealArray(*other.m_value.realArrayValue);
        break;

      case STRING_ARRAY_TYPE:
        m_value.stringArrayValue = new StringArray(*other.m_value.stringArrayValue);
        break;

      default:
        assertTrue_2(ALWAYS_FAIL, "Value copy constructor: unknown type");
        break;
      }
    }
    return *this;
  }

  // Do whatever is necessary to delete the previous contents
  void Value::cleanup()
  {
    if (!m_known)
      return;

    switch (m_type) {
    case STRING_TYPE:
      delete m_value.stringValue;
      m_value.stringValue = NULL;
      break;
      
    case BOOLEAN_ARRAY_TYPE:
      delete m_value.booleanArrayValue;
      m_value.booleanArrayValue = NULL;
      break;

    case INTEGER_ARRAY_TYPE:
      delete m_value.integerArrayValue;
      m_value.integerArrayValue = NULL;
      break;

    case REAL_ARRAY_TYPE:
      delete m_value.realArrayValue;
      m_value.realArrayValue = NULL;
      break;

    case STRING_ARRAY_TYPE:
      delete m_value.stringArrayValue;
      m_value.stringArrayValue = NULL;
      break;

    default:
      break;
    }
  }

  ValueType Value::valueType() const
  {
    return m_type;
  }

  bool Value::isKnown() const
  {
    return m_known;
  }

  bool Value::getValue(bool &result) const
  {
    if (m_type != BOOLEAN_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValue: type error");
      return false;
    }
    if (!m_known)
      return false;
    result = m_value.booleanValue;
    return true;
  }

  bool Value::getValue(int32_t &result) const
  {
    if (m_type != INTEGER_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValue: type error");
      return false;
    }
    if (!m_known)
      return false;
    result = m_value.integerValue;
    return true;
  }

  bool Value::getValue(double &result) const
  {
    if (!m_known)
      return false;
    switch (m_type) {
    case REAL_TYPE:
      result = m_value.realValue;
      return true;

    case INTEGER_TYPE:
      result = (double) m_value.integerValue;
      return true;

    default:
      assertTrue_2(ALWAYS_FAIL, "Value::getValue: type error");
      return false;
    }
  }

  bool Value::getValue(std::string &result) const
  {
    if (m_type != STRING_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValue: type error");
      return false;
    }
    if (!m_known)
      return false;
    result = *m_value.stringValue;
    return true;
  }

  bool Value::getValuePointer(std::string const *&ptr) const
  {
    if (m_type != STRING_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
    if (!m_known)
      return false;
    ptr = m_value.stringValue;
    return true;
  }

  bool Value::getValuePointer(Array const *&ptr) const
  {
    if (!m_known)
      return false;
    switch (m_type) {
    case BOOLEAN_ARRAY_TYPE:
      ptr = static_cast<Array const *>(m_value.booleanArrayValue);
      return true;

    case INTEGER_ARRAY_TYPE:
      ptr = static_cast<Array const *>(m_value.integerArrayValue);
      return true;

    case REAL_ARRAY_TYPE:
      ptr = static_cast<Array const *>(m_value.realArrayValue);
      return true;

    case STRING_ARRAY_TYPE:
      ptr = static_cast<Array const *>(m_value.stringArrayValue);
      return true;

    default:
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
  }

  bool Value::getValuePointer(BooleanArray const *&ptr) const
  {
    if (m_type != BOOLEAN_ARRAY_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
    if (!m_known)
      return false;
    ptr = m_value.booleanArrayValue;
    return true;
  }

  bool Value::getValuePointer(IntegerArray const *&ptr) const
  {
    if (m_type != INTEGER_ARRAY_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
    if (!m_known)
      return false;
    ptr = m_value.integerArrayValue;
    return true;
  }

  bool Value::getValuePointer(RealArray const *&ptr) const
  {
    if (m_type != REAL_ARRAY_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
    if (!m_known)
      return false;
    ptr = m_value.realArrayValue;
    return true;
  }

  bool Value::getValuePointer(StringArray const *&ptr) const
  {
    if (m_type != STRING_ARRAY_TYPE) {
      assertTrue_2(ALWAYS_FAIL, "Value::getValuePointer: type error");
      return false;
    }
    if (!m_known)
      return false;
    ptr = m_value.stringArrayValue;
    return true;
  }

  void Value::print(std::ostream &s) const
  {
    if (!m_known) {
      s << "<UNKNOWN>"; 
      return;
    }
    switch (m_type) {
    case BOOLEAN_TYPE:
      printValue<bool>(m_value.booleanValue, s);
      break;

    case INTEGER_TYPE:
      printValue<int32_t>(m_value.integerValue, s);
      break;

    case REAL_TYPE:
      printValue<double>(m_value.realValue, s);
      break;

    case STRING_TYPE:
      printValue<bool>(m_value.stringValue, s);
      break;

    case BOOLEAN_ARRAY_TYPE:
      printValue<bool>(m_value.booleanArrayValue, s);
      break;

    case INTEGER_ARRAY_TYPE:
      printValue<int32_t>(*m_value.integerArrayValue, s);
      break;

    case REAL_ARRAY_TYPE:
      printValue<double>(*m_value.realArrayValue, s);
      break;

    case STRING_ARRAY_TYPE:
      printValue<std::string>(*m_value.stringArrayValue, s);
      break;

    case NODE_STATE_TYPE:
    case OUTCOME_TYPE:
    case FAILURE_TYPE:
    case COMMAND_HANDLE_TYPE:
      s << "<INTERNAL_TYPE_NYI>";
      break;

    default:
      s << "<INVALID_TYPE>";
      break;
    }
  }

  std::ostream &operator<<(std::ostream &s, Value const &v)
  {
    v.print(s);
    return s;
  }

  // Issues:
  // - is unknown always equal to unknown?
  bool Value::equals(Value const &other) const
  {
    if (m_known != other.m_known)
      return false; // known != unknown, always
    switch (m_type) {
      
    case INTEGER_TYPE:
      if (other.m_type == m_type) {
        if (!m_known)
          return true;
        return m_value.integerValue == other.m_value.integerValue;
      }
      else if (other.m_type == REAL_TYPE) {
        if (!m_known)
          return true;
        return other.m_value.realValue == (double) m_value.integerValue;
      }
      else
        return false; // type mismatch
      
    case REAL_TYPE:
      if (other.m_type == m_type) {
        if (!m_known)
          return true;
        return m_value.realValue == other.m_value.realValue;
      }
      else if (other.m_type == INTEGER_TYPE) {
        if (!m_known)
          return true;
        return m_value.realValue == (double) other.m_value.integerValue;
      }
      else
        return false; // type mismatch

    default: 
      if (other.m_type != m_type)
        return false;
      if (!m_known)
        return true;
      switch (m_type) {
      case BOOLEAN_TYPE:
        return m_value.booleanValue == other.m_value.booleanValue;

      case NODE_STATE_TYPE:
      case OUTCOME_TYPE:
      case FAILURE_TYPE:
      case COMMAND_HANDLE_TYPE:
        return m_value.enumValue == other.m_value.enumValue;
      
      case STRING_TYPE:
        return *m_value.stringValue == *other.m_value.stringValue;

      case BOOLEAN_ARRAY_TYPE:
        return *m_value.booleanArrayValue == *other.m_value.booleanArrayValue;

      case INTEGER_ARRAY_TYPE:
        return *m_value.integerArrayValue == *other.m_value.integerArrayValue;

      case REAL_ARRAY_TYPE:
        return *m_value.realArrayValue == *other.m_value.realArrayValue;

      case STRING_ARRAY_TYPE:
        return *m_value.stringArrayValue == *other.m_value.stringArrayValue;

      default:
        assertTrue_2(ALWAYS_FAIL, "Value::equals: unknown value type");
        return false;
      }
    }
  }

  bool Value::lessThan(Value const &other) const
  {
    // unknown < known
    if (!m_known && other.m_known)
      return true;
    if (m_known && !other.m_known)
      return false;

    switch (m_type) {
    case INTEGER_TYPE:
      if (m_type == other.m_type) {
        if (m_known)
          return m_value.integerValue < other.m_value.integerValue;
        else 
          return false; // unknown integer values are equal
      }
      else if (REAL_TYPE == other.m_type) {
        if (m_known)
          return ((double) m_value.integerValue) < other.m_value.realValue;
        else 
          return true; // real unknown > int unknown
      }
      else 
        return m_type < other.m_type;

    case REAL_TYPE:
      if (m_type == other.m_type) {
        if (m_known)
          return m_value.realValue < other.m_value.realValue;
        else
          return false; // unknown real values are equal
      }
      else if (INTEGER_TYPE == other.m_type) {
        if (m_known)
          return m_value.realValue < (double) other.m_value.integerValue;
        else
          return false; // real unknown > int unknown
      }
      else 
        return m_type < other.m_type;

    default:
      // Unequal types 
      if (m_type < other.m_type)
        return true;
      else if (m_type > other.m_type)
        return false;
    }

    // Types are equal
    if (!m_known)
      return false; // unknowns of same type are equal

    switch (m_type) {
      case BOOLEAN_TYPE:
        return ((int) m_value.booleanValue) < ((int) other.m_value.booleanValue);

      case NODE_STATE_TYPE:
      case OUTCOME_TYPE:
      case FAILURE_TYPE:
      case COMMAND_HANDLE_TYPE:
        return m_value.enumValue < other.m_value.enumValue;
      
      case STRING_TYPE:
        return *m_value.stringValue < *other.m_value.stringValue;

      case BOOLEAN_ARRAY_TYPE:
        return *m_value.booleanArrayValue < *other.m_value.booleanArrayValue;

      case INTEGER_ARRAY_TYPE:
        return *m_value.integerArrayValue < *other.m_value.integerArrayValue;

      case REAL_ARRAY_TYPE:
        return *m_value.realArrayValue < *other.m_value.realArrayValue;

      case STRING_ARRAY_TYPE:
        return *m_value.stringArrayValue < *other.m_value.stringArrayValue;

      default:
        assertTrue_2(ALWAYS_FAIL, "Value::lessThan: unknown value type");
        return false;
      }
  }

} // namespace PLEXIL