#include "onnxcc/cli/dump_command.h"

#include <filesystem>
#include <ostream>
#include <string>
#include <system_error>

#include "onnxcc/cli/exit_codes.h"
#include "onnxcc/third_party/cxxopts.hpp"
#include "onnxcc/version.h"

namespace onnxcc::cli {
    namespace {

        // Kept in one place so `dump --help` and the parser can never disagree.
        cxxopts::Options make_options() {
            cxxopts::Options options("onnxcc dump", "print what is inside an onnx model");
            options.add_options()
                ("model", "path to the .onnx model to read", cxxopts::value<std::string>(), "<path>")
                ("show-graph", "list the nodes of the graph")
                ("verbose", "print extra detail while working")
                ("h,help", "show this help and exit");
            return options;
        }

    }  // namespace

    int run_dump(int argc, const char* const argv[], std::ostream& out, std::ostream& err) {
        cxxopts::Options options = make_options();

        cxxopts::ParseResult args;
        try {
            args = options.parse(argc, argv);
        } catch (const cxxopts::exceptions::exception& error) {
            // Unknown flag, or --model given without a value.
            err << "onnxcc dump: error: " << error.what() << "\n"
                   "try 'onnxcc dump --help'\n";
            return kExitUsage;
        }

        // Help was asked for.
        if (args.count("help") > 0) {
            out << options.help() << "\n";
            return kExitOk;
        }

        // Bare words like `onnxcc dump model` match no option, so cxxopts parks them here.
        // Without this they would be silently ignored.
        if (!args.unmatched().empty()) {
            err << "onnxcc dump: error: unexpected argument '" << args.unmatched().front() << "'\n"
                   "try 'onnxcc dump --help'\n";
            return kExitUsage;
        }

        if (args.count("model") == 0) {
            err << "onnxcc dump: error: missing required option --model\n"
                   "try 'onnxcc dump --help'\n";
            return kExitUsage;
        }
        
        const std::string model = args["model"].as<std::string>();
        const std::filesystem::path model_path(model);

        // Only the path is checked here. Opening and parsing the model not happening now.
        // The error_code overloads report a broken path instead of throwing.
        std::error_code ec;
        if (!std::filesystem::exists(model_path, ec) || ec) {
            err << "onnxcc dump: error: model file '" << model << "' does not exist\n";
            return kExitFailure;
        }
        if (!std::filesystem::is_regular_file(model_path, ec) || ec) {
            err << "onnxcc dump: error: '" << model << "' is not a regular file\n";
            return kExitFailure;
        }

        out << "model: " << model << "\n";

        if (args.count("verbose") > 0) {
            // uintmax_t because a model can be far larger than an int holds.
            const std::uintmax_t size = std::filesystem::file_size(model_path, ec);
            out << "onnxcc: " << get_version() << " (" << get_version_codename() << ")\n";
            if (!ec) {
                out << "size: " << size << " bytes\n";
            }
        }

        if (args.count("show-graph") > 0) {
            // The real node listing needs the protobuf parser, which lands in phase 1.
            out << "graph: not parsed yet, dump only validates its arguments for now\n";
        }

        return kExitOk;
    }

}  // namespace onnxcc::cli
