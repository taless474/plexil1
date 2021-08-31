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

#include "SymbolTable.hh"

#include "Debug.hh"

#include <stack>

namespace PLEXIL
{
  //
  // Symbol
  //

  Symbol::Symbol()
    : m_name(),
      m_paramTypes(),
      m_symbolType(NO_SYMBOL_TYPE),
      m_returnType(UNKNOWN_TYPE),
      m_anyParams(false)
  {
  }

  Symbol::Symbol(Symbol const &orig)
    : m_name(orig.m_name),
      m_paramTypes(orig.m_paramTypes),
      m_symbolType(orig.m_symbolType),
      m_returnType(orig.m_returnType),
      m_anyParams(orig.m_anyParams)
  {
  }

  Symbol::Symbol(char const *name, SymbolType t)
    : m_name(name),
      m_paramTypes(),
      m_symbolType(t),
      m_returnType(UNKNOWN_TYPE),
      m_anyParams(false)
  {
  }

  Symbol::~Symbol()
  {
  }

  Symbol &Symbol::operator=(Symbol const &orig)
  {
    m_name = orig.m_name;
    m_paramTypes = orig.m_paramTypes;
    m_symbolType = orig.m_symbolType;
    m_returnType = orig.m_returnType;
    m_anyParams = orig.m_anyParams;
    return *this;
  }

  std::string const &Symbol::name() const
  {
    return m_name;
  }

  SymbolType Symbol::symbolType() const
  {
    return (SymbolType) m_symbolType;
  }

  ValueType Symbol::returnType() const
  {
    return (ValueType) m_returnType;
  }

  ValueType Symbol::parameterType(size_t n) const
  {
    if (n > m_paramTypes.size())
      return UNKNOWN_TYPE;
    return (ValueType) m_paramTypes[n];
  }

  void Symbol::setReturnType(ValueType t)
  {
    m_returnType = (uint8_t) t;
  }

  void Symbol::setAnyParameters()
  {
    m_anyParams = true;
  }

  bool Symbol::anyParameters() const
  {
    return m_anyParams;
  }

  void Symbol::addParameterType(ValueType t)
  {
    m_paramTypes.push_back((uint8_t) t);
  }

  size_t Symbol::parameterCount() const
  {
    return m_paramTypes.size();
  }

  //
  // LibraryNodeSymbol
  //

  LibraryNodeSymbol::LibraryNodeSymbol()
    : m_name(),
      m_paramInOutMap(),
      m_paramTypeMap()
  {
  }

  LibraryNodeSymbol::LibraryNodeSymbol(LibraryNodeSymbol const &orig)
    : m_name(orig.m_name),
      m_paramInOutMap(orig.m_paramInOutMap),
      m_paramTypeMap(orig.m_paramTypeMap)
  {
  }

  LibraryNodeSymbol::LibraryNodeSymbol(char const *name)
    : m_name(name),
      m_paramInOutMap(),
      m_paramTypeMap()
  {
  }

  LibraryNodeSymbol::~LibraryNodeSymbol()
  {
  }

  LibraryNodeSymbol &LibraryNodeSymbol::operator=(LibraryNodeSymbol const &orig)
  {
    m_name = orig.m_name;
    m_paramInOutMap = orig.m_paramInOutMap;
    m_paramTypeMap = orig.m_paramTypeMap;
    return *this;
  }

  std::string const &LibraryNodeSymbol::name() const
  {
    return m_name;
  }

  SymbolType LibraryNodeSymbol::symbolType() const
  {
    return LIBRARY_NODE_TYPE;
  }

  void LibraryNodeSymbol::addParameter(char const *pname,
                                       ValueType t,
                                       bool isInOut)
  {
    std::string const pnameStr(pname);
    std::map<std::string, bool>::const_iterator it =
      m_paramInOutMap.find(pnameStr);
    if (it != m_paramInOutMap.end()) {
      // TODO Parser exception - duplicate name
    }
    m_paramInOutMap[pnameStr] = isInOut;
    m_paramTypeMap[pnameStr] = t;
  }

  bool LibraryNodeSymbol::isParameterDeclared(char const *pname)
  {
    std::string const pnameStr(pname);
    std::map<std::string, bool>::const_iterator it =
      m_paramInOutMap.find(pnameStr);
    return it != m_paramInOutMap.end();
  }

  bool LibraryNodeSymbol::isParameterInOut(char const *pname)
  {
    std::string const pnameStr(pname);
    std::map<std::string, bool>::const_iterator it =
      m_paramInOutMap.find(pnameStr);
    if (it == m_paramInOutMap.end())
      return false;
    return it->second;
  }

  ValueType LibraryNodeSymbol::parameterValueType(char const *pname)
  {
    std::string const pnameStr(pname);
    std::map<std::string, ValueType>::const_iterator it =
      m_paramTypeMap.find(pnameStr);
    if (it == m_paramTypeMap.end())
      return UNKNOWN_TYPE;
    return it->second;
  }


  //! \class SymbolTableImpl
  //! \brief Implementation of the PLEXIL symbol table.
  //! \ingroup XML-parser
  class SymbolTableImpl
    : public SymbolTable
  {
  private:
    typedef std::map<std::string, Symbol *> SymbolMap;
    typedef std::map<std::string, LibraryNodeSymbol *> LibraryMap;

    SymbolMap m_commandMap;     //!< Map of declared Commands.
    SymbolMap m_lookupMap;      //!< Map of declared Lookups.
    // SymbolMap m_functionMap; // future
    LibraryMap m_libraryMap;    //!< Map of declared LibraryNodes.

  public:

    //! \brief Default constructor.
    SymbolTableImpl()
    {
    }

    //! \brief Virtual destructor.
    virtual ~SymbolTableImpl()
    {
      SymbolMap::iterator it = m_commandMap.begin();
      while (it != m_commandMap.end()) {
        delete it->second;
        m_commandMap.erase(it);
        it = m_commandMap.begin();
      }
      it = m_lookupMap.begin();
      while (it != m_lookupMap.end()) {
        delete it->second;
        m_lookupMap.erase(it);
        it = m_lookupMap.begin();
      }
      LibraryMap::iterator lit = m_libraryMap.begin();
      while (lit != m_libraryMap.end()) {
        delete lit->second;
        m_libraryMap.erase(lit);
        lit = m_libraryMap.begin();
      }
    }

    //! \brief Add a declared Command with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    Symbol *addCommand(char const *name)
    {
      std::string const namestr(name);
      SymbolMap::const_iterator it =
        m_commandMap.find(namestr);
      if (it != m_commandMap.end())
        return NULL; // duplicate
      return (m_commandMap[namestr] = new Symbol(name, COMMAND_TYPE));
    }

    //! \brief Add a declared Lookup with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    Symbol *addLookup(char const *name)
    {
      std::string const namestr(name);
      SymbolMap::const_iterator it =
        m_lookupMap.find(namestr);
      if (it != m_lookupMap.end())
        return NULL; // duplicate
      return (m_lookupMap[namestr] = new Symbol(name, LOOKUP_TYPE));
    }

    //! \brief Add a declared Library Node with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    LibraryNodeSymbol *addLibraryNode(char const *name)
    {
      std::string const namestr(name);
      LibraryMap::const_iterator it =
        m_libraryMap.find(namestr);
      if (it != m_libraryMap.end())
        return NULL; // duplicate
      return (m_libraryMap[namestr] = new LibraryNodeSymbol(name));
    }

    //! \brief Get the symbol for the Command with the given name.
    //! \param name The name.
    //! \return Const pointer to the Symbol.  Will be NULL if not found.
    Symbol const *getCommand(char const *name)
    {
      std::string const namestr(name);
      SymbolMap::const_iterator it =
        m_commandMap.find(namestr);
      if (it == m_commandMap.end())
        return NULL;
      return it->second;
    }

    //! \brief Get the symbol for the Lookup with the given name.
    //! \param name The name.
    //! \return Const pointer to the Symbol.  Will be NULL if not found.
    Symbol const *getLookup(char const *name)
    {
      std::string const namestr(name);
      SymbolMap::const_iterator it =
        m_lookupMap.find(namestr);
      if (it == m_lookupMap.end())
        return NULL;
      return it->second;
    }

    //! \brief Get the symbol for the LibraryNode with the given name.
    //! \param name The name.
    //! \return Const pointer to the LibraryNodeSymbol.  Will be NULL if not found.
    LibraryNodeSymbol const *getLibraryNode(char const *name)
    {
      std::string const namestr(name);
      LibraryMap::const_iterator it =
        m_libraryMap.find(namestr);
      if (it == m_libraryMap.end())
        return NULL;
      return it->second;
    }

  };

  SymbolTable *makeSymbolTable()
  {
    return new SymbolTableImpl();
  }

  static std::stack<SymbolTable *> s_symtabStack;

  static SymbolTable *s_symbolTable = NULL;

  void pushSymbolTable(SymbolTable *s)
  {
    debugMsg("pushSymbolTable", ' ' << s);
    if (s_symbolTable)
      s_symtabStack.push(s_symbolTable);
    s_symbolTable = s;
  }

  void popSymbolTable()
  {
    debugMsg("popSymbolTable", ' ' << s_symbolTable);
    if (s_symtabStack.empty()) {
      // Back at top level
      s_symbolTable = NULL;
      return;
    }
    else {
      s_symbolTable = s_symtabStack.top();
      s_symtabStack.pop();
    }
  }

  extern Symbol const *getLookupSymbol(char const *name)
  {
    if (s_symbolTable)
      return s_symbolTable->getLookup(name);
    else
      return NULL;
  }

  extern Symbol const *getCommandSymbol(char const *name)
  {
    if (s_symbolTable)
      return s_symbolTable->getCommand(name);
    else
      return NULL;
  }

  extern LibraryNodeSymbol const *getLibraryNodeSymbol(char const *name)
  {
    if (s_symbolTable)
      return s_symbolTable->getLibraryNode(name);
    else
      return NULL;
  }

}
