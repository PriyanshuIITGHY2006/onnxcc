#include <gtest/gtest.h>

#include "onnxcc/cli/exit_codes.h"
#include "onnxcc/cli/parse.h"

namespace {

using onnxcc::cli::ParseResult;

TEST(Parse, NoArgumentsIsAUsageError) {
    const char* argv[] = {"onnxcc"};

    const ParseResult result = onnxcc::cli::parse(1, argv);

    EXPECT_EQ(ParseResult::Kind::Error, result.kind);
    EXPECT_EQ(onnxcc::cli::kExitUsage, result.exit_code);
    EXPECT_NE(std::string::npos, result.message.find("usage"));
}

TEST(Parse, TopLevelHelpListsSubcommands) {
    const char* argv[] = {"onnxcc", "--help"};

    const ParseResult result = onnxcc::cli::parse(2, argv);

    EXPECT_EQ(ParseResult::Kind::Help, result.kind);
    EXPECT_EQ(onnxcc::cli::kExitOk, result.exit_code);
    EXPECT_NE(std::string::npos, result.message.find("dump"));
}

TEST(Parse, UnknownSubcommandIsNamed) {
    const char* argv[] = {"onnxcc", "bogus"};

    const ParseResult result = onnxcc::cli::parse(2, argv);

    EXPECT_EQ(ParseResult::Kind::Error, result.kind);
    EXPECT_EQ(onnxcc::cli::kExitUsage, result.exit_code);
    EXPECT_NE(std::string::npos, result.message.find("bogus"));
}

TEST(Parse, DumpKeepsModelPathAndFlags) {
    const char* argv[] = {"onnxcc", "dump", "--model", "tiny.onnx", "--verbose", "--show-graph"};

    const ParseResult result = onnxcc::cli::parse(6, argv);

    EXPECT_EQ(ParseResult::Kind::Dump, result.kind);
    EXPECT_EQ("tiny.onnx", result.dump.model);
    EXPECT_TRUE(result.dump.verbose);
    EXPECT_TRUE(result.dump.show_graph);
}

TEST(Parse, DumpFlagsDefaultToOff) {
    const char* argv[] = {"onnxcc", "dump", "--model", "tiny.onnx"};

    const ParseResult result = onnxcc::cli::parse(4, argv);

    EXPECT_EQ(ParseResult::Kind::Dump, result.kind);
    EXPECT_FALSE(result.dump.verbose);
    EXPECT_FALSE(result.dump.show_graph);
}

TEST(Parse, DumpHelpWorksWithoutAModel) {
    const char* argv[] = {"onnxcc", "dump", "--help"};

    const ParseResult result = onnxcc::cli::parse(3, argv);

    EXPECT_EQ(ParseResult::Kind::Help, result.kind);
    EXPECT_NE(std::string::npos, result.message.find("--model"));
    EXPECT_NE(std::string::npos, result.message.find("--show-graph"));
    EXPECT_NE(std::string::npos, result.message.find("--verbose"));
}

TEST(Parse, DumpWithoutModelIsAUsageError) {
    const char* argv[] = {"onnxcc", "dump"};

    const ParseResult result = onnxcc::cli::parse(2, argv);

    EXPECT_EQ(ParseResult::Kind::Error, result.kind);
    EXPECT_EQ(onnxcc::cli::kExitUsage, result.exit_code);
    EXPECT_NE(std::string::npos, result.message.find("--model"));
}

TEST(Parse, UnknownOptionIsAUsageError) {
    const char* argv[] = {"onnxcc", "dump", "--nonsense"};

    const ParseResult result = onnxcc::cli::parse(3, argv);

    EXPECT_EQ(ParseResult::Kind::Error, result.kind);
    EXPECT_NE(std::string::npos, result.message.find("nonsense"));
}

TEST(Parse, StrayWordIsRejected) {
    const char* argv[] = {"onnxcc", "dump", "extra", "--model", "tiny.onnx"};

    const ParseResult result = onnxcc::cli::parse(5, argv);

    EXPECT_EQ(ParseResult::Kind::Error, result.kind);
    EXPECT_NE(std::string::npos, result.message.find("extra"));
}

}  // namespace
