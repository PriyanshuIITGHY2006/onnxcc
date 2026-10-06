#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "onnxcc/ir/node.h"
#include "onnxcc/ir/tensor.h"
#include "onnxcc/ir/tensor_shape.h"

namespace onnxcc {

// Represents a model. The parser fills it, and later stages operate on it
// Do not add methods. Producer/consumer maps, topological order and shape inference are all
// computed differently and have their own lifetimes, so they live elsewhere
class Graph {
public:
    // Declaring the moves suppresses the implicit default constructor, so it is defined explicitly
    Graph() = default;
    // Graphs can be very large so disallow copy
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    // Moves leave the source graph empty
    Graph(Graph&&) noexcept;
    Graph& operator=(Graph&&) noexcept;

    // Returns nullptr if the name is not found
    // The returned pointer is invalidated if the vector is modified
    const Tensor* find_initializer(std::string_view) const;
    const Tensor* find_value_info(std::string_view) const;

    std::string name;
    std::int64_t ir_version = 0;
    std::int64_t opset_version = 0;
    std::vector<Tensor> inputs, outputs, initializers;
    // Tensor and not TensorShape. A shape has no name and we have to lookup by name
    std::vector<Tensor> value_info;
    std::vector<Node> nodes;
};

using TensorShapeMap = std::unordered_map<std::string, TensorShape>;
}