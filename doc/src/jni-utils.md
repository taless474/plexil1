\page jni-utils Java Native Interface utilities

\ingroup Utils

Utilities for C++ code interacting with (called by) the Java Native
Interface.  Developed when the %PLEXIL Executive was ported to the
Android operating system in 2012.


+ File JNIHelper.hh

  - `class JNIHelper`

    Singleton class simplifying access to several key JNI objects.

  - `class JavaObject`

    Wrapper class for Java object references (`jobject`).

  - `class JavaClass`

    Wrapper class for Java class references (`jclass`).

+ File JNIUtils.hh

  - `class PLEXIL::JNIUtils`
  
    Provides utilities for converting Java String objects to C strings
    and back again, and for getting the argument vector of a Java
    application in traditional C-style argc, argv form.
