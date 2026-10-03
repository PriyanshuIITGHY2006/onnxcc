#include <iostream>
#include "parser.hpp"
#include "../third_party/cxxopts.hpp"

namespace onnxcc::cli {
    ParseResult parse_arguments(int argc, char* argv[]) {
        ParseResult result;

        // onnxcc (no arguments) -> exit non-zero; usage on stderr
        if (argc == 1) {
            std::cerr << "Too few arguments.\n";
            std::cerr << "Usage: onnxcc <command> [options]\n";
            std::cerr << "Available commands: dump\n";
            std::cerr << "Try 'onnxcc --help' for more information.\n";
            result.should_exit = true;
            result.exit_code = 1;
            return result;
        }

        std::string first_arg = argv[1];

        // onnxcc --help -> prints top-level usage listing available subcommands; exit 0 
        if (first_arg == "--help") {
            std::cout << "onnxcc - C++ ONNX Model Compiler\n\n";
            std::cout << "Usage: onnxcc <command> [options]\n";
            std::cout << "Commands:\n"; 
            std::cout << "  dump        Dump ONNX model information\n";
            // Future commands will be added here
            result.should_exit = true;
            result.exit_code = 0;
            return result;
        }

        // Route based on subcommand
        if (first_arg == "dump") {
            result.command = "dump";
            cxxopts::Options options("onnxcc dump", "Dump ONNX model details");
            options.add_options()
                ("help", "Print usage for dump")
                ("model", "Path to the ONNX file", cxxopts::value<std::string>())
                ("show-graph", "Display the computation graph", cxxopts::value<bool>()->default_value("false"))
                ("verbose", "Enable verbose output", cxxopts::value<bool>()->default_value("false"));
            
            try {
                auto parsed = options.parse(argc, argv);

                // onnxcc dump --help -> prints dump usage, exit 0
                if (parsed.count("help")) {
                    std::cout << options.help() << "\n";
                    result.should_exit = true;
                    result.exit_code = 0;
                    return result;
                }

                // onnxcc dump (no --model) -> exit non-zero; readable error on stderr
                if (!parsed.count("model")) {
                    std::cerr << "Error: The --model argument is required for the 'dump' command.\n";
                    std::cerr << options.help() << "\n";
                    result.should_exit = true;
                    result.exit_code = 1;
                    return result;
                }

                result.model_path = parsed["model"].as<std::string>();
                result.show_graph = parsed["show-graph"].as<bool>();
                result.verbose = parsed["verbose"].as<bool>();
            } 
            catch (const cxxopts::exceptions::exception& e) {
                std::cerr << "Error parsing options: " << e.what() << "\n";
                result.should_exit = true;
                result.exit_code = 1;
            }
        } else {
            // onnxcc bogus -> Exit non-zero; readable error on stderr naming bad subcommand
            std::cerr << "Error: Unrecognized command '" << first_arg << "'\n";
            std::cerr << "Try 'onnxcc --help' for a list of available commands.\n";
            result.should_exit = true;
            result.exit_code = 1;
        }

        return result;
    }
} // namespace onnxcc::cli