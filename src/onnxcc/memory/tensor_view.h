#pragma once

#include <cstddef>
#include <span>
#include "onnxcc/ir/datatype.h"
#include "onnxcc/ir/tensor_shape.h"

namespace onnxcc {

// Used by reference only
class MemoryArena;
struct Tensor;

class TensorView {
public:
    TensorView() = default;
    // Throws std::logic_error if the shape is dynamic. A view must have a concrete element count
    TensorView(std::byte* data, std::size_t size_bytes, TensorShape, DataType);

    static TensorView over_arena(MemoryArena&, const TensorShape&, DataType);
    // Initializers live in Graph, not the arena. Note this gives write access to model weights
    // treat initializer views as read only by convention
    static TensorView over_tensor(Tensor&);

    // Checks the dtype before creating the span
    // Throws std::logic_error if the type does not match(it is bug)
    // acts as a runtime type guard
    // defined in tensor_view.cpp, so that file must explicitly instantiate both overloads for float, double, std::int32_t, std::int64_t and bool. Any other T is a link error as<bool>() assumes sizeof(bool) == 1, which is what ONNX BOOL is
    template <typename T> std::span<T> as();
    // const overload for a const view. Does not make over_tensor views read only
    template <typename T> std::span<const T> as() const;

    const TensorShape& shape() const noexcept;
    DataType dtype() const noexcept;
    // Derived from the byte count. 0 for a default constructed view
    std::size_t num_elements() const noexcept;
    std::byte* data() noexcept;
    const std::byte* data() const noexcept;

private:
    std::byte* m_data = nullptr;
    std::size_t m_size_bytes = 0;
    TensorShape m_shape;
    DataType m_dtype = DataType::UNDEFINED;
};
} 