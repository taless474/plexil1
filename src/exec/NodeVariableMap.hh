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

#ifndef PLEXIL_NODE_VARIABLE_MAP_HH
#define PLEXIL_NODE_VARIABLE_MAP_HH

#include "SimpleMap.hh"

namespace PLEXIL
{
  // Forward references
  class Expression;

  class NodeVariableMap :
    public SimpleMap<char const *, Expression *, CStringComparator>
  {
  public:
    NodeVariableMap(NodeVariableMap *parentMap = NULL);
    virtual ~NodeVariableMap();

    void setParentMap(NodeVariableMap *parentMap);

    void optimizeParentMap();
    
    virtual void clear();

    Expression *findVariable(char const *name);

  protected:

    virtual iterator 
    insertEntry(iterator it, char const * const &k, Expression * const &v);

  private:
    // No copying allowed
    NodeVariableMap(NodeVariableMap const &);
    NodeVariableMap &operator=(NodeVariableMap const &);

    NodeVariableMap *m_parentMap;
  };

} // namespace PLEXIL

#endif // PLEXIL_NODE_VARIABLE_MAP_HH