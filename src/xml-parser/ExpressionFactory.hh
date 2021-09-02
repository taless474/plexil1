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

#ifndef EXPRESSION_FACTORY_HH
#define EXPRESSION_FACTORY_HH

#include "ValueType.hh" // ValueType, UNKNOWN_TYPE

// Forward declaration
namespace pugi
{
  class xml_node;
}

namespace PLEXIL
{
  // Forward declarations
  class Expression;
  class NodeConnector;

  //! \class ExpressionFactory
  //! \brief Virtual base class defining the API for factories for Expression subtypes.
  //! \ingroup XML-parser
  class ExpressionFactory
  {
  public:

    //! \brief Constructor.
    //! \param name The name of this factory. 
    ExpressionFactory(const std::string& name);

    //! \brief Virtual destructor.
    virtual ~ExpressionFactory();

    //! \brief Check the ValueType returned by expressions constructed by the factory.
    //! \param nodeId Pointer to the ID of the Node in which the expression occurs.
    //! \param expr Const handle to the expression's XML.
    //! \note The default method returns UNKNOWN_TYPE as a convenience.
    virtual ValueType check(char const *nodeId, pugi::xml_node const expr) const;

    //! \brief Return a pointer to the expression represented by the XML.
    //! \param expr Const handle to the expression's XML.
    //! \param node The Node in which the expression occurs.
    //! \param wasCreated Reference to a bool variable to mark whether the
    //!                   expression was constructed (if true) or shared (if false).
    //! \param returnType The desired return type of the expression to construct.
    //! \return Pointer to the resulting Expression.
    //! \note If wasCreated is false upon return, the result is a shared pointer
    //!       and should not be deleted.
    virtual Expression *allocate(pugi::xml_node const expr,
                                 NodeConnector *node,
                                 bool & wasCreated,
                                 ValueType returnType = UNKNOWN_TYPE) const
    = 0;

  private:

    // Deliberately unimplemented
#if __cplusplus >= 201103L
    ExpressionFactory() = delete;
    ExpressionFactory(const ExpressionFactory &) = delete;
    ExpressionFactory(ExpressionFactory &&) = delete;
    ExpressionFactory &operator=(const ExpressionFactory &) = delete;
    ExpressionFactory &operator=(ExpressionFactory &&) = delete;
#else
    ExpressionFactory();
    ExpressionFactory(const ExpressionFactory &);
    ExpressionFactory &operator=(const ExpressionFactory &);
#endif

  protected:

    const std::string m_name; /*!< Name used for lookup and error reporting */
  };

} // namespace PLEXIL

#endif // EXPRESSION_FACTORY_HH
