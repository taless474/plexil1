// Copyright (c) 2006-2021, Universities Space Research Association (USRA).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Universities Space Research Association nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef PLEXIL_COMMAND_HH
#define PLEXIL_COMMAND_HH

#include "CommandFunction.hh"
#include "CommandHandleVariable.hh"
#include "State.hh"
#include "SimpleBooleanVariable.hh"
#include "Value.hh"

namespace PLEXIL
{
  // Forward reference
  class ExprVec;

  //! \class ResourceSpec
  //! \brief Represents a command resource specification.
  //! \note Used only in Command class, but exposed to parser
  //! \ingroup External-Interface
  class ResourceSpec
  {
  public:

    //! \brief Default constructor.
    ResourceSpec();

    // Use of ResourceSpec in vector requires these
    // Replaced by move constructor/move assignment in C++11 and later

    //! \brief Copy constructor.
    //! \param orig Const reference to the instance to be copied.
    ResourceSpec(ResourceSpec const &);

    //! \brief Copy assignment.
    //! \param orig Const reference to the instance to be copied.
    ResourceSpec &operator=(ResourceSpec const &);

    //! \brief Destructor.
    ~ResourceSpec();

    //! \brief Delete references to all expressions.  Delete the expressions if requested.
    //! \note This must be accessible outside the destructor because all expressions
    //!       owned by a Node must be unlinked before they are deleted.
    void cleanUp();

    //! \brief Make the expressions referenced by this object active.
    void activate();

    //! \brief Make the expressions referenced by this object inactive.
    void deactivate();

    //
    // Plan reader access
    //

    //! \brief Set the name expression.
    //! \param e Pointer to the expression.
    //! \param isGarbage true if the expression is to be deleted with the object, false if not.
    void setNameExpression(Expression *e, bool isGarbage);

    //! \brief Set the priority expression.
    //! \param e Pointer to the expression.
    //! \param isGarbage true if the expression is to be deleted with the object, false if not.
    void setPriorityExpression(Expression *e, bool isGarbage);

    //! \brief Set the lower bound expression.
    //! \param e Pointer to the expression.
    //! \param isGarbage true if the expression is to be deleted with the object, false if not.
    void setLowerBoundExpression(Expression *e, bool isGarbage);

    //! \brief Set the upper bound expression.
    //! \param e Pointer to the expression.
    //! \param isGarbage true if the expression is to be deleted with the object, false if not.
    void setUpperBoundExpression(Expression *e, bool isGarbage);

    //! \brief Set the release-at-termination expression.
    //! \param e Pointer to the expression.
    //! \param isGarbage true if the expression is to be deleted with the object, false if not.
    void setReleaseAtTerminationExpression(Expression *e, bool isGarbage);

    Expression *nameExp;            //!< Pointer to resource name expression.
    Expression *priorityExp;        //!< Pointer to priority expression.
    Expression *lowerBoundExp;      //!< Pointer to lower bound expression.  May be NULL.
    Expression *upperBoundExp;      //!< Pointer to upper bound expression.  May be NULL.
    Expression *releaseAtTermExp;   //!< Pointer to release-at-termination expression.  May be NULL.

  private:
    bool nameIsGarbage;             //!< True if name expression should be deleted with object.
    bool priorityIsGarbage;         //!< True if priority expression should be deleted with object.
    bool lowerBoundIsGarbage;       //!< True if lower bound expression should be deleted with object.
    bool upperBoundIsGarbage;       //!< True if upper bound expression should be deleted with object.
    bool releaseIsGarbage;          //!< True if release-at-termination expression should be deleted with object.
  };

  //! \brief Shorthand for the ResourceSpec container type.
  //! \ingroup External-Interface
  typedef std::vector<ResourceSpec> ResourceList;

  //! \struct ResourceValue
  //! \brief Container for actual resource values after fixing.
  //!        Used by Command and ResourceArbiterInterface.
  //! \ingroup External-Interface
  struct ResourceValue
  {
    std::string name;
    double lowerBound;
    double upperBound;
    int32_t priority;
    bool releaseAtTermination;
  };

  //! \brief Shorthand for the ResourceValue container type.
  //! \ingroup External-Interface
  typedef std::vector<ResourceValue> ResourceValueList;

  //! \class Command
  //! \brief Internal representation of a PLEXIL Command.
  //! \ingroup External-Interface
  class Command 
  {
    friend class CommandHandleVariable;

  public:

    //! \brief Constructor with node name.
    //! \param nodeName Const reference to the ID of the owning Node.
    Command(std::string const &nodeName);

    //! \brief Destructor.
    ~Command();

    //! \brief Get the destination variable as an expression.
    //! \return Pointer to the expression. May be NULL.
    Expression *getDest();

    //! \brief Get the ackowledgement (command handle) variable as an expression.
    //! \return Pointer to the CommandHandle variable.  Will never be NULL.
    Expression *getAck() {return &m_ack;}

    //! \brief Get the abort-complete variable as an expression.
    //! \return Pointer to the Boolean variable.  Will never be NULL.
    Expression *getAbortComplete() {return &m_abortComplete;}

    //! \brief Get the command name and parameters after fixing.
    //! \return Const reference to the State holding the values.
    State const &getCommand() const;
    
    //! \brief Get the command name after fixing.
    //! \return Const reference to the name string.
    std::string const &getName() const;

    //! \brief Get the command parameter values after fixing.
    //! \return Const reference to the parameter value vector.
    std::vector<Value> const &getArgValues() const;

    //! \brief Get the resource values after fixing.
    //! \return Const reference to the resource value vector.
    const ResourceValueList &getResourceValues() const;

    //! \brief Get the current value of the CommandHandle variable.
    //! \return The value.
    CommandHandleValue getCommandHandle() const {return (CommandHandleValue) m_commandHandle;}

    //! \brief Get the command-handle-known expression.
    //! \return Pointer to the expression.
    //! \note Used by the owning CommandNode as its action-complete condition.
    Expression *getCommandHandleKnownFn() { return &m_handleKnownFn; }

    //! \brief Is the Command active?
    //! \return true if active, false if not.
    bool isActive() const { return m_active; }

    //
    // Interface to plan parser
    //

    //! \brief Set the destination (assignment variable) expression.
    //! \param dest Pointer to the expression.
    //! \param isGarbage true if the expression should be deleted with the Command, false if not.
    void setDestination(Expression *dest, bool isGarbage);

    //! \brief Set the command name expression.
    //! \param nameExpr Pointer to the expression.
    //! \param isGarbage true if the expression should be deleted with the Command, false if not.
    void setNameExpr(Expression *nameExpr, bool isGarbage);

    //! \brief Set the vector of command argument expressions.
    //! \param vec Pointer to the vector.
    void setArgumentVector(ExprVec *vec);

    //! \brief Set the resource specification list.
    //! \param l Pointer to the resource list. 
    void setResourceList(ResourceList *l);

    //
    // Interface to CommandNode
    //

    //! \brief Make the Command active.
    void activate();

    //! \brief Make the Command inactive.
    void deactivate();

    //! \brief Execute the Command.
    void execute();

    //! \brief Abort the Command.
    void abort();

    //! \brief Delete references to all expressions.  Delete the expressions if requested.
    //! \note This must be accessible outside the destructor because all expressions
    //!       owned by a Node must be unlinked before they are deleted.
    void cleanUp();

    //
    // Interface to ExternalInterface
    //

    //! \brief Return the given value from command execution.
    //! \param val Const reference to the return value.
    void returnValue(Value const &val);

    //! \brief Return the given command handle value.
    //! \param handle The return command handle value.
    void setCommandHandle(CommandHandleValue handle);

    //! \brief Return an abort acknowledgement.
    //! \param ack Whether the abort was successfully completed.
    void acknowledgeAbort(bool ack);

    //! \brief Fix the command values from the current values of
    //!        the name and parameter expressions.
    //! \note Member function is public to allow for unit testing.
    void fixValues();

    //! \brief Fix the resource values from the current values of
    //!        their expressions.
    //! \note Member function is public to allow for unit testing.
    void fixResourceValues();

    //
    // LinkedQueue participant API
    //

    //! \brief Get the pointer to the next queue item.
    //! \return Pointer to the item.  May be NULL.
    Command *next() const
    {
      return m_next;
    }

    //! \brief Get the pointer to the pointer to the next queue item.
    //! \return Location of the pointer.  Must not be NULL.
    Command **nextPtr()
    {
      return &m_next;
    }

  private:
    // Deliberately not implemented
    Command();
    Command(const Command&);
    Command& operator=(const Command&);

    //
    // Helpers
    //

    //! \brief Is the name expression of this command constant?
    //! \return true if constant, false if not.
    bool isCommandNameConstant() const;

    //! \brief Are the command name expression and all parameter value expressions constant?
    //! \return true if all are constant, false if any are not.
    bool isCommandConstant() const;

    //! \brief Are all the resource expressions for this command constant?
    //! \return true if all are constant, false if any are not.
    bool areResourcesConstant() const;

    Command *m_next;                         //!< Pointer for LinkedQueue.
    CommandFunction m_handleKnownFn;         //!< CommandHandleKnown function.
    CommandHandleVariable m_ack;             //!< CommandHandle variable.
    SimpleBooleanVariable m_abortComplete;   //!< Abort-complete variable.

    //! \brief The concrete values of the command name and parameters.
    //!        Valid when m_commandFixed is true, or if m_commandIsConstant is true.
    State m_command;
    
    Expression *m_nameExpr;                  //!< Pointer to the name expression.
    Expression *m_dest;                      //!< Pointer to the result variable.  May be NULL.
    ExprVec *m_argVec;                       //!< Pointer to the parameter expression vector.  May be NULL.
    ResourceList *m_resourceList;            //!< Pointer to the resource list.  May be NULL.
    ResourceValueList *m_resourceValueList;  //!< Pointer to the resource value list.  May be NULL.

    //! \brief Current value of the CommandHandle.  Accessed by CommandHandleVariable.
    uint16_t m_commandHandle;

    bool m_active;                           //!< true when the command is active.

    //! \brief true when the command name and value expressions have been
    //!        fixed to concrete values.
    //! \note Can only be true if m_active or m_commandIsConstant are true.
    bool m_commandFixed;

    //! \brief true if the command name expression is constant.  Initialized to false.
    //! \see Command::isCommandNameConstant
    bool m_commandNameIsConstant;

    //! \brief true if the command name and parameter expressions are
    //!        all constant.  Initialized to false.
    //! \see Command::isCommandConstant
    bool m_commandIsConstant;

    //! \brief true when the resource value expressions have been
    //!        fixed to concrete values.
    //! \note Can only be true if m_active or m_resourcesAreConstant are true.
    bool m_resourcesFixed;

    //! \brief true if all resource expressions are constant.  Initialized to false.
    //! \see Command::areResourcesConstant
    bool m_resourcesAreConstant;

    //! \brief Indicates whether expressions have been checked for
    //!        constancy.  Initialized to false.  Set to true after
    //!        first activation.
    bool m_checkedConstant;

    bool m_nameIsGarbage;                   //!< true if name expression is to be deleted with the Command.
    bool m_destIsGarbage;                   //!< true if result variable expression is to be deleted with the Command.
  };

}

#endif // PLEXIL_COMMAND_HH
