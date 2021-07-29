# The plan parser {#parser}

This section describes how plans are read in as XML source and
converted to internal representations.

## XML representation choice

Some years ago, dissatisfied with the performance of the previous XML
representation package, the PLEXIL team surveyed the available XML
packages for C and C++.  [pugixml](https://pugixml.org/ "The pugixml
web site") stood out for its efficiency in both execution time and
memory space.  Its DOM APIs were designed to simplify extracting
information from an XML document.  We have been very happy with this
choice.

## The phases of parsing

Parsing of PLEXIL plans is performed in several phases.

### Reading the XML file

The XML is read from a file and converted into a pugixml DOM document.

### Checking

The DOM representation is scanned to check its validity.  The PLEXIL
XML parser does the vast majority of its validity checking in this
phase.

If the plan file is being read as a library node, the parsing process
stops here, and the DOM representation is stored for expansion in
subsequent library calls.  This is the primary motivation for
performing all validity checks on the DOM.

### Constructing the node tree

If plan checking is successful, the internal representations of the
node tree can be constructed.  Some references within the plan
cannot be resolved until the entire node tree has been constructed;
those are deferred to the next phase.

An error detected at this stage means the partially-built plan must be
deleted, another motivation for performing all checks prior to
constructing the internal representation.

### Finalizing the plan

This is the stage where all of the references between nodes are
resolved and finalized.  Once complete, the plan is handed off to the
Exec for execution.

Again, recovering from an error detected at this stage requires even
more cleanup work.
