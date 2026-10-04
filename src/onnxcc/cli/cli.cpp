#include "onnxcc/cli/cli.h"

#include <ostream>

#include "onnxcc/cli/dump_command.h"
#include "onnxcc/cli/exit_codes.h"
#include "onnxcc/cli/parse.h"

namespace onnxcc::cli {

    int run(int argc, const char* const argv[], std::ostream& out, std::ostream& err) {
        const ParseResult parsed = parse(argc, argv);

        switch (parsed.kind) {
            case ParseResult::Kind::Help:
                out << parsed.message;
                return parsed.exit_code;

            case ParseResult::Kind::Error:
                err << parsed.message;
                return parsed.exit_code;

            case ParseResult::Kind::Dump:
                return run_dump(parsed.dump, out, err);
        }

        return kExitFailure;
    }
}
