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

#ifndef PLEXIL_PARSER_UTILS_HH
#define PLEXIL_PARSER_UTILS_HH

#include "Error.hh" // PLEXIL_NORETURN macro
#include "ValueType.hh"

//
// General purpose xml parsing utilities
//

namespace pugi
{
  class xml_node;
}

namespace PLEXIL
{

  //! \ingroup XML-parser
  ///@{

  //! \brief Does the string begin with the given prefix?
  //! \param prefix The prefix.
  //! \param str The string to test.
  //! \return true if the string begins with the prefix, false if not.
  extern bool testPrefix(char const *prefix, char const *str);

  //! \brief Does the string end with the given suffix?
  //! \param suffix The suffix.
  //! \param str The string to test.
  //! \return true if the string ends with the suffix, false if not.
  extern bool testSuffix(char const *suffix, char const *str);

  //! \brief Does the XML element have the given tag (name)?
  //! \param t The name.
  //! \param e The XML element.
  //! \return true if the element has the name, false if not.
  extern bool testTag(const char *t, pugi::xml_node const e);

  //! \brief Does the XML element's name begin with the given prefix?
  //! \param prefix The prefix.
  //! \param e The XML element.
  //! \return true if the element name begins with the prefix, false if not.
  extern bool testTagPrefix(const char *prefix, pugi::xml_node const e);

  //! \brief Does the XML element's name end with the given suffix?
  //! \param suffix The suffix.
  //! \param e The XML element.
  //! \return true if the element name ends with the suffix, false if not.
  extern bool testTagSuffix(const char *suffix, pugi::xml_node const e);

  //! \brief Does the XML element have any element children?
  //! \param e The XML element.
  //! \return true if the element has element children, false if not.
  extern bool hasChildElement(pugi::xml_node const e);

  //! \brief Check that the XML element has the given tag (name).
  //!        Throw a ParserException if it doesn't.
  //! \param t The tag name.
  //! \param e The XML element.
  extern void checkTag(const char *t, pugi::xml_node const e);

  //! \brief Check that the XML element's name ends with the given suffix.
  //!        Throw a ParserException if it doesn't.
  //! \param t The suffix.
  //! \param e The XML element.
  extern void checkTagSuffix(const char *t, pugi::xml_node const e);

  //! \brief Check that the XML element has an attribute with the given name.
  //!        Throw a ParserException if it doesn't.
  //! \param t The attribute name.
  //! \param e The XML element.
  extern void checkAttr(const char *t, pugi::xml_node const e);

  //! \brief Check that the given XML node is not empty.
  //!        Throw a ParserException if it is.
  //! \param e The XML element.
  extern void checkNotEmpty(pugi::xml_node const e);

  //! \brief Check that the given XML element has element children.
  //!        Throw a ParserException if it does not.
  //! \param e The XML element.
  extern void checkHasChildElement(pugi::xml_node const e);

  //! \brief Does the given string represent a valid Boolean literal value?
  //! \param initval The string.
  //! \return true if the string is a valid Boolean value, false if not.
  extern bool isBoolean(const char *initval);

  //! \brief Does the given string represent a valid Integer literal value?
  //! \param initval The string.
  //! \return true if the string is a valid Integer value, false if not.
  extern bool isInteger(const char *initval);

  //! \brief Does the given string represent a valid Real literal value?
  //! \param initval The string.
  //! \return true if the string is a valid Real value, false if not.
  extern bool isDouble(const char *initval);

  //! \brief Return the tag name for a literal value of the given ValueType.
  //! \param ty The type.
  //! \return Pointer to const null-terminated string.
  extern char const *typeNameAsValue(ValueType ty);

  //! \brief Construct a ParserException with the given error message
  //!        and location, and throw the exception.
  //! \param msg Const reference to the error message.
  //! \param location The XML node which triggered the error.
  //! \note Helper for checkParserExceptionWithLocation
  extern void throwParserException(std::string const &msg, pugi::xml_node location) PLEXIL_NORETURN;

  ///@}
} // namespace PLEXIL

//! \def reportParserExceptionWithLocation
//! \brief Throw a ParserException unconditionally
//! \param loc A pugi::xml_node with the location of the exception
//! \param msg An expression which writes the required message to a stream
//! \ingroup XML-parser
#define reportParserExceptionWithLocation(loc, msg) { \
  std::ostringstream whatstr; \
  whatstr << msg; \
  throwParserException(whatstr.str().c_str(), loc); \
}

//! \def checkParserExceptionWithLocation
//! \brief If the condition is false, throw a ParserException
//! \param cond The condition to test; if false, throw the exception
//! \param loc A pugi::xml_node with the location of the exception
//! \param msg An expression which writes the required message to a stream
//1 \ingroup XML-parser
#define checkParserExceptionWithLocation(cond, loc, msg) { \
  if (!(cond)) { \
    reportParserExceptionWithLocation(loc, msg); \
  } \
}

#endif // PLEXIL_PARSER_UTILS_HH
