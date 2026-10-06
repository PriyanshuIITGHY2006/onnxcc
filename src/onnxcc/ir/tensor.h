#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "onnxcc/ir/datatype.h"
#include "onnxcc/ir/tensor_shape.h"

namespace onnxcc {

// DO NOT include attribute.h here. Tensor is one alternative of the Attribute variant so attribute.h includes this file. Including it back is a cycle
// Copyable on purpose, because Attribute needs it. Pass by const& anyway, an initializer can be tens of MB
struct Tensor {
    std::string name;
    TensorShape shape;
    DataType dtype = DataType::UNDEFINED;
    // No alignment guarantee, unlike arena memory. TensorView::over_tensor must not assume 64 byte alignment
    std::vector<std::uint8_t> data;
    // is_initializer is the truth, not data.empty(). A zero element initializer is still an initializer
    bool is_initializer = false;
};
}
