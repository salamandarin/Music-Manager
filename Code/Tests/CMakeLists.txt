cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# metadata_tests exe
add_executable(metadata_tests
        main.cpp
        print_structs.cpp
        metadata_tests.cpp
)
target_link_libraries(metadata_tests PRIVATE Core) # Core
target_include_directories(metadata_tests PRIVATE ${doctest_SOURCE_DIR}/doctest) # doctest

# database_tests exe
add_executable(database_tests
        main.cpp
        print_structs.cpp
        database_tests.cpp
)
target_link_libraries(database_tests PRIVATE Core) # Core
target_include_directories(database_tests PRIVATE ${doctest_SOURCE_DIR}/doctest) # doctest

# core_tests exe
add_executable(core_tests
        main.cpp
        print_structs.cpp
        core_tests.cpp
)
target_link_libraries(core_tests PRIVATE Core) # Core
target_include_directories(core_tests PRIVATE ${doctest_SOURCE_DIR}/doctest) # doctest


# Core folder
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Core ${CMAKE_BINARY_DIR}/Core)

# doctest
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG ae7a13539fb71f270b87eb2e874fbac80bc8dda2
)
FetchContent_MakeAvailable(doctest)
