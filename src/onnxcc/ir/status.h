#pragma once

#include <utility>
#include <source_location>
#include <string>
#include <string_view>

namespace onnxcc {
// Error Handling
//
// 1. Status: Used for recoverable errors and validation failures. Examples are shape mismatch, unsupported operators, etc
// 2. Exceptions: Used for programmer bugs and unrecoverable state. Examples are calling dtype_size(UNDEFINED), memory allocator failure, etc

// encourages the compiler to issue a warning if a function's return value is ignored by the caller
class [[nodiscard]] Status {
private:
    explicit Status(std::string msg) : m_message(std::move(msg)) {}
    std::string m_message; 
    Status() = default;
public:
    // empty string is a success status
    // Rule of zero: Let compiler handle the things if not managing raw resources
    static Status ok_value() { return Status{}; }
    // Always embeds file:line, so an error message is never empty.
    // USAGE NOTE: To build messages with context, use std::format at the calling site
    // Example - Status::error(std::format("shape mismatch at node {}: expected {}", node.name, a.to_string()));
    static Status error(std::string_view, std::source_location loc = std::source_location::current());
    bool ok() const noexcept {
        return m_message.empty();
    }
    // Returns the error message with file:line context.
    // LIFETIME: This returns a view into the Status object's internal string.
    // The view is only valid for the lifetime of this Status object.
    std::string_view message() const noexcept {
        return m_message;
    }
};
}