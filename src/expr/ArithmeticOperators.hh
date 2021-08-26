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

#ifndef PLEXIL_ARITHMETIC_OPERATORS_HH
#define PLEXIL_ARITHMETIC_OPERATORS_HH

#include "OperatorImpl.hh"

namespace PLEXIL
{

  template <typename NUM>
  class Addition : public OperatorImpl<NUM>
  {
  public:
    Addition();
    ~Addition();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    bool calc(NUM &result, Function const &args) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Addition<NUM>);

  private:
    Addition(const Addition &);
    Addition &operator=(const Addition &);
  };

  template <typename NUM>
  class Subtraction : public OperatorImpl<NUM>
  {
  public:
    Subtraction();
    ~Subtraction();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    bool calc(NUM &result, Function const &args) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Subtraction<NUM>);

  private:
    Subtraction(const Subtraction &);
    Subtraction &operator=(const Subtraction &);
  };

  template <typename NUM>
  class Multiplication : public OperatorImpl<NUM>
  {
  public:
    Multiplication();
    ~Multiplication();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    bool calc(NUM &result, Function const &args) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Multiplication<NUM>);

  private:
    Multiplication(const Multiplication &);
    Multiplication &operator=(const Multiplication &);
  };

  template <typename NUM>
  class Division : public OperatorImpl<NUM>
  {
  public:
    Division();
    ~Division();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Division<NUM>);

  private:
    Division(const Division &);
    Division &operator=(const Division &);
  };

  template <typename NUM>
  class Modulo : public OperatorImpl<NUM>
  {
  public:
    Modulo();
    ~Modulo();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Modulo<NUM>);

  private:
    Modulo(const Modulo &);
    Modulo &operator=(const Modulo &);
  };

  template <typename NUM>
  class Minimum : public OperatorImpl<NUM>
  {
  public:
    Minimum();
    ~Minimum();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    bool calc(NUM &result, Function const &args) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Minimum<NUM>);

  private:
    Minimum(const Minimum &);
    Minimum &operator=(const Minimum &);
  };

  template <typename NUM>
  class Maximum : public OperatorImpl<NUM>
  {
  public:
    Maximum();
    ~Maximum();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    bool calc(NUM &result, Expression const *arg0, Expression const *arg1) const;
    bool calc(NUM &result, Function const &args) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Maximum<NUM>);

  private:
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
    AbsoluteValue();
    ~AbsoluteValue();
    bool checkArgCount(size_t count) const;
    bool calc(NUM &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(AbsoluteValue<NUM>);

  private:
    AbsoluteValue(const AbsoluteValue &);
    AbsoluteValue &operator=(const AbsoluteValue &);
  };
  

  template <typename NUM>
  class SquareRoot : public OperatorImpl<NUM>
  {
  public:
    SquareRoot();
    ~SquareRoot();
    bool checkArgCount(size_t count) const;
    bool checkArgTypes(std::vector<ValueType> const &typeVec) const;
    bool calc(NUM &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(SquareRoot<NUM>);

  private:
    SquareRoot(const SquareRoot &);
    SquareRoot &operator=(const SquareRoot &);
  };

}

#endif // PLEXIL_ARITHMETIC_OPERATORS_HH
