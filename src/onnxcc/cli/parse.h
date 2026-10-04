#pragma once

#include <string>

#include "onnxcc/cli/exit_codes.h"

namespace onnxcc::cli {

    struct DumpOptions {
        std::string model;
        bool show_graph = false;
        bool verbose = false;
    };

    struct ParseResult {
        enum class Kind { Dump, Help, Error };

        Kind kind = Kind::Error;
        int exit_code = kExitUsage;
        std::string message;
        DumpOptions dump;
    };

    // prints nothing, so tests can check the result instead of scraping output
    ParseResult parse(int argc, const char* const argv[]);
}
