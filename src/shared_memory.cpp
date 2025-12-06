#include <cutty/shared_memory.hpp>

#include <cutty/print.hpp>

#if defined(__linux__)
#define HAVE_MREMAP 1
#define DEFAULT_ADDRESS 0x600000000000
#else
#define HAVE_MREMAP 0
#define DEFAULT_ADDRESS 0x0
#endif

#if WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace cy = cutty;

cy::shared_memory::shared_memory() : m_data(0), m_size(0), m_fd(-1)
{
#if WIN32
    m_map_handle = INVALID_HANDLE_VALUE;
    m_file_handle = INVALID_HANDLE_VALUE;
#endif
}

cy::shared_memory &cy::shared_memory::operator=(cy::shared_memory &&src)
{
    close();

    m_data = src.m_data;
    m_size = src.m_size;
    m_fd = src.m_fd;
    m_map_handle = src.m_map_handle;
    m_file_handle = src.m_file_handle;
    m_map_flags = src.m_map_flags;

    src.m_data = 0;
    src.m_size = 0;
    src.m_fd = -1;

#if WIN32
    src.m_map_handle = INVALID_HANDLE_VALUE;
    src.m_file_handle = INVALID_HANDLE_VALUE;
#endif

    return *this;
}

cy::shared_memory::~shared_memory()
{
    close();
}

void cy::shared_memory::close()
{
    if (m_data)
    {
#if WIN32
        UnmapViewOfFile(m_data);
        CloseHandle(m_map_handle);
        CloseHandle(m_file_handle);
        m_file_handle = INVALID_HANDLE_VALUE;
        m_map_handle = INVALID_HANDLE_VALUE;
#else
        msync(m_data, m_size, MS_SYNC);
        munmap(m_data, m_size);
        ::close(m_fd);
#endif
        m_data = 0;
        m_size = 0;
        m_fd = -1;
    }
}

cy::shared_memory::shared_memory(const char *filename, std::error_code &ec, int flags, size_type initial_size,
                                 void *hint)
    : shared_memory()
{
#if WIN32

    int open_flags = (flags & trunc) ? CREATE_ALWAYS : OPEN_ALWAYS;

    auto hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, open_flags,
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, 0);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        ec = {int(GetLastError()), std::system_category()};
        return;
    }

    // Get the size of the file
    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        ec = {int(GetLastError()), std::system_category()};
        CloseHandle(hFile);
        return;
    }

    if (initial_size < size.QuadPart)
    {
        initial_size = size.QuadPart;
    }

    if (flags & readonly)
        m_map_flags = PAGE_READONLY;
    else if (flags & Private)
        m_map_flags = PAGE_WRITECOPY;
    else
        m_map_flags = PAGE_READWRITE;

    auto hMapFile = CreateFileMapping(hFile, 0, m_map_flags, 0, initial_size, 0);

    if (!hMapFile)
    {
        ec = {int(GetLastError()), std::system_category()};
        CloseHandle(hFile);
        return;
    }

    auto p = MapViewOfFileEx(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, initial_size, hint);

    if (!p)
    {
        ec = {int(GetLastError()), std::system_category()};
        CloseHandle(hMapFile);
        return;
    }

    m_data = p;
    m_size = initial_size;
    m_file_handle = hFile;
    m_map_handle = hMapFile;

#else
    int fd_flags = 0;
    if (flags & create)
        fd_flags |= O_CREAT;
    if (flags & readonly)
        fd_flags |= O_RDONLY;
    else
        fd_flags |= O_RDWR;
    if (flags & exclusive)
        fd_flags |= O_EXCL;
    if (flags & trunc)
        fd_flags |= O_TRUNC;

    int fd = -1;
    size_type mapped_size = initial_size;

    fd = open(filename, fd_flags, 0600); // ?? Permissions

    if (fd < 0)
    {
        ec = {errno, std::generic_category()};
        return;
    }

    struct stat st;
    fstat(fd, &st);
    mapped_size = st.st_size;

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
    m_map_flags = MAP_SHARED;

    if(!hint) hint = (void*)DEFAULT_ADDRESS;

    auto data = mmap(hint, mapped_size, prot_flags, m_map_flags, fd, 0);

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
#endif
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
#if WIN32
        UnmapViewOfFile(m_data);

        CloseHandle(m_map_handle);
        m_map_handle = CreateFileMapping(m_file_handle, 0, m_map_flags, 0, mapped_size, 0);

        auto p = MapViewOfFileEx(m_map_handle, FILE_MAP_ALL_ACCESS, 0, 0, mapped_size, m_data);

        if (!p)
        {
            ec = {int(GetLastError()), std::system_category()};
            close();
            return; // Failed
        }

        m_data = p;
        m_size = mapped_size;

#else

#if HAVE_MREMAP
        auto data = mremap(m_data, m_size, mapped_size, MREMAP_MAYMOVE, 0);
#else
        munmap(m_data, m_size);
        auto data = mmap(m_data, mapped_size, PROT_READ | PROT_WRITE, m_map_flags, m_fd, m_data);
#endif

        // TODO: We need to implement a "pinned" flag here

        if (data == MAP_FAILED)
        {
            ec = {errno, std::generic_category()};
            print(ec.message());
            close();
            return;
        }
        m_data = data;
        m_size = mapped_size;
#endif
    }
}

bool cy::shared_memory::truncate(std::error_code &ec, size_type new_size)
{
#if WIN32

    LARGE_INTEGER li;
    li.QuadPart = new_size;
    if (!SetFilePointerEx(m_file_handle, li, nullptr, FILE_BEGIN))
    {
        ec = {int(GetLastError()), std::system_category()};
        return true;
    }

    // Set the end of file at the current pointer position
    if (!SetEndOfFile(m_file_handle))
    {
        ec = {int(GetLastError()), std::system_category()};
        return true;
    }

    return false;
#else
    if (ftruncate(m_fd, new_size))
    {
        // resize failed
        ec = {errno, std::generic_category()};
        return true;
    }
    return false;
#endif
}

void cy::shared_memory::reserve(std::error_code &ec, size_type new_min)
{
    size_type mapped_size = get_size();

    if (mapped_size < new_min)
    {
        // Need to extend the file
        if (truncate(ec, new_min))
        {
            return;
        }
        mapped_size = new_min;
    }
    remap(ec, mapped_size);
}

cy::shared_memory::size_type cy::shared_memory::get_size() const
{
#if WIN32
    LARGE_INTEGER size;
    GetFileSizeEx(m_file_handle, &size);
    return size.QuadPart;
#else
    struct stat st;
    fstat(m_fd, &st);
    return st.st_size;
#endif
}

void cy::shared_memory::unmap()
{
    if (m_data)
    {

#if WIN32
        UnmapViewOfFile(m_data);
        m_data = 0;
        m_size = 0;
        CloseHandle(m_map_handle);
        m_map_handle = INVALID_HANDLE_VALUE;
#else
#endif
    }
}

void cy::shared_memory::resize(std::error_code &ec, size_type new_size)
{
    if (get_size() != new_size)
    {
        unmap();
        // Need to extend or shrink the file
        if (truncate(ec, new_size))
        {
            return;
        }
        remap(ec, new_size);
    }
}

void cy::shared_memory::reopen_at(std::error_code &ec, void *new_address)
{
    if (new_address != m_data)
    {
#if WIN32
        UnmapViewOfFile(m_data);

        auto p = MapViewOfFileEx(m_map_handle, FILE_MAP_ALL_ACCESS, 0, 0, m_size, new_address);

        if (!p)
        {
            ec = {int(GetLastError()), std::system_category()};
            close();
            return; // Failed
        }

        m_data = p;

#else

#if HAVE_MREMAP
        auto data = mremap(m_data, m_size, m_size, MREMAP_MAYMOVE | MREMAP_FIXED, new_address);
#else
        // !! TODO: Need to respect the map flags here
        // ?? Should we use MAP_FIXED here and fail

        // !! This really needs a remap as it will just lose all existing data
        munmap(m_data, m_size);
        int map_flags = MAP_SHARED | MAP_FIXED;
        if (m_fd < 0)
            map_flags |= MAP_ANON;
        auto data = mmap(new_address, m_size, PROT_READ | PROT_WRITE, map_flags, m_fd, 0);
#endif
        if (data == MAP_FAILED)
        {
            ec = {errno, std::generic_category()};
            close();
            return;
        }
        m_data = data;
#endif
    }
}