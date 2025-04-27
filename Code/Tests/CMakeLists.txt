cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(tests
        main.cpp
        print_structs.cpp
        metadata_tests.cpp
        database_tests.cpp
)
target_compile_options(tests PUBLIC -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Core folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Core ${CMAKE_BINARY_DIR}/Core)
target_link_libraries(tests PRIVATE Core)

# doctest
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG ae7a13539fb71f270b87eb2e874fbac80bc8dda2
)
FetchContent_MakeAvailable(doctest)
target_include_directories(tests PRIVATE ${doctest_SOURCE_DIR}/doctest)
