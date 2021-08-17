# Values {#values}

\namespace PLEXIL
\addtogroup Values

The %PLEXIL language is nominally *statically typed*; that is, the
type of an expression in %PLEXIL must be explicitly declared, or be
derivable, at load time.

This is true of the restricted Safe %PLEXIL dialect.  However,
practical %PLEXIL plans must allow for some expression types to be
determined dynamically.  For example, the type of a Lookup whose state
name is a variable cannot be determined statically at load time in the
%PLEXIL language as defined in this release.

Further, expressions in %PLEXIL (e.g. uninitialized variables) can
take on the out-of-band value **UNKNOWN**, irrespective of their
declared or inferred types.

Therefore there is a need to represent the types of values at run
time, and a need for a polymorphic value representation.

## Value types in PLEXIL

The %PLEXIL language provides these user data types:

* \ref Boolean
* \ref Integer
* \ref Real
* \ref String
* \ref Array

Arrays are further subdivided by element type:

* \ref BooleanArray
* \ref IntegerArray
* \ref RealArray
* \ref StringArray

The %PLEXIL language also provides the following internal data types:

* \ref NodeState
* \ref NodeOutcome
* \ref FailureType
* \ref CommandHandleValue

## Value representations

The following C++ representations are used for these types in the
%PLEXIL Executive.

### Scalar types

Scalar (i.e. non-array) types are represented as follows:

* The \ref Boolean type is represented as a `bool`.
* The \ref Integer type is represented as an `int32_t`.
* The \ref Real type is represented as a `double`.
* The \ref String type is represented as a `std::string`.

### Internal types

Internal types \ref NodeState,
\ref NodeOutcome,
\ref FailureType, 
and \ref CommandHandleValue
are represented as enumerations. 

In %PLEXIL 4.6, these enumerations coexist as subdivisions within the
`uint16_t` integer type, because C++98 treats all enumeration types as
`int` values.  Validity checking functions help ensure that only valid
members of an enumeration are stored in a particular internal
variable.

In the forthcoming %PLEXIL 6 release, which requires C++11 or newer,
the C++ language treats expressions of an enumeration type as a
distinct type, and allows deriving enumeration types from specific
integer subtypes.  Therefore each of the above internal types has a
unique C++ representation.

### Array types

The API for arrays in the %PLEXIL Executive is defined by the 
\ref Array
abstract base class.  The \ref ArrayImpl
class template derives from Array,
and specializations are defined for each element type.

Within \ref Array
and \ref ArrayImpl, arrays are represented as
`std::vector` instances.

For convenience, typedefs are defined for each of the instantiable
array subtypes: \ref BooleanArray,
\ref IntegerArray,
\ref RealArray,
\ref StringArray.

### UNKNOWN

The distinguished value **UNKNOWN** has no literal representation in
%PLEXIL, nor is there a particular internal representation.  However,
the %PLEXIL language provides an `isKnown` predicate.  Internally, the
Value and Expression APIs implement `isKnown` functions, and provide
additional ways of determining whether an expression's value is known.

### The ValueType enumeration

The \ref ValueType enumeration represents the type of a particular
expression in the %PLEXIL language.  Functions taking ValueType as a
parameter can classify a given type value along several dimensions,
e.g:

* \ref isUserType
* \ref isInternalType
* \ref isNumericType
* \ref isScalarType
* \ref isArrayType

ValueType has a unique enumeration value for values and expressions
whose type is not yet known: `UNKNOWN_TYPE`.

## The Value class

The \ref Value
class can represent any value of any of the above types, including
**UNKNOWN**. 

