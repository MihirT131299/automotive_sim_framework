# Build commands (run from the build/ directory):
#   cmake ..                                                        — regenerate build files
#   cmake --build .                                                 — compile all targets
#   .\Debug\AutomotiveDigitalTwin.exe --config "../config.toml"    — run simulation
#   .\Debug\run_tests.exe                                           — run test suite

cmake_minimum_required(VERSION 3.15)
project(AutomotiveDigitalTwin LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Dependencies ---

FetchContent_Declare(
    tomlplusplus
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(tomlplusplus)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# --- Framework library ---
# All framework source files compiled once into a static library.
# Both the simulation executable and the test executable link against this.

add_library(adt_core STATIC
    src/signal_bus.cpp
    src/bus_connector.cpp
    src/config.cpp
    src/ev_model.cpp
    src/logger.cpp
    src/engine.cpp
)
target_include_directories(adt_core PUBLIC include/)
target_link_libraries(adt_core PUBLIC tomlplusplus::tomlplusplus)

# --- Simulation executable ---

add_executable(AutomotiveDigitalTwin src/main.cpp)
target_link_libraries(AutomotiveDigitalTwin PRIVATE adt_core)

# --- Test executable ---

add_executable(run_tests
    tests/test_main.cpp
    tests/test_signal_bus.cpp
    tests/test_config.cpp
    tests/test_bus_connector.cpp
    tests/test_regression.cpp
)
target_link_libraries(run_tests PRIVATE adt_core GTest::gtest_main)
target_include_directories(run_tests PRIVATE include/)

# --- Runtime file copies ---

file(COPY interfaces/ DESTINATION interfaces/)
file(COPY tests/data/ DESTINATION tests/data/)