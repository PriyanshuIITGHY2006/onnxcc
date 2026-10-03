#pragma once
#include <string>
#include <optional>

namespace onnxcc::cli {
    // A struct to hold the parsed command line arguments
    struct ParseResult {
        bool should_exit = false;
        int exit_code = 0;

        std::string command = "";
        std::string model_path = "";
        bool show_graph = false;
        bool verbose = false;
    };

    // Main entry point for the CLI parsing
    ParseResult parse_arguments(int argc, char* argv[]);
} // namespace onnxcc::cli