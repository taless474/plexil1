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

#ifndef PLEXIL_SIMPLE_MAP_HH
#define PLEXIL_SIMPLE_MAP_HH

#include <algorithm>
#include <vector>
#include <utility>

namespace PLEXIL
{
  /**
   * \class SimpleKeyComparator
   * \brief A comparator class template, specialized by key type, for
   *        sorting and inserting keys in SimpleMap.  Any class used
   *        as a key comparator must implement this API.
   * \ingroup Utils
   */
  template <typename KEY_TYPE>
  struct SimpleKeyComparator
  {

    //! \brief Less-than comparison operator.
    //! \param a Const reference to a key.
    //! \param b Const reference to another key.
    //! \return true if a is strictly less than b in the key ordering, false otherwise.
    bool operator()(KEY_TYPE const &a, KEY_TYPE const &b) const
    {
      return a < b;
    }

    //! \brief Equality comparison operator.
    //! \param a Const reference to a key.
    //! \param b Const reference to another key.
    //! \return true if a and b are equal in the key ordering, false otherwise.
    bool equal(KEY_TYPE const &a, KEY_TYPE const &b) const
    {
      return a == b;
    }
  };

  /**
   * \class SimpleIndexComparator
   * \brief A comparator class, specialized on key and index types,
   *        for lookups in SimpleMap.  Any class used as an index comparator
   *        must implement this API.
   * \ingroup Utils
   */
  template <typename KEY_TYPE, typename INDEX_TYPE>
  struct SimpleIndexComparator
  {
    //! \brief Is the entry less than the index?
    //! \param Const reference to a key.
    //! \param Const reference to an index.
    //! \return True if the key is less than the index, false otherwise.
    bool operator()(KEY_TYPE const &a, INDEX_TYPE const &b) const
    {
      return a < b;
    }

    //! \brief Is the entry equal to the index?
    //! \param Const reference to a key.
    //! \param Const reference to an index.
    //! \return True if the key is equal to the index, false otherwise.
    bool equal(KEY_TYPE const &a, INDEX_TYPE const &b) const
    {
      return a == b;
    }
  };

  /**
   * \class SimpleMap
   * \brief An ordered key-value mapping.  Stores key-value pairs in a
   *        vector and retrieves them via binary search.
   * \ingroup Utils
   */
  template <typename KEY_TYPE,
            typename VALUE_TYPE,
            class KEY_COMP = SimpleKeyComparator<KEY_TYPE> >
  class SimpleMap
  {
  public:
    typedef std::pair<KEY_TYPE, VALUE_TYPE> MAP_ENTRY_TYPE;
    typedef std::vector<MAP_ENTRY_TYPE> MAP_STORE_TYPE;

    typedef typename MAP_STORE_TYPE::const_iterator const_iterator;
    typedef typename MAP_STORE_TYPE::iterator iterator;

    //! \brief Default constructor.
    SimpleMap()
    {
    }

    //! \brief Constructor specifying an initial capacity,
    //! \param initialCapacity The requested capacity.
    SimpleMap(size_t initialCapacity)
    {
      m_store.reserve(initialCapacity);
    }

    //! \brief Virtual destructor.
    virtual ~SimpleMap()
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

    //! \brief Insert the key-value pair in sorted order, as
    //!        directed by the key comparator.
    //!
    //! Searches for the given key.  If found, no insertion is
    //! performed, and the function returns false.  If the key is not
    //! found, the key-value pair is inserted in sorted order, and the
    //! function returns true.
    //!
    //! \param index Const reference to the key.
    //! \parm val Const reference to the value to associate with the key.
    //! \return true if the key was not previously inserted;
    //!         false if the key was found.
    bool insert(KEY_TYPE const &index, VALUE_TYPE const &val)
    {
      static EntryComparator s_comp;
      typename MAP_STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it != m_store.end() && s_comp.equal(*it, index))
        return false; // duplicate
      this->insertEntry(it, index, val);
      return true;
    }

    //! \brief Get a const reference to the pair with the given key,
    //!        if it exists.
    //! \param index Const reference to the key.
    //! \return Const reference to the key-value pair, if found;
    //!         end() otherwise.
    const_iterator find(KEY_TYPE const &index) const
    {
      static EntryComparator s_comp;
      typename MAP_STORE_TYPE::const_iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it == m_store.end() || s_comp.equal(*it, index))
        return it;
      else
        return m_store.end();
    }

    //! \brief Get a reference to the pair with the given key,
    //!        if it exists.
    //! \param index Const reference to the key.
    //! \return Reference to the key-value pair, if found;
    //!         end() otherwise.
    iterator find(KEY_TYPE const &index)
    {
      static EntryComparator s_comp;
      typename MAP_STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it == m_store.end() || s_comp.equal(*it, index))
        return it;
      else
        return m_store.end();
    }

    //! \brief Get a reference to the value at the given key.
    //! \param index The key.
    //! \return If the key was found, reference to the corresponding value;
    //!         otherwise, the key is inserted with a default-constructed value,
    //!         and the reference to the new value is returned.
    VALUE_TYPE &operator[](KEY_TYPE const &index)
    {
      static EntryComparator s_comp;
      typename MAP_STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it == m_store.end() || !s_comp.equal(*it, index))
        it = this->insertEntry(it, index, VALUE_TYPE());
      return it->second;
    }

    //! \brief Get a const reference to the value at the given key.
    //! \param index The key.
    //! \return If the key was found, a const reference to the
    //!         corresponding value; otherwise, a const reference to a
    //!         statically-allocated default-constructed value.
    VALUE_TYPE const &operator[](KEY_TYPE const &index) const
    {
      static EntryComparator s_comp;
      typename MAP_STORE_TYPE::const_iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it == m_store.end() || !s_comp.equal(*it, index)) {
        static VALUE_TYPE const sl_empty;
        return sl_empty;
      }
      return it->second;
    }

    //
    // Support for map lookups using non-key-type indices
    // Insertion is not possible with these template members
    //

    //! \brief Return the entry whose key compares equal to the index,
    //!        as determined by an index comparator.
    //! \param index The index.
    //! \return If an equal key is found, iterator pointing to the entry,
    //!         otherwise end().
    template <typename INDEX_TYPE, class INDEX_COMP>
    iterator find(INDEX_TYPE const &index)
    {
      static IndexComparator<INDEX_TYPE, INDEX_COMP> s_comp;
      typename MAP_STORE_TYPE::iterator it = 
        std::lower_bound(m_store.begin(), m_store.end(), index, s_comp);
      if (it == m_store.end() || s_comp.equal(*it, index))
        return it;
      else
        return m_store.end();
    }

    //! \brief Get a const iterator pointing to the first key-value
    //!        pair in the map.
    //! \return The const iterator.
    const_iterator begin() const
    {
      return m_store.begin();
    }

    //! \brief Get an iterator pointing to the first key-value
    //!        pair in the map.
    //! \return The iterator.
    iterator begin()
    {
      return m_store.begin();
    }

    //! \brief Get a const iterator pointing past the last key-value
    //!        pair in the map.
    //! \return The const iterator.
    const_iterator end() const
    {
      return m_store.end();
    }

    //! \brief Get an iterator pointing past the last key-value
    //!        pair in the map.
    //! \return The const iterator.
    iterator end()
    {
      return m_store.end();
    }

    //! \brief Erase the map.
    virtual void clear()
    {
      m_store.clear();
    }

    //! \brief Query whether the map is empty.
    //! \return true if empty, false if not.
    bool empty() const
    {
      return m_store.empty();
    }

    //! \brief Get the number of entries in the map.
    //! \return The entry count.
    size_t size() const
    {
      return m_store.size();
    }

    //! \brief Get the current capacity of the map's store, that is,
    //!        the maximum number of entries it can hold before
    //!        expansion is required.
    //! \return The capacity.
    size_t capacity() const
    {
      return m_store.capacity();
    }

  protected:

    //
    // Extension API provided for implementors of derived classes
    //

    //! \brief Insert a new map entry at the location pointed to by the iterator.
    //! \param it Iterator pointing to the location where the entry should be inserted.
    //! \param k Const reference to the key to insert.
    //! \param v Const reference to the value to insert.
    //! \return Iterator pointing to the new entry.
    virtual iterator insertEntry(iterator it, KEY_TYPE const &k, VALUE_TYPE const &v)
    {
      return m_store.insert(it, MAP_ENTRY_TYPE(k, v));
    }
    
    MAP_STORE_TYPE m_store; //!< Storage for the map data.

  private:

    // Not implemented
#if __cplusplus >= 201103L
    SimpleMap(SimpleMap const &) = delete;
    SimpleMap(SimpleMap &&) = delete;
    SimpleMap &operator=(SimpleMap const &) = delete;
    SimpleMap &operator=(SimpleMap &&) = delete;
#else
    SimpleMap(SimpleMap const &);
    SimpleMap &operator=(SimpleMap const &);
#endif

    // Wrap a used-defined comparator class for map entries

    struct EntryComparator
    {
      bool operator() (MAP_ENTRY_TYPE const &a, MAP_ENTRY_TYPE const &b)
      {
        return KEY_COMP()(a.first, b.first);
      }

      bool operator() (MAP_ENTRY_TYPE const &a, KEY_TYPE const &b)
      {
        return KEY_COMP()(a.first, b);
      }

      bool equal(MAP_ENTRY_TYPE const &a, KEY_TYPE const &b)
      {
        return KEY_COMP().equal(a.first, b);
      }
    };

    // Wrap a used-defined comparator class for comparing map entries to indices

    template <typename INDEX_TYPE, class INDEX_COMP>
    struct IndexComparator
    {
      bool operator() (MAP_ENTRY_TYPE const &a, INDEX_TYPE const &b)
      {
        return INDEX_COMP()(a.first, b);
      }

      // Only used in SimpleMap::find().
      bool equal(MAP_ENTRY_TYPE const &a, INDEX_TYPE const &b)
      {
        return INDEX_COMP().equal(a.first, b);
      }
    };
    
  };

} // namespace PLEXIL

#endif // PLEXIL_SIMPLE_MAP_HH
