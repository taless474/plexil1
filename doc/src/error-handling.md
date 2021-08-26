\page error-handling Error handling

\namespace PLEXIL
\ingroup Utils

The Utilities module provides the basis of the *error handling system*
in the %PLEXIL Executive.

The error handling system can be configured at run time to throw
exceptions or to assert; some of its error checking macros can be
disabled at configuration time.

The error handling system makes use of the 
[logging facility](\ref logging).

To use the error handling facilities described here in C++ code,
simply add the line:

    #include "Error.hh"

### Exception classes

+ Class `Error`

  `Error` is the base class for general error handling.  It is
  derived directly from `std::exception`.

  Many of the functions which control how errors are handled and
  reported are static member functions of `Error`.

+ Class `PlanError`

  `PlanError` is used to report errors which might originate
  in an invalid %PLEXIL plan.  It is derived from `Error`.

+ Class `ParserException`

  `ParserException` is the base class for reporting errors in
  parsing text input.  It is derived directly from std::exception.  It
  does *not* derive from `Error`, so that an error parsing a
  %PLEXIL plan can be handled in a way which does not bring down the
  Executive.

The [core external interface module](group___external-_interface.html)
defines an additional exception class, InterfaceError, used for
reporting and recovering from errors in interfacing the Executive to
external systems.

### Functions for controlling how errors are handled

The functions below are mostly static member functions of the Error
class.

- Function `Error::getStream`

  Returns a reference to the current stream for error output.
  
- Function `Error::setStream`

  Sets its argument as the current stream for error output.

- Function `Error::printingErrors`

  Returns true if displaying errors is enabled.

- Function `Error::doDisplayErrors`

  Enables displaying errors.

- Function `Error::doNotDisplayErrors`

  Disables displaying errors.

- Function `Error::displayWarnings`

  Returns true if displaying warnings is enabled.

- Function `Error::doDisplayWarnings`

  Enables displaying warnings.

- Function `Error::doNotDisplayWarnings`

  Disables displaying warnings.

- Function `Error::throwEnabled`

  Returns true if throwing exceptions is enabled, false if calling
  assert() instead.

- Function `Error::doThrowExceptions`

  Enables throwing exceptions on error.

- Function `Error::doNotThrowExceptions`

  Disables throwing exceptions on error.  Errors are reported by
  calling assert() instead.

- Function `PlanError::throwEnabled`

  Returns true if throwing exceptions is enabled for plan errors,
  false if calling assert() instead.

- Function `PlanError::doThrowExceptions`

  Enables throwing exceptions on plan error.

- Function `PlanError::doNotThrowExceptions`

  Disables throwing exceptions on error.  Plan errors are reported by
  calling assert() instead.

### Error checking and reporting macros

+ Macro `DECLARE_ERROR`

  Declares a static function named `error` which takes no arguments,
  and returns a const reference to a std::string which contains the
  argument to the DECLARE_ERROR macro.

+ Macro `ALWAYS_FAIL`

  Expands to `(false)`.  Meant to be used as a condition in calls to
  conditional error reporting macros.

+ Macro `errorMsg`

  Uses its argument as an expression for formatted output and writes
  it to a string, then unconditionally constructs an Error instance
  with the string and the location of the macro call, and throws or
  calls `assert()`, as determined by the function
  `Error::throwEnabled`.
  
+ Macro `assertTrue_1`

  Evaluate the argument.  If it evaluates to zero (false), constructs
  an Error instance with a message including the text of the argument
  and the location of the macro call, and throws or calls `assert()`,
  as determined by the function `Error::throwEnabled`.
  
+ Macro `assertTrue_2`

  Evaluate the first argument.  If it evaluates to zero (false),
  constructs an Error instance with a message including the text of
  the first argument, the second argument (which must be convertible
  to std::string), and the location of the macro call, and throws or
  calls `assert()`, as determined by the function
  `Error::throwEnabled`.

+ Macro `assertTrueMsg`

  Convenience macro for formatted error messages.  Like
  `assertTrue_2`, but accepts as its second argument any expression
  which could appear to the right of the `<<` formatted output
  operator
  
+ Macros `check_error_1`, `check_error_2`, `checkError`

  If the macro `PLEXIL_UNSAFE` is not defined at compile time (the
  usual case), these macros expands to code similar to `assertTrue_1`,
  `assertTrue_2`, and `assertTrueMsg` respectively.

  If `PLEXIL_UNSAFE` is defined at compile time, these macros expand
  to nothing.

+ Macro `warn`

  Uses the argument as an expression for generating formatted output,
  saves the result to a string, and calls Error::printWarning with
  that string and the location of the macro call.

+ Macro `reportPlanError`

  Uses its argument as an expression for formatted output and writes
  it to a string, then unconditionally constructs a PlanError instance
  with the string and the location of the macro call, and throws or
  calls `assert()`, as determined by the function
  `PlanError::throwEnabled`.

+ Macro `checkPlanError`

  Evaluates its first argument. If the result is zero (false), uses
  its argument as an expression for formatted output and writes it to
  a string, then unconditionally constructs a PlanError instance with
  the string and the location of the macro call, and throws or calls
  `assert()`, as determined by the function `PlanError::throwEnabled`.
