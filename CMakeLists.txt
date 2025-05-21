cmake_minimum_required(VERSION 3.14)
project(GraphAlgorithmImplementations LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all core source files (excluding test files)
file(GLOB CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cc
)

# Remove test files from the list if any accidentally match
list(FILTER CORE_SOURCES EXCLUDE REGEX "tests/")

add_library(graph_core STATIC ${CORE_SOURCES})
target_include_directories(graph_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann_json/single_include
)

# GoogleTest setup (FetchContent downloads if not found)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Find all test files recursively in src/core/tests
file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/tests/test_*.cc
)

add_executable(core_tests ${TEST_SOURCES})
target_link_libraries(core_tests PRIVATE graph_core gtest_main)

target_include_directories(core_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann_json/single_include
)

include(GoogleTest)
gtest_discover_tests(core_tests)