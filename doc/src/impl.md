# The PLEXIL Executive Implementation {#impl}

[TOC]

## Overview

This document presents the internal workings of the %PLEXIL Executive.
The Executive is primarily coded in C++, specifically in the C++98
standard dialect.  It is broken into sections which roughly correspond
to the source code directory structure.

* [Utilities](group___utils.html)

These are generally low-level facilities for common tasks.

* [Values](group___values.html)

How the %PLEXIL Executive represents values internally.

* [Expressions](@ref expressions)

How expressions in the plan are represented.

* [An internal look at external interfaces](@ref commands-lookups)

How Commands and Lookups are represented internally to the Executive.

* [Nodes](@ref nodes)

How %PLEXIL Nodes are represented.

* [The Executive core](@ref exec-core)

Inner workings of the Executive's central algorithms and structures.

* [The plan parser](@ref parser)

How a plan goes from an XML file to its internal representation.

* [Application Framework](group___app-_framework.html)

The main loop and interfacing facilities of the Universal Executive.

* [Standard interface library](@ref interface-library)

The standard interfaces provided with the %PLEXIL Executive.
