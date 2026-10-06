#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "onnxcc/ir/attribute.h"

namespace onnxcc {

// Node is one operator invocation in the graph. Plain data, no behaviour
// Large, because an Attribute can hold a Tensor. Always pass by const&
struct Node {
    // Guaranteed non-empty. The parser auto-generates a name if the ONNX file omits it because diagnostics and dump output need this
    std::string name;
    std::string op_type;
    // An empty string is an absent OPTIONAL input, not a missing entry.
    // Example - Conv without bias is {"x", "w", ""}. Every consumer must skip empty names
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::unordered_map<std::string, Attribute> attributes;
};
}