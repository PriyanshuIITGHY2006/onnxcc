#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "onnxcc/ir/status.h"
#include "onnxcc/operators/op_compiler.h"

namespace onnxcc {

// Maps op_type to its compiler and owns the compilers.
// No global instance. The engine owns one which avoids static initialisation order problems and keeps tests able to build a registry with two operators in it.
// Not thread safe. Registration happens once at startup before anything runs.
class OpRegistry {
public:
    // Rejects duplicates with an error Status. By value so callers can move a temporary it becomes the map key.
    Status register_op(std::string op_type, std::unique_ptr<OpCompiler>);

    // Returns nullptr if the op_type is not registered.
    OpCompiler* get(std::string_view op_type) const;
    bool has(std::string_view op_type) const;

    // For "unsupported op" messages.
    // LIFETIME: views into the keys. Valid while the registry lives, unordered_map keeps element references stable across rehash.
    std::vector<std::string_view> registered() const;

private:
    std::unordered_map<std::string, std::unique_ptr<OpCompiler>> m_compilers;
};
}