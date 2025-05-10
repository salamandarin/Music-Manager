add_library(GUI
        Main/gui_utils.cpp
        Main/main_window.cpp Main/main_window.ui
        Main/settings_popup.cpp Main/settings_popup.ui

        Tracks/tracks_page.cpp Tracks/tracks_page.ui
        Tracks/track_popup.cpp Tracks/track_popup.ui
        Tracks/add_track_popup.cpp Tracks/add_track_popup.ui

        Albums/albums_page.cpp Albums/albums_page.ui
        Albums/album_widget.cpp Albums/album_widget.ui
        Albums/album_popup.cpp Albums/album_popup.ui
        Albums/add_album_popup.cpp Albums/add_album_popup.ui

        Artists/artists_page.cpp Artists/artists_page.ui
)
target_compile_options(GUI PUBLIC -std=c++17)

target_include_directories(GUI PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

        # GUI folders
        ${CMAKE_CURRENT_SOURCE_DIR}/Main
        ${CMAKE_CURRENT_SOURCE_DIR}/Tracks
        ${CMAKE_CURRENT_SOURCE_DIR}/Albums
        ${CMAKE_CURRENT_SOURCE_DIR}/Artists

        # Core folder
        ${CMAKE_SOURCE_DIR}/Core
        ${CMAKE_SOURCE_DIR}/Core/Data_Types # Data_Types
)
# Core folder
target_link_libraries(GUI PRIVATE Core)

# Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
target_link_libraries(GUI PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
