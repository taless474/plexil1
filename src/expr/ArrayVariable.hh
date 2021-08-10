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

#ifndef PLEXIL_ARRAY_VARIABLE_HH
#define PLEXIL_ARRAY_VARIABLE_HH

#include "Assignable.hh"
#include "GetValueImpl.hh"
#include "Notifier.hh"

namespace PLEXIL
{

  //! \class ArrayVariable
  //! \brief An abstract base class similar to UserVariable, which
  //!        adds accessors required by the ArrayReference and
  //!        MutableArrayReference expression classes.  ArrayVariable
  //!        methods implement the type-invariant operations;
  //!        type-specific operations are delegated to a derived
  //!        class.
  //! \ingroup Expressions

  class ArrayVariable :
    public Assignable,
    virtual public Expression,
    public Notifier
  {
  public:

    //! \brief Virtual destructor.
    virtual ~ArrayVariable();

    // Listenable API

    //! \brief Query whether an object can generate its own change
    //!        notifications independent of other objects in the
    //!        notification graph.
    //! \return True if the object can change of its own accord, false if not.
    virtual bool isPropagationSource() const;

    //
    // Essential Expression API
    //

    //! \brief Return the name of this expression.
    //! \return Pointer to const character string.
    virtual char const *getName() const;

    //! \brief Return a print name for the expression type.
    //! \return Pointer to const character string.
    virtual char const *exprName() const;

    //! \brief Return the type of the expression's value.
    //! \return The value type.
    virtual bool isKnown() const;

    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if known, false if unknown.
    virtual bool getValuePointer(Array const *&ptr) const;

    //! \brief Print additional specialized information about an expression to a stream.
    //! \param s Reference to the output stream.
    virtual void printSpecialized(std::ostream &s) const;

    //
    // Assignable API
    //

    //! \brief Temporarily store the current value of this variable.
    //! \note Used to implement recovery from failed Assignment nodes.
    virtual void saveCurrentValue();

    // Provided by derived class
    // virtual void restoreSavedValue() = 0;

    //! \brief Read the saved value of this variable.
    //! \return The saved value.
    virtual Value getSavedValue() const;

    //! \brief Get the real variable for which this may be a proxy.
    //! \return Pointer to the base variable.
    virtual Assignable *getBaseVariable();

    //! \brief Get the real variable for which this may be a proxy.
    //! \return Const pointer to the base variable.
    virtual Assignable const *getBaseVariable() const;

    //! \brief Set the expression from which this object gets its initial value.
    //! \param expr Pointer to an Expression.
    //! \param garbage True if the expression should be deleted with this object, false otherwise.
    virtual void setInitializer(Expression *expr, bool garbage);

    //! \brief Set the value of this expression to unknown.
    virtual void setUnknown();

    //! \brief Set the value for this object.
    //! \param val The new value.
    virtual void setValue(Value const &val);

    //! \brief Set the value for this object.
    //! \param val The expression with the new value for this object.
    virtual void setValue(Expression const &val);

    //
    // Access needed by ArrayReference
    //

    bool elementIsKnown(size_t idx) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param result Reference to the result variable.
    //! \return True if the value is known, false otherwise.
    //! \note Default methods throw a PlanError.
    virtual bool getElement(size_t idx, Boolean &result) const; 
    virtual bool getElement(size_t idx, Integer &result) const;
    virtual bool getElement(size_t idx, Real &result) const;
    virtual bool getElement(size_t idx, String &result) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param ptr Reference to the result pointer variable.
    //! \return True if the value is known, false otherwise.
    //! \note Default methods throw a PlanError.
    virtual bool getElementPointer(size_t idx, String const *&ptr) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \return The value; may be unknown.
    virtual Value getElementValue(size_t idx) const;

    //
    // Access needed by MutableArrayReference
    //

    //! \brief Set the array element at the given index to unknown.
    //! \param idx The index.
    void setElementUnknown(size_t idx);

    //! \brief Store a new value at the given index of the array.
    //! \param idx The index.
    //! \param value Const reference to the new value.
    //! \note Delegated to ArrayVariableImpl.
    virtual void setElement(size_t idx, Value const &value) = 0;

  protected:

    //
    // Notifier API
    //

    //! \brief Perform any necessary actions to enter the active state.
    virtual void handleActivate();

    //! \brief Perform any necessary actions to enter the inactive state.
    virtual void handleDeactivate();

    //
    // API to derived classes
    //

    //! \brief Default constructor.
    ArrayVariable();

    //! \brief Constructor for plan loading.
    //! \param name The name of this variable in the parent node.
    //! \param size Expression whose value is the initial size of the array.
    //! \param sizeIsGarbage True if the size expression should be deleted 
    //!                      with the array variable.
    ArrayVariable(char const *name,
                  Expression *size = NULL,
                  bool sizeIsGarbage = false);

    //! \brief Copy from a generic array.
    //! \param a Pointer to array whose contents are to be copied.
    virtual void setValueImpl(Array const *a) = 0;

    //
    // API provided by derived classes
    //

    //! \brief Compare the given array with the current value.
    //! \param ary Const pointer to an Array.
    //! \return True if equal, false if not.
    virtual bool equals(Array const *ary) const = 0;

    //! \brief Construct an array of the appropriate type and requested size.
    //! \return Pointer to an Array.
    virtual Array *makeArray(size_t n) const = 0;

    //
    // Member variables
    //

    Array* m_value;
    Array* m_savedValue;   // for undoing assignment 

    Expression *m_size;
    Expression *m_initializer;
    char const *m_name;
    size_t m_maxSize;

    bool m_known;
    bool m_savedKnown;
    bool m_sizeIsGarbage;
    bool m_initializerIsGarbage;
    bool m_sizeIsConstant;
    bool m_initializerIsConstant;
  };

  //
  // Implementation classes
  //

  //! \brief Templatized class for arrays with a specific element type.
  //! \ingroup Expressions
  template <typename T>
  class ArrayVariableImpl :
    public GetValueImpl<ArrayImpl<T> >,
    public ArrayVariable
  {
  public:

    //! \brief Default constructor.
    ArrayVariableImpl();

    //! \brief Constructor for plan loading.
    //! \param name The name of this variable in the parent node.
    //! \param size Expression whose value is the initial size of the array.
    //! \param sizeIsGarbage True if the size expression should be deleted 
    //!                      with the array variable.
    ArrayVariableImpl(char const *name,
                      Expression *size = NULL,
                      bool sizeIsGarbage = false);

    //! \brief Virtual destructor.
    virtual ~ArrayVariableImpl() {}

    //! \brief Return the type of the expression's value.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    //! \note Just defer to ArrayVariable method.
    virtual bool isKnown() const
    {
      return ArrayVariable::isKnown();
    }

    //! \brief Copy from a generic array.
    //! \param a Pointer to array whose contents are to be copied.
    virtual void setValueImpl(Array const *a);

    //! \brief Restore the value set aside by saveCurrentValue().
    virtual void restoreSavedValue();

    //! \brief Copy a pointer to the (const) value of this object to a resut variable.
    //! \param ptr Reference to an appropriately typed pointer variable.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    //! \note Simply delegates to ArrayVariable method.
    virtual bool getValuePointer(Array const *& ptr) const
    {
      return ArrayVariable::getValuePointer(ptr);
    }
    
    //! \brief Copy a pointer to the (const) value of this object to a resut variable.
    //! \param ptr Reference to an appropriately typed pointer variable.
    //! \return True if the value is known, false if unknown or the value cannot be
    //!         represented as the desired type.
    virtual bool getValuePointer(ArrayImpl<T> const *&ptr) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param result Reference to the result variable.
    //! \return True if the value is known, false otherwise.
    virtual bool getElement(size_t idx, T &result) const; 

    //! \brief Store a new value at the given index of the array.
    //! \param idx The index.
    //! \param value Const reference to the new value.
    virtual void setElement(size_t idx, Value const &value);

    //
    // API to base class
    //

    //! \brief Compare the given array with the current value.
    //! \param ary Const pointer to an Array.
    //! \return True if equal, false if not.
    virtual bool equals(Array const *ary) const;

    //! \brief Construct an array of the appropriate type and requested size.
    //! \param n Size of the array.
    //! \return Pointer to an Array.
    virtual Array *makeArray(size_t n) const;

  private:

    //
    // Internal utilities
    //

    //! \brief Get a const pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<T> const *typedArrayPointer() const;

    //! \brief Get a pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<T> *typedArrayPointer();

  };

  template <>
  class ArrayVariableImpl<Integer> :
    public GetValueImpl<ArrayImpl<Integer> >,
    public ArrayVariable
  {
  public:

    //! \brief Default constructor.
    ArrayVariableImpl();

    //! \brief Constructor for plan loading.
    //! \param name The name of this variable in the parent node.
    //! \param size Expression whose value is the initial size of the array.
    //! \param sizeIsGarbage True if the size expression should be deleted 
    //!                      with the array variable.
    ArrayVariableImpl(char const *name,
                      Expression *size = NULL,
                      bool sizeIsGarbage = false);

    //! \brief Virtual destructor.
    virtual ~ArrayVariableImpl() {}

    //! \brief Return the type of the expression's value.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    //! \note Just defer to ArrayVariable method.
    virtual bool isKnown() const
    {
      return ArrayVariable::isKnown();
    }

    //! \brief Copy from a generic array.
    //! \param a Pointer to array whose contents are to be copied.
    virtual void setValueImpl(Array const *a);

    //! \brief Restore the value set aside by saveCurrentValue().
    virtual void restoreSavedValue();

    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if known, false if unknown.
    //! \note Simply delegates to ArrayVariable method.
    virtual bool getValuePointer(Array const *& ptr) const
    {
      return ArrayVariable::getValuePointer(ptr);
    }
    
    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if known, false if unknown.
    virtual bool getValuePointer(ArrayImpl<Integer> const *&ptr) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param result Reference to the result variable.
    //! \return True if the value is known, false otherwise.
    virtual bool getElement(size_t idx, Integer &result) const; 

    //! \brief Get the value of the element of the array as a Real.
    //! \param idx The index.
    //! \param result Reference to the result variable.
    //! \return True if the value is known, false otherwise.
    //! \note Conversion method.
    virtual bool getElement(size_t idx, Real &result) const; 

    //! \brief Store a new value at the given index of the array.
    //! \param idx The index.
    //! \param value Const reference to the new value.
    virtual void setElement(size_t idx, Value const &value);

    //! \brief Compare the given array with the current value.
    //! \param ary Const pointer to an Array.
    //! \return True if equal, false if not.
    virtual bool equals(Array const *ary) const;

    //! \brief Construct an array of the appropriate type and requested size.
    //! \param n Size of the array.
    //! \return Pointer to an Array.
    virtual Array *makeArray(size_t n) const;

  private:

    //
    // Internal utilities
    //

    //! \brief Get a const pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<Integer> const *typedArrayPointer() const;

    //! \brief Get a pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<Integer> *typedArrayPointer();

  };

  template <>
  class ArrayVariableImpl<String> :
    public GetValueImpl<ArrayImpl<String> >,
    public ArrayVariable
  {
  public:

    //! \brief Default constructor.
    ArrayVariableImpl();

    //! \brief Constructor for plan loading.
    //! \param name The name of this variable in the parent node.
    //! \param size Expression whose value is the initial size of the array.
    //! \param sizeIsGarbage True if the size expression should be deleted 
    //!                      with the array variable.
    ArrayVariableImpl(char const *name,
                      Expression *size = NULL,
                      bool sizeIsGarbage = false);

    //! \brief Virtual destructor.
    virtual ~ArrayVariableImpl() {}

    //! \brief Return the type of the expression's value.
    //! \return The value type.
    virtual ValueType valueType() const;

    //! \brief Determine whether the value is known or unknown.
    //! \return True if known, false otherwise.
    //! \note Just defer to ArrayVariable method.
    virtual bool isKnown() const
    {
      return ArrayVariable::isKnown();
    }

    //! \brief Copy from a generic array.
    //! \param a Pointer to array whose contents are to be copied.
    virtual void setValueImpl(Array const *a);

    //! \brief Restore the value set aside by saveCurrentValue().
    virtual void restoreSavedValue();

    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if known, false if unknown.
    //! \note Simply delegates to ArrayVariable method.
    virtual bool getValuePointer(Array const *& ptr) const
    {
      return ArrayVariable::getValuePointer(ptr);
    }
    
    //! \brief Retrieve a pointer to the (const) value of this Expression.
    //! \param ptr Reference to the pointer variable to receive the result.
    //! \return True if known, false if unknown.
    virtual bool getValuePointer(ArrayImpl<String> const *&ptr) const;

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param result Reference to the result variable.
    //! \return True if the value is known, false otherwise.
    virtual bool getElement(size_t idx, String &result) const; 

    //! \brief Get the value of the element of the array.
    //! \param idx The index.
    //! \param ptr Reference to the result pointer variable.
    //! \return True if the value is known, false otherwise.
    virtual bool getElementPointer(size_t idx, String const *&ptr) const;

    //! \brief Store a new value at the given index of the array.
    //! \param idx The index.
    //! \param value Const reference to the new value.
    virtual void setElement(size_t idx, Value const &value);

    //! \brief Compare the given array with the current value.
    //! \return True if equal, false if not.
    virtual bool equals(Array const *ary) const;

    //! \brief Construct an array of the appropriate type and requested size.
    //! \return Pointer to an Array.
    virtual Array *makeArray(size_t n) const;

  private:

    //
    // Internal utilities
    //

    //! \brief Get a const pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<String> const *typedArrayPointer() const;

    //! \brief Get a pointer to the native array object.
    //! \return The pointer.
    ArrayImpl<String> *typedArrayPointer();

  };

  typedef ArrayVariableImpl<Boolean> BooleanArrayVariable;
  typedef ArrayVariableImpl<Integer> IntegerArrayVariable;
  typedef ArrayVariableImpl<Real>    RealArrayVariable;
  typedef ArrayVariableImpl<String>  StringArrayVariable;

} // namespace PLEXIL

#endif // PLEXIL_ARRAY_VARIABLE_HH
