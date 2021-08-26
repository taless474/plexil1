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

#ifndef PLEXIL_COMMAND_HANDLE_VARIABLE_HH
#define PLEXIL_COMMAND_HANDLE_VARIABLE_HH

#include "CommandHandle.hh"
#include "GetValueImpl.hh" // indirectly includes plexil-stddef.h, plexil-stdint.h
#include "Notifier.hh"

namespace PLEXIL
{
  // Forward reference
  class Command;

  //! \class CommandHandleVariable
  //! \brief An Expression derivative for CommandHandle variables.
  //!        Used as a member variable by Command.
  //! \ingroup External-Interface
  class CommandHandleVariable :
    public GetValueImpl<uint16_t>,
    public Notifier
  {
  public:

    //! \brief Constructor.
    //! \param cmd Const reference to the owning Command.
    CommandHandleVariable(Command const &cmd);

    //! \brief Virtual destructor.
    virtual ~CommandHandleVariable();

    //
    // Expression API
    //

    //! \brief Is this object a source of change events?
    //! \return true if the object can change of its own accord, false if not.
    virtual bool isPropagationSource() const;

    //! \brief Get the name of this instance.
    //! \return Pointer to const null-terminated string.
    virtual char const *getName() const;

    //! \brief Return a print name for the expression type.
    //! \return Pointer to const null-terminated string.
    virtual char const *exprName() const
    {
      return "CommandHandleVariable";
    }

    //! \brief Get the type of the expression's value.
    //! \return The ValueType. 
    virtual ValueType valueType() const
    {
      return COMMAND_HANDLE_TYPE;
    }

    //! \brief Is the value of this expression known?
    //! \return true if known, false if unknown.
    virtual bool isKnown() const;

    //! \brief Get the current value of this expression.
    //! \param result Reference to place to store the value.
    //! \return true if the value is known, false if not.
    virtual bool getValue(uint16_t &result) const;

    //! \brief Print the expression's value to the given stream.
    //! \param s The output stream.
    virtual void printValue(std::ostream& s) const;

    //! \brief Set the name of this CommandHandleVariable instance.
    //! \param name Const reference to the name.
    void setName(const std::string &name);

  private:

    // Not implemented
    CommandHandleVariable();
    CommandHandleVariable(const CommandHandleVariable &);
    CommandHandleVariable &operator=(const CommandHandleVariable &);

    Command const & m_command; //!< The Command which owns this variable.
    char const *m_name;        //!< Print name of this instance.
  };

} // namespace PLEXIL

#endif // PLEXIL_COMMAND_HANDLE_VARIABLE_HH
