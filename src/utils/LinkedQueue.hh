/* Copyright (c) 2006-2017, Universities Space Research Association (USRA).
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

#include <cstddef> // size_t

namespace PLEXIL
{
  //*
  // @class LinkedQueue
  // @brief Simple unidirectional linked-list queue implementation.
  //        Participant classes must only provide two member functions:
  //        T *next() const and T **nextPtr()
  //

  template <typename T>
  class LinkedQueue
  {
  protected: // for use by PriorityQueue
    T *m_head;
    T *m_tail;
    size_t m_count;

  private:
    LinkedQueue(LinkedQueue const &) = delete;
    LinkedQueue(LinkedQueue &&) = delete;
    LinkedQueue &operator=(LinkedQueue const &) = delete;
    LinkedQueue &operator=(LinkedQueue &&) = delete;

  public:
    LinkedQueue()
      : m_head(nullptr),
        m_tail(nullptr),
        m_count(0)
    {
    }
    
    virtual ~LinkedQueue()
    {
    }

    T *front() const
    {
      return m_head; // may be null
    }

    size_t size() const
    {
      return m_count;
    }

    bool empty() const
    {
      return (m_head == nullptr);
    }

    void pop()
    {
      if (empty())
        return;

      T *oldHead = m_head; // temp? see below
      if (m_head == m_tail) {
        // Exactly one item was in queue, is now empty
        m_head = m_tail = nullptr;
      }
      else {
        m_head = m_head->next();
        assertTrue_1(m_head); // temp?
      }

      // temp? Ensure queue item's next ptr is null when dequeuing
      *(oldHead->nextPtr()) = nullptr;

      --m_count; // better be 0 if empty!
    }

    void push(T *item)
    {
      assertTrue_1(item);
      assertTrue_1(!item->next()); // temp?

      *(item->nextPtr()) = nullptr; // mark as end of queue
      if (empty())
        m_head = item;
      else
        *(m_tail->nextPtr()) = item;
      m_tail = item;
      ++m_count;
    }

    void remove(T *item)
    {
      if (item == nullptr || empty())
        return;

      T *prev = nullptr; // last entry we looked at
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
        prevNextPtr = prev->nextPtr();
        cur = cur->next();
      }
      if (!cur)
        return; // not found

      *(item->nextPtr()) = nullptr; // temp?
      --m_count;
    }

    void clear()
    {
      m_head = m_tail = nullptr;
      m_count = 0;
    }

  };

  /**
   * @class PriorityQueue
   * @brief A variant of LinkedQueue that stores its entries in nondecreasing sorted order
   *        as determined by Compare.
   * @note Compare must implement a strict less-than comparison.
   * @note Callers should not use push() member function!!
   */

  template <typename T, typename Compare = std::less<T> >
  class PriorityQueue :
    public LinkedQueue<T>
  {
  public:
    PriorityQueue()
      : LinkedQueue<T>()
    {
    }

    ~PriorityQueue()
    {
    }

    // Inserts item after all entries less than or equal to item.
    void insert(T *item)
    {
      if (!this->m_head) {
        // Is empty - trivial case
        LinkedQueue<T>::push(item);
        return;
      }

      static Compare comp;

      T *prev = nullptr;               // last entry we looked at
      T **prevNextPtr = &this->m_head; // pointer to last entry's "next" pointer
      T *cur = *prevNextPtr;           // the item being compared

      // Find the first entry greater than item
      while (cur && !comp(*item, *cur)) {
        prev = cur;
        prevNextPtr = prev->nextPtr();
        cur = cur->next();
      }
      if (cur) {
        // Insert in front of cur
        *(item->nextPtr()) = cur;
        *(prevNextPtr) = item;
      }
      else {
        // Tack it onto the end
        *(item->nextPtr()) = nullptr;
        *(this->m_tail->nextPtr()) = item;
        this->m_tail = item;
      }
      ++this->m_count;
    }

  private:

    PriorityQueue(PriorityQueue const &) = delete;
    PriorityQueue(PriorityQueue &&) = delete;
    PriorityQueue &operator=(PriorityQueue const &) = delete;
    PriorityQueue &operator=(PriorityQueue &&) = delete;

    void push(T *); // callers should not use this base class member function
  };

}

#endif // LINKED_QUEUE_HH
