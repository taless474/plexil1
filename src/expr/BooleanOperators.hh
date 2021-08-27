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

#ifndef PLEXIL_BOOLEAN_OPERATORS_HH
#define PLEXIL_BOOLEAN_OPERATORS_HH

#include "OperatorImpl.hh"

namespace PLEXIL
{

  //! \class BooleanNot
  //! \brief Implements the NOT operator.
  //! \ingroup Expressions
  class BooleanNot : public OperatorImpl<bool>
  {
  public:

    //! \brief Virtual destructor.
    virtual ~BooleanNot();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Check that the argument types are valid for this Operator.
    //! \param typeVec The vector of argument types.
    //! \return true if valid, false if not.
    bool checkArgTypes(std::vector<ValueType> const &typeVec) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *arg) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(BooleanNot)

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    BooleanNot();

    // Not implemented.
    BooleanNot(const BooleanNot &);
    BooleanNot &operator=(const BooleanNot &);
  };

  //! \class BooleanOr
  //! \brief Implements the OR operator.
  //! \ingroup Expressions
  class BooleanOr : public OperatorImpl<bool>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~BooleanOr();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Check that the argument types are valid for this Operator.
    //! \param typeVec The vector of argument types.
    //! \return true if valid, false if not.
    bool checkArgTypes(std::vector<ValueType> const &typeVec) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *argA, Expression const *argB) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(BooleanOr)

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    BooleanOr();

    // Not implemented.
    BooleanOr(const BooleanOr &);
    BooleanOr &operator=(const BooleanOr &);
  };

  //! \class BooleanAnd
  //! \brief Implements the AND operator.
  //! \ingroup Expressions
  class BooleanAnd : public OperatorImpl<bool>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~BooleanAnd();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Check that the argument types are valid for this Operator.
    //! \param typeVec The vector of argument types.
    //! \return true if valid, false if not.
    bool checkArgTypes(std::vector<ValueType> const &typeVec) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *argA, Expression const *argB) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(BooleanAnd)

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    BooleanAnd();

    // Not implemented.
    BooleanAnd(const BooleanAnd &);
    BooleanAnd &operator=(const BooleanAnd &);
  };

  //! \class BooleanXOR
  //! \brief Implements the XOR operator.
  //! \ingroup Expressions
  class BooleanXor : public OperatorImpl<bool>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~BooleanXor();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Check that the argument types are valid for this Operator.
    //! \param typeVec The vector of argument types.
    //! \return true if valid, false if not.
    bool checkArgTypes(std::vector<ValueType> const &typeVec) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Expression const *argA, Expression const *argB) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool operator()(bool &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(BooleanXor)

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    BooleanXor();

    // Not implemented.
    BooleanXor(const BooleanXor &);
    BooleanXor &operator=(const BooleanXor &);
  };

} // namespace PLEXIL

#endif // PLEXIL_BOOLEAN_OPERATORS_HH
