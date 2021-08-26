\page misc-utils Miscellaneous utilities

\ingroup Utils

Other utilities used in the %PLEXIL Executive.

- File bitsetUtils.hh

  Functions and function templates for use with `std::bitset`, and
  `unsigned long` used as a bitset.

  - `int PLEXIL::findFirstOne(unsigned long const b)`

    Finds the rightmost (least significant) 1 bit in an `unsigned
    long` value.

  - `int PLEXIL::findFirstZero(unsigned long const b)`

    Finds the rightmost (least significant) 0 bit in an `unsigned
    long` value.

  - `template <size_t N_ENTRIES> int PLEXIL::findFirstOne(std::bitset<N_ENTRIES> const &b)`
  
    Finds the rightmost 1 bit in a `std::bitset`.

  - `template <size_t N_ENTRIES> int PLEXIL::findFirstZero(std::bitset<N_ENTRIES> const &b)`
  
    Finds the rightmost 0 bit in a `std::bitset`.

+ File ConstantMacros.hh

  - Macro `DECLARE_STATIC_CLASS_CONST(TYPE, NAME, VALUE)`
  
    A macro to define a class-scoped singleton.  Meant to be used
    within a class declaration.
    
    Replaced by the `constexpr` keyword, introduced by C++11, in
    %PLEXIL 6.

- File ScopedOstreamRedirect.hh

  - `Class PLEXIL::ScopedOstreamRedirect`

    A class whose constructor redirects output from one `std::ostream`
    instance to another, and whose destructor ends the redirection.
    Meant to be used as a local variable, hence the name.

+ File stricmp.h

  - `int stricmp(const char * s1, const char * s2)`

    A C function implementing case-insensitive comparison of two
    null-terminated character strings.

+ File TestSupport.hh

  Macros used in the %PLEXIL module test suite.  Not referenced in
  user-facing code.
