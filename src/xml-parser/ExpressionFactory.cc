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

#include "ExpressionFactory.hh"

#include "ArrayReference.hh"
#include "ConcreteExpressionFactory.hh"
#include "Debug.hh"
#include "Error.hh"
#include "NodeConnector.hh"
#include "ParserException.hh"
#include "parser-utils.hh"
#include "PlexilSchema.hh"
#include "SimpleMap.hh"

#include "pugixml.hpp"

namespace PLEXIL
{

  // Revise this when we have the actual number.
  // See purgeExpressionFactories() below and Expressions.cc in this directory.
  static size_t const EST_N_EXPR_FACTORIES = 60;

  static SimpleMap<std::string, ExpressionFactory *> s_expressionFactoryMap(EST_N_EXPR_FACTORIES);

  static void registerExpressionFactory(const std::string& name,
                                        ExpressionFactory* factory) 
  {
    check_error_1(factory != NULL);
    checkError(s_expressionFactoryMap.find(name) == s_expressionFactoryMap.end(),
               "Error:  Attempted to register a factory for name \"" << name <<
               "\" twice.");
    s_expressionFactoryMap[name] = factory;
    debugMsg("ExpressionFactory:registerFactory",
             "Registered factory for name \"" << name << "\"");
  }

  ExpressionFactory::ExpressionFactory(const std::string& name)
    : m_name(name)
  {
    registerExpressionFactory(m_name, this);
  }

  ExpressionFactory::~ExpressionFactory()
  {
  }

  const std::string& ExpressionFactory::getName() const
  {
    return m_name;
  }

  Expression *createExpression(pugi::xml_node const expr,
                               NodeConnector *node)
    throw (ParserException)
  {
    bool dummy;
    return createExpression(expr, node, dummy);
  }

  Expression *createExpression(pugi::xml_node const expr,
                               NodeConnector *node,
                               bool& wasCreated)
    throw (ParserException)
  {
    checkParserExceptionWithLocation(expr.type() == pugi::node_element,
                                     expr,
                                     "createExpression: argument is not an XML element");
    std::string const name = expr.name();
    // Delegate to factory
    debugMsg("createExpression", " name = " << name);
    SimpleMap<std::string, ExpressionFactory*>::const_iterator it =
      s_expressionFactoryMap.find(name);
    checkParserException(it != s_expressionFactoryMap.end(),
                         "createExpression: No factory registered for name \"" << name << "\".");
    Expression *retval = it->second->allocate(expr, node, wasCreated);
    debugMsg("createExpression",
             " Created " << (wasCreated ? "" : "reference to ") << retval->toString());
    return retval;
  }

  //
  // createAssignable
  //

  Assignable *createAssignable(pugi::xml_node const expr,
                               NodeConnector *node,
                               bool& wasCreated)
    throw (ParserException)
  {
    checkParserExceptionWithLocation(expr.type() == pugi::node_element,
                                     expr,
                                     "Not an XML element");
    assertTrue_2(node, "createAssignable: Internal error: Null node argument");
    Expression *resultExpr = NULL;
    if (testTagSuffix(VAR_SUFFIX, expr))
      resultExpr = createExpression(expr, node, wasCreated);
    else if (testTag(ARRAYELEMENT_TAG, expr))
      resultExpr = createMutableArrayReference(expr, node, wasCreated);
    else
      checkParserExceptionWithLocation(ALWAYS_FAIL,
                                       expr,
                                       "Invalid Assignment or InOut alias target");
    assertTrue_2(resultExpr, "createAssignable: Internal error: Null expression")
    checkParserExceptionWithLocation(resultExpr->isAssignable(),
                                     expr,
                                     "Expression is not assignable");
    return resultExpr->asAssignable();
  }

  void purgeExpressionFactories()
  {
    // Uncomment this to get a better estimate of factory map size.
    // std::cout << "ExpressionFactory map has " << s_expressionFactoryMap.size() << " entries" << std::endl;

    for (SimpleMap<std::string, ExpressionFactory*>::iterator it =
           s_expressionFactoryMap.begin();
         it != s_expressionFactoryMap.end();
         ++it) {
      ExpressionFactory* tmp = it->second;
      it->second = NULL;
      delete tmp;
    }
    s_expressionFactoryMap.clear();
  }

}
