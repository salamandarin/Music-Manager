// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"
#include <vector>

class FileManager {
public:
    FileManager() = default;

    std::string create_new_path(const std::string& file_path, const Track& track_data); // add new file to proper directory, return new path
    void move_file(const std::string& current_path, const std::string& new_path);
    void update_file_name(const std::string& file_path, const std::string& new_file_name);
    
    std::vector<std::string> get_files_from_folder(const std::string& folder_path);
};