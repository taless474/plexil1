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

#ifndef PLEXIL_NEW_XML_PARSER
#define PLEXIL_NEW_XML_PARSER

#include "ParserException.hh"

// Forward declaration
namespace pugi
{
  class xml_document;
  class xml_node;
}

namespace PLEXIL
{
  class NodeImpl;
  class SymbolTable;

  //! \defgroup XML-parser The PLEXIL XML Plan Parser

  //! \ingroup XML-parser
  ///@{

  //! \brief The options to pass to the pugixml reader.
  extern unsigned int const PUGI_PARSE_OPTIONS;

  //! \brief Load an XML file and return a freshly constructed XML document object.
  //! \param filename Name of the file to load.
  //! \return Pointer to the xml_document.
  extern pugi::xml_document *loadXmlFile(std::string const &filename);

  //! \brief Perform a battery of checks on the plan represented by
  //!        this XML element, and build a symbol table from any
  //!        global declarations found in it.
  //! \param xml Const handle to the XML element.
  //! \return Pointer to the symbol table.
  //! \note Throws ParserException in the event of a plan error.
  extern SymbolTable *checkPlan(pugi::xml_node const xml);

  //! \brief Construct, but do not finalize, the Node structures for
  //!        the plan represented by the XML element, given the global
  //!        symbol table and the parent node.
  //! \param xml Const handle to the plan XML.
  //! \param symtab Pointer to the symbol table.
  //! \param parent Pointer to the parent Node.  May be NULL.
  //! \return Pointer to the root Node of the newly constructed plan.
  //! \note Presumes the XML has been checked by checkPlan() first.
  extern NodeImpl *constructPlan(pugi::xml_node const xml,
                                 SymbolTable *symtab,
                                 NodeImpl *parent);

  //! \brief Parse the plan represented by the XML element, construct
  //!        the Node tree, and finalize all references within the
  //!        tree.
  //! \param xml Const handle to the plan XML.
  //! \return Pointer to the root Node of the newly constructed plan.
  //! \note Presumes the XML has been checked by checkPlan() first.
  extern NodeImpl *parsePlan(pugi::xml_node const xml);

  ///@}
}

#endif // PLEXIL_NEW_XML_PARSER
