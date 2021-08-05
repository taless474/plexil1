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

#ifndef PLEXIL_USER_VARIABLE_HH
#define PLEXIL_USER_VARIABLE_HH

#include "Assignable.hh"
#include "GetValueImpl.hh"
#include "Notifier.hh"

namespace PLEXIL 
{

  //! \brief Class template for user-created plan variables of scalar types.
  //! \addtogroup Expression The %PLEXIL Expression subsystem
  template <typename T>
  class UserVariable :
    public Assignable,
    public GetValueImpl<T>,
    public Notifier
  {
  public:

    //! \brief Default constructor.
    UserVariable();

    //! \brief Constructor with initial value.
    //! \param initVal Const reference to the initial value.
    UserVariable(T const &initVal);

    //! \brief Constructor with name.
    //! \param name Pointer to const character string representing the name of this
    //!             variable in the parent node.
    //! \note Used by the plan parser.
    UserVariable(char const *name);
    
    //! \brief Virtual destructor.
    virtual ~UserVariable();

    //
    // Listenable API
    //

    //! \brief Query whether an object can generate change notifications.
    //! \return True if so, false if not.
    virtual bool isPropagationSource() const;

    //
    // Essential Expression API
    //

    //! \brief Query whether this expression is assignable in the %PLEXIL language.
    //! \return True if assignable, false otherwise.
    bool isAssignable() const;

    //! \brief Get a const pointer to this expression as an instance of Assignable.
    //! \return The pointer. Will be NULL if the expression does not permit assignment.
    Assignable const *asAssignable() const;

    //! \brief Get a pointer to this expression as an instance of Assignable.
    //! \return The pointer. Will be NULL if the expression does not permit assignment.
    Assignable *asAssignable();

    //! \brief Return the name of this expression.
    //! \return Pointer to const character string.
    char const *getName() const;

    //! \brief Return a print name for the expression type.
    //! \return Pointer to const character string.
    char const *exprName() const;

    //
    // GetValueImpl API
    //

    //! \brief Determine whether the value of this expression is known or unknown.
    //! \return True if known, false otherwise.
    bool isKnown() const;

    //! \brief Copy the value of this object to a result of the same type.
    //! \param result Reference to an appropriately typed place to store the value.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    //! \note The value is not copied if the return value is false.
    bool getValue(T &result) const;

    //
    // Assignable API
    //

    //! \brief Set the value of this expression to unknown.
    void setUnknown();

    //! \brief Temporarily store the previous value of this variable.
    void saveCurrentValue();

    //! \brief Restore the value set aside by saveCurrentValue.
    void restoreSavedValue();

    //! \brief Read the saved value of this variable as a Value instance.
    //! \return The saved value.
    Value getSavedValue() const;
    
    //! \brief Get the real variable for which this may be a proxy.
    //! \return Pointer to the base variable.
    Assignable *getBaseVariable();

    //! \brief Get the real variable for which this may be a proxy.
    //! \return Const pointer to the base variable.
    Assignable const *getBaseVariable() const;

    //! \brief Set the expression from which this object gets its initial value.
    //! \param expr Pointer to an Expression.
    //! \param garbage True if the expression should be deleted with this object, false otherwise.
    virtual void setInitializer(Expression *expr, bool garbage);

    //! \brief Set the value for this object.
    //! \param val The new value for this object.
    virtual void setValue(Value const &val);

    //! \brief Set the value for this object.
    //! \param val Const reference to the expression providing the new value for this object.
    virtual void setValue(Expression const &val);

    void handleActivate();

    void handleDeactivate();

    void printSpecialized(std::ostream &s) const;

  protected:
    
    //! \brief Assign a new value.
    //! \param value Const reference to the value to assign.
    void setValueImpl(T const &value);

  private:

    // N.B. Ordering is suboptimal for bool because of required padding;
    // fine for Integer and Real

    //! \brief The current value.
    T m_value;

    //! \brief The saved value.
    T m_savedValue;

    //! \brief Pointer to the initial value expression.
    Expression *m_initializer;

    //! \brief The variable's name.
    char const *m_name;

    //! \brief True if the current value is known, false otherwise.
    bool m_known;

    //! \brief True if the saved value is known, false otherwise.
    bool m_savedKnown;

    //! \brief If true, delete the initializer expression when this object is deleted.
    bool m_initializerIsGarbage;
  };

  //! \brief Class template for user-created plan variables, specialized for String type.
  //! \ingroup Expression
  template <>
  class UserVariable<String> :
    public Assignable,
    public GetValueImpl<String>,
    public Notifier
  {
  public:

    //! \brief Default constructor.
    UserVariable();

    //! \brief Constructor with initial value.
    //! \param initVal Const reference to the initial value.
    explicit UserVariable(String const &initVal);

    //! \brief Constructor with name.
    //! \param name Pointer to const character string representing the name of this
    //!             variable in the parent node.
    //! \note Used by the plan parser.
    UserVariable(char const *name);
    
    //! \brief Virtual destructor.
    virtual ~UserVariable();

    //
    // Listenable API
    //

    //! \brief Query whether an object can generate change notifications.
    //! \return True if so, false if not.
    virtual bool isPropagationSource() const;

    //
    // Essential Expression API
    //

    //! \brief Query whether this expression is assignable in the %PLEXIL language.
    //! \return True if assignable, false otherwise.
    bool isAssignable() const;

    //! \brief Get a const pointer to this expression as an instance of Assignable.
    //! \return The pointer. Will be NULL if the expression does not permit assignment.
    Assignable const *asAssignable() const;

    //! \brief Get a pointer to this expression as an instance of Assignable.
    //! \return The pointer. Will be NULL if the expression does not permit assignment.
    Assignable *asAssignable();

    //! \brief Return the name of this expression.
    //! \return Pointer to const character string.
    char const *getName() const;

    //! \brief Return a print name for the expression type.
    //! \return Pointer to const character string.
    char const *exprName() const;

    //
    // GetValueImpl API
    //

    //! \brief Determine whether the value of this expression is known or unknown.
    //! \return True if known, false otherwise.
    bool isKnown() const;

    //! \brief Copy the value of this object to a result of the same type.
    //! \param result Reference to an appropriately typed place to store the value.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    //! \note The value is not copied if the return value is false.
    bool getValue(String &result) const;

    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    //! \note The value is not copied if the return value is false.
    bool getValuePointer(String const *&ptr) const;
    template <typename U>
    bool getValuePointer(U const *&ptr) const;

    // *** FIXME: Why is this member function public? ***

    //! \brief Assign a new value.
    //! \param value The value to assign.
    //! \note Type conversions must go on derived classes.
    void setValueImpl(String const &value);

    //
    // Assignable API
    //

    //! \brief Set the value of this expression to unknown.
    void setUnknown();

    //! \brief Temporarily store the previous value of this variable.
    void saveCurrentValue();

    //! \brief Restore the value set aside by saveCurrentValue.
    void restoreSavedValue();

    //! \brief Read the saved value of this variable as a Value instance.
    //! \return The saved value.
    Value getSavedValue() const;

    //! \brief Get the real variable for which this may be a proxy.
    //! \return Pointer to the base variable.
    Assignable *getBaseVariable();

    //! \brief Get the real variable for which this may be a proxy.
    //! \return Const pointer to the base variable.
    Assignable const *getBaseVariable() const;

    //! \brief Set the expression from which this object gets its initial value.
    //! \param expr Pointer to an Expression.
    //! \param garbage True if the expression should be deleted with this object, false otherwise.
    void setInitializer(Expression *expr, bool garbage);

    //! \brief Set the value for this object.
    //! \param val The new value for this object.
    virtual void setValue(Value const &val);

    //! \brief Set the value for this object.
    //! \param val Const reference to the expression providing the new value for this object.
    virtual void setValue(Expression const &val);

    void handleActivate();

    void handleDeactivate();

    void printSpecialized(std::ostream &s) const;

  private:

    //! \brief The current value.
    String m_value;

    //! \brief The saved value.
    String m_savedValue;

    //! \brief Pointer to the initial value expression.
    Expression *m_initializer;

    //! \brief The variable's name.
    char const *m_name;

    //! \brief True if the current value is known, false otherwise.
    bool m_known;

    //! \brief True if the saved value is known, false otherwise.
    bool m_savedKnown;

    //! \brief If true, delete the initializer expression when this object is deleted.
    bool m_initializerIsGarbage;
  };

  //
  // Convenience typedefs 
  //

  //! \ingroup Expression
  typedef UserVariable<Boolean>     BooleanVariable;

  //! \ingroup Expression
  typedef UserVariable<Integer>     IntegerVariable;

  //! \ingroup Expression
  typedef UserVariable<Real>        RealVariable;

  //! \ingroup Expression
  typedef UserVariable<String>      StringVariable;
} // namespace PLEXIL

#endif // PLEXIL_USER_VARIABLE_HH
