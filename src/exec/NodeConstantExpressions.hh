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

#ifndef PLEXIL_NODE_CONSTANT_EXPRESSIONS_HH
#define PLEXIL_NODE_CONSTANT_EXPRESSIONS_HH

#include "Constant.hh"

#include "CommandHandle.hh"
#include "ConstantMacros.hh"
#include "NodeConstants.hh"

namespace PLEXIL
{

  class NodeStateConstant : public Constant<uint16_t>
  {
  public:
    NodeStateConstant(NodeState value);
    ~NodeStateConstant();
    const ValueType valueType() const;
    char const *exprName() const;

  private:
    // Disallow default, copy, assign
    NodeStateConstant();
    NodeStateConstant(NodeStateConstant const &);
    NodeStateConstant &operator=(NodeStateConstant const &);
  };

  DECLARE_GLOBAL_CONST(NodeStateConstant, INACTIVE_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, WAITING_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, EXECUTING_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, ITERATION_ENDED_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, FINISHED_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, FAILING_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeStateConstant, FINISHING_CONSTANT);
  
  class NodeOutcomeConstant : public Constant<uint16_t>
  {
  public:
    NodeOutcomeConstant(NodeOutcome value);
    ~NodeOutcomeConstant();
    const ValueType valueType() const;
    char const *exprName() const;

  private:
    // Disallow default, copy, assign
    NodeOutcomeConstant();
    NodeOutcomeConstant(NodeOutcomeConstant const &);
    NodeOutcomeConstant &operator=(NodeOutcomeConstant const &);
  };

  DECLARE_GLOBAL_CONST(NodeOutcomeConstant, SUCCESS_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeOutcomeConstant, FAILURE_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeOutcomeConstant, SKIPPED_CONSTANT);
  DECLARE_GLOBAL_CONST(NodeOutcomeConstant, INTERRUPTED_CONSTANT);
  
  class FailureTypeConstant : public Constant<uint16_t>
  {
  public:
    FailureTypeConstant(FailureType value);
    ~FailureTypeConstant();
    const ValueType valueType() const;
    char const *exprName() const;

  private:
    // Disallow default, copy, assign
    FailureTypeConstant();
    FailureTypeConstant(FailureTypeConstant const &);
    FailureTypeConstant &operator=(FailureTypeConstant const &);
  };

  DECLARE_GLOBAL_CONST(FailureTypeConstant, PRE_CONDITION_FAILED_CONSTANT);
  DECLARE_GLOBAL_CONST(FailureTypeConstant, POST_CONDITION_FAILED_CONSTANT);
  DECLARE_GLOBAL_CONST(FailureTypeConstant, INVARIANT_CONDITION_FAILED_CONSTANT);
  DECLARE_GLOBAL_CONST(FailureTypeConstant, PARENT_FAILED_CONSTANT);
  DECLARE_GLOBAL_CONST(FailureTypeConstant, EXITED_CONSTANT);
  DECLARE_GLOBAL_CONST(FailureTypeConstant, PARENT_EXITED_CONSTANT);
  
  class CommandHandleConstant : public Constant<uint16_t>
  {
  public:
    CommandHandleConstant(CommandHandleValue value);
    ~CommandHandleConstant();
    const ValueType valueType() const;
    char const *exprName() const;

  private:
    // Disallow default, copy, assign
    CommandHandleConstant();
    CommandHandleConstant(CommandHandleConstant const &);
    CommandHandleConstant &operator=(CommandHandleConstant const &);
  };

  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_SENT_TO_SYSTEM_CONSTANT);
  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_ACCEPTED_CONSTANT);
  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_RCVD_BY_SYSTEM_CONSTANT);
  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_FAILED_CONSTANT);
  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_DENIED_CONSTANT);
  DECLARE_GLOBAL_CONST(CommandHandleConstant, COMMAND_SUCCESS_CONSTANT);

} // namespace PLEXIL

#endif // PLEXIL_NODE_CONSTANT_EXPRESSIONS_HH