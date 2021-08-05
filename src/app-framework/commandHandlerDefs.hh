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

#ifndef ABSTRACT_COMMAND_HANDLER_HH
#define ABSTRACT_COMMAND_HANDLER_HH

//
// Command handler typedefs and classes
//

namespace PLEXIL
{

  // Forward references
  class Command;
  class AdapterExecInterface;

  //
  // Type aliases for functions which perform Commands in PLEXIL
  //
  // Interface implementors can choose to implement Commands either as
  // ordinary C++ functions, or as instances of classes derived from
  // CommandHandler.
  //
  // A single command handler, whether a function or a CommandHandler
  // instance, may handle multiple command names.
  //
  
  //! \typedef ExecuteCommandHandler
  //! \brief Prototype for a function which implements a PLEXIL Command.

  //! An ExecuteCommandHandler function executes or initiates
  //! the requested command, and reports command status
  //! (as a CommandHandleValue) and return values (if any) to the
  //! AdapterExecInterface.

  //! \ingroup App-Framework
  //! \see AdapterExecInterface::handleCommandAck
  //! \see AdapterExecInterface::handleCommandReturn
  //! \see CommandHandleValue

  typedef void (*ExecuteCommandHandler)(Command *, AdapterExecInterface *);
  
  //! \typedef AbortCommandHandler
  //! \brief Prototype for a function called to abort a PLEXIL Command.

  //! An AbortCommandHandler function aborts the command which
  //! is (or should be) already in execution, and reports whether the
  //! abort was successful to the AdapterExecInterface.

  //! \ingroup App-Framework
  //! \see AdapterExecInterface::handleCommandAbortAck

  typedef void (*AbortCommandHandler)(Command *, AdapterExecInterface *);

  //! \brief A function which reports a successful command abort to the AdapterExecInterface.
  //! \param cmd Pointer to the command being aborted.
  //! \param intf Pointer to the AdapterExecInterface.
  //! \note Calls AdapterExecInterface::handleCommandAbortAck.
  //! \see AdapterExecInterface::handleCommandAbortAck
  //! \ingroup App-Framework

  void defaultAbortCommandHandler(Command *cmd, AdapterExecInterface *intf);

  //! \class CommandHandler
  //! \brief An abstract base class specifying the API of a command handler object.
  //! \note Implementors of derived classes must provide both execute and abort methods.
  //! \see AdapterExecInterface::handleCommandAck
  //! \see AdapterExecInterface::handleCommandReturn
  //! \see AdapterExecInterface::handleCommandAbortAck
  //! \see defaultAbortCommandHandler
  //! \ingroup App-Framework

  class CommandHandler
  {
  public:

    //! \brief Virtual destructor
    virtual ~CommandHandler()
    {
    }

    //! \brief Execute the requested command, and report command
    //! status (CommandHandleValue) and return values (if any) to the
    //! AdapterExecInterface.
    //! \param cmd Pointer to the command to be performed.
    //! \param intf Pointer to the AdapterExecInterface.
    //! \note User-defined methods MUST call AdapterExecInterface::handleCommandAck; they may also call AdapterExecInterface::handleCommandReturn.
    //! \see AdapterExecInterface::handleCommandAck
    //! \see AdapterExecInterface::handleCommandReturn
    //! \see CommandHandleValue

    virtual void executeCommand(Command *cmd, AdapterExecInterface *intf) = 0;

    //! \brief Abort the command in execution, and report whether the
    //! abort was successful to the AdapterExecInterface.
    //! \param cmd Pointer to the command to be aborted.
    //! \param intf Pointer to the AdapterExecInterface.
    //! \note User-defined methods MUST call AdapterExecInterface::handleCommandAbortAck.
    //! \see AdapterExecInterface::handleCommandAbortAck
    //! \see defaultAbortCommandHandler
    
    virtual void abortCommand(Command *cmd, AdapterExecInterface *intf) = 0;
  };
  
}

#endif // ABSTRACT_COMMAND_HANDLER_HH
