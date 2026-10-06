#pragma once

#include <cstddef>

namespace onnxcc {

// Bump allocator backed by one 64-byte aligned buffer
class MemoryArena {
public:
    // total_bytes is rounded UP to a multiple of 64, because std::aligned_alloc is undefined when size is not a multiple of alignment. bytes_total() reports the rounded value
    // Throws std::bad_alloc if the underlying allocation fails
    explicit MemoryArena(std::size_t total_bytes);
    ~MemoryArena();

    MemoryArena(const MemoryArena&) = delete;
    MemoryArena& operator=(const MemoryArena&) = delete;    
    // Must leave the source buffer null, otherwise both destructors free it
    MemoryArena(MemoryArena&&) noexcept;
    MemoryArena& operator=(MemoryArena&&) noexcept;

    // Returns nullptr if the arena is exhausted or the request overflows
    // alignment must be a power of two
    // Zero-byte allocations return a valid pointer without advancing the offset
    void* allocate(std::size_t bytes, std::size_t alignment = 64);

    // Resets the allocation offset without freeing the buffer.
    void reset() noexcept;

    std::size_t bytes_used() const noexcept;
    // Peak usage is preserved across reset().
    std::size_t bytes_peak() const noexcept;
    std::size_t bytes_total() const noexcept;

private:
    std::byte* m_buffer = nullptr;
    std::size_t m_offset = 0;
    std::size_t m_total = 0;
    std::size_t m_peak = 0;
};
}