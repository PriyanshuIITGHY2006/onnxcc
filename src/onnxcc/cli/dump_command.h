#pragma once

#include <iosfwd>

namespace onnxcc::cli {
    // Handles `onnxcc dump ...`.
    // argc/argv start at the subcommand itself, so argv[0] is "dump" and argv[1] is its
    // first option. cxxopts wants that shape, since it treats argv[0] as the program name.
    // Normal output goes to `out`, errors to `err`. Returns an exit code from exit_codes.h.
    int run_dump(int argc, const char* const argv[], std::ostream& out, std::ostream& err);
}
