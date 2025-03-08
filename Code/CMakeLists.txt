cmake_minimum_required(VERSION 3.13)
project(music)

add_executable(music
        main.cpp
        metadata_manager.cpp
        file_manager.cpp
        database_manager.cpp
        core.cpp
)
target_compile_options(music PUBLIC -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Data_Types folder
add_subdirectory(Data_Types)
target_link_libraries(music PUBLIC Data_Types)

# TagLib
find_package(PkgConfig REQUIRED)
pkg_search_module(TAGLIB REQUIRED taglib)
target_include_directories(music PRIVATE ${TAGLIB_INCLUDE_DIRS})
target_link_directories(music PRIVATE ${TAGLIB_LIBRARY_DIRS})
target_link_libraries(music PRIVATE ${TAGLIB_LIBRARIES})

find_package(SQLite3 REQUIRED)
target_link_libraries(music PRIVATE SQLite::SQLite3)


# TEST EXECUTABLE
add_executable(metadata_tests
        metadata_tests.cpp
        metadata_manager.cpp
)

# doctest
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG ae7a13539fb71f270b87eb2e874fbac80bc8dda2
)
FetchContent_MakeAvailable(doctest)
target_include_directories(metadata_tests PRIVATE ${doctest_SOURCE_DIR}/doctest)

# Data_Types folder
target_link_libraries(metadata_tests PUBLIC Data_Types)

# TagLib
target_include_directories(metadata_tests PRIVATE ${TAGLIB_INCLUDE_DIRS})
target_link_directories(metadata_tests PRIVATE ${TAGLIB_LIBRARY_DIRS})
target_link_libraries(metadata_tests PRIVATE ${TAGLIB_LIBRARIES})
