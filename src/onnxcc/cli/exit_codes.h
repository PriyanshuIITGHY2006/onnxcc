#pragma once

// 0 ok, 1 command failed, 2 used wrong. same split bash and gnu tools use
namespace onnxcc::cli {
    inline constexpr int kExitOk = 0;
    inline constexpr int kExitFailure = 1;
    inline constexpr int kExitUsage = 2;
}
