/* Copyright (c) 2006-2016, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_USER_VARIABLE_HH
#define PLEXIL_USER_VARIABLE_HH

#include "Assignable.hh"
#include "GetValueImpl.hh"
#include "NotifierImpl.hh"

namespace PLEXIL 
{

  /**
   * @class UserVariable
   * @brief Templatized class for user-created plan variables.
   */

  // Scalar case
  template <typename T>
  class UserVariable final :
    public GetValueImpl<T>,
    public Assignable,
    public NotifierImpl
  {
  public:

    /**
     * @brief Default constructor.
     */
    UserVariable();

    /**
     * @brief Constructor for plan loading.
     * @param node The node to which this variable belongs (default none).
     * @param name The name of this variable in the parent node.
     */
    UserVariable(NodeConnector *node,
                 char const *name = "");
    
    /**
     * @brief Destructor.
     */
    virtual ~UserVariable();

    //
    // Essential Expression API
    //

    bool isAssignable() const override;

    Assignable const *asAssignable() const override;
    Assignable *asAssignable() override;

    char const *getName() const override;

    char const *exprName() const override;

    //
    // GetValueImpl API
    //

    bool isKnown() const override;

    /**
     * @brief Get the expression's value.
     * @param result The variable where the value will be stored.
     * @return True if known, false if unknown.
     */
    bool getValue(T &result) const override;

    //
    // Assignable API
    //

    /**
     * @brief Set the current value unknown.
     */
    void setUnknown() override;

    /**
     * @brief Reset to initial status.
     */
    void reset() override;

    void saveCurrentValue() override;

    void restoreSavedValue() override;

    Value getSavedValue() const override;

    NodeConnector *getNode() override;
    NodeConnector const *getNode() const override;

    Expression *getBaseVariable() override;
    Expression const *getBaseVariable() const override;

    /**
     * @brief Set the expression from which this object gets its initial value.
     * @param expr Pointer to an Expression.
     * @param garbage True if the expression should be deleted with this object, false otherwise.
     */
    virtual void setInitializer(Expression *expr, bool garbage) override;

    /**
     * @brief Set the value for this object.
     * @param val The new value for this object.
     */
    virtual void setValue(Value const &val) override;

    void handleActivate() override;

    void handleDeactivate() override;

    void printSpecialized(std::ostream &s) const override;

  protected:
    
    /**
     * @brief Assign a new value.
     * @param value The value to assign.
     */
    void setValueImpl(T const &value);

  private:

    // N.B. Ordering is suboptimal for bool because of required padding;
    // fine for int32_t and double
    T m_value;
    T m_savedValue;   // for undoing assignment 

    Expression *m_initializer;
    char const *m_name;

    // Only used by LuvListener at present. Eliminate?
    NodeConnector *m_node;

    bool m_known;
    bool m_savedKnown;
    bool m_initializerIsGarbage;

  };

  // Integer case - base template class is specialized
  template <>
  class UserVariable<Integer> final :
    public GetValueImpl<Integer>,
    public Assignable,
    public NotifierImpl
  {
  public:

    /**
     * @brief Default constructor.
     */
    UserVariable();

    /**
     * @brief Constructor for plan loading.
     * @param node The node to which this variable belongs (default none).
     * @param name The name of this variable in the parent node.
     */
    UserVariable(NodeConnector *node,
                 char const *name = "");
    
    /**
     * @brief Destructor.
     */
    virtual ~UserVariable();

    //
    // Essential Expression API
    //

    bool isAssignable() const override;

    Assignable const *asAssignable() const override;
    Assignable *asAssignable() override;

    char const *getName() const override;

    char const *exprName() const override;

    //
    // GetValueImpl API
    //

    bool isKnown() const override;

    /**
     * @brief Get the expression's value.
     * @param result The variable where the value will be stored.
     * @return True if known, false if unknown.
     */
    bool getValue(Integer &result) const override;

    //
    // Assignable API
    //

    /**
     * @brief Set the current value unknown.
     */
    void setUnknown() override;

    /**
     * @brief Reset to initial status.
     */
    void reset() override;

    void saveCurrentValue() override;

    void restoreSavedValue() override;

    Value getSavedValue() const override;

    NodeConnector *getNode() override;
    NodeConnector const *getNode() const override;

    Expression *getBaseVariable() override;
    Expression const *getBaseVariable() const override;

    /**
     * @brief Set the expression from which this object gets its initial value.
     * @param expr Pointer to an Expression.
     * @param garbage True if the expression should be deleted with this object, false otherwise.
     */
    virtual void setInitializer(Expression *expr, bool garbage) override;

    /**
     * @brief Set the value for this object.
     * @param val The new value for this object.
     */
    virtual void setValue(Value const &val) override;

    void handleActivate() override;

    void handleDeactivate() override;

    void printSpecialized(std::ostream &s) const override;

  protected:
    
    /**
     * @brief Assign a new value.
     * @param value The value to assign.
     */
    void setValueImpl(Integer const &value);

  private:

    // N.B. Ordering is suboptimal for bool because of required padding;
    // fine for int32_t and double
    Integer m_value;
    Integer m_savedValue;   // for undoing assignment 

    Expression *m_initializer;
    char const *m_name;

    // Only used by LuvListener at present. Eliminate?
    NodeConnector *m_node;

    bool m_known;
    bool m_savedKnown;
    bool m_initializerIsGarbage;

  };

  // String case
  template <>
  class UserVariable<String> final :
    public GetValueImpl<String>,
    public Assignable,
    public NotifierImpl
  {
  public:

    /**
     * @brief Default constructor.
     */
    UserVariable();

    /**
     * @brief Constructor for plan loading.
     * @param node The node to which this variable belongs (default none).
     * @param name The name of this variable in the parent node.
     */
    UserVariable(NodeConnector *node,
                 char const *name = "");

    /**
     * @brief Destructor.
     */
    virtual ~UserVariable();

    //
    // Essential Expression API
    //

    bool isAssignable() const override;

    Assignable const *asAssignable() const override;
    Assignable *asAssignable() override;

    char const *getName() const override;

    char const *exprName() const override;

    bool isKnown() const override;

    /**
     * @brief Get the expression's value.
     * @param result The variable where the value will be stored.
     * @return True if known, false if unknown.
     */
    bool getValue(String &result) const override;

    /**
     * @brief Retrieve a pointer to the (const) value of this Expression.
     * @param ptr Reference to the pointer variable to receive the result.
     * @return True if known, false if unknown.
     */
    bool getValuePointer(String const *&ptr) const override;
    template <typename U>
    bool getValuePointer(U const *&ptr) const;

    /**
     * @brief Assign a new value.
     * @param value The value to assign.
     * @note Type conversions must go on derived classes.
     */
    void setValueImpl(String const &value);

    /**
     * @brief Set the current value unknown.
     */
    void setUnknown() override;

    /**
     * @brief Reset to initial status.
     */
    void reset() override;

    void saveCurrentValue() override;

    void restoreSavedValue() override;

    Value getSavedValue() const override;

    NodeConnector *getNode() override;
    NodeConnector const *getNode() const override;

    Expression *getBaseVariable() override;
    Expression const *getBaseVariable() const override;

    /**
     * @brief Set the expression from which this object gets its initial value.
     * @param expr Pointer to an Expression.
     * @param garbage True if the expression should be deleted with this object, false otherwise.
     */
    void setInitializer(Expression *expr, bool garbage) override;

    /**
     * @brief Set the value for this object.
     * @param val The new value for this object.
     */
    virtual void setValue(Value const &val) override;

    void handleActivate() override;

    void handleDeactivate() override;

    void printSpecialized(std::ostream &s) const override;

  private:

    String m_value;
    String m_savedValue;   // for undoing assignment 

    Expression *m_initializer;
    char const *m_name;

    // Only used by LuvListener at present. Eliminate?
    NodeConnector *m_node;

    bool m_known;
    bool m_savedKnown;
    bool m_initializerIsGarbage;

  };

  //
  // Convenience typedefs 
  //

  typedef UserVariable<Boolean>     BooleanVariable;
  typedef UserVariable<Integer>     IntegerVariable;
  typedef UserVariable<Real>        RealVariable;
  typedef UserVariable<String>      StringVariable;

} // namespace PLEXIL

#endif // PLEXIL_USER_VARIABLE_HH
