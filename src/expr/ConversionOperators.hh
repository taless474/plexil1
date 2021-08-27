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

#ifndef PLEXIL_CONVERSION_OPERATORS_HH
#define PLEXIL_CONVERSION_OPERATORS_HH

#include "OperatorImpl.hh"

namespace PLEXIL
{


  //! \brief A specialization of OperatorImpl for numeric type conversion operators
  //!        Ceiling, Floor, Round, and Truncate.
  //! \ingroup Expressions
  template <typename NUM>
  class ConversionOperator : public OperatorImpl<NUM>
  {
  public:
    virtual ~ConversionOperator();

    virtual bool checkArgCount(size_t count) const;
    virtual bool checkArgTypes(std::vector<ValueType> const &typeVec) const;

    // Overrides
    virtual bool operator()(NUM &result, Expression const *arg0, Expression const *arg1) const;
    virtual bool operator()(NUM &result, Function const &args) const;

    virtual bool calc(NUM &result, Expression const *arg) const;
    virtual bool calcInternal(Real &result, Expression const *arg) const = 0;

  protected:
    ConversionOperator(std::string const &name);

  private:
    // Unimplemented
    ConversionOperator();
    ConversionOperator(ConversionOperator const &);
    ConversionOperator &operator=(ConversionOperator const &);
  };

  //
  // Real to Integer conversions
  //

  //! \brief Implements the CEIL conversion operator, with semantics
  //!        as specified in the C standard.
  //! \ingroup Expressions
  template <typename NUM>
  class Ceiling : public ConversionOperator<NUM>
  {
  public:
    Ceiling();
    virtual ~Ceiling();
    virtual bool calcInternal(Real &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Ceiling);

  private:
    Ceiling(const Ceiling &);
    Ceiling &operator=(const Ceiling &);
  };

  //! \brief Implements the FLOOR conversion operator, with semantics
  //!        as specified in the C standard.
  //! \ingroup Expressions
  template <typename NUM>
  class Floor : public ConversionOperator<NUM>
  {
  public:
    Floor();
    virtual ~Floor();
    virtual bool calcInternal(Real &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Floor);

  private:
    Floor(const Floor &);
    Floor &operator=(const Floor &);
  };

  //! \brief Implements the ROUND conversion operator, with semantics
  //!        as specified in the C standard.
  //! \ingroup Expressions
  template <typename NUM>
  class Round : public ConversionOperator<NUM>
  {
  public:
    Round();
    ~Round();
    virtual bool calcInternal(Real &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Round);

  private:
    Round(const Round &);
    Round &operator=(const Round &);
  };

  //! \brief Implements the TRUNC conversion operator, with semantics
  //!        as specified in the C standard.
  //! \ingroup Expressions
  template <typename NUM>
  class Truncate : public ConversionOperator<NUM>
  {
  public:
    Truncate();
    virtual ~Truncate();
    virtual bool calcInternal(Real &result, Expression const *arg) const;
    DECLARE_OPERATOR_STATIC_INSTANCE(Truncate);

  private:
    Truncate(const Truncate &);
    Truncate &operator=(const Truncate &);
  };

  //! \brief Implements the REAL_TO_INT conversion operator.
  //! \ingroup Expressions
  class RealToInteger : public OperatorImpl<Integer>
  {
  public:
    RealToInteger();
    virtual ~RealToInteger();
    virtual bool checkArgCount(size_t count) const;
    virtual bool checkArgTypes(std::vector<ValueType> const &typeVec) const;
    virtual bool calc(Integer &result, Expression const *arg) const;

    DECLARE_OPERATOR_STATIC_INSTANCE(RealToInteger);

  private:
    RealToInteger(const RealToInteger &);
    RealToInteger &operator=(const RealToInteger &);
  };

}

#endif // PLEXIL_CONVERSION_OPERATORS_HH
