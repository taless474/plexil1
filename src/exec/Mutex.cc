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

#include "Mutex.hh"

#include "Debug.hh"
#include "Node.hh" 
#include "PlanError.hh"

#include <algorithm> // std::find
#include <map>
#include <memory> // std::unique_ptr

namespace PLEXIL
{

  Mutex::Mutex(char const *name)
    : m_name(name),
      m_holder(nullptr)
  {
  }

  Mutex::~Mutex()
  {
  }

  void Mutex::acquire(Node const *acquirer)
  {
    assertTrue_2(acquirer, "Mutex::acquire: null argument!");

    assertTrueMsg(!m_holder,
                  "Mutex::acquire called with mutex " << m_name << " already held");

    m_holder = acquirer;
    debugMsg("Mutex:acquire",
             ' ' << m_name << " node " << acquirer->getNodeId() << " succeeded");
  }

  void Mutex::release()
  {
    assertTrue_2(m_holder, "Releasing mutex which was not held");
    if (m_holder) {
      debugMsg("Mutex:release",' ' << m_name << " by node " << m_holder->getNodeId());
      m_holder = nullptr;
    }
  }

  std::string const &Mutex::getName() const
  {
    return m_name;
  }

  Node const *Mutex::getHolder() const
  {
    return m_holder;
  }

  //
  // Global Mutex management
  //

  typedef std::map<std::string, std::unique_ptr<Mutex> > MutexMap;

  static MutexMap s_globalMutexes;

  Mutex *getGlobalMutex(char const *name)
  {
    assertTrue_2(name && *name,
                 "getGlobalMutex: null or empty name");
    MutexMap::const_iterator it = s_globalMutexes.find(name);
    if (it != s_globalMutexes.end())
      return it->second.get();
    else
      return nullptr;
  }
    
  static Mutex *createGlobalMutex(char const *name)
  {
    debugMsg("Mutex:ensureGlobalMutex", " constructing new mutex " << name);
    Mutex *result = new Mutex(name);
    s_globalMutexes.emplace(std::make_pair(std::string(name),
                                           std::unique_ptr<Mutex>(result)));
    return result;
  }

  Mutex *ensureGlobalMutex(char const *name)
  {
    Mutex *result = getGlobalMutex(name);
    if (result) {
      debugMsg("Mutex:ensureGlobalMutex", " returning existing mutex " << name);
      return result;
    }
    else
      return createGlobalMutex(name);
  }

}
