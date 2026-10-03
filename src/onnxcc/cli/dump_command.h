#pragma once

#include <iosfwd>

#include "onnxcc/cli/parse.h"

namespace onnxcc::cli {
    int run_dump(const DumpOptions& options, std::ostream& out, std::ostream& err);
}
