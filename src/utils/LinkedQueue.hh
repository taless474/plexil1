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

#ifndef LINKED_QUEUE_HH
#define LINKED_QUEUE_HH

#include "Error.hh"

#include "plexil-stddef.h" // size_t, NULL

namespace PLEXIL
{

  //! @class LinkedQueue
  //! @brief Simple singly-linked queue template.

  //! LinkedQueue implements a classic singly-linked queue,
  //! with head and tail pointers.
  //! Classes which participate in the queue need only provide
  //! two member functions:
  //! - T *next() - return pointer to the next item in the queue
  //! - const T **nextPtr() - return pointer to the pointer to next 
  //! \see PriorityQueue
  //! \ingroup Utils
  template <typename T>
  class LinkedQueue
  {
  protected:

    //
    // Member variables
    // 

    T *m_head;      //!< Pointer to queue head. Accessible to derived classes.
    T *m_tail;      //!< Pointer to queue tail. Accessible to derived classes.
    size_t m_count; //!< The number of items in the queue.

  public:

    //! \brief Default constructor. Constructs an empty queue.
    LinkedQueue()
      : m_head(NULL),
        m_tail(NULL),
        m_count(0)
    {
    }
    
    //! \brief Virtual destructor.
    virtual ~LinkedQueue()
    {
    }

    //! \brief Get the first item in the queue.
    //! \return Pointer to the first item, if any; null if queue is empty.
    T *front() const
    {
      return m_head; // may be null
    }

    //! \brief Return the number of items in the queue.
    //! \return The number of items in the queue.
    //! \note Executes in O(1) time.
    size_t size() const
    {
      return m_count;
    }

    //! \brief Query whether the queue is empty.
    //! \return true if empty, false if non-empty.
    bool empty() const
    {
      return (m_head == NULL);
    }

    //! \brief Remove the first item from the queue. If empty, do nothing.
    //! \note Clears the 'next' pointer of the item removed.
    void pop()
    {
      if (empty())
        return;

      T *oldHead = m_head; // temp? see below
      if (m_head == m_tail) {
        // Exactly one item was in queue, is now empty
        m_head = m_tail = NULL;
      }
      else {
        m_head = m_head->next();
        assertTrue_1(m_head); // temp?
      }

      // temp? Ensure queue item's next ptr is null when dequeuing
      *(oldHead->nextPtr()) = NULL;

      --m_count; // better be 0 if empty!
    }

    //! \brief Insert an item on the tail end of the queue.
    //! \param item Pointer to the item to be inserted.
    //! \note Clears the 'next' pointer of the new item.
    void push(T *item)
    {
      assertTrue_1(item);
      assertTrue_1(!item->next()); // temp?

      *(item->nextPtr()) = NULL; // mark as end of queue
      if (empty())
        m_head = item;
      else
        *(m_tail->nextPtr()) = item;
      m_tail = item;
      ++m_count;
    }

    //! \brief Remove the given item from the queue, if it exists.
    //! \param item Pointer to the item to be removed.
    //! \note If the item is found, clears its 'next' pointer.
    //! \note If the item is not found, no side effects are performed.
    void remove(T *item)
    {
      if (item == NULL || empty())
        return;

      T *prev = NULL; // last entry we looked at
      T **prevNextPtr = &m_head; // pointer to last entry's "next" pointer
      T *cur = m_head; // the item being compared
      while (cur) {
        if (item == cur) {
          *(prevNextPtr) = cur->next();
          if (cur == m_tail)
            m_tail = prev;
          break;
        }
        prev = cur;
        prevNextPtr = cur->nextPtr();
        cur = cur->next();
      }
      if (!cur)
        return; // not found

      *(item->nextPtr()) = NULL; // no dangling pointers!
      --m_count;
    }
 
    //! \brief Find an item satisfying a predicate.
    //! \param pred A predicate functor. Must implement an operator() method with the signature:
    //!             bool operator()(T* item)
    //! \return Pointer to the first item satisfying the predicate; null if not found.
    template <typename Predicate>
    T* find_if(Predicate const &pred)
    {
      T *cur = m_head; // the item being compared
      while (cur) {
        if (pred(cur))
          // Found it
          return cur;
        // Step to next item if any
        cur = cur->next();
      }
      // not found
      return NULL;
    }
 
    //! \brief Remove the first item satisfying the predicate.
    //! \param pred A predicate functor. Must implement an operator() method with the signature:
    //!             bool operator()(T* item)
    //! \return The removed queue item, or null if not found.
    template <typename Predicate>
    T* remove_if(Predicate const &pred)
    {
      T *result = NULL;

      T *prev = NULL; // last entry we looked at
      T **prevNextPtr = &m_head; // pointer to last entry's "next" pointer
      T *cur = m_head;

      while (cur) {
        if (pred(cur)) {
          // Found one, splice it out
          result = cur;
          *(prevNextPtr) = cur->next();
          if (cur == m_tail)
            m_tail = prev;
          break;
        }
        // Step to next item if any
        prev = cur;
        prevNextPtr = prev->nextPtr();
        cur = cur->next();
      }

      if (result) {
        *(result->nextPtr()) = NULL; // no dangling pointers!
        --m_count;
      }
      return result;
    }

    //! \brief Make this queue empty.
    //! \note Any items on the queue at entry are not modified.
    void clear()
    {
      m_head = m_tail = NULL;
      m_count = 0;
    }

  private:
    
    //
    // Unimplented constructors and assignment operators
    //

#if __cplusplus >= 201103L
    LinkedQueue(LinkedQueue const &) = delete;
    LinkedQueue(LinkedQueue &&) = delete;
    LinkedQueue &operator=(LinkedQueue const &) = delete;
    LinkedQueue &operator=(LinkedQueue &&) = delete;
#else
    LinkedQueue(LinkedQueue const &);
    LinkedQueue &operator=(LinkedQueue const &);
#endif

  };

  //! \class PriorityQueue
  //! \brief A variant of LinkedQueue which stores its entries in nondecreasing sorted order
  //!        as determined by the Compare class.
  //! \note Compare must implement a strict less-than comparison.
  //! \note Callers should not use push() member function!!
  //! \ingroup Utils
  template <typename T, typename Compare = std::less<T> >
  class PriorityQueue :
    public LinkedQueue<T>
  {
  public:
    
    //! \brief Default constructor. Constructs an empy queue.
    PriorityQueue()
      : LinkedQueue<T>()
    {
    }

    //! \brief Virtual destructor.
    virtual ~PriorityQueue()
    {
    }

    //! \brief Insert item in sorted order, after all entries less than or equal to item.
    //! \param item Pointer to the item to insert.
    void insert(T *item)
    {
      if (!this->m_head) {
        // Is empty - trivial case
        LinkedQueue<T>::push(item);
        return;
      }

      static Compare comp;

      T **prevNextPtr = &this->m_head; // pointer to last entry's "next" pointer
      T *cur = *prevNextPtr;           // the item being compared

      // Find the first entry greater than item
      while (cur && !comp(*item, *cur)) {
        prevNextPtr = cur->nextPtr();
        cur = cur->next();
      }
      if (cur) {
        // Insert in front of cur
        *(item->nextPtr()) = cur;
        *(prevNextPtr) = item;
      }
      else {
        // Tack it onto the end
        *(item->nextPtr()) = NULL;
        *(this->m_tail->nextPtr()) = item;
        this->m_tail = item;
      }
      ++this->m_count;
    }

  private:
    
    //
    // Unimplented constructors and assignment operators
    //

#if __cplusplus >= 201103L
    PriorityQueue(PriorityQueue const &) = delete;
    PriorityQueue(PriorityQueue &&) = delete;
    PriorityQueue &operator=(PriorityQueue const &) = delete;
    PriorityQueue &operator=(PriorityQueue &&) = delete;
#else
    PriorityQueue(PriorityQueue const &);
    PriorityQueue &operator=(PriorityQueue const &);
#endif

    void push(T *); // Callers must not use this base class member function

  };

}

#endif // LINKED_QUEUE_HH
