#pragma once

// Process exit codes returned by every onnxcc subcommand.
// 0 : success, 1 : command valid but failed, 2 : command was used wrong

namespace onnxcc::cli {
    inline constexpr int kExitOk = 0;
    inline constexpr int kExitFailure = 1;
    inline constexpr int kExitUsage = 2;
}
