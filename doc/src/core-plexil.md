# Core %PLEXIL {#core-plexil}

The %PLEXIL Executive directly implements the language known as *Core
%PLEXIL*.  Core %PLEXIL is XML-based, and has a *schema* which dictates
the legal constructs.  (The Executive can execute some plans which are
not strictly conformant to the schema.  Users should not depend on
this fact.)

Like most XML, Core %PLEXIL is not designed to be human-readable.  Most
users will not work directly with the Core %PLEXIL representations of
their plans.

However, Core %PLEXIL's XML base makes it a good target for mechanical
translations from other action representations.  Two of these are
[Extended PLEXIL](@ref extended-plexil), a streamlined XML
representation offering higher-level constructs, and
[Standard PLEXIL](@ref standard-plexil), whose C-like structure makes
it more accessible to users.

Many other source representations have been translated to Core %PLEXIL
and executed in real-world applications.
