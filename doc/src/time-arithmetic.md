\page time-arithmetic Time arithmetic utilities

\ingroup Utils

Sometimes it is necessary to perform arithmetic and comparisons on
`struct timeval` and `struct timespec` objects.  This can get unwieldy
fast.  Fortunately C++ allows us to overload the usual arithmetic
operators.

Conversions between those representations and `double` values are also
provided.

+ timeval arithmetic - declared in timeval-utils.hh

  - `bool operator<(const struct timeval &, const struct timeval &)`

    Returns true if the first argument is strictly less than the second argument.

  - `bool operator>(const struct timeval &, const struct timeval &)`

    Returns true if the first argument is strictly greater than the second argument.

  - `bool operator==(const struct timeval &, const struct timeval &)`

    Returns true if the first argument is exactly equal to the second argument.
    
  - `struct timeval operator+(const struct timeval &, const struct timeval &)`
 
    Adds its two arguments and returns a new `struct timeval` containing their sum.
    
  - `struct timeval operator-(const struct timeval &, const struct timeval &)`
 
    Subtracts its second argument from the first argument, and returns
    a new `struct timeval` containing the result.

  - `struct timeval doubleToTimeval(double)`
  
    Constructs a `struct timeval` approximating the time represented
    by the `double` argument.
    
  - `void doubleToTimeval(double, struct timeval &)`
  
    Sets the `struct timeval` referenced by its second argument to a
    value approximating the time represented by the first argument.

  - `double timevalToDouble(const struct timeval &)`
  
    Returns a `double` approximating the time represented by the
    `struct timeval` argument.
    
+ timespec arithmetic - declared in timespec-utils.hh

  - `bool operator<(const struct timespec &, const struct timespec &)`

    Returns true if the first argument is strictly less than the second argument.

  - `bool operator>(const struct timespec &, const struct timespec &)`

    Returns true if the first argument is strictly greater than the second argument.

  - `bool operator==(const struct timespec &, const struct timespec &)`

    Returns true if the first argument is exactly equal to the second argument.
    
  - `struct timespec operator+(const struct timespec &, const struct timespec &)`
 
    Adds its two arguments and returns a new `struct timespec` containing their sum.
    
  - `struct timespec operator-(const struct timespec &, const struct timespec &)`
 
    Subtracts its second argument from the first argument, and returns
    a new `struct timespec` containing the result.

  - `struct timespec doubleToTimespec(double)`
  
    Constructs a `struct timespec` approximating the time represented
    by the `double` argument.
    
  - `void doubleToTimespec(double, struct timespec &)`
  
    Sets the `struct timespec` referenced by its second argument to a
    value approximating the time represented by the first argument.

  - `double timespecToDouble(const struct timespec &)`
  
    Returns a `double` approximating the time represented by the
    `struct timespec` argument.
