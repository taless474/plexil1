\page data-structure-templates Data structure templates

\namespace PLEXIL
\ingroup Utils

The %PLEXIL Executive makes heavy use of the C++ Standard Library
container templates.  But for some specialized purposes, the standard
library containers are less than optimal.

These container class templates serve important roles in the Executive
implementation.

+ File LinkedQueue.hh

  - `template <typename T> class LinkedQueue`

    A classic singly-linked queue with head and tail pointers.  The
    type `T` must provide member functions `T *next()` and `const T
    **nextPtr()`.  This representation occupies constant space, and
    enqueueing and dequeueing take constant time, unlike comparable
    C++ standard containers.  It is used in several key data
    structures inside the Executive core.

  - `template <typename T, typename Compare = std::less<T> > class PriorityQueue`

    Derived from `LinkedQueue<T>`, this container is sorted in
    nondecreasing order as determined by the `Compare` class.
    Enqueueing is O(n) due to the need to traverse the list; however,
    dequeueing is still constant-time, and aside from an additional
    vtable pointer, it occupies no more space than `LinkedQueue<T>`.
    
    Not currently used in the %PLEXIL 4.6 implementation, but performs
    a key role in the %PLEXIL 6 Executive.

+ File SimpleSet.hh

  - `template <typename VALUE_TYPE, class COMP = std::less<VALUE_TYPE> > class SimpleSet`
 
    A set which stores its members in nondecreasing sorted order using
    `std::vector`.  It occupies O(n) space.  Membership test is
    implemented as binary search, therefore O(log2(n)) in time.  More
    importantly, there is no pointer chasing involved because of the
    contiguous storage.  There is some additional time overhead for
    insertion, due to `std::vector::insert()` possibly needing to
    expand storage, and likely needing to move all members after the
    insertion point.
   
    Especially useful in applications where the set is of modest size,
    and populated once, but referenced many times.

+ File SimpleMap.hh

  - `template <typename KEY_TYPE> struct SimpleKeyComparator`

    A comparator class template specialized by key type, for sorting
    and inserting keys in SimpleMap.

  - `template <typename KEY_TYPE, typename INDEX_TYPE> struct SimpleIndexComparator`

    A comparator class template specialized by key and index types,
    for lookups in SimpleMap.

  - `template <typename KEY_TYPE, typename VALUE_TYPE, class KEY_COMP = SimpleKeyComparator<KEY_TYPE> > class SimpleMap`
  
    A key-value mapping.  Key-value pairs are stored in sorted order
    and retrieved via binary search.  Like SimpleMap, optimized for
    applications where the map is built once but referenced many
    times.  Used in multiple applications in the Executive's plan
    parser.

+ File map-utils.hh

  - `struct CStringComparator`

    A comparator object, meant to be used with `SimpleMap`,
    specialized for comparing instances of `std::string` to
    null-terminated C strings.
