// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>
#include <filesystem>

class FileManager {
public:
    FileManager() = default;

    // file functions
    std::string create_new_path(const std::string& current_path, const Track& track_data, bool is_nested); // returns new path
    void move_file(const std::string& current_path, const std::string& new_path);
    std::string rename_file(const std::string& file_path, const std::string& new_file_name); // returns new path
    static std::string get_file_name(const std::string& file_path); // get just file name (without full path)
    void delete_file(const std::string& file_path);
    
    // folder functions
    std::vector<std::string> get_files_from_folder(const std::string& folder_path);
    bool is_folder_empty(const std::string& folder_path);
    void delete_empty_parent_folders(std::filesystem::path starting_path);
};