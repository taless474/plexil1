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

#ifndef _H_TestExternalInterface
#define _H_TestExternalInterface

#include "ExecDefs.hh"
#include "ExternalInterface.hh"
#include "ParserException.hh"

#include <iostream>
#include <map>
#include <set>

// Forward reference
namespace pugi
{
  class xml_node;
}

namespace PLEXIL 
{
  // Forward declaration
  class ParserException;

  class TestExternalInterface : public ExternalInterface 
  {
  public:
    TestExternalInterface();
    ~TestExternalInterface();

    void run(const pugi::xml_node& input)
    throw(ParserException);

    void lookupNow(State const &state, StateCacheEntry &cacheEntry);

    // LookupOnChange
    void subscribe(const State& state);
    void unsubscribe(const State& state);
    void setThresholds(const State& state, double hi, double lo);
    void setThresholds(const State& state, int32_t hi, int32_t lo);

    // Commands
    void executeCommand(Command *cmd);
    void invokeAbort(Command *cmd);

    // Updates
    void executeUpdate(Update * update);

    double currentTime();

  private:
    
    typedef std::map<State, Command *> StateCommandMap;
    typedef std::map<State, Value>        StateMap;

    void handleInitialState(const pugi::xml_node& input);
    void handleState(const pugi::xml_node& elt);
    void handleCommand(const pugi::xml_node& elt);
    void handleCommandAck(const pugi::xml_node& elt);
    void handleCommandAbort(const pugi::xml_node& elt);
    void handleUpdateAck(const pugi::xml_node& elt);
    void handleSendPlan(const pugi::xml_node& elt);
    void handleSimultaneous(const pugi::xml_node& elt);

    std::map<std::string, Update *> m_waitingUpdates;
    StateCommandMap m_executingCommands; //map from state to the command objects
    StateCommandMap m_commandAcks; //map from state to commands awaiting ack
    StateCommandMap m_abortingCommands; // map from state to commands expecting abort ack
    StateMap m_states; //uniquely identified states and their values
  };
}

#endif
