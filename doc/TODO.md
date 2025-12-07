# Next steps
- Fix the bug in speed conversion

- Convert speeds etc 
- Convert speeds with different units
- Add tests for mph conversions
- Tidy up text of units e.g. plurals and per hour.
- Auto print unit using the type name

- tagged<bool, plurality>


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

Delta types:
- delta<celcius>
- delta<time>

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

# Test suite library

```
test_suite("foo", []
{

});
```

# Documentation
Satellite article

# Persist
- shared_memory working on Windows
- Test create_new?
- Test also on Linux
- Rewrite persist so that the pointer is to map_file and not shared_record
- Can be inline
- Think about allocators, for example we moved the file descriptor into the shared memory. Could we use satellite instead?
- Problem is growing the heap.

## Headers that are in combination
For example <iostream> + <string> = <iostream_string>

# Tags
- [ ] Ensure all units are singular, for example second, meter, 