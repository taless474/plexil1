\page dynamic-loading Dynamic library loading

\ingroup Utils

On platforms which implement the POSIX dynamic linking API (i.e. the
functions `dlopen()`, `dlclose()`, and `dlsym()`) , the function
`dynamicLoadModule` implements dynamic loading of libraries as
modules.

+ Function `dynamicLoadModule`

  Looks for the named shared library, and if the library is found,
  loads it and calls its initialization function.  See the API
  reference for more details.

