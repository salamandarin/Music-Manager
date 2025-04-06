// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>
#include <filesystem>

namespace filesystem = std::filesystem;

class FileManager {
public:
    FileManager() = default;

    // generate file paths
    static std::string make_image_file_path(const std::string& name, const std::string& image_extension); // returns new image path
    std::string make_music_file_path(const std::string& current_path, const Track& track_data, bool is_nested); // returns new file path

    // file operations
    void move_music_file(const std::string& old_path, const std::string& new_path); // handles folder cleanup too
    void delete_music_file(const std::string& file_path); // handles folder cleanup too
    static std::string rename_file(const std::string& file_path, const std::string& new_file_name); // returns new path

    // helper functions
    static std::string get_file_name(const std::string& file_path); // get just file name (without full path or extension)
    static std::string get_extension(const std::string& file_path);
    static std::string get_parent_path(const std::string& file_path); // get parent path (without "/" at end)
    static void plain_delete(const filesystem::path& path); // delete file or folder (without cleanup)

    // folder functions
    std::vector<std::string> get_files_from_folder(const std::string& folder_path);
    bool is_folder_empty(const std::string& folder_path);
    void delete_empty_parent_folders(filesystem::path current_path, const filesystem::path& boundary_folder);

private:
    // backend of delete_empty_parent_folders()
    void recursive_delete_empty_parent_folders(const filesystem::path& current_path, const filesystem::path& boundary_folder);
};