add_library(Core
        metadata_manager.cpp
        file_manager.cpp
        database_manager.cpp
        core.cpp

        # Data_Types
        Data_Types/duration.cpp
        Data_Types/date.cpp
)
target_compile_options(Core PUBLIC -std=c++17)

target_include_directories(Core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Data_Types # include Data_Types
)

# TagLib
find_package(PkgConfig REQUIRED)
pkg_search_module(TAGLIB REQUIRED taglib)
target_include_directories(Core PUBLIC ${TAGLIB_INCLUDE_DIRS})
target_link_directories(Core PUBLIC ${TAGLIB_LIBRARY_DIRS})
target_link_libraries(Core PUBLIC ${TAGLIB_LIBRARIES})

# SQLite3
find_package(SQLite3 REQUIRED)
target_link_libraries(Core PRIVATE SQLite::SQLite3)