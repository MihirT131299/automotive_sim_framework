#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "config.h"
#include "engine.h"

static const std::string TEST_CONFIG    = "tests/data/test_config.toml";
static const std::string GOLDEN_FILE    = "tests/data/golden_reference.csv";
static const std::string TEST_OUTPUT    = "tests/data/test_output.csv";

TEST(Regression, EVModel_MatchesGoldenReference) {
    // Run simulation with test config
    Config config;
    config.load(TEST_CONFIG);
    Engine engine(config);
    engine.run(TEST_OUTPUT);

    // Open both files
    std::ifstream golden(GOLDEN_FILE);
    std::ifstream output(TEST_OUTPUT);

    ASSERT_TRUE(golden.is_open()) << "Golden reference file not found: " << GOLDEN_FILE;
    ASSERT_TRUE(output.is_open()) << "Test output file not found: " << TEST_OUTPUT;

    // Compare line by line
    std::string golden_line, output_line;
    int line_number = 0;

    while (std::getline(golden, golden_line)) {
        line_number++;
        ASSERT_TRUE(std::getline(output, output_line))
            << "Test output ended early at line " << line_number;
        EXPECT_EQ(golden_line, output_line)
            << "Mismatch at line " << line_number;
    }

    // Confirm output has no extra lines
    EXPECT_FALSE(std::getline(output, output_line))
        << "Test output has extra lines beyond golden reference";
}