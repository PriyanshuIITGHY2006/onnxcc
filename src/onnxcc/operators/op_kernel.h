#pragma once

#include <vector>
#include "onnxcc/memory/tensor_view.h"

namespace onnxcc {

// executes one operator. Produced by OpCompiler::compile
// we do not generate code, each kernel is pre-compiled and execution walks the graph in topological order
// costs one virtual call per node, which is small next to the arithmetic and easier to debug
class OpKernel {
public:
    // Virtual, so that it does not bypass derived class destructors
    virtual ~OpKernel() = default;

    // validation happens before execution. Unexpected errors should throw
    // pointers allow outputs to be modified. Inputs are read-only by convention
    // cpp does not allow vector of references
    virtual void execute(const std::vector<TensorView*>& inputs, std::vector<TensorView*>& outputs) = 0;
};
}