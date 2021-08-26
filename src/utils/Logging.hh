/* Copyright (c) 2006-2021, Universities Space Research Association (USRA).
 * All rights reserved.
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

#ifndef LOGGING_HH
#define LOGGING_HH

//! \defgroup Utils Common programming utilities

//! \class Logging
//! \brief Declares static member variables and functions to assist
//!        with output logging.
//! \ingroup Utils
class Logging {
public:

  //! \brief Enumeration to specify the purpose of a log message.
  enum LogType {
    LOG_ERROR         = 0,  //!< Prompts user if ENABLE_E_PROMPT is nonzero.
    WARNING           = 1,  //!< Prompts user if ENABLE_W_PROMPT is nonzero. 
    INFO              = 2   //!< Will never prompt user.
  };

  //! \brief Controls logging. If nonzero, messages are written to the log file.
  //! \note Initial value is 0.
  static int ENABLE_LOGGING;

  //! \brief Controls user prompting when a LOG_ERROR message is
  //!        reported. If nonzero, the user will be prompted.
  //! \note Initial value is 0.
  static int ENABLE_E_PROMPT;

  //! \brief Controls user prompting when a WARNING message is
  //!        reported. If nonzero, the user will be prompted.
  //! \note Initial value is 0.
  static int ENABLE_W_PROMPT;

  //! \brief Set the name of the file used for log output.
  //! \param file Pointer to const null-terminated string. May be NULL.
  //! \note If file is NULL, defaults to "universalexec.log".
  static void set_log_file_name(const char * file);

  //! \brief Print the given message to the log.
  //! \param fullmsg Pointer to const null-terminated string.
  //! \note Writes a header if this is a new session.  The header
  //! includes the process ID if the platform provides a getpid()
  //! function.
  static void print_to_log(const char * fullmsg); 

  //! \brief Print num strings from the array of string pointers
  //!        run_command to the log, with spaces between strings.
  //! \param run_command Pointer to array of pointers to null-terminated strings.
  //! \param num Number of the strings to print.
  //! \note Intended to be used with the standard C main() arguments
  //! `argv` and `argc`, respectively; e.g.
  //! `Logging::print_to_log(argv, argc + 1)`
  static void print_to_log(char** run_command, int num);                

  //! \brief Write a message to the console, and to the log file, if
  //!        enabled by configuration variable ENABLE_LOGGING;
  //!        optionally prompt the user as determined by msg_type and
  //!        configuration variables ENABLE_E_PROMPT and
  //!        ENABLE_W_PROMPT.
  //! \param msg_type The message type; should be a valid value of the
  //!                 LogType enumeration.
  //! \param msg The message to be printed; pointer to const
  //!            null-terminated string.
  //!
  //! \see Logging::LogType
  //! \see Logging::ENABLE_LOGGING
  //! \see Logging::ENABLE_E_PROMPT
  //! \see Logging::ENABLE_W_PROMPT
  static void handle_message(int msg_type, const char * msg);

  //! \brief Write a message to the console, including the given file
  //!        name and offset in characters, and to the log file, if
  //!        enabled; optionally prompt the user as determined by
  //!        msg_type, if enabled.
  //! \param msg_type The message type; should be a valid value of the
  //!                 LogType enumeration.
  //! \param file File name to be printed; pointer to const
  //!             null-terminated string.
  //! \param offset Offset to be printed.
  //! \param msg The message to be printed; pointer to const
  //!            null-terminated string.
  //!
  //! \see Logging::LogType
  //! \see Logging::ENABLE_LOGGING
  //! \see Logging::ENABLE_E_PROMPT
  //! \see Logging::ENABLE_W_PROMPT
  static void handle_message(int msg_type, const char * file, int offset, const char * msg);

  //! \brief Write a message to the console, including the given file
  //!        name, line number, and column, and to the log file, if
  //!        enabled; optionally prompt the user as determined by
  //!        msg_type, if enabled.
  //! \param msg_type The message type; should be a valid value of the
  //!                 LogType enumeration.
  //! \param file File name to be printed; pointer to const
  //!             null-terminated string.
  //! \param line Line number to be printed.
  //! \param col Column number to be printed.
  //! \param msg The message to be printed; pointer to const
  //!            null-terminated string.
  //!
  //! \see Logging::LogType
  //! \see Logging::ENABLE_LOGGING
  //! \see Logging::ENABLE_E_PROMPT
  //! \see Logging::ENABLE_W_PROMPT
  static void handle_message(int msg_type, const char * file, int line, int col, const char * msg);

private:

  // Deliberately not implemented -- all members are static
  Logging();
  Logging(const Logging&);
  Logging& operator=(const Logging&);
  ~Logging();

};

#endif /* _LOGGING_HH */
