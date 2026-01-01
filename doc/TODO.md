# Dynamic
- Indexes working a lot better

# Units
- Unify scalar and dscalar
- Bug converting $/mile int o £/km
- Literals prevent mixins

# Tags
- [ ] Get rid of fraction, and use the builtin `kilo`, etc.

# What about a "indexer" that can be assignable

```c++
template<typename Container, typename Key>
struct index
{

};

index_type operator[](const key_type&);
```



# Docs
- [ ] Separator output

# Print
- [ ] Make it configurable
print_opts
- beg = ""
- sep = " "
- end = "\n"

- Compare with std::string.
- Test suite, docs, samples

# Tests
- [ ] Test should allow command line options
- Run specific tests, selected by number
- Specify a format string?

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

# Projects to add
Entropy
Numbers and traits
Parsing

# Persist
- Rewrite persist so that the pointer is to map_file and not shared_record
- Think about allocators, for example we moved the file descriptor into the shared memory. Could we use satellite instead?
- Problem is growing the heap.
