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

#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

#include "Error.hh" // PLEXIL_NOEXCEPT

/**
 * \brief Unconditionally construct a ParserException with the given
 *        message, and throw the exception.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \see ParserException
 *
 * \ingroup Utils
 */
#define reportParserException(msg) { \
    std::ostringstream whatstr; \
    whatstr << msg; \
    throw PLEXIL::ParserException(whatstr.str().c_str()); \
}

/**
 * \brief If cond evaluates to false (nonzero), construct a
 *        ParserException with the given message, and throw the
 *        exception.
 * \param cond The condition to test.
 * \param msg Anything suitable as the right-hand side of <<.
 *
 * \see ParserException
 *
 * \ingroup Utils
 */
#define checkParserException(cond, msg) { \
  if (!(cond)) \
    { \
      std::ostringstream whatstr; \
      whatstr << msg; \
      throw PLEXIL::ParserException(whatstr.str().c_str()); \
    } \
}

namespace PLEXIL
{

  //! \class ParserException
  //! \brief Exception class used to report parsing errors.
  //!
  //! \see reportParserException
  //! \see checkParserException
  //!
  //! \ingroup Utils
  class ParserException : public std::exception
  {
  public:

    //! \brief Default constructor.
    ParserException() PLEXIL_NOEXCEPT;

    //! \brief Constructor with an error message.
    //! \param msg The error messsage; pointer to const
    //!            null-terminated string.
    ParserException(const char* msg) PLEXIL_NOEXCEPT;

    //! \brief Constructor with an error message, file name, and
    //!        offset in the file.
    //! \param msg The error messsage; pointer to const
    //!            null-terminated string.
    //! \param filename The file name; pointer to const
    //!                 null-terminated string.
    //! \param offset The offset in the file, in characters.
    ParserException(const char* msg, const char* filename, int offset) PLEXIL_NOEXCEPT;

    //! \brief Constructor with error message, file name, line number,
    //!        and column number.
    //! \param msg The error messsage; pointer to const
    //!            null-terminated string.
    //! \param filename The file name; pointer to const
    //!                 null-terminated string.
    //! \param line The line number.
    //! \param col The column number.
    ParserException(const char* msg, const char* filename, int line, int col) PLEXIL_NOEXCEPT;

    //! \brief Copy assignment operator.
    //! \param other Const reference to the instance being copied.
    //! \return Reference to *this.
    ParserException& operator=(const ParserException&) PLEXIL_NOEXCEPT;

    //! \brief Virtual destructor.
    virtual ~ParserException() PLEXIL_NOEXCEPT;

    //! \brief Get the message string.
    //! \return The message, as a pointer to const null-terminated string.
    virtual const char *what() const PLEXIL_NOEXCEPT;

  private:
    std::string m_what; /**< The user-provided message. */
    std::string m_file; /**< The source file in which the error was detected (__FILE__). */
    int m_line; /**< Line number of the error (__LINE__). */
	int m_char; /**< The character offset of the error. */
  };

}


#endif // PARSER_EXCEPTION_H
