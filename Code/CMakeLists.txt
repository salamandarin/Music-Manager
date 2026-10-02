cmake_minimum_required(VERSION 3.16)
project(music)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(music
        main.cpp
)
target_compile_options(music PUBLIC -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Backend folder
add_subdirectory(Backend)
target_link_libraries(music PRIVATE Backend)

# GUI folder
add_subdirectory(GUI) 
target_link_libraries(music PRIVATE GUI)
