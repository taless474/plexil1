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

//
// Member function definitions and explicit instantiation for Constant classes
//

#include "Constant.hh"

namespace PLEXIL {

  /**
   * @brief Default constructor.
   */
  template <typename T>
  Constant<T>::Constant()
    : Expression(),
      m_unknown(true)
  {
  }

  /**
   * @brief Copy constructor.
   */
  template <typename T>
  Constant<T>::Constant(const Constant &other)
  : Expression(),
    m_value(other.m_value),
    m_unknown(false)
  {
  }

  /**
   * @brief Constructor from value type.
   */
  template <typename T>
  Constant<T>::Constant(const T &value)
  : Expression(),
    m_value(value),
    m_unknown(false)
  {
  }

  // TODO: Constructor from const char * representation?

  /**
   * @brief Destructor.
   */
  template <typename T>
  Constant<T>::~Constant()
  {
  }

  /**
   * @brief Return a print name for the expression type.
   * @return A constant character string.
   */
  template <typename T>
  const char *Constant<T>::exprName() const
  {
    return "Constant";
  }

  /**
   * @brief Return a print name for the value type.
   * @return A constant character string.
   * @note Specialized by type name.
   */
  template <typename T>
  const char *Constant<T>::typeName() const
  {
    return "UNKNOWN_TYPE";
  }

  template <>
  const char *Constant<double>::typeName() const
  {
    return "Real";
  }

  template <>
  const char *Constant<int32_t>::typeName() const
  {
    return "Integer";
  }

  template <>
  const char *Constant<bool>::typeName() const
  {
    return "Boolean";
  }

  template <>
  const char *Constant<std::string>::typeName() const
  {
    return "String";
  }

  /**
   * @brief Retrieve the value of this Expression.
   * @return The value of this Expression.
   */
  template <typename T>
  void Constant<T>::getValue(T& result) const
  {
    // FIXME: error if unknown?
    result = m_value;
  }

  /**
   * @brief Print the expression's value to the given stream.
   * @param s The output stream.
   * @note Default method, specializations should be implemented as appropriate.
   */
  template <typename T>
  void Constant<T>::printValue(std::ostream& s) const
  {
    if (m_unknown)
      s << "UNKNOWN";
    else
      s << m_value;
  }

  /**
   * @brief Query whether the expression's value is unknown.
   * @return True if unknown, false otherwise.
   */
  template <typename T>
  bool Constant<T>::isUnknown() const
  {
    return m_unknown;
  }

  /**
   * @brief Query whether this expression is constant, i.e. incapable of change.
   * @return True if assignable, false otherwise.
   */
  template <typename T>
  bool Constant<T>::isConstant() const
  {
    return true;
  }

  /**
   * @brief Is this expression active (i.e. propagating value changes?)
   * @return true if this Expression is active, false if it is not.
   */
  template <typename T>
  bool Constant<T>::isActive() const
  {
    return true; // constants are always active
  }

  /**
   * @brief Make this expression active.
   * @note No-op for constants.
   */
  template <typename T>
  void Constant<T>::activate()
  {
  }

  /**
   * @brief Make this expression inactive.
   * @note No-op for constants.
   */
  template <typename T>
  void Constant<T>::deactivate()
  {
  }

  /**
   * @brief Add a listener for changes to this Expression's value.
   * @param id The Id of the listener to notify.
   * @note No-op for constants.
   */
  template <typename T>
  void Constant<T>::addListener(ExpressionListenerId /* id */)
  {
  }

  /**
   * @brief Remove a listener from this Expression.
   * @param id The Id of the listener to remove.
   * @note No-op for constants.
   */
  template <typename T>
  void Constant<T>::removeListener(ExpressionListenerId /* id */)
  {
  }

  /**
   * @brief Notify this expression that a subexpression's value has changed.
   * @note No-op for constants.
   */
  template <typename T>
  void Constant<T>::notifyChanged()
  {
  }

  //
  // Explicit instantiations
  //
  template class Constant<double>;
  template class Constant<int32_t>;
  // template class Constant<uint16_t>;
  template class Constant<bool>;
  template class Constant<std::string>;

} // namespace PLEXIL
