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

#ifndef PLEXIL_CONCRETE_EXPRESSION_FACTORY_HH
#define PLEXIL_CONCRETE_EXPRESSION_FACTORY_HH

#include "ExpressionFactory.hh"
#include "NodeConnector.hh"
#include "PlexilExpr.hh"

namespace PLEXIL
{

  /**
   * @class ConcreteExpressionFactory
   * @brief Concrete factory side of the "abstract factory" design pattern for expressions.
   */
  template<class FactoryType>
  class ConcreteExpressionFactory : public ExpressionFactory
  {
  public:
    ConcreteExpressionFactory(const std::string& name) 
      : ExpressionFactory(name) 
    {}

  private:
    /**
     * @brief Instantiates a new Expression of the appropriate type.
     * @param expr The PlexilExprId for the instantiated Expression.
     * @param node
     * @return The Id for the new Expression.
     * @note This default definition can be overridden in a template specialization.
     */
    ExpressionId create(const PlexilExprId& expr,
                        const NodeConnectorId& node = NodeConnectorId::noId()) const
    {
      return (new FactoryType(expr, node))->getId();
    }
  };

} // namespace PLEXIL

// Convenience macros
#define ENSURE_EXPRESSION_FACTORY(CLASS) template class PLEXIL::ConcreteExpressionFactory<CLASS >;
#define REGISTER_EXPRESSION(CLASS,NAME) {new PLEXIL::ConcreteExpressionFactory<CLASS >(#NAME);}

#endif // PLEXIL_CONCRETE_EXPRESSION_FACTORY_HH