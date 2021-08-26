\page logging Logging facility

\ingroup Utils

The static members of the `Logging` class implement a *logging
facility*.  Output can be written to an error output stream, and/or
logged to a file if enabled.  If prompting the user is enabled for a
particular message type, the user is given the choice of continuing,
aborting, or printing the call stack.

To use these logging facilities in C++ code, simply add the line:

    #include "Logging.hh"

+ The enumeration `Logging::LogType` describes the available message
  types: `LOG_ERROR`, `WARNING`, and `INFO`.

+ The `int` static member variable `Logging::ENABLE_LOGGING` controls
  whether log output is written to a file. If it is 0 (the default),
  no output is written. If it is non-zero, output is logged to file.
  
+ The `int` static member variables `Logging::ENABLE_E_PROMPT` and
  `Logging::ENABLE_W_PROMPT` control whether the user is prompted for
  messages of type `LOG_ERROR` and `WARNING`, respectively.  Prompting
  is enabled when these variables are set to non-zero values.  Their
  default values are both 0.
  
+ Static member function `Logging::set_log_file_name` sets the name of
  the log file.  If its argument is `NULL`, the file name is set to
  `universalexec.log` in the current working directory.

+ Overloaded static member function `Logging::print_to_log` writes a
  time stamp and its argument(s) to the log file and also writes a
  session header including the process ID to the log file if this is a
  new session.

+ Overloaded static member function `Logging::handle_message` takes a
  `Logging::LogType` and messages in several formats, unconditionally
  logs to the stream returned by `PLEXIL::Error::getStream`,
  conditionally writes to the log file depending on the value of
  `Logging::ENABLE_LOGGING`, and conditionally prompts the user
  depending on the log message type and the values of
  `Logging::ENABLE_E_PROMPT`and `Logging::ENABLE_W_PROMPT`.
