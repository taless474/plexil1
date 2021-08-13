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

//  Copyright Notices

//  This software was developed for use by the U.S. Government as
//  represented by the Administrator of the National Aeronautics and
//  Space Administration. No copyright is claimed in the United States
//  under 17 U.S.C. 105.

//  This software may be used, copied, and provided to others only as
//  permitted under the terms of the contract or other agreement under
//  which it was acquired from the U.S. Government.  Neither title to nor
//  ownership of the software is hereby transferred.  This notice shall
//  remain on all copies of the software.

// !! Two doxygen comments for each #define depending on another #define is
// !!   a very bad idea.  There's no way to keep them in sync and no point
// !!   in doing so if doxygen is not using the C/C++ preprocessor.
// !! --wedgingt 2004 Oct 5

/**
   @file Error.hh
   @author Will Edgington

   @brief Numerous declarations related to error generation and handling.

   @note These macros should be used rather than direct calls to throw
   unless an existing Error is being "re-thrown" from a catch block.

   @note This is presently only the "interface for programmers";
   nothing has been tested.

   @note Think of the first section, #ifdef PLEXIL_UNSAFE, as the
   "prototypes" and documentation as well as the "production" variant.
*/

#ifndef PLEXIL_ERROR_HH
#define PLEXIL_ERROR_HH

#include <exception>
#include <iostream>
#include <string>
#include <sstream>

// Helper macro for telling compiler that error handler won't return
// Oh fooey, syntax is different between C++ standard and GCC extension.
// C++11 on expects [[noreturn]] immediately after function name.
// GCC expects __attribute__ after prototype.
// Yecch. Punt for now.
#if __GNUC__
// This version for g++ and clang++
#define PLEXIL_NORETURN __attribute__((noreturn))
#else
// fallback (no-op)
#define PLEXIL_NORETURN
#endif

// Helper macro for noexcept specification
// Some compilers use non-standard _NOEXCEPT and declare it to be more stringent
// than the C++11 standard 'noexpect' keyword.
#if defined(_NOEXCEPT)
#define PLEXIL_NOEXCEPT _NOEXCEPT
#elif __cplusplus >= 201103L
#define PLEXIL_NOEXCEPT noexcept
#else // older than C++11
#define PLEXIL_NOEXCEPT throw ()
#endif

/**
 * \def DECLARE_ERROR
 * Declare an error as a function that returns a string naming itself.
 * \param error The error to declare.
 * \ingroup Utils
 */
#define DECLARE_ERROR(error) \
  static const std::string& error() { \
    static const std::string sl_lblStr(#error); \
    return(sl_lblStr); \
  }

namespace PLEXIL
{

  /**
   * \class Error
   * \brief Base class for exceptions in PLEXIL.
   * \ingroup Utils
  */
  class Error : public std::exception
  {
  public:

    /**
     * \brief Constructor from a description of the false/failed condition,
     *        and the source file and line where it was tested/detected.
     * \param condition Description of the cause of the exception.
     * \param file The source file name.
     * \param line The line number in the source file.
     */
    Error(const std::string& condition, const std::string& file, const int& line);

    /**
     * \brief Constructor from a description of the false/failed condition,
     *        and the source file and line where it was tested/detected.
     * \param condition Description of the cause of the exception.
     * \param msg Message with additional information about the exception.
     * \param file The source file name.
     * \param line The line number in the source file.
     */
    Error(const std::string& condition, const std::string& msg, const std::string& file, const int& line);

    /**
     * \brief Constructor with description and location information,
     *        including another Error object's info.
     * \param condition Description of the cause of the exception.
     * \param exception Another Error to include in the report.
     * \param file The source file name.
     * \param line The line number in the source file.
     */
    Error(const std::string& condition, const Error& exception, const std::string& file, const int& line);

    /**
     * \brief Constructor from a message.
     * \param msg The message.
     *
     * \note Should only be used when setCause() will be called before
     *       the Error is thrown.
     * \note Never prints anything, unlike the other constructors.
     * \see Error::setCause()
     */
    Error(const std::string& msg);

    /**
     * \brief Copy constructor.
     * \param err The Error instance to copy.
     */
    Error(const Error& err);

    /**
     * \brief Virtual destructor.
     * \note Should only be used implicitly.
     */
    virtual ~Error() PLEXIL_NOEXCEPT;

    /**
     * \brief Assignment operator.
     * \param err The Error to copy.
     * \return Reference to *this.
     */
    Error& operator=(const Error& err);

    /**
     * \brief Get the message from this Error.
     * \return Pointer to const null-terminated string.
     */
    virtual char const *what() const PLEXIL_NOEXCEPT;

    /**
     * \brief Query whether all error information should be printed when detected.
     * \return true if printing enabled when detected, false if not.
     */
    static bool printingErrors();

    /**
     * \brief Enable printing error information at detection.
     */
    static void doDisplayErrors();

    /**
     * \brief Disable printing error information at detection
     */
    static void doNotDisplayErrors();

    /**
     * \brief Get the output stream to which error information should be printed.
     * \return Reference to the stream.
     */
    static std::ostream& getStream();

    /**
     * \brief Set the output stream to which error information should be printed.
     * \param os Reference to the stream.
     */
    static void setStream(std::ostream& os);

    /**
     * \brief Print the contents of an Error in "error format" (e.g. for Emacs)
     *        on the selected output stream.
     * \see Error::getStream
     * \see Error::setStream
     */
    void display();

    /**
     * \brief Print the contents of an Error as if calling an Error constructor.
     * \param os The stream on which to print. Defaults to std::cerr.
     */
    void print(std::ostream& os = std::cerr) const;

    /**
     * \brief Equality comparison operator.
     * \param err Const reference to the Error being compared.
     * \return true if both Error instances are equal, false if not.
     */
    bool operator==(const Error& err) const;

    /**
     * \brief Return true iff the two Error instances "match" in condition, message, and file name.
     * \param err Const reference to the Error being compared.
     * \return true if the instances match, false if they do not match.
     */
    bool matches(const Error& err) const;

    /**
     * \brief Print a warning in the format used by Error::display()
     *        if printing warnings is enabled.
     * \param msg The message to print.
     * \param file The file name to print.
     * \param line The line number to print.
     *
     * \see Error::displayWarnings
     * \see Error::doDisplayWarnings
     * \see Error::doNotDisplayWarnings
     */
    static void printWarning(const std::string& msg,
                             const std::string& file,
                             const int& line);

    /**
     * \brief Query whether warning should be printed.
     * \return True if warnings should be printed, false if not.
     *
     * \see Error::doDisplayWarnings
     * \see Error::doNotDisplayWarnings
     */
    static bool displayWarnings();

    /**
     * \brief Request that warnings be printed.
     *
     * \see Error::displayWarnings
     * \see Error::doNotDisplayWarnings
     */
    static void doDisplayWarnings();

    /**
     * \brief Request that warnings not be printed.
     *
     * \see Error::displayWarnings
     * \see Error::doDisplayWarnings
     */
    static void doNotDisplayWarnings();

    /**
     * \brief Request that errors should be thrown upon detection.
     *
     * \see Error::throwEnabled
     * \see Error::doNotThrowExceptions
     */
    static void doThrowExceptions();

    /**
     * \brief Request that when an error is detected, the message
     *        should be printed, and the program should abort.
     *
     * \see Error::throwEnabled
     * \see Error::doThrowExceptions
     */
    static void doNotThrowExceptions();

    /**
     * \brief Query whether errors will throw exceptions.
     * \return true if throwing is enabled; false if errors will complain and abort.
     */
    static bool throwEnabled();

    DECLARE_ERROR(GeneralMemoryError);
    DECLARE_ERROR(GeneralUnknownError);

    /**
     * \brief Throw the exception, or complain and abort, as directed by Error::throwEnabled.
     *
     * \see Error::throwEnabled
     * \see Error::doThrowExceptions
     * \see Error::doNotThrowExceptions
     */
    void handleAssert() PLEXIL_NORETURN;

  protected:

    //
    // Data members which may be used by derived classes
    //

    std::string m_condition; /**<The condition that, being false, implies the error has occurred. */
    std::string m_msg; /**<Additional info about the error. */
    std::string m_file; /**<The source file in which the error was detected (__FILE__). */
    int m_line; /**<The source line on which the error detected (__LINE__). */

  private:


    Error(); /**<The zero argument constructor should not be used. */
  };

  /**
   * \brief Overloaded formatted output operator.
   * \param os Reference to the output stream.
   * \param err Const reference to the Error instance to be printed.
   * \return Reference to the output stream.
   */
  std::ostream& operator<<(std::ostream& os, const Error& err);

} // namespace PLEXIL

//
// Error macros
//

/**
 * \def ALWAYS_FAIL
 * False.
 * \note Used as argument to assert() and similar functions to
 * make clear that the assertion will fail, throw an error, etc.
 */
#define ALWAYS_FAIL (false)

//
// assertTrue & friends
//

/**
 * \brief Test a condition; if false, construct an Error with the
 *        location of the macro call, and throw or abort as requested.
 * \param cond Expression that yields a true/false result.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see check_error_1
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define assertTrue_1(cond) { \
  if (!(cond)) { \
    PLEXIL::Error(#cond, __FILE__, __LINE__).handleAssert(); \
  } \
}

/**
 * \brief Test a condition; if false, construct an Error with the
 *        given message and location of the macro call, and throw or
 *        abort as requested.
 * \param cond Expression that yields a true/false result.
 * \param msg A string or Error instance.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see check_error_2
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define assertTrue_2(cond, msg) { \
  if (!(cond)) { \
    PLEXIL::Error(#cond, msg, __FILE__, __LINE__).handleAssert(); \
  } \
}

/**
 * \def errorMsg
 * \brief Unconditionally construct an Error with the given message
 *        and location of the macro call, and throw or abort as
 *        requested.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \note Will not return to the location of the call.
 *
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define errorMsg(msg) { \
  std::ostringstream sstr; \
  sstr << msg; \
  PLEXIL::Error("", sstr.str(), __FILE__, __LINE__).handleAssert(); \
} 

/**
 * \brief Test a condition; if false, construct an Error with the text
 *        of the condition, the given message, and the location of the
 *        macro call, and throw or abort as requested.
 * \param cond Expression that yields a true/false result.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see checkError
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define assertTrueMsg(cond, msg) { \
  if (!(cond)) { \
    std::ostringstream sstr; \
    sstr << msg; \
    PLEXIL::Error(#cond, sstr.str(), __FILE__, __LINE__).handleAssert(); \
  } \
}

//
// The follwing macros do nothing if the macro PLEXIL_UNSAFE is
// defined at compile time.  They are meant to be used to check for
// unlikely conditions during development.
//
// PLEXIL_UNSAFE should only be defined when the developer is
// confident that the conditions being checked will not happen, and
// run time must be minimized.
//

#ifdef PLEXIL_UNSAFE

#define check_error_1(cond)
#define check_error_2(cond, msg)
#define checkError(cond, msg)

#define warn(msg)

#else

//
// check_error implementations
//

/**
 * \brief Test a condition; if false, construct an Error with the
 *        location of the macro call, and throw or abort as requested.
 *        If macro PLEXIL_UNSAFE is defined at compile time, does
 *        nothing.
 * \param cond Expression that yields a true/false result.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see assertTrue_1
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define check_error_1(cond) { \
  if (!(cond)) { \
    PLEXIL::Error(#cond, __FILE__, __LINE__).handleAssert(); \
  } \
}

/**
 * \brief Test a condition; if false, construct an Error with the
 *        given message and location of the macro call, and throw or
 *        abort as requested. If macro PLEXIL_UNSAFE is defined at
 *        compile time, does nothing.
 * \param cond Expression that yields a true/false result.
 * \param msg A string or Error instance.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see assertTrue_2
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define check_error_2(cond, msg) { \
  if (!(cond)) { \
    PLEXIL::Error(#cond, msg, __FILE__, __LINE__).handleAssert(); \
  } \
}

/**
 * \brief Test a condition; if false, construct an Error with the text
 *        of the condition, the given message, and the location of the
 *        macro call, and throw or abort as requested.  If macro
 *        PLEXIL_UNSAFE is defined at compile time, does nothing.
 * \param cond Expression that yields a true/false result.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \note If cond evaluates to false, will not return to the location of the call.
 *
 * \see assertTrueMsg
 * \see Error::throwEnabled
 * \see Error::doThrowExceptions
 * \see Error::doNotThrowExceptions
 *
 * \ingroup Utils
 */
#define checkError(cond, msg) { \
  if (!(cond)) { \
    std::ostringstream sstr; \
    sstr << msg; \
    PLEXIL::Error(#cond, sstr.str(), __FILE__, __LINE__).handleAssert(); \
  } \
}

/**
 * \brief If warning messagess are enabled, print a warning with the
 *        given message and the source location of the macro call.
 *        Does nothing if macro PLEXIL_UNSAFE is defined at compile
 *        time.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \see Error::displayWarnings
 * \see Error::doDisplayWarnings
 * \see Error::doNotDisplayWarnings
 *
 * \ingroup Utils
 */
#define warn(msg) {                                             \
      std::ostringstream sstr;                                   \
      sstr << msg;                                              \
      PLEXIL::Error::printWarning(sstr.str(), __FILE__, __LINE__);  \
}

#endif /* PLEXIL_UNSAFE */

#endif /* PLEXIL_ERROR_HH */
