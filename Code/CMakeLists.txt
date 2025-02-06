cmake_minimum_required(VERSION 3.13)
project(music)

add_executable(music 
            main.cpp
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

