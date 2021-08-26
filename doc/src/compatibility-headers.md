\page compatibility-headers Header compatibility files

\ingroup Utils

The %PLEXIL Executive has been built for a wide variety of platforms,
under many different development environments.  Some compilers may not
provide all of the C standard library header files.  Some C++
compilers do not provide these files under the file names in the C++
standard.

The header compatibility files in the Utilities module provide a
uniform way to access these standard header files, without adding
conditional macros to the source files needing the headers.  The build
configuration process detects the compilers' quirks, and generates a
file `plexil-config.h` with macros describing the compiler
environment.  Each of the compatibility files includes this file, and
uses the macros it defines to select the appropriate include file for
the compiler and the source language being compiled.

Each of the provided headers corresponds to a particular standard
include file, identifiable by removing `plexil-` from the file name.

The compatibility headers provided in this release:

+ plexil-assert.h
+ plexil-ctype.h
+ plexil-errno.h
+ plexil-float.h
+ plexil-inttypes.h
+ plexil-limits.h
+ plexil-math.h
+ plexil-signal.h
+ plexil-stddef.h
+ plexil-stdint.h
+ plexil-stdio.h
+ plexil-stdlib.h
+ plexil-string.h
+ plexil-time.h
