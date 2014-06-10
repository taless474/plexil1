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

#include "PlexilExpr.hh"

namespace PLEXIL
{
  //
  // PlexilExpr
  //
  PlexilExpr::PlexilExpr(std::string const &factoryName,
                         ValueType typ) 
    : m_id(this),
      m_name(factoryName),
      m_lineNo(0),
      m_colNo(0),
      m_type(typ)
  {
  }

  PlexilExpr::~PlexilExpr() 
  {
    m_id.remove();
  }

  const PlexilExprId& PlexilExpr::getId() const
  {
    return m_id;
  }
  
  const std::string& PlexilExpr::name() const 
  {
    return m_name;
  }
  
  ValueType PlexilExpr::type() const 
  {
    return m_type;
  }

  bool PlexilExpr::typed() const 
  {
    return (m_type != UNKNOWN_TYPE);
  }
  
  int PlexilExpr::lineNo() const 
  {
    return m_lineNo;
  }
  
  int PlexilExpr::colNo() const 
  {
    return m_colNo;
  }

  // FIXME: move this to common location, IF anything else needs it
  static char const *WHITESPACE_CHARS = " \t\n";

  void PlexilExpr::setName(const std::string& name)
  {
    m_name = name;
    size_t nonBlank = m_name.find_first_not_of(WHITESPACE_CHARS);
    if (nonBlank != 0)
      m_name.erase(0, nonBlank);
    nonBlank = m_name.find_last_not_of(WHITESPACE_CHARS);
    if (nonBlank + 1 < m_name.length())
      m_name.erase(nonBlank + 1);
  }

  void PlexilExpr::setType(ValueType type)
  {
    m_type = type; 
  }
  
  void PlexilExpr::setLineNo(int n)
  {
    m_lineNo = n;
  }
  
  void PlexilExpr::setColNo(int n) 
  {
    m_colNo = n;
  }

  //
  // PlexilOp
  //

  // FIXME: where to get type?
  PlexilOp::PlexilOp(std::string const &op,
                     ValueType typ)
    : PlexilExpr(op, typ)
  {
  }
  
  PlexilOp::~PlexilOp()
  {
    for (std::vector<PlexilExprId>::iterator it = m_subExprs.begin();
         it != m_subExprs.end();
         ++it)
      delete (PlexilExpr*) *it;
    m_subExprs.clear();
  }

  const std::vector<PlexilExprId>& PlexilOp::subExprs() const 
  {
    return m_subExprs;
  }
  
  void PlexilOp::addSubExpr(PlexilExprId expr)
  {
    m_subExprs.push_back(expr);
  }

  //
  // PlexilArrayElement
  //

  PlexilArrayElement::PlexilArrayElement(PlexilExprId const &array,
                                         PlexilExprId const &index)
    : PlexilExpr("ArrayElement"),
      m_array(array),
      m_subExprs(1, index)
  {
  }
  
  PlexilArrayElement::~PlexilArrayElement()
  {
    for (std::vector<PlexilExprId>::iterator it = m_subExprs.begin();
         it != m_subExprs.end();
         ++it)
      delete (PlexilExpr*) *it;
    m_subExprs.clear();
    delete (PlexilExpr*) m_array;
  }

  std::string const &PlexilArrayElement::getArrayName() const
  {
    PlexilArrayVar const *arr = dynamic_cast<PlexilArrayVar const *>((PlexilExpr const *) m_array);
    if (arr)
      return arr->varName();
    else {
      static std::string const empty;
      return empty;
    }
  }

  const std::vector<PlexilExprId>& PlexilArrayElement::subExprs() const
  {
    return m_subExprs;
  }
  
  void PlexilArrayElement::addSubExpr(PlexilExprId expr)
  {
    m_subExprs.push_back(expr);
  }

  //
  // PlexilValue
  //

  PlexilValue::PlexilValue(ValueType type, const std::string& value)
    : PlexilExpr("", type),
      m_value(value)
  {
  }

  PlexilValue::~PlexilValue()
  {
  }

  const std::string& PlexilValue::value() const
  {
    return m_value;
  }

  std::string const &PlexilValue::name() const
  {
    return typeNameAsValue(this->type());
  }

  //
  // PlexilArrayValue
  //   
         
  PlexilArrayValue::PlexilArrayValue(ValueType eltType,
                                     unsigned maxSize,
                                     const std::vector<std::string>& values)
    : PlexilValue(arrayType(eltType)),
      m_maxSize(maxSize),
      m_values(values)
  {
  }

  PlexilArrayValue::~PlexilArrayValue()
  {
  } 

  const std::vector<std::string>& PlexilArrayValue::values() const
  {
    return m_values;
  }

  unsigned PlexilArrayValue::maxSize() const
  {
    return m_maxSize;
  }

  ValueType PlexilArrayValue::elementType() const 
  {
    return arrayElementType(m_type);
  }

  //
  // PlexilVarRef
  //
  PlexilVarRef::PlexilVarRef(std::string const &varName, ValueType type)
    : PlexilExpr(typeNameAsVariable(type), type),
      m_defaultValue(PlexilExprId::noId()),
      m_varName(varName)
  {
  }

  PlexilVarRef::~PlexilVarRef()
  {
    if (m_defaultValue.isId())
      delete (PlexilExpr*) m_defaultValue;
  }
  
  const PlexilExprId& PlexilVarRef::defaultValue() const 
  {
    return m_defaultValue;
  }

  std::string const &PlexilVarRef::varName() const
  {
    return m_varName;
  }
  
  PlexilVar const *PlexilVarRef::variable() const 
  {
    return m_variable;
  }

  void PlexilVarRef::setVariable(PlexilVar const *var)
  {
    m_variable = var;
    m_varName = var->varName();
    setType(var->type());
    if (var->value().isId())
      m_defaultValue = var->value()->getId();
  }

  //
  // PlexilVar
  //
   
  PlexilVar::PlexilVar(const std::string& varName,
                       ValueType type)
    : PlexilExpr(typeNameAsVariable(type), type),
      m_value(),
      m_varName(varName)
  {
  }
   
  PlexilVar::PlexilVar(const std::string& varName,
                       ValueType type, 
                       const std::string& value)
    : PlexilExpr(typeNameAsVariable(type), type),
      m_value((new PlexilValue(type, value))->getId()),
      m_varName(varName)
  {
  }
   
  PlexilVar::PlexilVar(const std::string& varName, 
                       ValueType type, 
                       PlexilExprId const &value)
    : PlexilExpr(typeNameAsVariable(type), type),
      m_value(value),
      m_varName(varName)
  {
  }
   
  PlexilVar::~PlexilVar()
  {
    if (m_value.isId())
      delete (PlexilExpr *) m_value;
  }

  bool PlexilVar::isArray() const
  {
    return false;
  }

  std::string const &PlexilVar::varName() const
  {
    return m_varName;
  }

  PlexilExprId const &PlexilVar::value() const
  {
    return m_value;
  }

  //
  // PlexilArrayVar
  //

  PlexilArrayVar::PlexilArrayVar(const std::string& varName, 
                                 ValueType eltType, 
                                 const unsigned maxSize)
    : PlexilVar(varName, arrayType(eltType), PlexilExprId::noId()),
      m_maxSize(maxSize)
  {
  }

  PlexilArrayVar::PlexilArrayVar(const std::string& varName, 
                                 ValueType eltType, 
                                 const unsigned maxSize, 
                                 std::vector<std::string>& values)
    : PlexilVar(varName, arrayType(eltType), new PlexilArrayValue(eltType, maxSize, values)),
      m_maxSize(maxSize)
  {
  }
   
  PlexilArrayVar::~PlexilArrayVar() 
  {
  }

  bool PlexilArrayVar::isArray() const
  {
    return true;
  }

  ValueType PlexilArrayVar::elementType() const 
  {
    return arrayElementType(m_type);
  }
  
  unsigned PlexilArrayVar::maxSize() const 
  {
    return m_maxSize;
  }

} // namespace PLEXIL
