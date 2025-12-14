# Documentation priorities
- Units
- Mixins
- Satellite
- Links to samples

# Units
- Make naming consistent
- Bug converting $/mile int o £/km
- Support lightyear and radian
- Support femto etc using doubles.
- Unit literals

# Satellite
- Think about the API a bit. We have mixed up the getter and setter API.
`auto _ satellite<T>::set(abc)`.
- The getter API could be easier, e.g. satellite<T>()
- Throw on null?

# Print
- Print containers etc.
- Make it configurable?
- Compare with string.
- Test suite, docs, samples

# Tests
- Should we have a "test case" local variable, and use satellite to specify it?

# Command line argument parser

# Sequence
Deprecate filters etc.
Maybe based on mixins
Rename pointer_sequence
Maybe typedef to template<typename T> using sequence = const sequence_base<T>&
Output sequence is a bit weird as it's not a sequence.
In/out sequence?
What about a movable sequence to ensure no reuse/reentry?

# Dynamic
Get map test passing
Finish stuff
Strings that are char*
Objects and dicts with string keys
Find a use case

# Pretty-type
Robustify it

# Mixins
Static mixins
Dynamic has mixins

# Projects to add
Entropy
Numbers and traits
Parsing

# Persist
- Rewrite persist so that the pointer is to map_file and not shared_record
- Think about allocators, for example we moved the file descriptor into the shared memory. Could we use satellite instead?
- Problem is growing the heap.
