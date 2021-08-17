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

#ifndef PLEXIL_SIMPLE_SET_HH
#define PLEXIL_SIMPLE_SET_HH

#include <algorithm>  // std::lower_bound, std::binary_search
#include <functional> // std::less
#include <vector>

namespace PLEXIL
{
  /**
   * \class SimpleSet
   * \brief A set stored in a vector, sorted by value.  Membership is
   *        tested using binary search.
   * \ingroup Utils
   */
  template <typename VALUE_TYPE,
            class COMP = std::less<VALUE_TYPE> >
  class SimpleSet
  {
  public:
    typedef std::vector<VALUE_TYPE> STORE_TYPE;

    typedef typename STORE_TYPE::const_iterator const_iterator;
    typedef typename STORE_TYPE::iterator iterator;

    //! \brief Default constructor.
    SimpleSet()
    {
    }

    //! \brief Constructor specifying an initial capacity.
    //! \param initialCapacity The requested capacity.
    SimpleSet(size_t initialCapacity)
    {
      m_store.reserve(initialCapacity);
    }

    //! \brief Virtual destructor.
    virtual ~SimpleSet()
    {
    }

    /*
     * \brief Reserve space for n additional entries.
     * \param n The number of additional entries.
     */
    void grow(size_t n)
    {
      size_t desired = m_store.size() + n;
      m_store.reserve(desired);
    }

    //! \brief Insert the given value into the set, if it is not already a member.
    //! \param val Const reference to the value to insert.
    //! \return false if the value was already a member of the set;
    //!         true otherwise.
    bool insert(VALUE_TYPE const &val)
    {
      static Comparator s_comp;
      typename STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), val, s_comp);
      if (it != m_store.end() && s_comp.equal(*it, val))
        return false; // duplicate
      this->insertEntry(it, val);
      return true;
    }

    // TODO: emplace() method

    //! \brief Get a const iterator pointing to the given value, if it
    //!        is in the set.
    //! \param val Const reference to the desired value.
    //! \return If the value is in the set, a const iterator pointing
    //!         to the value; otherwise end().
    const_iterator find(VALUE_TYPE const &val) const
    {
      static Comparator s_comp;
      typename STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), val, s_comp);
      if (it == m_store.end() || s_comp.equal(*it, val))
        return it;
      else
        return m_store.end();
    }

    //! \brief Get an iterator pointing to the given value, if it
    //!        is in the set.
    //! \param val Const reference to the desired value.
    //! \return If the value is in the set, an iterator pointing
    //!         to the value; otherwise end().
    iterator find(VALUE_TYPE const &val)
    {
      static Comparator s_comp;
      typename STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), val, s_comp);
      if (it == m_store.end() || s_comp.equal(*it, val))
        return it;
      else
        return m_store.end();
    }

    //! \brief Does the set contain this value?
    //! \param val Const reference to the desired value.
    //! \return true if the value is a member of the set, false otherwise.
    bool contains(VALUE_TYPE const &val) const
    {
      static Comparator s_comp;
      return std::binary_search(m_store.begin(), m_store.end(), val, s_comp);
    }

    //! \brief Erase the set member pointed to by an iterator.
    //! \param pos The iterator.
    //! \return Iterator to the next member of the set; end if the
    //!         removed member was the last in the set.
    iterator erase(iterator pos)
    {
      return m_store.erase(pos);
    }

    //! \brief Erase the set member pointed to by a const iterator.
    //! \param pos The const iterator.
    //! \return Iterator to the next member of the set; end if the
    //!         removed member was the last in the set.
    iterator erase(const_iterator pos)
    {
      return m_store.erase(pos);
    }

    //! \brief If the set contains the given value, erase it.
    //! \param val Const reference to the value to erase.
    //! \return 1 if the value was in the set, 0 otherwise.
    typename STORE_TYPE::size_type erase(VALUE_TYPE const &val)
    {
      typename STORE_TYPE::iterator it = find(val);
      if (it == m_store.end())
        return 0;
      erase(it);
      return 1;
    }
    
    //! \brief Return a const iterator pointing to the first item in the set.
    //! \return The const iterator; end() if the set is empty.
    const_iterator begin() const
    {
      return m_store.begin();
    }

    //! \brief Return an iterator pointing to the first item in the set.
    //! \return The iterator; end() if the set is empty.
    iterator begin()
    {
      return m_store.begin();
    }

    //! \brief Return a const iterator pointing past the last item in the set.
    //! \return The const iterator.
    const_iterator end() const
    {
      return m_store.end();
    }

    //! \brief Return am iterator pointing past the last item in the set.
    //! \return The iterator.
    iterator end()
    {
      return m_store.end();
    }

    //! \brief Erase every member of the set.
    virtual void clear()
    {
      m_store.clear();
    }

    //! \brief Is the set empty?
    //! \return true if the set is empty, false if not.
    bool empty() const
    {
      return m_store.empty();
    }

    //! \brief Get the count of members of the set.
    //! \return The size of the set.
    size_t size() const
    {
      return m_store.size();
    }

  protected:

    //
    // Extension API provided for implementors of derived classes
    //

    //! \brief Insert a value into the set at the given location.
    //! \param it Iterator pointing to the insertion point.
    //! \param v Const reference to the value being inserted.
    //! \return Iterator pointing to the new member.
    virtual iterator insertEntry(iterator it, VALUE_TYPE const &v)
    {
      return m_store.insert(it, v);
    }
    
    STORE_TYPE m_store; //!< The data store for the set.

  private:

    // Not implemented
#if __cplusplus >= 201103L
    SimpleSet(SimpleSet const &) = delete;
    SimpleSet(SimpleSet &&) = delete;
    SimpleSet &operator=(SimpleSet const &) = delete;
    SimpleSet &operator=(SimpleSet &&) = delete;
#else
    SimpleSet(SimpleSet const &);
    SimpleSet &operator=(SimpleSet const &);
#endif

    //! \brief Wrapper class for the comparison operator.
    struct Comparator
    {
      //! \brief Is the first value less than the second value?
      //! \param a Const reference to a value to compare.
      //! \param b Const reference to the other value.
      //! \return true if a is strictly less than b, as determined
      //!         by the comparator; false otherwise.
      bool operator() (VALUE_TYPE const &a, VALUE_TYPE const &b)
      {
        return COMP()(a, b);
      }

      //! \brief Is the first value equal to the second value?
      //! \param a Const reference to a value to compare.
      //! \param b Const reference to the other value.
      //! \return true if neither a is less than b, nor b is less than
      //!         a, as determined by the comparator; false otherwise.
      bool equal(VALUE_TYPE const &a, VALUE_TYPE const &b)
      {
        return !COMP()(a, b) && !COMP()(b, a);
      }
    };
    
  };

} // namespace PLEXIL

#endif // PLEXIL_SIMPLE_SET_HH
