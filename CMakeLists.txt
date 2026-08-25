cmake_minimum_required(VERSION 3.16)

project(CanSatXBee LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Library
# -----------------------------

add_library(cansat
    src/xbee.cpp
    src/payload.cpp
)

target_include_directories(cansat
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# -----------------------------
# GoogleTest
# -----------------------------

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

# Prevent GoogleTest from overriding MSVC runtime settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# -----------------------------
# Testing
# -----------------------------

enable_testing()

add_executable(test_payload
    tests/test_payload.cpp
)

target_link_libraries(test_payload
    PRIVATE
        cansat
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_payload)