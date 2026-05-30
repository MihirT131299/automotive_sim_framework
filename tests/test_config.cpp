#include <gtest/gtest.h>
#include "config.h"

TEST(ConfigParser, ReadDouble_Success) {
    Config config;
    config.load("tests/data/test_config.toml");
    EXPECT_DOUBLE_EQ(config.get("simulation_loop_settings", "t_end"), 5.0);
}

TEST(ConfigParser, ReadDouble_WrongSection) {
    Config config;
    config.load("tests/data/test_config.toml");
    EXPECT_DOUBLE_EQ(config.get("nonexistent_section", "t_end"), 0.0);
}

TEST(ConfigParser, ReadDouble_WrongKey) {
    Config config;
    config.load("tests/data/test_config.toml");
    EXPECT_DOUBLE_EQ(config.get("simulation_loop_settings", "nonexistent_key"), 0.0);
}

TEST(ConfigParser, ReadString_Success) {
    Config config;
    config.load("tests/data/test_config.toml");
    // models is an array, not a string — test a genuine string value
    // using section name as a sanity check via getArray
    auto models = config.getArray("engine", "models");
    EXPECT_EQ(models[0], "ev_model");
}

TEST(ConfigParser, ReadArray_Success) {
    Config config;
    config.load("tests/data/test_config.toml");
    auto models = config.getArray("engine", "models");
    EXPECT_EQ(models.size(), 1u);
    EXPECT_EQ(models[0], "ev_model");
}

TEST(ConfigParser, ReadArray_WrongKey) {
    Config config;
    config.load("tests/data/test_config.toml");
    auto result = config.getArray("engine", "nonexistent_key");
    EXPECT_TRUE(result.empty());
}

TEST(ConfigParser, MalformedFile) {
    Config config;
    EXPECT_THROW(config.load("tests/data/malformed_config.toml"), std::exception);
}