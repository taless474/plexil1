# Getting started with PLEXIL {#getting-started}

## Getting PLEXIL

%PLEXIL can be cloned from the official 
[Git repository](https://sourceforge.net/p/plexil/git/ci/releases/plexil-4.6/~/tree/)
at SourceForge, or downloaded as a 
[compressed tar archive](https://sourceforge.net/projects/plexil/files/) 
(or *tarball*), also from
SourceForge.

The tarball format includes several key programs and intermediate
products prebuilt, so that the user need not build everything from
scratch.  This is probably the best way to get one's feet wet with
%PLEXIL.

Tarball releases are infrequent, though.  If having the most recent
version of the %PLEXIL software is important, the user should prefer
the Git repository.

## Prerequisites for building PLEXIL

%PLEXIL software is built and tested by the development team on the
following platforms:

+ Linux: Ubuntu 18.04 and 20.04, including Windows Subsystem for Linux
  (WSL); Red Hat Enterprise Linux 7

+ macOS: 10.14, 10.15, 11.x

Most of the distribution should run with minimal modifications on
platforms conforming to the
[POSIX.1-2008 standards](https://pubs.opengroup.org/onlinepubs/9699919799.2008edition/).

The following software is necessary to build and run the tarball
distribution:

+ [GNU Make](https://www.gnu.org/software/make/).

+ C99- and C++98-compliant compilers.  The development team uses
  several versions of the
  [GNU Compiler Collection (GCC)](https://gcc.gnu.org/) and the 
  [LLVM Clang compiler](https://clang.llvm.org/).
  
+ A POSIX-compliant command shell interpreter, such as 
  [GNU bash](https://www.gnu.org/software/bash/),
  [Dash](https://wiki.archlinux.org/title/Dash), or
  [zsh](https://zsh.sourceforge.io/).  This list is not exhaustive.
  Shells such as csh and tcsh are not supported.

+ The Java programs require the Java 8 runtime environment (JRE) at a
  minimum.  The development team uses the OpenJDK 8 implementation.
  
The following additional software is required to build the %PLEXIL
suite from source:

+ Either the GNU Autotools
  ([m4](https://www.gnu.org/software/m4/m4.html),
  [libtool](https://www.gnu.org/software/libtool/libtool.html),
  [automake](https://www.gnu.org/software/automake/automake.html),
  and [autoconf](https://www.gnu.org/software/autoconf/) ); or

+ [CMake](https://cmake.org/) release 3.6 or newer.

+ [GNU gperf](https://www.gnu.org/software/gperf/) release 3.0.1 or
  newer.

+ The Java Development Kit (JDK), including a Java compiler, for Java
  8 or newer.

* [Apache ant](http://ant.apache.org/) release 1.8.x or newer.

## Building and installing PLEXIL

The easiest way to build and install the %PLEXIL suite is to download
the tarball, expand it, and use the top-level Makefile:

    tar xzf plexil-4.6.0.tar.gz
    cd plexil-4.6.0
    make

By default, the top-level Makefile builds the `universalExec` and
`testExec` command line applications, the IpcAdapter and UpdAdapter
interface adapters, the Standard %PLEXIL compiler, the Plexilscript
compiler, the %PLEXIL Checker, and the %PLEXIL Viewer.

If your application needs a customized build of the %PLEXIL Executive,
you can build it using either the GNU Autotools or CMake.  Which to
use depends on several factors, most notably:

+ Whether you are integrating the Executive into a program built using CMake;
+ Whether you intend to do an in-tree or out-of-tree build.

The CMake approach is recommended for integration into larger packages
built with CMake, and for out-of-tree builds.

The Autotools approach is more convenient for in-tree builds.
Out-of-tree builds are possible, but not well tested by the
development team.

Both approaches support cross-compilation.  Detailed configuration for
cross-compilation is beyond the scope of this "Getting Started"
document.
    
### Custom PLEXIL Executive builds using GNU Autotools

The Autotools build uses the familiar './configure; make; make
install' approach to building from source.  The top level Makefile
uses this approach to build the Executive.

The `configure` script is prebuilt in the tarball distribution.  If
you have cloned from the Git repository, first you will need to build the
`configure` script:

    cd plexil-4.6
    make src/configure

Once `configure` has been built, it's straightforward to configure and
build the Executive:

    mkdir -p /install/dir
    cd src
    ./configure --prefix=/install/dir ... options ...
    make
    make install

#### Options to the configure script

To get an up-to-date list of the options supported by `configure`,
simply type:

    ./configure --help

### Custom PLEXIL Executive builds using CMake

Building the Executive using CMake uses the standard approach:

    mkdir -p /build/dir /install/dir
    cd /build/dir
    cmake --install-prefix /install/dir ... options ... /downloads/plexil-4.6/src
    make
    make install

#### Options to CMake

To be supplied.

## Running example applications

## Writing PLEXIL plans

## Going further

If your goal is to integrate the %PLEXIL Executive into a larger
project, you will likely need to implement custom interfaces, or even
integrate the Executive into a larger body of C and C++ code.  The
[PLEXIL Executive Application Framework](group___app-_framework.html)
has the tools to help you accomplish these tasks.

