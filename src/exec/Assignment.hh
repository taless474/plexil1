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

#ifndef PLEXIL_ASSIGNMENT_HH
#define PLEXIL_ASSIGNMENT_HH

#include "SimpleBooleanVariable.hh"
#include "Value.hh"

namespace PLEXIL
{

  // forward reference
  class Assignable;

  //! \class Assignment
  //! \brief The executable component of an Assignment Node.
  //! \see AssignmentNode
  //! \ingroup Exec-Core
  class Assignment 
  {
  public:
    //! \brief Default constructor.
    Assignment();

    //! \brief Destructor.
    ~Assignment();

    //! \brief Get the destination variable as an Assignable.
    //! \return Pointer to the Assignable.
    Assignable *getDest();

    //! \brief Get the destination variable as an Assignable.
    //! \return Const pointer to the Assignable.
    Assignable const *getDest() const;

    //! \brief Get the acknowledgement variable.
    //! \return Pointer to the Expression.
    Expression *getAck();

    //! \brief Get the acknowledgement variable.
    //! \return Const pointer to the Expression.
    Expression const *getAck() const;

    //! \brief Get the abort-complete variable.
    //! \return Pointer to the Expression.
    Expression *getAbortComplete();

    //! \brief Get the abort-complete variable.
    //! \return Pointer to the Expression.
    Expression const *getAbortComplete() const;

    //! \brief Set the assignment variable.
    //! \param lhs Pointer to the variable, as an Assignable.
    //! \param garbage True if the variable is to be deleted with the Assignment, false if not.
    //! \note Used by the plan parser.
    void setVariable(Assignable *lhs, bool garbage);

    //! \brief Set the expression for the value to be assigned.
    //! \param rhs Pointer to the Expression.
    //! \param garbage True if the expression is to be deleted with the Assignment, false if not.
    //! \note Used by the plan parser.
    void setExpression(Expression *rhs, bool garbage);

    //! \brief Make the Assignment active.
    void activate();

    //! \brief Make the Assignment inactive.
    void deactivate();

    //! \brief Fix the value to be assigned.
    void fixValue();

    //! \brief Perform the assignment.
    void execute();

    //! \brief Retract the assignment.
    void retract();

    //! \brief Unlink and delete (if requestwed) the assignment variable and value expression.
    //! \note For use by AssignmentNode destructor.
    void cleanUp();

    //! \brief Get the pointer to the next item in the queue.
    //! \return The pointer.
    //! \see LinkedQueue
    Assignment *next() const;

    //! \brief Get the pointer to the pointer to the next item in the queue.
    //! \return The pointer.
    //! \see LinkedQueue
    Assignment **nextPtr();

  private:

    // Copy, move constructors, assignment operators explicitly not implemented
#if __cplusplus >= 201103L
    Assignment(Assignment const &) = delete;
    Assignment(Assignment &&) = delete;
    Assignment& operator=(Assignment const &) = delete;
    Assignment& operator=(Assignment &&) = delete;
#else
    Assignment(Assignment const &);
    Assignment& operator=(Assignment const &);
#endif

    SimpleBooleanVariable m_ack;            //!< The acknowledgement flag.  Used as the action-complete condition by AssignmentNode.
    SimpleBooleanVariable m_abortComplete;  //!< The abort-complete flag.  Used as the abort-complete condition by AssignmentNode.
    Value m_value;                          //!< The value to be assigned. Only valid after calling fixValue().
    Assignment *m_next;                     //!< Next item pointer for LinkedQueue.
    Expression *m_rhs;                      //!< Expression for the value to be assigned.
    Assignable *m_dest;                     //!< The assignment variable.
    bool m_deleteLhs;                       //!< If true, delete the assignment variable in cleanUp().
    bool m_deleteRhs;                       //!< If true, delete the value expression in cleanUp().
  };

}

#endif // PLEXIL_ASSIGNMENT_HH
