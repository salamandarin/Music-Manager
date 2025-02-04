// Samuel Sutton - 2025
#pragma once
#include <string>
#include "Data_Types/track.h"

class FileManager {
public:
    FileManager() = default;

    void move_file(const std::string& current_path, const std::string& new_path);
    std::string add_file(const std::string& file_path, const Track& track_data); // add new file to proper directory, return new path


    // ---------- class for iterating over all files in a folder ----------
    class DirectoryIterator {
    public:
        DirectoryIterator(const std::string& directory_path);
    };
};