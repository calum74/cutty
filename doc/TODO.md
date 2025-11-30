# Project
Doxygen
Readme
Look into calumgrant account

# Sequence
Model range concepts
Deprecate filters etc.
Maybe based on mixins
Rename pointer_sequence
Maybe typedef to template<typename T> using sequence = const sequence_base<T>&
Output sequence is a bit weird as it's not a sequence.
In/out sequence?

# Tags
What about difference types
- temperature
- times and dates
Multiple conversions
Requirements and concepts
Addition
Multiplication
Complex numbers

Implement!
- temperature
- times
- policies on addition and multiplication
- complex numbers

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
rename macros
fix gcc build

We need a way to chain mixins

# Projects to add
Entropy
Numbers and traits
Parsing
Persist

# Test suite

# Documentation
Satellite article

# Persist
Need a better abstraction for shared memory.

```
shared_memory x(filename, flags, size=all, map_address_hint=all);
x.data()
x.size();

```
