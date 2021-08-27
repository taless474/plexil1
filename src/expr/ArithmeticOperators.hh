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

#ifndef PLEXIL_ARITHMETIC_OPERATORS_HH
#define PLEXIL_ARITHMETIC_OPERATORS_HH

#include "OperatorImpl.hh"

namespace PLEXIL
{

  //! \class Addition
  //! \brief Implements the ADD operators.
  //! \ingroup Expressions
  template <typename NUM>
  class Addition : public OperatorImpl<NUM>
  {
  public:

    //! \brief Virtual destructor.
    virtual ~Addition();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Addition<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Addition();

    // Not implemented.
    Addition(const Addition &);
    Addition &operator=(const Addition &);
  };

  template <typename NUM>
  class Subtraction : public OperatorImpl<NUM>
  {
  public:

    //! \brief Virtual destructor.
    virtual ~Subtraction();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Subtraction<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Subtraction();

    // Not implemented.
    Subtraction(const Subtraction &);
    Subtraction &operator=(const Subtraction &);
  };

  template <typename NUM>
  class Multiplication : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~Multiplication();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Multiplication<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Multiplication();

    // Not implemented.
    Multiplication(const Multiplication &);
    Multiplication &operator=(const Multiplication &);
  };

  template <typename NUM>
  class Division : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~Division();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Division<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Division();

    // Not implemented.
    Division(const Division &);
    Division &operator=(const Division &);
  };

  template <typename NUM>
  class Modulo : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~Modulo();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Modulo<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Modulo();

    // Not implemented.
    Modulo(const Modulo &);
    Modulo &operator=(const Modulo &);
  };

  template <typename NUM>
  class Minimum : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~Minimum();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Minimum<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Minimum();

    // Not implemented.
    Minimum(const Minimum &);
    Minimum &operator=(const Minimum &);
  };

  template <typename NUM>
  class Maximum : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~Maximum();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Perform the operation on the expressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param argA Const pointer to the first subexpression.
    //! \param argB Const pointer to the second subexpression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;

    //! \brief Perform the operation on the Function's subexpressions, and store the result.
    //! \param result Reference to the result variable.
    //! \param args Const reference to the Function.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Function const &args) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(Maximum<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    Maximum();

    // Not implemented.
    Maximum(const Maximum &);
    Maximum &operator=(const Maximum &);
  };

  //
  // Strictly unary operations
  //

  template <typename NUM>
  class AbsoluteValue : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~AbsoluteValue();

    //! \brief Check that the number of arguments is valid for this Operator.
    //! \param count The number of arguments.
    //! \return true if valid, false if not.
    bool checkArgCount(size_t count) const;

    //! \brief Perform the operation on the expression, and store the result.
    //! \param result Reference to the result variable.
    //! \param arg Const pointer to the argument Expression.
    //! \return true if the result is known, false if not.
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(AbsoluteValue<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    AbsoluteValue();

    // Not implemented.
    AbsoluteValue(const AbsoluteValue &);
    AbsoluteValue &operator=(const AbsoluteValue &);
  };
  

  template <typename NUM>
  class SquareRoot : public OperatorImpl<NUM>
  {
  public:
    
    //! \brief Virtual destructor.
    virtual ~SquareRoot();

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
    bool calc(NUM &result, Expression const *arg) const;

    //! \brief Singleton accessor.
    DECLARE_OPERATOR_STATIC_INSTANCE(SquareRoot<NUM>);

  private:

    //! \brief Private default constructor.  Accessible to singleton accessor only.
    SquareRoot();

    // Not implemented.
    SquareRoot(const SquareRoot &);
    SquareRoot &operator=(const SquareRoot &);
  };

}

#endif // PLEXIL_ARITHMETIC_OPERATORS_HH
