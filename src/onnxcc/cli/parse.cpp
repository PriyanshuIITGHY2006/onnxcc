#include "onnxcc/cli/parse.h"

#include <string_view>
#include <utility>

#include "onnxcc/third_party/cxxopts.hpp"

namespace onnxcc::cli {
    namespace {

        constexpr std::string_view kUsage =
            "usage: onnxcc <subcommand> [options]\n"
            "\n"
            "subcommands:\n"
            "  dump    print what is inside an onnx model\n"
            "\n"
            "options:\n"
            "  -h, --help    show this help and exit\n";

        cxxopts::Options dump_options() {
            cxxopts::Options options("onnxcc dump", "print what is inside an onnx model");
            options.add_options()
                ("model", "path to the .onnx model to read", cxxopts::value<std::string>(), "<path>")
                ("show-graph", "list the nodes of the graph", cxxopts::value<bool>()->default_value("false"))
                ("verbose", "print extra detail while working", cxxopts::value<bool>()->default_value("false"))
                ("h,help", "show this help and exit");
            return options;
        }

        ParseResult help(std::string text) {
            ParseResult result;
            result.kind = ParseResult::Kind::Help;
            result.exit_code = kExitOk;
            result.message = std::move(text);
            return result;
        }

        ParseResult error(std::string text) {
            ParseResult result;
            result.kind = ParseResult::Kind::Error;
            result.exit_code = kExitUsage;
            result.message = std::move(text);
            return result;
        }

        ParseResult parse_dump(int argc, const char* const argv[]) {
            cxxopts::Options options = dump_options();

            cxxopts::ParseResult parsed;
            try {
                parsed = options.parse(argc, argv);
            } catch (const cxxopts::exceptions::exception& bad_options) {
                return error("onnxcc dump: error: " + std::string(bad_options.what()) +
                             "\ntry 'onnxcc dump --help'\n");
            }

            if (parsed.count("help") > 0) {
                return help(options.help() + "\n");
            }

            // bare words match no option and would otherwise be dropped silently
            if (!parsed.unmatched().empty()) {
                return error("onnxcc dump: error: unexpected argument '" + parsed.unmatched().front() +
                             "'\ntry 'onnxcc dump --help'\n");
            }

            if (parsed.count("model") == 0) {
                return error("onnxcc dump: error: missing required option --model\n"
                             "try 'onnxcc dump --help'\n");
            }

            ParseResult result;
            result.kind = ParseResult::Kind::Dump;
            result.exit_code = kExitOk;
            result.dump.model = parsed["model"].as<std::string>();
            result.dump.show_graph = parsed["show-graph"].as<bool>();
            result.dump.verbose = parsed["verbose"].as<bool>();
            return result;
        }

    }  // namespace

    ParseResult parse(int argc, const char* const argv[]) {
        if (argc < 2) {
            return error(std::string(kUsage));
        }

        const std::string_view first = argv[1];

        if (first == "--help" || first == "-h") {
            return help(std::string(kUsage));
        }

        // cxxopts treats slot 0 as the program name, so the subcommand goes there
        if (first == "dump") {
            return parse_dump(argc - 1, argv + 1);
        }

        return error("onnxcc: error: unknown subcommand '" + std::string(first) + "'\n"
                     "try 'onnxcc --help'\n");
    }
}
