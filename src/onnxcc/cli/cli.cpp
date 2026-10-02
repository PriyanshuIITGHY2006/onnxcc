#include "onnxcc/cli/cli.h"

#include <ostream>
#include <string_view>

#include "onnxcc/cli/dump_command.h"
#include "onnxcc/cli/exit_codes.h"

namespace onnxcc::cli {
    namespace {

        // Every subcommand handler has this shape, so the table can hold any of them.
        using Handler = int (*)(int argc, const char* const argv[], std::ostream& out, std::ostream& err);

        // One row per subcommand. run, compile and benchmark get added here later.
        struct Subcommand {
            std::string_view name;
            std::string_view summary;
            Handler handler;
        };

        constexpr Subcommand kSubcommands[] = {
            {"dump", "print what is inside an onnx model", &run_dump},
        };

        // Written to `out` when help is asked for, to `err` when the command was wrong.
        void print_usage(std::ostream& os) {
            os << "usage: onnxcc <subcommand> [options]\n"
                  "\n"
                  "subcommands:\n";
            for (const Subcommand& sub : kSubcommands) {
                os << "  " << sub.name << "    " << sub.summary << "\n";
            }
            os << "\n"
                  "options:\n"
                  "  -h, --help    show this help and exit\n";
        }

        // Returns nullptr when no subcommand has that name.
        const Subcommand* find_subcommand(std::string_view name) {
            for (const Subcommand& sub : kSubcommands) {
                if (sub.name == name) {
                    return &sub;
                }
            }
            return nullptr;
        }

    }  // namespace

    int run(int argc, const char* const argv[], std::ostream& out, std::ostream& err) {
        // Bare `onnxcc`: usage on stderr, non-zero exit.
        if (argc < 2) {
            print_usage(err);
            return kExitUsage;
        }

        const std::string_view first = argv[1];

        // Help was asked for, so it is normal output.
        if (first == "--help" || first == "-h") {
            print_usage(out);
            return kExitOk;
        }

        // Hand the rest of the command line to the subcommand, with its own name first,
        // so `onnxcc dump --model x` reaches run_dump as `dump --model x`.
        if (const Subcommand* sub = find_subcommand(first)) {
            return sub->handler(argc - 1, argv + 1, out, err);
        }

        err << "onnxcc: error: unknown subcommand '" << first << "'\n"
            << "try 'onnxcc --help'\n";
        return kExitUsage;
    }

}  // namespace onnxcc::cli
