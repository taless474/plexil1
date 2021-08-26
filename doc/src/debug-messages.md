\page debug-messages Debug message facility

\namespace PLEXIL
\ingroup Utils

The *debug message facility* provides a way to put print statements,
or executable statements in general, into C++ source code, to be
selectively enabled based on the contents of a *debug configuration
file* read at startup.

Each debug message or statement is labeled with a *marker*, a simple
character string.  Markers need not be unique; in fact, it can be very
useful to have many related debug messages share the same marker.  If
a *pattern* in the configuration file is found as a substring in the
marker, that message or statement is enabled.

The debug message facility can be enabled or disabled at build
configuration time.  It is enabled by default.

When the debug message facility is enabled, by convention, most
applications in the %PLEXIL distribution attempt to load the debug
configuration from the file `Debug.cfg` in the current working
directory.  This default can be overridden on the command line with a
user supplied file name in most cases.

To use the debug message facility in C++ source qcode, simply add this
line:

    #include "Debug.hh"

## Configuration file format

The file consists of patterns, one per line.  If the pattern begins
with a colon `:`, the colon is ignored; this allows backward
compatibility with an earlier version of the facility.

The character `#` or `\` denotes a comment. Everything after the
comment marker to the end of the line is ignored.

## Macros

The following macros are available, and can be inserted into any
executable C++ code.  The `msg` arguments to each of these macros can
be any expression legal for use on the right side of the `<<`
formatted output operator.

If the debug message facility is disabled at build configuration time,
these macros expand to nothing.

+ Macro `debugMsg(marker,msg)`

  When the marker is enabled, print the marker and the message to the debug stream.

+ Macro `condDebugMsg(cond,marker,msg)`

  If the condition evaluates to nonzero, and the marker is enabled,
  print the marker and message to the debug stream.

+ Macro `debugStmt(marker,stmt)`

  Execute the statement(s) when the given marker is enabled.

+ Macro `condDebugStmt(cond,marker,stmt)`

  If the condition evaluates to nonzero, and the marker is enabled,
  execute the statement(s).

+ Macro `SHOW(thing)`

  Print the file and line number of the call, the text of `thing`, and
  the result of formatting `thing` to std::cout.

+ Macro `MARK`

  Print the file name and the line number, and the word `MARK`, to std::cout.

## Functions

+ Function `getDebugOutputStream`

  Returns the output stream used for printing debug messages.

  \warning No default is provided for the debug stream.  It must be
  explicitly set by the application before calling this function.

+ Function `setDebugOutputStream`

  Set the output stream to be used for debug messages.  Any stream
  derived from std::ostream may be used.

+ Function `readDebugConfigStream`

  Read the given input stream and enable the debug messages specified
  by its contents.

+ Function `enableMatchingDebugMessages`

  Enable all debug messages with markers which match the given string.
