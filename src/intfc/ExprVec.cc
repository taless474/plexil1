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

#include "ExprVec.hh"
#include "Error.hh"
#include "Expression.hh"

namespace PLEXIL
{

  //! \class FixedExprVec
  //! \brief Concrete class template for expression vectors of small constant size.
  //! \ingroup External-Interface
  template <unsigned N>
  class FixedExprVec : public ExprVec
  {
  public:

    //! \brief Default constructor.
    FixedExprVec()
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i] = NULL;
      for (size_t i = 0; i < N; ++i)
        garbage[i] = false;
    }

    //! \brief Destructor.
    ~FixedExprVec()
    {
      for (size_t i = 0; i < N; ++i)
        if (exprs[i] && garbage[i])
          delete exprs[i];
    }

    //! \brief Get the size of this vector.
    //! \return The size.
    size_t size() const 
    {
      return N; 
    }

    //! \brief Get the expression at the given index.
    //! \param n The index.
    //! \return Const pointer to the expression.
    Expression const *operator[](size_t n) const
    {
      check_error_1(n < N);
      return exprs[n]; 
    }

    //! \brief Get the expression at the given index.
    //! \param n The index.
    //! \return Pointer to the expression.
    Expression *operator[](size_t n)
    {
      check_error_1(n < N);
      return exprs[n]; 
    }

    //! \brief Set the expression at the index.
    //! \param i The index.
    //! \param exp Pointer to the expression.
    //! \param garbage true if the expression should be deleted with this object, false otherwise.
    void setArgument(size_t i, Expression *exp, bool isGarbage)
    {
      assertTrue_2(i < N, "setArgument(): too many args");
      exprs[i] = exp;
      garbage[i] = isGarbage;
    }

    //! \brief Make this object active if it is not already.
    void activate()
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->activate();
    }

    //! \brief Request that this object become inactive if it is not already.
    void deactivate()
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->deactivate();
    }

    //! \brief Add an expression listener to this object.
    //! \param l Pointer to the listener.
    void addListener(ExpressionListener *l)
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->addListener(l);
    }

    //! \brief Remove an expression listener from this object.
    //! \param l Pointer to the listener.
    void removeListener(ExpressionListener *l)
    {
      for (size_t i = 0; i < N; ++i)
        exprs[i]->removeListener(l);
    }

    //! \brief Call a function on all subexpressions of this object.
    //! \param oper A functor; it must implement an operator() method
    //!             of one argument, a pointer to Listenable,
    //!             returning void.
    void doSubexprs(ListenableUnaryOperator const &opr)
    {
      for (size_t i = 0; i < N; ++i)
        (opr)(exprs[i]);
    }

    //! \brief Print this object to an output stream.
    //! \param s Reference to the stream
    void print(std::ostream & str) const
    {
      for (size_t i = 0; i < N; ++i) {
        str << ' ';
        exprs[i]->print(str);
      }
    }

  private:

    // Not implemented
    FixedExprVec(FixedExprVec const &);
    FixedExprVec &operator=(FixedExprVec const &);

    Expression *exprs[N]; //!< Storage for the expressions.
    bool garbage[N];      //!< Storage for the garbage flags.
  };

  //! \class GeneralExprVec
  //! \brief Concrete variable-length variant of ExprVec which uses dynamically allocated arrays.
  //! \ingroup External-Interface
  class GeneralExprVec : public ExprVec
  {
  public:

    //! \brief Constructor with size.
    //! \param n The size of the vector.
    GeneralExprVec(size_t n)
      : m_size(n),
        exprs(new Expression*[n]()),
        garbage(new bool[n]())
    {
    }

    //! \brief Destructor.
    ~GeneralExprVec()
    {
      for (size_t i = 0; i < m_size; ++i)
        if (exprs[i] && garbage[i])
          delete exprs[i];
      delete[] garbage;
      delete[] exprs;
    }

    //! \brief Get the size of this vector.
    //! \return The size.
    size_t size() const
    {
      return m_size; 
    }

    //! \brief Get the expression at the given index.
    //! \param n The index.
    //! \return Const pointer to the expression.
    Expression const *operator[](size_t n) const
    {
      check_error_1(n < m_size);
      return exprs[n]; 
    }

    //! \brief Get the expression at the given index.
    //! \param n The index.
    //! \return Pointer to the expression.
    Expression *operator[](size_t n)
    {
      check_error_1(n < m_size);
      return exprs[n]; 
    }

    //! \brief Set the expression at the index.
    //! \param i The index.
    //! \param exp Pointer to the expression.
    //! \param garbage true if the expression should be deleted with this object, false otherwise.
    void setArgument(size_t i, Expression *exp, bool isGarbage)
    {
      assertTrue_2(i < m_size, "setArgument(): too many args");
      exprs[i] = exp;
      garbage[i] = isGarbage;
    }

    //! \brief Make this object active if it is not already.
    void activate()
    {
      for (size_t i = 0; i < m_size; ++i)
        exprs[i]->activate();
    }
      
    //! \brief Request that this object become inactive if it is not already.
    void deactivate()
    {
      for (size_t i = 0; i < m_size; ++i)
        exprs[i]->deactivate();
    }

    //! \brief Print this object to an output stream.
    //! \param str Reference to the stream
    void print(std::ostream &str) const
    {
      for (size_t i = 0; i < m_size; ++i) {
        str << ' ';
        exprs[i]->print(str);
      }
    }

    //! \brief Add an expression listener to this object.
    //! \param l Pointer to the listener.
    void addListener(ExpressionListener *l)
    {
      for (size_t i = 0; i < m_size; ++i)
        exprs[i]->addListener(l);
    }

    //! \brief Remove an expression listener from this object.
    //! \param l Pointer to the listener.
    void removeListener(ExpressionListener *l)
    {
      for (size_t i = 0; i < m_size; ++i)
        exprs[i]->addListener(l);
    }

    //! \brief Call a function on all subexpressions of this object.
    //! \param oper A functor; it must implement an operator() method
    //!             of one argument, a pointer to Listenable,
    //!             returning void.
    virtual void doSubexprs(ListenableUnaryOperator const &opr)
    {
      for (size_t i = 0; i < m_size; ++i)
        (opr)(exprs[i]);
    }

  private:

    // Not implemented
    GeneralExprVec();
    GeneralExprVec(GeneralExprVec const &);
    GeneralExprVec &operator=(GeneralExprVec const &);

    size_t m_size;       //!< The size of this vector.
    Expression **exprs;  //!< Pointer to dynamically allocated array of Expression pointers.
    bool *garbage;       //!< Pointer to dynamically allocated array of garbage flags.
  };

  //
  // Factory function
  //

  ExprVec *makeExprVec(size_t n)
  {
    switch (n) {
    case 0:
      assertTrue_2(false, "makeExprVec: zero-length vector not implemented");
      return NULL;
      
    case 1:
      return static_cast<ExprVec *>(new FixedExprVec<1>());

    case 2:
      return static_cast<ExprVec *>(new FixedExprVec<2>());

    case 3:
      return static_cast<ExprVec *>(new FixedExprVec<3>());

    case 4:
      return static_cast<ExprVec *>(new FixedExprVec<4>());

    default: // anything greater than 4
      return static_cast<ExprVec *>(new GeneralExprVec(n));
    }
  }

  // Concrete instantiations (redundant with above?)
  // template class FixedExprVec<1>;
  // template class FixedExprVec<2>;
  // template class FixedExprVec<3>;
  // template class FixedExprVec<4>;

} // namespace PLEXIL
