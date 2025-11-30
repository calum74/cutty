// Copyright (C) Calum Grant 2003
// Copying permitted under the terms of the GNU Public Licence (GPL)

#include <cutty/persist.hpp>

#include <cassert>
#include <iostream>  // Debug only

namespace cy = cutty;

// Whether to reuse freed memory (yes, you want to do this)
#define RECYCLE 1 

// Whether to report memory allocations and deallocations
#define TRACE_ALLOCS 0

// Whether to include extra debugging information (slightly slower, bigger heap)
#define CHECK_MEM 0


// operator new
//
// Allocates space for one object in the shared memory

void *operator new(size_t size, cy::map_file &file)
{
    void *p = file.malloc(size);

    if(!p) throw std::bad_alloc();

    return p;
}


// operator delete
//
// Matches operator new.  Not used.

void operator delete(void *p, cy::map_file &file)
{
}



// object_cell
//
// "free_space" is a table of free blocks.  We round the size up using
// object_cell() into 64 discrete sizes, 4, 6, 8, 12, 16, 24, 32 ...
//
// Returns the cell number, and also rounds req_size up to the cell size

inline int object_cell(size_t &req_size)
{
    int cell=0;
    size_t cell_size=sizeof(void*);

    // TODO: A more efficient way of extracting just the top two bits of a number

    while(cell<64)  // NB that's just 32 bits!
    {
        size_t s0 = cell_size>>1;

        if(req_size <= cell_size) { req_size = cell_size; return cell; }
        cell++;        
        cell_size += s0; 

        if(req_size <= cell_size) { req_size = cell_size; return cell; }
        cell++;
        cell_size += s0;
    }

    return 0;   // Failure
}


// map_file::malloc
//
// Allocates an object of size @size from the shared memory
// If possible, use a block in the free_space instead of growing the heap.
// Mutexed, threadsafe - very important.

void *cy::map_file::malloc(size_t size)
{
    auto &d = data();
    if(size==0) return d.top;  // A valid address?  TODO

    d.lockMem();
    
    int free_cell = object_cell(size);

#if RECYCLE   
    if(d.free_space[free_cell])
    {
        // We have a free cell of the desired size

        void *block = d.free_space[free_cell];
        d.free_space[free_cell] = *(void**)block;

#if CHECK_MEM
        ((int*)block)[-1] = size;
#endif

#if TRACE_ALLOCS
        std::cout << " +" << block << "(" << size << ")";
#endif

        d.unlockMem();
        return block;
    }
#endif

#if CHECK_MEM
    *(int*)map_address->top = size;
    map_address->top += sizeof(int);
#endif

    void *t = d.top;
        
    auto new_top = d.top + size;

    if(new_top > d.end)
    {
        if(d.max_size <= d.current_size || !extend_to(new_top))
        {
            d.unlockMem();
            return nullptr;
        }
    }

    d.top = new_top;

#if TRACE_ALLOCS
    std::cout << " +" << t << "(" << size << ")";
#endif

    d.unlockMem();
    return t;
}


// map_file::free
//
// Marks the given memory block as "free"
// Free blocks are stored in a linked list, starting at the vector free_cell.
// The minimum allocation size is 4 bytes to accomodate the pointer

void cy::detail::shared_record::free(void* block, size_t size)
{
    lockMem();

#if TRACE_ALLOCS
    std::cout << " -" << block << "(" << size << ")";
#endif
    if(size==0) return;  // Do nothing    

    if(block < this || block >= end)
    {
        // We have attempted to "free" data not allocated by this memory manager
        // This is a serious fault, but we carry on

        std::cout << "Block out of range!\n";  // This is a serious error!

        // This happens in basic_string...
        unlockMem();
        return;
    }

    assert(block>=this && block<end);
        // This means that the address is not managed by this heap!

#if CHECK_MEM
        assert(((int*)block)[-1] == size);
        ((int*)block)[-1] = 0;  // This is now DEAD!
#endif

#if RECYCLE   // Enable this to enable block to be reused
    int free_cell = object_cell(size);
    // free_cell is the cell number for blocks of size "size"

    // Add the free block to the linked list in free_space
    *(void**)block = free_space[free_cell];
    free_space[free_cell] = block;
#endif

    unlockMem();
}

// map_file::root
//
// Returns a pointer to the first object in the heap.

void *cy::detail::shared_record::root()
{
    return this+1;
}

const void *cy::detail::shared_record::root() const
{
    return this+1;
}


// map_file::empty
//
// Returns true of the heap is empty - no objects have been allocated.
// This tells us if we need to construct a root object.

bool cy::detail::shared_record::empty() const
{
    return root() == top;  // No objects allocated
}

void cy::detail::shared_record::clear()
{
    top = (char*)root();
    for(int i=0; i<64; ++i)
        free_space[i] = nullptr;
}

size_t cy::detail::shared_record::capacity() const
{
    return (end-top) + (max_size - current_size);
}

size_t cy::detail::shared_record::size() const
{
    return top-(char*)root();
}

size_t cy::detail::shared_record::limit() const
{
    return max_size;
}

void cy::detail::shared_record::limit(size_t size)
{
    max_size = size;
}

cy::InvalidVersion::InvalidVersion() : std::runtime_error("Version number mismatch")
{
}


cy::map_file::map_file(const char *filename, int applicationId, short majorVersion, short minorVersion,
                   size_t length, size_t limit, int flags, size_t base)
{
    open(filename, applicationId, majorVersion, minorVersion, length, limit, flags, base);
}

void cy::map_file::open(const char *filename,  int applicationId, short majorVersion, short minorVersion, size_t length, size_t limit, int flags, size_t base)
{
    close();
    
    const int persistMagic = 0x99a10f0f;
    const int hardwareId = 0x00000001;
    
    std::error_code ec;
    int sh_flags = 0;
    if (flags & create_new)
    {
        sh_flags |= shared_memory::create | shared_memory::trunc;
    }
    else
    {
        sh_flags = shared_memory::create;
    }
    shared_memory mem(filename, ec, sh_flags, length, (void*)base);

    detail::shared_record *map_address = (detail::shared_record*)mem.data();

    if(mem)
    {
        // Remap the file according to the specifications in the header file

        void *previous_address = map_address->address;
        size_t previous_length = map_address->current_size;
        
        if(previous_address && (previous_length != length || previous_address != map_address))
        {
            mem.reopen_at(ec, previous_address);
            detail::shared_record *map_address = (detail::shared_record*)mem.data();
        }
    }

    if(map_address)
    {
        if(map_address->address)
        {
            // Check the versions
            if(map_address->magic != persistMagic ||
               map_address->applicationId != applicationId ||
               map_address->hardwareId != hardwareId ||
               map_address->majorVersion != majorVersion ||
               map_address->minorVersion != minorVersion)
            {
                close();
                throw InvalidVersion();
            }
            
            if(map_address->address != map_address)
            {
                // This is a failure!
                map_address = 0;
                return;
            }
        }
        else
        {
            // This is a new address
            map_address->address = map_address;
            map_address->current_size = length;
            map_address->max_size = limit;
            map_address->end = (char*)map_address + length;
            map_address->top = (char*)map_address->root();
            map_address->magic = persistMagic;
            map_address->applicationId = applicationId;
            map_address->hardwareId = hardwareId;
            map_address->majorVersion = majorVersion;
            map_address->minorVersion = minorVersion;

            new(&map_address->extra.mem_mutex) std::mutex();
            new(&map_address->extra.user_mutex) std::mutex();

            // This is not needed
            for(int i=0; i<64; ++i) map_address->free_space[i] = 0;
        }
    }
    memory = std::move(mem);

    // Report on where it ended up
    // std::cout << "Mapped to " << map_address << std::endl;
}


cy::map_file::~map_file()
{
}

void cy::map_file::close()
{
    memory.close();
}

bool cy::map_file::extend_to(void * new_top)
{
    auto &d = data();

    if(d.current_size == d.max_size) return false;
    
    // assert(map_address == base_address);
    size_t old_length = d.current_size;
    size_t new_length = old_length + (old_length>>1);
    size_t min_length = (char*)new_top - (char*)&d;
    
    
    while(new_length < d.max_size && new_length < min_length)
        new_length += (new_length>>1);

    if(new_length > d.max_size)
        new_length = d.max_size;

    if(new_length < min_length) return false;

    std::error_code ec;
    memory.reserve(ec, new_length-1);  // ?? Should we try to recover?

    if(!memory) return false;  // Catastrophe

    assert(memory.data() == (char*)&d);
    data().current_size = new_length;
    data().end = (char*)&data() + new_length;
    return true;
}


bool cy::detail::shared_record::lock(int ms)
{
    extra.user_mutex.lock();
    return true;
}


void cy::detail::shared_record::unlock()
{
    extra.user_mutex.unlock();
}

void cy::detail::shared_record::lockMem()
{
    extra.mem_mutex.lock();
}


void cy::detail::shared_record::unlockMem()
{
    extra.mem_mutex.unlock();
}

cy::map_file::map_file()
{
}
