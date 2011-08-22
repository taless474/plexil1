/* Copyright (c) 2006-2008, Universities Space Research Association (USRA).
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

#include "Expression.hh"
#include "Debug.hh"
#include "ExpressionFactory.hh"
#include "NodeConnector.hh"
#include "StoredArray.hh"
#include "Variable.hh"

#include <limits>
#include <algorithm>
#include <iomanip> // for setprecision()

namespace PLEXIL {

  ExpressionListener::ExpressionListener() : m_id(this), m_activeCount(0)/*m_active(true)*/ {}

  ExpressionListener::~ExpressionListener() {
    m_id.remove();
  }

  void ExpressionListener::activate() {
    m_activeCount++;
  }

  void ExpressionListener::deactivate() {
    checkError(m_activeCount > 0,
	       "Attempted to deactivate an expression listener too many times.");
    m_activeCount--;
  }

  SubexpressionListener::SubexpressionListener(const ExpressionId& exp)
    : ExpressionListener(), m_exp(exp) {}

  void SubexpressionListener::notifyValueChanged(const ExpressionId& exp) {
    m_exp->handleChange(exp);
  }

  Expression::Expression()
    : m_id(this),
      m_activeCount(0), 
      m_value(UNKNOWN()), 
      m_savedValue(UNKNOWN()),
      m_dirty(false), 
      m_lock(false), 
      m_ignoreCachedValue(false)
  {
  }

  Expression::~Expression() {
    checkError(m_outgoingListeners.empty(),
	       "Error: Expression '" << toString() << "' still has outgoing listeners.");
    m_id.remove();
  }

  double Expression::getValue() const {
    return (isActive() ? m_value : UNKNOWN());
  }

  void Expression::setValue(const double val) {
    internalSetValue(val);
  }

  void Expression::activate() {
    bool changed = (m_activeCount == 0);
    m_activeCount++;
    debugMsg("Expression:activate", "Activating " << getId());
    handleActivate(changed);
  }

  void Expression::deactivate() {
    checkError(m_activeCount > 0,
	       "Attempted to deactivate expression " << getId() << " too many times.");
    bool changed = (m_activeCount == 1);
    m_activeCount--;
    debugMsg("Expression:deactivate", "Deactivating " << getId());
    handleDeactivate(changed);
  }

  void Expression::addListener(ExpressionListenerId id) {
    check_error(id.isValid());
    if(std::find(m_outgoingListeners.begin(), m_outgoingListeners.end(), id) !=
       m_outgoingListeners.end())
      return;
    m_outgoingListeners.push_back(id);
  }

  void Expression::removeListener(ExpressionListenerId id) {
    check_error(id.isValid());
    std::list<ExpressionListenerId>::iterator it = std::find(m_outgoingListeners.begin(),
							     m_outgoingListeners.end(), id);
    if(it == m_outgoingListeners.end())
      return;
    m_outgoingListeners.erase(it);
  }

  std::string Expression::toString() const {
    std::ostringstream str;
    str << "(" << getId() << "[" << (isActive() ? "a" : "i") << (isLocked() ? "l" : "u") <<
      "](" << valueString() << "): ";
    return str.str();
  }

  // Much-needed static member function to construct the One True Printed Representation of a value.

  std::string Expression::valueToString(const double val) {
	if (val == UNKNOWN())
	  return std::string("UNKNOWN");
    else if (LabelStr::isString(val))
      return std::string(LabelStr(val).toString());
    else if (StoredArray::isKey(val))
      return StoredArray(val).toString();
	// below this point must be a number
    else if (val == REAL_PLUS_INFINITY)
      return std::string("inf");
    else if (val == REAL_MINUS_INFINITY)
      return std::string("-inf");
	else {
	  std::ostringstream str;
      str << std::setprecision(15) << val;
	  return str.str();
	}
  }

  std::string Expression::valueString() const {
    return valueToString(getValue());
  }

  void Expression::lock() {
    checkError(!isLocked(), toString() << " already locked.");
    checkError(isActive(), "Attempt to lock inactive expression " << toString());
    m_lock = true;
    m_savedValue = m_value;
    handleLock();
  }

  void Expression::unlock() {
    checkError(isLocked(), toString() << " not locked.");
    m_lock = false;
    if(m_dirty)
      internalSetValue(m_savedValue);
    handleUnlock();
  }

  void Expression::internalSetValue(const double value) {
    checkError(checkValue(value), 
			   "Value " << valueToString(value) << " invalid for " << toString());
    if (isLocked()) {
      if (m_savedValue != value || m_value != value) {
		m_dirty = true;
		m_savedValue = value;
      }
    }
    else {
      bool changed = (m_value != value);
      m_value = value;
      m_dirty = false;
      if(m_ignoreCachedValue || changed)
		publishChange();
    }
  }

  void Expression::publishChange() {
    if(!isActive())
      return;
    for(std::list<ExpressionListenerId>::iterator it = m_outgoingListeners.begin();
	it != m_outgoingListeners.end(); ++it) {
      check_error((*it).isValid());
      if((*it)->isActive())
	(*it)->notifyValueChanged(m_id);
    }
  }

  // This constructor used only in unit tests
  TransparentWrapper::TransparentWrapper(const ExpressionId& exp)
    : Expression(), m_listener(*this), m_exp(exp) 
  {
	commonInit(exp);
  }

  TransparentWrapper::TransparentWrapper(const ExpressionId& exp, const NodeConnectorId& node)
    : Expression(), m_listener(*this), m_exp(exp) 
  {
	commonInit(exp);
  }

  TransparentWrapper::~TransparentWrapper(){
    m_exp->removeListener(m_listener.getId());
  }

  void TransparentWrapper::commonInit(const ExpressionId& exp)
  {
    debugMsg("TransparentWrapper:TransparentWrapper",
	     "Constructing a transparent wrapper around " << exp->toString());
    m_exp->addListener(m_listener.getId());
       //m_listener.activate();
    m_value = exp->getValue();
    debugMsg("TransparentWrapper:TransparentWrapper",
	     "Constructed " << toString());
  }

  void TransparentWrapper::setValue(const double value) {
    debugMsg("TransparentWrapper:setValue",
			 "Setting " << toString() << " to value " << Expression::valueToString(value));
    internalSetValue(value);
    m_exp->setValue(value);
  }

  bool TransparentWrapper::checkValue(const double value) {
    debugMsg("TransparentWrapper:checkValue",
			 "Checking " << toString() << " value " << Expression::valueToString(value));
    return m_exp->checkValue(value);
  }

  std::string TransparentWrapper::toString() const {
    std::ostringstream str;
    str << "TransparentWrapper(" << getId() << "[" << (isActive() ? "a" : "i") << (isLocked() ? "l" : "u") <<
       "T]{" << valueString() <<"}(" << m_exp->toString() << ")";
    return str.str();
  }

  std::string TransparentWrapper::valueString() const {
    return m_exp->valueString();
  }

  void TransparentWrapper::handleActivate(const bool /* changed */) {
    m_listener.activate();
    m_exp->activate();
    if(m_exp->isActive())
      internalSetValue(m_exp->getValue());
  }

  void TransparentWrapper::handleDeactivate(const bool /* changed */) {
    m_exp->deactivate();
    m_listener.deactivate();
  }

  void TransparentWrapper::handleChange(const ExpressionId& expression) {
    debugMsg("TransparentWrapper:handleChange",
	     "Changing from wrapped value of " << toString() << " to " <<
	     expression->toString());
    internalSetValue(expression->getValue());
  }

  // Used below in Expression::UNKNOWN_EXP().
  class UnknownVariable : public VariableImpl
  {
  public:
	UnknownVariable() : VariableImpl(true) {}
	~UnknownVariable() {}

	// Don't assign to this variable!
	bool checkValue(const double /* value */) { return false; }
	
  private:
	// Deliberately unimplemented
	UnknownVariable(const UnknownVariable&);
	UnknownVariable& operator=(const UnknownVariable&);
  };

  ExpressionId& Expression::UNKNOWN_EXP() {
    static ExpressionId sl_exp;
    if (sl_exp.isNoId())
      sl_exp = (new UnknownVariable())->getId();
    return sl_exp;
  }

}
