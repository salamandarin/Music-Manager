add_library(Backend INTERFACE)

# ADD FOLDERS
add_subdirectory(Core)
add_subdirectory(Audio_Analyzer)

# LINK FOLDERS
target_link_libraries(Backend INTERFACE
    Core
    Audio_Analyzer
)