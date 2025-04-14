cmake_minimum_required(VERSION 3.16)
project(music)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(music
        main.cpp
        metadata_manager.cpp
        file_manager.cpp
        database_manager.cpp
        core.cpp
        main_window.cpp # TODO: DELETE ?
        new_main_window.cpp new_main_window.ui # TODO: DELETE ?
        track_popup.cpp track_popup.ui
        settings_popup.cpp settings_popup.ui
        albums_page.cpp albums_page.ui
        tracks_page.cpp tracks_page.ui
        artists_page.cpp artists_page.ui
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

# SQLite3
find_package(SQLite3 REQUIRED)
target_link_libraries(music PRIVATE SQLite::SQLite3)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
target_link_libraries(music PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# ---------------------- TEST EXECUTABLE ----------------------
add_executable(metadata_tests
        metadata_tests.cpp
        metadata_manager.cpp
        file_manager.cpp
        database_manager.cpp
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

# SQLite3
find_package(SQLite3 REQUIRED)
target_link_libraries(metadata_tests PRIVATE SQLite::SQLite3)
