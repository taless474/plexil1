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

#ifndef PLEXIL_COMMAND_HH
#define PLEXIL_COMMAND_HH

#include "CommandHandleVariable.hh"
#include "State.hh"
#include "SimpleBooleanVariable.hh"
#include "Value.hh"

namespace PLEXIL
{
  // Forward reference
  class ExprVec;

  //
  // Used only in Command class, but exposed to parser
  //

  struct ResourceSpec
  {
    void activate();
    void deactivate();

    Expression *nameExp;
    Expression *priorityExp;
    Expression *lowerBoundExp;
    Expression *upperBoundExp;
    Expression *releaseAtTermExp;
  };

  typedef std::vector<ResourceSpec> ResourceList;

  /**
   * @brief A structure to represent actual resource values after fixing.
   *        Used by Command and ResourceArbiterInterface.
   */

  struct ResourceValue
  {
    double lowerBound;
    double upperBound;
    std::string name;
    int32_t priority;
    bool releaseAtTermination;
  };

  typedef std::vector<ResourceValue> ResourceValueList;

  class Command 
  {
    friend class CommandHandleVariable;

  public:

    Command(std::string const &nodeName);
    ~Command();

    Expression *getDest();
    Expression *getAck() {return &m_ack;}
    Expression *getAbortComplete() {return &m_abortComplete;}
    State const &getCommand() const;
    std::string const &getName() const;
    std::vector<Value> const &getArgValues() const;
    const ResourceValueList &getResourceValues() const;
    CommandHandleValue getCommandHandle() const {return m_commandHandle;}
    bool isActive() const { return m_active; }

    // Interface to plan parser
    void setDestination(Assignable *dest, bool isGarbage);
    void setNameExpr(Expression *nameExpr, bool isGarbage);
    void setArgumentVector(ExprVec *vec);
    ResourceList &getResourceList();
    void addGarbageExpression(Expression *exp);

    // Interface to CommandNode
    void activate();
    void deactivate();
    void reset();

    void execute();
    void abort();

    // Interface to ExternalInterface

    // Delegates to m_dest
    void returnValue(Value const &val);

    void setCommandHandle(CommandHandleValue handle);
    // Delegates to m_abortComplete
    void acknowledgeAbort(bool ack);

    // Public only for testing
    void fixValues();
    void fixResourceValues();

    void cleanUp();

  private:
    // Deliberately not implemented
    Command();
    Command(const Command&);
    Command& operator=(const Command&);

    CommandHandleVariable m_ack;
    SimpleBooleanVariable m_abortComplete;
    State m_command;
    std::vector<Expression *> m_garbage;
    ResourceList m_resourceList;
    ResourceValueList m_resourceValueList;
    Expression *m_nameExpr;
    Assignable *m_dest;
    ExprVec *m_argVec;
    CommandHandleValue m_commandHandle; // accessed by CommandHandleVariable
    bool m_fixed, m_resourceFixed, m_active, m_cleaned;
  };

}

#endif // PLEXIL_COMMAND_HH
