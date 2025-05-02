add_library(GUI
        gui_utils.cpp
        main_window.cpp main_window.ui
        track_popup.cpp track_popup.ui
        settings_popup.cpp settings_popup.ui
        albums_page.cpp albums_page.ui
        tracks_page.cpp tracks_page.ui
        artists_page.cpp artists_page.ui
        add_track_popup.cpp add_track_popup.ui
)
target_compile_options(GUI PUBLIC -std=c++17)

# Core folder
target_include_directories(GUI PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Core # include Core
        ${CMAKE_SOURCE_DIR}/Core/Data_Types # include Data_Types

)
target_link_libraries(GUI PRIVATE Core)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
target_link_libraries(GUI PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)