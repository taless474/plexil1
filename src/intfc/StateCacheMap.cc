/* Copyright (c) 2006-2018, Universities Space Research Association (USRA).
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

#include "StateCacheMap.hh"

#include "State.hh"
#include "StateCacheEntry.hh"

#include <map>

namespace PLEXIL
{

  //! \class StateCacheMapImpl
  //! \brief Class implementing the StateCacheMap API.
  class StateCacheMapImpl : public StateCacheMap
  {
  private:

    //! \brief Shorthand for the type of the map data structure.
    typedef std::map<State, StateCacheEntry> EntryMap;

    //! \brief The map itself.
    EntryMap m_map;

    // Grant the base class singleton accessor a right to use the derived constructor.
    friend StateCacheMap &StateCacheMap::instance();
    
    //! \brief Default constructor.
    //! \note Should only be accessible to the base class singleton accessor.
    StateCacheMapImpl()
      : StateCacheMap()
    {
      // Initialize time state to 0
      ensureStateCacheEntry(State::timeState())->update((double) 0);
    }

    // Copy, assignment disallowed
    StateCacheMapImpl(StateCacheMapImpl const &);
    StateCacheMapImpl &operator=(StateCacheMapImpl const &);

  public:

    //! \brief Virtual destructor.
    virtual ~StateCacheMapImpl()
    {
    }

    virtual StateCacheEntry *ensureStateCacheEntry(State const &state)
    {
      EntryMap::iterator iter = m_map.find(state);
      if (iter == m_map.end())
        iter = m_map.insert(std::make_pair(state, StateCacheEntry())).first;
      return &(iter->second);
    }

    virtual StateCacheEntry *findStateCacheEntry(State const &state)
    {
      EntryMap::iterator iter = m_map.find(state);
      if (iter == m_map.end())
        return NULL;
      return &(iter->second);
    }

    virtual void removeStateCacheEntry(State const &state)
    {
      EntryMap::iterator iter = m_map.find(state);
      if (iter == m_map.end())
        return;
      m_map.erase(iter);
    }
  };

  StateCacheMap &StateCacheMap::instance()
  {
    static StateCacheMapImpl sl_instance;
    return static_cast<StateCacheMap &>(sl_instance);
  }

} // namespace PLEXIL
