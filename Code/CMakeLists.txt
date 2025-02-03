cmake_minimum_required(VERSION 3.13)
project(music)

add_executable(music main.cpp)
target_compile_options(music PUBLIC -std=c++17)

# Use pkg-config to locate TagLib
find_package(PkgConfig REQUIRED)
pkg_search_module(TAGLIB REQUIRED taglib)

# Include TagLib's header directories
target_include_directories(music PRIVATE ${TAGLIB_INCLUDE_DIRS})

# Add the library search directories
target_link_directories(music PRIVATE ${TAGLIB_LIBRARY_DIRS})

# Link against TagLib's libraries
target_link_libraries(music ${TAGLIB_LIBRARIES})