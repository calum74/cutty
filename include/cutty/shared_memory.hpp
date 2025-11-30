#pragma once

#include <cstdint>
#include <system_error>

namespace cutty
{
/// Creates or opens a mapped view of a file
/// The entire file is mapped as a contiguous region
class shared_memory
{
  public:
    using size_type = std::uint64_t;

    enum flags
    {
        create=1,
        readonly=2,
        anonymous=4,
        shared=8
    };

    /// Creates an empty shared_memory
    shared_memory();

    /// Attempts to open or create shared memory
    /// If the operation failed, the shared_memory is empty and ec contains the error code
    /// If the operation succeeds, then the size() is the size of the file, not initial_length
    shared_memory(const char *filename, std::error_code&ec, int flags = create, size_type initial_size = 0, void *hint = 0x0);

    // Moves shared memory object, leaving src in an invalid state
    shared_memory(shared_memory &&src);
    shared_memory &operator=(shared_memory &&);

    ~shared_memory();

    /// Returns the current data, or nullptr if empty
    void *data()
    {
        return m_data;
    }

    const void *data() const
    {
        return m_data;
    }


    explicit operator bool() const
    {
        return data();
    }

    /// Returns the size of the mapped data, or 0 if empty
    size_type size() const
    {
        return m_size;
    }

    void resize(size_type, std::error_code&ec);

    /// Closes the file if open, otherwise does nothing
    void close();

  private:
    void *m_data;
    size_type m_size;
    int m_fd;
    void *m_handle;
};
} // namespace cutty