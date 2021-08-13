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

#ifndef PLEXIL_PLAN_ERROR_HH
#define PLEXIL_PLAN_ERROR_HH

#include "Error.hh"

namespace PLEXIL
{

  //! \class PlanError
  //! \brief An exception class derived from Error, meant to report
  //!        errors caused by an erroneous PLEXIL plan.
  //! \ingroup Utils
  class PlanError : public Error
  {
  public:

    /**
     * \brief Construct a PlanError object, given a condition
     *        description, a message, a file name, and a line number.
     * \param condition The condition description; const reference to std::string.
     * \param msg The message; const reference to std::string.
     * \param file The file name; const reference to std::string.
     * \param line The line number; const reference to int.
     */
    PlanError(const std::string& condition,
              const std::string& msg,
              const std::string& file,
              const int& line);
    
    //! \brief Copy constructor.
    //! \param orig Const reference to the instance being copied.
    PlanError(const PlanError &orig);

    //! \brief Copy assignment operator.
    //! \param other Const reference to the instance being copied.
    PlanError &operator=(const PlanError &other);

    //! \brief Virtual destructor.
    virtual ~PlanError() PLEXIL_NOEXCEPT;

    //! \brief Equality test operator.
    //! \param other The PlanError instance being compared.
    //! \return true if this instance is identical to the other, false if not.
    bool operator==(const PlanError &other);

    /**
     * \brief Report and throw this exception, or assert, as
     *        directed by PlanError::throwEnabled.
     * \see PlanError::throwEnabled
     * \see PlanError::doThrowExceptions
     * \see PlanError::doNotThrowExceptions
     */
    void report() PLEXIL_NORETURN;

    /**
     * \brief Request that errors should be thrown upon detection.
     * \see PlanError::throwEnabled
     * \see PlanError::doNotThrowExceptions
     */
    static void doThrowExceptions();

    /**
     * \brief Request that when an error is detected, the message
     *        should be printed, and the program should abort.
     *
     * \see PlanError::throwEnabled
     * \see PlanError::doThrowExceptions
     */
    static void doNotThrowExceptions();

    /**
     * \brief Query whether errors will throw exceptions.
     * \return true if throwing is enabled; false if errors will complain and abort.
     * \see PlanError::doThrowExceptions
     * \see PlanError::doNotThrowExceptions
     */
    static bool throwEnabled();

  private:

    PlanError(); // not implemented

  };

} // namespace PLEXIL


/**
 * \def reportPlanError

 * \brief Unconditionally construct a PlanError with the given
 *        message, and report it as directed by
 *        PlanError::throwEnabled.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \see PlanError::throwEnabled
 * \see PlanError::doThrowExceptions
 * \see PlanError::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define reportPlanError(msg) { \
  std::ostringstream sstr; \
  sstr << msg; \
  PLEXIL::PlanError("", sstr.str(), __FILE__, __LINE__).report(); \
}

/**
 * \def checkPlanError
 * \brief Test a condition; if false, construct a PlanError from the
 *        condition and the given message, and report it as directed by
 *        PlanError::throwEnabled.
 * \param cond Expression that yields a true/false result.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \see PlanError::throwEnabled
 * \see PlanError::doThrowExceptions
 * \see PlanError::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define checkPlanError(cond, msg) { \
  if (!(cond)) { \
    std::ostringstream sstr; \
    sstr << msg; \
    PLEXIL::PlanError(#cond, sstr.str(), __FILE__, __LINE__).report(); \
  } \
}

#endif // PLEXIL_PLAN_ERROR_HH
