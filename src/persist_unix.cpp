
#include <cutty/persist.hpp>

#include <iostream>  // tmp
using namespace std; // tmp

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

namespace cy = cutty;


// constructor::map_file
//
// Opens the file to get the file descriptor (fd).  Then calls mmap to map the file to memory.
// It first read the first part of the file, and the might need to remap the file to the 
// location specified by the first invocation of the map.

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

                munmap((char*)map_address, length);
                map_address = 0;
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

void cy::detail::shared_record::unmap()
{
    munmap((char*)this, current_size);
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
