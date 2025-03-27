// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>

class FileManager {
public:
    FileManager() = default;

    std::string add_new_file(const std::string& file_path, const Track& track_data); // returns new path
    std::string create_new_path(const std::string& current_path, const Track& track_data); // returns new path
    void move_file(const std::string& current_path, const std::string& new_path);
    std::string rename_file(const std::string& file_path, const std::string& new_file_name); // returns new path
    void delete_file(const std::string& file_path);
    
    std::vector<std::string> get_files_from_folder(const std::string& folder_path);
    bool is_folder_empty(const std::string& folder_path);
    void delete_empty_parent_folders(const std::string& starting_folder_path);
};