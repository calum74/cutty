#include <cutty/shared_memory.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

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
    if (m_data)
    {
        msync(m_data, m_size, MS_SYNC);
        munmap(m_data, m_size);
        ::close(m_fd);
        m_data = 0;
        m_size = 0;
        m_fd = -1;
    }
}

cy::shared_memory::shared_memory(const char *filename, std::error_code &ec, int flags, size_type initial_size,
                                 void *hint)
    : shared_memory()
{
    int fd_flags = 0;
    if (flags & create)
        fd_flags |= O_CREAT;
    if (flags & readonly)
        fd_flags |= O_RDONLY;
    else
        fd_flags |= O_RDWR;
    if (flags & exclusive)
        fd_flags |= O_EXCL;

    int fd = open(filename, fd_flags, 0600); // ?? Permissions

    if (fd < 0)
    {
        ec = {errno, std::generic_category()};
        // std::cout << "Failed to open file! " << ec.message() << std::endl;
        return;
    }

    struct stat st;
    fstat(fd, &st);
    size_type mapped_size = st.st_size;

    if (mapped_size < initial_size)
    {
        if (ftruncate(fd, initial_size))
        {
            // resize failed
            ec = {errno, std::generic_category()};
            ::close(fd);
            return;
        }
        mapped_size = initial_size;
    }

    int prot_flags = flags & readonly ? PROT_READ : PROT_READ | PROT_WRITE;
    int map_flags = MAP_SHARED;
    auto data = mmap(hint, mapped_size, prot_flags, map_flags, fd, 0);

    if (data == MAP_FAILED)
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

void cy::shared_memory::sync(std::error_code &ec)
{
    struct stat st;
    if (fstat(m_fd, &st))
    {
        ec = {errno, std::generic_category()};
        close(); // ??
        return;
    }
    remap(ec, st.st_size);
}

void cy::shared_memory::remap(std::error_code &ec, size_type mapped_size)
{
    if (m_size != mapped_size)
    {
        // Need to remap
        // !! TODO: Need to respect the map flags here
        munmap(m_data, m_size);
        auto data = mmap(m_data, mapped_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);

        if (data == MAP_FAILED)
        {
            ec = {errno, std::generic_category()};
            close();
            return;
        }
        m_data = data;
        m_size = mapped_size;
    }
}

void cy::shared_memory::reserve(std::error_code &ec, size_type new_min)
{
    struct stat st;
    fstat(m_fd, &st);
    size_type mapped_size = st.st_size;

    if (mapped_size < new_min)
    {
        // Need to extend the file
        if (ftruncate(m_fd, new_min))
        {
            // resize failed
            ec = {errno, std::generic_category()};
            return;
        }
        mapped_size = new_min;
    }
    remap(ec, mapped_size);
}

void cy::shared_memory::resize(std::error_code &ec, size_type new_min)
{
    struct stat st;
    fstat(m_fd, &st);
    size_type mapped_size = st.st_size;

    if (mapped_size != new_min)
    {
        // Need to extend the file
        if (ftruncate(m_fd, new_min))
        {
            // resize failed
            ec = {errno, std::generic_category()};
            return;
        }
        mapped_size = new_min;
    }
    remap(ec, mapped_size);
}

void cy::shared_memory::reopen_at(std::error_code &ec, void *new_address)
{
    if (new_address != m_data)
    {
        // !! TODO: Need to respect the map flags here
        // ?? Should we use MAP_FIXED here and fail
        munmap(m_data, m_size);
        auto data = mmap(new_address, m_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, m_fd, 0);

        if (data == MAP_FAILED)
        {
            ec = {errno, std::generic_category()};
            close();
            return;
        }
        m_data = data;
    }
}