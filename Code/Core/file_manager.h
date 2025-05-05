// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>
#include <filesystem>

const std::filesystem::path MUSIC_FOLDER = "Music_Files";
const std::filesystem::path IMAGES_FOLDER = "Images";

class FileManager {
public:
    FileManager() = default;

    // save files (generate path + move/copy to correct location)
    static std::string save_music_file(const std::filesystem::path& current_path,const Track& track,
                                bool is_nested, bool copy_music_files);
    static std::string save_image_file(const std::filesystem::path& current_path, const std::string& image_name);

    // generate file paths
    static std::string make_image_file_path(const std::string& image_name, const std::string& image_extension); // returns new image path
    static std::string make_music_file_path(const std::filesystem::path& current_path, const Track& track_data, bool is_nested); // returns new file path

    // file operations
    static std::string move_file(const std::filesystem::path& old_path, std::filesystem::path new_path, const std::filesystem::path& boundary_folder=""); // handles folder cleanup too
    static std::string copy_file(const std::filesystem::path& file_path, std::filesystem::path new_path);
    static void delete_file(const std::filesystem::path& file_path, const std::filesystem::path& boundary_folder=""); // handles folder cleanup too

    // helper functions
    static std::string rename_file(const std::filesystem::path& file_path, const std::string& new_file_name); // returns new path
    static bool exists(const std::filesystem::path& path); // check if file/folder exists
    static std::filesystem::path get_file_name(const std::filesystem::path& file_path); // without extension or full path
    
    // file name helpers
    static std::string sanitize_file_name(std::string name);
    static std::filesystem::path number_duplicate_files(const std::filesystem::path& desired_file_path);

    // folder functions
    static void delete_folder(const std::filesystem::path& folder_path); // delete folder and all files inside (without any empty folder cleanup)
    static std::vector<std::string> get_files_from_folder(const std::filesystem::path& folder_path);
    static bool is_folder_empty(const std::filesystem::path& folder_path);
    static void delete_empty_parent_folders(std::filesystem::path current_path, const std::filesystem::path& boundary_folder);

private:
    // backend of delete_empty_parent_folders()
    static void recursive_delete_empty_parent_folders(const std::filesystem::path& current_path, const std::filesystem::path& boundary_folder);
};
