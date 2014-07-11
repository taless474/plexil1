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

#include "ExprVec.hh"
#include "Error.hh"
#include "Expression.hh"
#include "Operator.hh"
#include "ParserException.hh"

namespace PLEXIL
{
  //
  // ExprVec base class methods
  //

  void ExprVec::addListener(ExpressionListener * ptr)
  {
    for (size_t i = 0; i < this->size(); ++i)
      (*this)[i]->addListener(ptr);
  }

  void ExprVec::removeListener(ExpressionListener * ptr)
  {
    for (size_t i = 0; i < this->size(); ++i)
      (*this)[i]->removeListener(ptr);
  }

  bool ExprVec::apply(Operator const *op, bool &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, int32_t &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, double &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, std::string &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, Array &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, BooleanArray &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, IntegerArray &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, RealArray &result) const
  {
    return (*op)(result, *this);
  }

  bool ExprVec::apply(Operator const *op, StringArray &result) const
  {
    return (*op)(result, *this);
  }

  /**
   * @class NullExprVec
   * @brief Concrete class template for empty expression vectors.
   */

  class NullExprVec : public ExprVec
  {
  public:
    NullExprVec(std::vector<Expression *> const &exps,
                std::vector<bool> const &garb)
    {
      assertTrue_1(exps.empty() && garb.empty());
    }

    ~NullExprVec()
    {
    }

    size_t size() const
    {
      return 0; 
    }

    Expression const *operator[](size_t /* n */) const
    {
      return NULL;
    }

    Expression *operator[](size_t /* n */)
    {
      return NULL;
    }

    void activate()
    {
    }

    void deactivate()
    {
    }

    void addListener(ExpressionListener * /* ptr */) 
    {
    }

    void removeListener(ExpressionListener * /* id */) 
    {
    }

  private:
    // Not implemented
    NullExprVec(const NullExprVec &);
    NullExprVec &operator=(const NullExprVec &);
  };

  //
  // FixedExprVec
  //
  // General cases - optimized cases below
  //

  /**
   * @class FixedExprVec
   * @brief Concrete class template for small expression vectors.
   * Allows optimization for common cases (specifically one and two parameter function calls).
   */

  template <unsigned N>
  class FixedExprVec : public ExprVec
  {
  public:
    FixedExprVec(std::vector<Expression *> const &exps,
                std::vector<bool> const &garb)
    : ExprVec()
    {
      check_error_1(exps.size() == N && garb.size() == N);
      for (size_t i = 0; i < N; ++i)
        exprs[i] = exps[i];
      for (size_t i = 0; i < N; ++i)
        garbage[i] = garb[i];
    }

    ~FixedExprVec()
    {
      for (size_t i = 0; i < N; ++i)
        if (garbage[i])
          delete (Expression *) exprs[i];
    }

    size_t size() const 
    {
      return N; 
    }

    Expression const *operator[](size_t n) const
    {
      check_error_1(n < N);
      return exprs[n]; 
    }

    Expression *operator[](size_t n)
    {
      check_error_1(n < N);
      return exprs[n]; 
    }

    void activate()
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->activate();
    }

    void deactivate()
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->deactivate();
    }

    // General case defers to base class for many of these operations
    void addListener(ExpressionListener * ptr) 
    {
      ExprVec::addListener(ptr);
    }

    void removeListener(ExpressionListener * ptr) 
    {
      ExprVec::removeListener(ptr);
    }

    bool apply(Operator const *op, bool &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, int32_t &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, double &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, std::string &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, Array &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, BooleanArray &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, IntegerArray &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, RealArray &result) const
    {
      return ExprVec::apply(op, result); 
    }

    bool apply(Operator const *op, StringArray &result) const
    {
      return ExprVec::apply(op, result); 
    }

  private:
    // Not implemented
    FixedExprVec(const FixedExprVec &);
    FixedExprVec &operator=(const FixedExprVec &);

    Expression *exprs[N];
    bool garbage[N];
  };

  // One-arg variants

  template <>
  FixedExprVec<1>::FixedExprVec(std::vector<Expression *> const &exps,
                              std::vector<bool> const &garb)
    : ExprVec()
  {
    check_error_1(exps.size() == 1 && garb.size() == 1);
    exprs[0] = exps[0];
    garbage[0] = garb[0];
  }

  template <>
  FixedExprVec<1>::~FixedExprVec()
  {
    if (garbage[0])
      delete (Expression *) exprs[0];
  }

  template <>
  void FixedExprVec<1>::activate() 
  {
    exprs[0]->activate();
  }

  template <>
  void FixedExprVec<1>::deactivate()
  {
    exprs[0]->deactivate();
  }

  template <>
  void FixedExprVec<1>::addListener(ExpressionListener * ptr)
  {
    exprs[0]->addListener(ptr);
  }

  template <>
  void FixedExprVec<1>::removeListener(ExpressionListener * ptr)
  {
    exprs[0]->removeListener(ptr);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, bool &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, int32_t &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, double &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, std::string &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, Array &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, BooleanArray &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, IntegerArray &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, RealArray &result) const
  {
    return (*op)(result, exprs[0]);
  }

  template <>
  bool FixedExprVec<1>::apply(Operator const *op, StringArray &result) const
  {
    return (*op)(result, exprs[0]);
  }

  //
  // Two-arg variants
  //

  template <>
  FixedExprVec<2>::FixedExprVec(std::vector<Expression *> const &exps,
                              std::vector<bool> const &garb)
    : ExprVec()
  {
    check_error_1(exps.size() == 2 && garb.size() == 2);
    exprs[0] = exps[0];
    exprs[1] = exps[1];
    garbage[0] = garb[0];
    garbage[1] = garb[1];
  }

  template <>
  FixedExprVec<2>::~FixedExprVec()
  {
    if (garbage[0])
      delete (Expression *) exprs[0];
    if (garbage[1])
      delete (Expression *) exprs[1];
  }

  template <>
  void FixedExprVec<2>::activate() 
  {
    exprs[0]->activate();
    exprs[1]->activate();
  }

  template <>
  void FixedExprVec<2>::deactivate()
  {
    exprs[0]->deactivate();
    exprs[1]->deactivate();
  }

  template <>
  void FixedExprVec<2>::addListener(ExpressionListener * ptr)
  {
    exprs[0]->addListener(ptr);
    exprs[1]->addListener(ptr);
  }

  template <>
  void FixedExprVec<2>::removeListener(ExpressionListener * ptr)
  {
    exprs[0]->removeListener(ptr);
    exprs[1]->removeListener(ptr);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, bool &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, int32_t &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, double &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, std::string &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, Array &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, BooleanArray &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, IntegerArray &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, RealArray &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  template <>
  bool FixedExprVec<2>::apply(Operator const *op, StringArray &result) const
  {
    return (*op)(result, exprs[0], exprs[1]);
  }

  //
  // GeneralExprVec
  //

  /**
   * @class GeneralExprVec
   * @brief Concrete variable-length variant of ExprVec which uses std::vector instead of arrays.
   */
  class GeneralExprVec : public ExprVec
  {
  public:
    GeneralExprVec(std::vector<Expression *> const &exps,
                   std::vector<bool> const &garb)
      : ExprVec(),
        exprs(exps),
        garbage(garb)
    {
      check_error_1(exps.size() == garb.size());
    }

    ~GeneralExprVec()
    {
      size_t n = exprs.size();
      for (size_t i = 0; i < n; ++i)
        if (garbage[i])
          delete (Expression *) exprs[i];
    }

    size_t size() const
    {
      return exprs.size(); 
    }

    Expression const *operator[](size_t n) const
    {
      check_error_1(n < exprs.size());
      return exprs[n]; 
    }

    Expression *operator[](size_t n)
    {
      check_error_1(n < exprs.size());
      return exprs[n]; 
    }

    void activate()
    {
      size_t n = exprs.size();
      for (size_t i = 0; i < n; ++i)
        exprs[i]->activate();
    }
      
    void deactivate()
    {
      size_t n = exprs.size();
      for (size_t i = 0; i < n; ++i)
        exprs[i]->deactivate();
    }

  private:
    // Not implemented
    GeneralExprVec(const GeneralExprVec &);
    GeneralExprVec &operator=(const GeneralExprVec &);

    std::vector<Expression *> exprs;
    std::vector<bool> garbage;
  };

  //
  // Factory function
  //

  // A crude but effective factory for ExprVec instances
  ExprVec *makeExprVec(std::vector<Expression *> const &exprs,
                       std::vector<bool> const &garbage)
  {
    checkParserException(exprs.size() == garbage.size(),
                         "makeExprVec: expression and garbage vectors of different lengths");
    switch (exprs.size()) {
    case 0:
      return static_cast<ExprVec *>(new NullExprVec(exprs, garbage));
    case 1:
      return static_cast<ExprVec *>(new FixedExprVec<1>(exprs, garbage));
    case 2:
      return static_cast<ExprVec *>(new FixedExprVec<2>(exprs, garbage));
    case 3:
      return static_cast<ExprVec *>(new FixedExprVec<3>(exprs, garbage));
    case 4:
      return static_cast<ExprVec *>(new FixedExprVec<4>(exprs, garbage));
    case 5:
      return static_cast<ExprVec *>(new FixedExprVec<5>(exprs, garbage));
    case 6:
      return static_cast<ExprVec *>(new FixedExprVec<6>(exprs, garbage));
    case 7:
      return static_cast<ExprVec *>(new FixedExprVec<7>(exprs, garbage));
    case 8:
      return static_cast<ExprVec *>(new FixedExprVec<8>(exprs, garbage));
    default: // anything greater than 8
      return static_cast<ExprVec *>(new GeneralExprVec(exprs, garbage));
    }
  }

  template class FixedExprVec<1>;
  template class FixedExprVec<2>;
  template class FixedExprVec<3>;
  template class FixedExprVec<4>;
  template class FixedExprVec<5>;
  template class FixedExprVec<6>;
  template class FixedExprVec<7>;
  template class FixedExprVec<8>;

} // namespace PLEXIL