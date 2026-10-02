#pragma once

#include<iosfwd>

namespace onnxcc::cli {

    // entry point of command line. main() calls this and returns its results
    // normal output goes to 'out' and errors to 'err' so tests can pass string streams and check exactly what was printed where
    // returns one of the exit codes in exit_codes.h
    int run(int argc, const char* const argv[], std::ostream& out, std::ostream& err);
}
