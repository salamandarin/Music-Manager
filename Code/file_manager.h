// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>
#include <filesystem>

namespace filesystem = std::filesystem;

const std::string MUSIC_FOLDER = "Music_Files";
const std::string IMAGES_FOLDER = "Images";

class FileManager {
public:
    FileManager() = default;

    // save / relocate files (generate path + move to correct location)
    static std::string relocate_music_file(const std::string& current_path, const Track& track, bool is_nested);
    static std::string save_image_file(const std::string& current_path, std::string image_name);

    // generate file paths
    static std::string make_image_file_path(const std::string& name, const std::string& image_extension); // returns new image path
    static std::string make_music_file_path(const std::string& current_path, const Track& track_data, bool is_nested); // returns new file path

    // file operations
    static void move_file(const std::string& old_path, const std::string& new_path, const std::string& boundary_folder=""); // handles folder cleanup too
    static void delete_file(const std::string& file_path, const std::string& boundary_folder=""); // handles folder cleanup too

    // helper functions
    static std::string rename_file(const std::string& file_path, const std::string& new_file_name); // returns new path
    static std::string get_file_name(const std::string& file_path); // get just file name (without full path or extension)
    static std::string get_extension(const std::string& file_path);
    static std::string get_parent_path(const std::string& file_path); // get parent path (without "/" at end)

    // folder functions
    static void delete_folder(const std::string& folder_path); // delete folder and all files inside (without any empty folder cleanup)
    static std::vector<std::string> get_files_from_folder(const std::string& folder_path);
    static bool is_folder_empty(const std::string& folder_path);
    static void delete_empty_parent_folders(filesystem::path current_path, const filesystem::path& boundary_folder);

private:
    // backend of delete_empty_parent_folders()
    static void recursive_delete_empty_parent_folders(const filesystem::path& current_path, const filesystem::path& boundary_folder);
};
