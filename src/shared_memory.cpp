#include <cutty/shared_memory.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace cy = cutty;

cy::shared_memory::shared_memory() : m_data(0), m_size(0), m_fd(-1)
{
}

cy::shared_memory::~shared_memory()
{
    close();
}

void cy::shared_memory::close()
{
    if(m_data)
    {
        msync(m_data, m_size, MS_SYNC);
        munmap(m_data, m_size);
        ::close(m_fd);
        m_data = 0;
        m_size = 0;
        m_fd = -1;
    }
}

cy::shared_memory::shared_memory(const char *filename, std::error_code&ec, int flags, size_type initial_size, void *hint) :
    shared_memory()
{
    int fd_flags = 0;
    if (flags & create) fd_flags |= O_CREAT;
    if (flags & readonly) fd_flags |= O_RDONLY; else fd_flags |= O_RDWR;
    auto fd = open(filename, fd_flags);

    if(fd == -1)
    {
        ec = {errno, std::generic_category()};
        return;
    }

    struct stat st;
    fstat(fd, &st);
    size_type mapped_size = st.st_size;

    if(mapped_size < initial_size)
    {
        if(ftruncate(fd, initial_size))
        {
            // resize failed
            ec = { errno, std::generic_category() };
            ::close(fd);
            return;
        }
        mapped_size = initial_size;
    }

    int prot_flags = flags & readonly ? PROT_READ : PROT_READ|PROT_WRITE;
    int map_flags = MAP_SHARED;
    auto data = mmap(hint, mapped_size, prot_flags, map_flags, fd, 0);

    if(data == MAP_FAILED)
    {
        ec = {errno, std::generic_category()};
        ::close(fd);
        return;
    }

    // All good
    m_fd = fd;
    m_size = mapped_size;
    m_data = data;
}
