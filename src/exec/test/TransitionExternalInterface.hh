/* Copyright (c) 2006-2020, Universities Space Research Association (USRA).
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

#ifndef PLEXIL_TRANSITION_EXTERNAL_INTERFACE_HH
#define PLEXIL_TRANSITION_EXTERNAL_INTERFACE_HH

#include "ExternalInterface.hh"

namespace PLEXIL
{

class TransitionExternalInterface final : public ExternalInterface
{
public:
  TransitionExternalInterface()
    : ExternalInterface()
  {
  }

  virtual ~TransitionExternalInterface()
  {
  }

  virtual void lookupNow(State const & /* state */, LookupReceiver * /* rcvr */) {}
  virtual void setThresholds(State const & /* state */, double /* hi */, double /* lo */) {}
  virtual void setThresholds(State const & /* state */, int32_t /* hi */, int32_t /* lo */) {}
  virtual void clearThresholds(State const & /* state */) {}
  virtual void enqueueCommand(CommandImpl *cmd) {}
  virtual void abortCommand(CommandImpl *cmd) {}
  virtual void enqueueUpdate(Update *update) {}
  virtual void executeOutboundQueue() {}

protected:
  virtual void executeCommand(Command * /* cmd */) {}
  virtual void reportCommandArbitrationFailure(Command * /* cmd */) {}
  virtual void invokeAbort(Command * /* cmd */) {}
  virtual void executeUpdate(Update * /* update */) {}

};

} // namespace PLEXIL

#endif // PLEXIL_TRANSITION_EXTERNAL_INTERFACE_HH
