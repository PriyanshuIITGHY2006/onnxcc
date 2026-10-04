#pragma once

#include <iosfwd>

namespace onnxcc::cli {
    // out and err are passed in so tests can capture what went to which stream
    int run(int argc, const char* const argv[], std::ostream& out, std::ostream& err);
}
