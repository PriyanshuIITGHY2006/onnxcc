#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "onnxcc/ir/tensor.h"

namespace onnxcc {

// Forward declared to avoid a circular dependency with node.h
struct Node;

// An ONNX attribute is an operator parameter. Conv wants kernel_shape, strides, pads, group
// BatchNormalization wants epsilon. float not double, ONNX AttributeProto has no double field
using Attribute = std::variant<std::int64_t, float, std::string, std::vector<std::int64_t>, std::vector<float>, Tensor>;

// The three functions are defined in attribute.cpp, so that file must explicitly instantiate each of them for all six alternatives. Any other T is a link error, which is the required answer - it is not an attribute type
// A type mismatch must report node name, op_type, attribute name and both types. Do not let std::bad_variant_access escape

// nullopt when the attribute is absent
template <typename T>
std::optional<T> get_attr(const Node&, std::string_view name);

// fallback when absent. The one operators use most
template <typename T>
T get_attr_or(const Node&, std::string_view name, T fallback);

// throws std::runtime_error naming node, op_type and attribute
template <typename T>
T get_attr_required(const Node&, std::string_view name);
}
