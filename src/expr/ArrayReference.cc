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

#include "ArrayReference.hh"
#include "ArrayImpl.hh"
#include "UserVariable.hh"

namespace PLEXIL
{
  template <typename T>
  ArrayReference<T>::ArrayReference(const ExpressionId &ary,
                                    const ExpressionId &idx,
                                    bool aryIsGarbage,
                                    bool idxIsGarbage)
    : NotifierImpl(),
      ExpressionImpl<T>(),
      m_array(ary),
      m_index(idx),
      m_arrayIsGarbage(aryIsGarbage),
      m_indexIsGarbage(idxIsGarbage)
  {
    assertTrue_2(ary.isId() && idx.isId(),
                 "ArrayReference constructor: Null subexpression");
    // TODO:
    // Check type of array, index
    m_array->addListener(ExpressionListener::getId());
    m_index->addListener(ExpressionListener::getId());
  }

  template <typename T>
  ArrayReference<T>::~ArrayReference()
  {
    m_array->removeListener(ExpressionListener::getId());
    m_index->removeListener(ExpressionListener::getId());
    if (m_arrayIsGarbage)
      delete (Expression *) m_array;
    if (m_indexIsGarbage)
      delete (Expression *) m_index;
  }

  template <typename T>
  const char *ArrayReference<T>::exprName() const
  {
    return "ArrayReference";
  }

  template <typename T>
  bool ArrayReference<T>::isKnown() const
  {
    ArrayImpl<T> const *dummyAry;
    size_t dummyIdx;
    return selfCheck(dummyAry, dummyIdx);
  }

  template <typename T>
  bool ArrayReference<T>::selfCheck(ArrayImpl<T> const *&valuePtr,
                                    size_t &idx) const
  {
    if (!(isActive() && m_array->isActive() && m_index->isActive()))
      return false;
    int32_t idxTemp;
    if (!m_index->getValue(idxTemp))
      return false; // index is unknown
    if (idxTemp < 0) {
      assertTrue_2(ALWAYS_FAIL, "ArrayReference: Array index is negative");
      return false;
    }
    idx = (size_t) idxTemp;
    if (!m_array->getValuePointer(valuePtr))
      return false; // array unknown or invalid
    if (idx >= valuePtr->size()) {
      assertTrue_2(ALWAYS_FAIL, "ArrayReference: Array index exceeds array size");
      return false;
    }
    return valuePtr->getKnownVector()[idx];
  }

  template <typename T>
  bool ArrayReference<T>::getValueImpl(T &result) const
  {
    ArrayImpl<T> const *ary;
    size_t idx;
    if (!selfCheck(ary, idx))
      return false;
    std::vector<T> const *resultTemp;
    ary->getContentsVector(resultTemp);
    result = (*resultTemp)[idx];
    return true;
  }

  template <typename T>
  bool ArrayReference<T>::getValuePointerImpl(T const *&ptr) const
  {
    ArrayImpl<T> const *ary;
    size_t idx;
    if (!selfCheck(ary, idx))
      return false;
    std::vector<T> const *resultTemp;
    ary->getContentsVector(resultTemp);
    ptr = &((*resultTemp)[idx]);
    return true;
  }

  // Not implemented for Boolean arrays
  template <>
  bool ArrayReference<bool>::getValuePointerImpl(bool const *&ptr) const
  {
    check_error_2(ALWAYS_FAIL, "ArrayReference::getValuePointerImpl not implemented for BooleanArray");
    return false;
  }
  
  template <typename T>
  void ArrayReference<T>::handleActivate()
  {
    m_array->activate();
    m_index->activate();
  }
  
  template <typename T>
  void ArrayReference<T>::handleDeactivate()
  {
    m_array->deactivate();
    m_index->deactivate();
  }

  template <typename T>
  MutableArrayReference<T>::MutableArrayReference(const ExpressionId &ary,
                                                  const ExpressionId &idx,
                                                  bool aryIsGarbage,
                                                  bool idxIsGarbage)
    : ArrayReference<T>(ary, idx, aryIsGarbage, idxIsGarbage),
      Assignable()
  {
    assertTrue_2((m_mutableArray = ary->getAssignableId()).isId(),
                 "MutableArrayReference: Not a writable array");
  }

  template <typename T>
  MutableArrayReference<T>::~MutableArrayReference()
  {
  }

  template <typename T>
  bool MutableArrayReference<T>::mutableSelfCheck(ArrayImpl<T> *&valuePtr,
                                                  size_t &idx)
  {
    if (!(isActive()
          && ArrayReference<T>::m_array->isActive()
          && ArrayReference<T>::m_index->isActive()))
      return false;
    int32_t idxTemp;
    if (!ArrayReference<T>::m_index->getValue(idxTemp))
      return false; // index is unknown
    if (idxTemp < 0) {
      assertTrue_2(ALWAYS_FAIL, "ArrayReference: Array index is negative");
      return false;
    }
    idx = (size_t) idxTemp;
    if (!m_mutableArray->getMutableValuePointer(valuePtr))
      return false; // array unknown
    if (idx >= valuePtr->size()) {
      assertTrue_2(ALWAYS_FAIL, "ArrayReference: Array index exceeds array size");
      return false;
    }
    return true;
  }

  template <typename T>
  void MutableArrayReference<T>::setValue(const T &value)
  {
    ArrayImpl<T> *ary;
    size_t idx;
    if (!mutableSelfCheck(ary, idx))
      return;
    bool changed = (!ary->m_known[idx] || (value != ary->m_contents[idx]));
    ary->m_contents[idx] = value;
    ary->m_known[idx] = true;
    if (changed) {
      NotifierImpl::publishChange(getId());
      m_mutableArray->getBaseVariable()->notifyChanged(getId()); // array might be alias
    }
  }

  template <typename T>
  void MutableArrayReference<T>::setValue(ExpressionId const &valex)
  {
    ArrayImpl<T> *ary;
    size_t idx;
    if (!mutableSelfCheck(ary, idx))
      return;
    bool changed = false;
    T newVal;
    if (valex->getValue(newVal)) {
      changed = !ary->m_known[idx] || newVal != ary->m_contents[idx];
      ary->m_contents[idx] = newVal;
      ary->m_known[idx] = true;
    }
    else { // value unknown
      changed = ary->m_known[idx];
      ary->m_known[idx] = false;
    }
    if (changed) {
      NotifierImpl::publishChange(getId());
      m_mutableArray->getBaseVariable()->notifyChanged(getId()); // array might be alias
    }
  }

  template <typename T>
  void MutableArrayReference<T>::setUnknown()
  {
    ArrayImpl<T> *ary;
    size_t idx;
    if (!mutableSelfCheck(ary, idx))
      return;
    bool changed = ary->m_known[idx];
    ary->m_known[idx] = false;
    if (changed) {
      NotifierImpl::publishChange(getId());
      m_mutableArray->getBaseVariable()->notifyChanged(getId()); // array might be alias
    }
  }

  template <typename T>
  void MutableArrayReference<T>::reset()
  {
    // No-op
  }

  template <typename T>
  void MutableArrayReference<T>::saveCurrentValue()
  {
    ArrayImpl<T> *ary;
    size_t idx;
    if (!mutableSelfCheck(ary, idx))
      return; // unknown or invalid
    if ((m_savedKnown = ary->m_known[idx]))
      m_savedValue = ary->m_contents[idx];
  }

  template <typename T>
  void MutableArrayReference<T>::restoreSavedValue()
  {
    ArrayImpl<T> *ary;
    size_t idx;
    if (!mutableSelfCheck(ary, idx))
      return;
    bool changed = (m_savedKnown != ary->m_known[idx])
      || (m_savedKnown && (m_savedValue != ary->m_contents[idx]));
    ary->m_known[idx] = m_savedKnown;
    if (m_savedKnown)
      ary->m_contents[idx] = m_savedValue;
    if (changed) {
      NotifierImpl::publishChange(getId());
      m_mutableArray->notifyChanged(getId());
    }
  }

  template <typename T>
  const std::string &MutableArrayReference<T>::getName() const
  {
    AssignableId base = getBaseVariable();
    if (base.isId()) {
      // TODO: add array subscript 
      return base->getName();
    }
    static const std::string sl_dummy;
    return sl_dummy;
  }

  template <typename T>
  const NodeId &MutableArrayReference<T>::getNode() const
  {
    AssignableId base = getBaseVariable();
    if (base.isNoId())
      return NodeId::noId();
    else
      return base->getNode();
  }

  template <typename T>
  const AssignableId &MutableArrayReference<T>::getBaseVariable() const
  {
    return m_mutableArray->getBaseVariable(); // could be an alias
  }

  //
  // Explicit instantiation
  //

  template class ArrayReference<bool>;
  template class ArrayReference<int32_t>;
  template class ArrayReference<double>;
  template class ArrayReference<std::string>;

  template class MutableArrayReference<bool>;
  template class MutableArrayReference<int32_t>;
  template class MutableArrayReference<double>;
  template class MutableArrayReference<std::string>;


} // namespace PLEXIL
