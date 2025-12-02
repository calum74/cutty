#pragma once

#include <cstdint>
#include <system_error>

namespace cutty
{
/**
    Manages a memory-mapped view of a file in a platform-independent way.

    The interface is fairly simple - you get a memory buffer to the entire file.
 */
class shared_memory
{
  public:
    using size_type = std::uint64_t;

    enum flags
    {
        create = 1,     /// File may be created if it does not exist
        readonly = 2,   /// File and contents are not writable
        Private = 4,    /// Changes are discarded
        exclusive = 8,  /// Ensure not open by anyone else
        pinned=16,      /// Fail if the specified address cannot be used
        trunc=32        /// Delete any existing file
    };

    /** Creates an empty shared_memory */
    shared_memory();

    /**
        Attempts to open or create shared memory.
        If the operation failed, the shared_memory is empty and ec contains the error code
        If the operation succeeds, then the size() is the size of the file, not initial_length
        If the file is empty or less than min_size in length, then the file is extended to the new size.

        Errors include:
        - the filename does not exist
        - the file could not be created
        - no space left on device
    */
    shared_memory(const char *filename, std::error_code &ec, int flags = create, size_type min_size = 0,
                  void *hint = 0x0);

    /**
        Moves shared memory object, leaving @p src in an invalid state.
     */
    shared_memory(shared_memory &&src);
    shared_memory &operator=(shared_memory &&src);

    ~shared_memory();

    /** Returns the current data, or nullptr if empty */
    void *data()
    {
        return m_data;
    }

    /** Returns the current data, or nullptr if empty */
    const void *data() const
    {
        return m_data;
    }

    /** Returns if this shared_memory is not empty. */
    explicit operator bool() const
    {
        return data();
    }

    /** Returns the size of the mapped data, or 0 if empty */
    size_type size() const
    {
        return m_size;
    }

    /**
        Resizes the current data to the current file size (if changed),
        or the new minimum size.
     */
    void reserve(std::error_code &ec, size_type min_size = 0);

    /**
        Resizes the current file if it has grown on disk
     */
    void sync (std::error_code &ec);

    /**
        Resizes (grows or shrinks) the current file to the new size.
     */
    void resize (std::error_code &ec, size_type new_size = 0);

    /**
        Attempts to map the memory at a specified address.
        If it fails, the object is left empty, and ec contains
        details of the error.
     */
    void reopen_at(std::error_code &ec, void *address);

    /**
        Closes the file if open, otherwise does nothing.
    */
    void close();

    /**
        Swaps this shared memory with another.
     */
    void swap(shared_memory &other);

  private:
    void *m_data;
    size_type m_size;
    int m_fd;
    void *m_file_handle, *m_map_handle;

    void remap(std::error_code &ec, size_type new_size);
    bool truncate(std::error_code &ec, size_type new_size);
    size_type get_size() const;
};
} // namespace cutty