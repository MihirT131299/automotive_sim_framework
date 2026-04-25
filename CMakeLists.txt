cmake_minimum_required(VERSION 3.15)
project(AutomotiveDigitalTwin LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    tomlplusplus 
    URL https://github.com/marzer/tomlplusplus/archive/refs/tags/v3.4.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(tomlplusplus)

add_executable(AutomotiveDigitalTwin src/main.cpp src/logger.cpp src/config.cpp src/ev_model.cpp)
target_include_directories(AutomotiveDigitalTwin PRIVATE include/)
target_link_libraries(AutomotiveDigitalTwin PRIVATE tomlplusplus::tomlplusplus)