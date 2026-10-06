#pragma once

#include <memory>
#include <string_view>
#include <vector>
#include "onnxcc/ir/graph.h"
#include "onnxcc/ir/status.h"
#include "onnxcc/ir/tensor_shape.h"
#include "onnxcc/operators/op_kernel.h"

namespace onnxcc {

// validates a node and produces a kernel for it. One instance per op_type, owned by the registry and shared across every node of that type, which is why every method is const
// Call order is validate -> infer_shape -> compile. compile may assume validate returned ok
class OpCompiler {
public:
    virtual ~OpCompiler() = default;

    // recoverable error, the user's model is wrong. Names the node and the dimensions
    virtual Status validate(const Node&, const TensorShapeMap&) const = 0;

    virtual std::unique_ptr<OpKernel> compile(const Node&, const TensorShapeMap&) const = 0;

    // vector because many operators may return multiple tensors
    virtual std::vector<TensorShape> infer_shape(const Node&, const TensorShapeMap&) const = 0;

    // Returns the operator name
    virtual std::string_view name() const = 0;
};
}