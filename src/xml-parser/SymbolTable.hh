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

#ifndef PLEXIL_SYMBOL_TABLE_HH
#define PLEXIL_SYMBOL_TABLE_HH

#include "plexil-stdint.h"

#include "ValueType.hh"

#include <map>
#include <string>
#include <vector>

namespace PLEXIL
{

  //! \enum SymbolTyope
  //! \brief Enumeration for the type of a symbol in the PLEXIL symbol table.
  //! \ingroup XML-parser
  enum SymbolType {
    NO_SYMBOL_TYPE = 0,
    COMMAND_TYPE,
    LOOKUP_TYPE,
    LIBRARY_NODE_TYPE,
    // FUNCTION_TYPE, // future
      
    SYMBOL_TYPE_MAX
  };

  //! \class Symbol
  //! \brief Represents a symbol naming a declared PLEXIL Lookup or Command.
  //! \ingroup XML-parser
  class Symbol
  {
  public:

    //! \brief Default constructor.
    Symbol();

    //! \brief Copy constructor.
    //! \param orig Const reference to the Symbol being copied.
    Symbol(Symbol const &orig);

    //! \brief Constructor from name and symbol type.
    //! \param name Pointer to const null-terminated string.
    //! \param t The type of the symbol to construct.
    Symbol(char const *name, SymbolType t);

    //! \brief Copy assignment operator.
    //! \param orig Const reference to the Symbol being copied.
    Symbol &operator=(Symbol const &orig);

    //! \brief Destructor.
    ~Symbol();

    //! \brief Get the name of this symbol.
    //! \return Const reference to the name.
    std::string const &name() const;

    //! \brief Get the type of this symbol.
    //! \return The SymbolType.
    SymbolType symbolType() const;

    //! \brief Set the return type of the thing represented by the symbol.
    //! \param t The ValueType returned by the thing.
    //! \note Lookups always have a return type. Commands may have one.
    void setReturnType(ValueType t);

    //! \brief Get the return type of the thing represented by the symbol.
    //! \return The ValueType returned by the thing.
    ValueType returnType() const;

    //! \brief Append a new required parameter of the given type to
    //!        the symbol's list of parameters.
    //! \param t The ValueType of the parameter.
    //! \note A Symbol may have 0 or more required, typed parameters.
    void addParameterType(ValueType t);

    //! \brief Get the type of the requested parameter from the symbol.
    //! \param n The zero-based index of the parameter.
    //! \return The ValueType.
    ValueType parameterType(size_t n) const;

    //! \brief Get the number of required parameters for this symbol.
    //! \return The count.
    size_t parameterCount() const;

    //! \brief Allow this symbol to take any number of parameters of
    //!       any type following the required parameters.
    void setAnyParameters();

    //! \brief Does this symbol accept an unlimited number of parameters?
    //! \return true if so, false if not.
    bool anyParameters() const;

  private:

    std::string m_name;                  //!< The symbol's name.
    std::vector<uint8_t> m_paramTypes;   //!< Vector of parameter types.
    uint8_t m_symbolType;                //!< The type of the symbol.
    uint8_t m_returnType;                //!< The return type of the thing represented by the symbol.
    bool m_anyParams;                    //!< True if unlimited parameters are allowed.
  };

  //! \class LibraryNodeSymbol
  //! \brief Represents a declared PLEXIL LibraryNode.
  //! \ingroup XML-parser
  class LibraryNodeSymbol
  {
  public:
    //! \brief Default constructor.
    LibraryNodeSymbol();

    //! \brief Copy constructor.
    //! \param orig Const reference to the object being copied.
    LibraryNodeSymbol(LibraryNodeSymbol const &orig);

    //! \brief Constructor from name string.
    //! \param name Pointer to the const null-terminated string.
    LibraryNodeSymbol(char const *name);

    //! \brief Copy assignment operator.
    //! \param orig Const reference to the object being copied.
    LibraryNodeSymbol &operator=(LibraryNodeSymbol const &orig);

    //! \brief Destructor.
    ~LibraryNodeSymbol();

    //! \brief Get the name of this symbol.
    //! \return Const reference to the name.
    std::string const &name() const;

    //! \brief Get the type of this symbol.
    //! \return The SymbolType.
    SymbolType symbolType() const;

    //! \brief Add a declared parameter to this LibraryNode.
    //! \param pname Name of the parameter.
    //! \param t Type of the parameter.
    //! \param isInOut true if the parameter is an InOut parameter, false if In.
    void addParameter(char const *pname, ValueType t, bool isInOut);

    //! \brief Does the LibraryNode have a declared parameter with this name?
    //! \param pname Name of the parameter.
    //! \return true if so, false if not.
    bool isParameterDeclared(char const *pname);

    //! \brief Is the named parameter of this LibraryNode an InOut parameter?
    //! \param pname Name of the parameter.
    //! \return true if InOut, false if In.
    bool isParameterInOut(char const *pname);

    //! \brief Get the value type of the named parameter.
    //! \param pname Name of the parameter.
    //! \return The value type.
    ValueType parameterValueType(char const *pname);

  private:

    std::string m_name;                               //!< The name of the LibraryNode.
    std::map<std::string, bool> m_paramInOutMap;      //!< Map of parameter name to InOut status.
    std::map<std::string, ValueType> m_paramTypeMap;  //!< Map of parameter name to type.
  };
  
  //! \class SymbolTable
  //! \brief Stateless abstract base class for a symbol table for the PLEXIL XML parser.
  //! \ingroup XML-parser
  class SymbolTable
  {
  public:

    //! \brief Virtual destructor.
    virtual ~SymbolTable()
    {
    }

    //! \brief Add a declared Command with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    virtual Symbol *addCommand(char const *name) = 0;

    //! \brief Add a declared Lookup with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    virtual Symbol *addLookup(char const *name) = 0;

    //! \brief Add a declared Library Node with the given name.
    //! \param name The name.
    //! \return Pointer to the Symbol constructed; will be NULL if the name is a duplicate.
    virtual LibraryNodeSymbol *addLibraryNode(char const *name) = 0;

    //! \brief Get the symbol for the Command with the given name.
    //! \param name The name.
    //! \return Const pointer to the Symbol.  Will be NULL if not found.
    virtual Symbol const *getCommand(char const *name) = 0;

    //! \brief Get the symbol for the Lookup with the given name.
    //! \param name The name.
    //! \return Const pointer to the Symbol.  Will be NULL if not found.
    virtual Symbol const *getLookup(char const *name) = 0;

    //! \brief Get the symbol for the LibraryNode with the given name.
    //! \param name The name.
    //! \return Const pointer to the LibraryNodeSymbol.  Will be NULL if not found.
    virtual LibraryNodeSymbol const *getLibraryNode(char const *name) = 0;
  };

  //! \ingroup XML-parser
  ///@{

  //! \brief Construct a new SymbolTable instance.
  //! \return Pointer to the new SymbolTable.
  extern SymbolTable *makeSymbolTable();

  //! \brief Set the current symbol table, saving the old one on a stack to restore later.
  //! \param s Pointer to the new SymbolTable.
  extern void pushSymbolTable(SymbolTable *s);

  //! brief Restore the previous symbol table from the top of the symbol table stack.
  extern void popSymbolTable();

  //
  // Parser queries
  //

  //! \brief Get the declared Lookup symbol with the given name from the current symbol table.
  //! \param name The name.
  //! \return Const pointer to the Symbol.  Will be NULL if not found.
  extern Symbol const *getLookupSymbol(char const *name);

  //! \brief Get the declared Command symbol with the given name from the current symbol table.
  //! \param name The name.
  //! \return Const pointer to the Symbol.  Will be NULL if not found.
  extern Symbol const *getCommandSymbol(char const *name);

  //! \brief Get the declared LibraryNode symbol with the given name from the current symbol table.
  //! \param name The name.
  //! \return Const pointer to the LibraryNodeSymbol.  Will be NULL if not found.
  extern LibraryNodeSymbol const *getLibraryNodeSymbol(char const *name);

  ///@}

}

#endif // PLEXIL_SYMBOL_TABLE_HH
