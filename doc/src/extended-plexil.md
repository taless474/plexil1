# Extended %PLEXIL {#extended-plexil}

[Core PLEXIL](@ref core-plexil) has no higher-level control
structures.  `NodeList` Nodes are inherently concurrent; any
sequencing between sibling Nodes must be explicitly expressed as
conditions.

*Extended %PLEXIL* provides some common higher-level programming
constructs: implicit sequencing, with and without checks for success;
if-then-elseif-else; while-, do-while, and for-loops; and other less
common constructs unique to %PLEXIL.
