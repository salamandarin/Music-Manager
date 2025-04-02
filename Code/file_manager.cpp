// Samuel Sutton - 2025
#include "file_manager.h"
#include "metadata_manager.h"
#include "database_manager.h"
#include "artist.h"
#include <iostream> //TODO: DELETE

namespace filesystem = std::filesystem;

//--------------------------------------------------------------------------------
//                              FILE FUNCTIONS
//--------------------------------------------------------------------------------
std::string FileManager::create_new_path(const std::string& current_path, const Track& track, bool is_nested) {
    filesystem::path old_path = current_path;
    std::string new_path = "../../Music_Files/"; // base path for music files

    // IF NESTED: add person/artist/album to file path
    if (is_nested) {
        // gather artist real name to add that + artist name to path
        if (!track.artist.empty()) { // if there is artist info
            DatabaseManager database_manager;
            std::optional<Artist> possible_artist = database_manager.get_artist(track.artist);
            Artist artist;
            if (possible_artist) { // if artist is in database already
                artist = *possible_artist; 
            } // TODO: possibly add file to db first so person is always known??? or not
            else { // if artist isn't in database already (therefore person behind isn't known)
                artist.name = track.artist;
                artist.person_behind = track.artist; // just use artist name as person name
            }
            // add person_behind + artist to path
            new_path += (artist.person_behind + "/" + artist.name + "/");
        }
        // if no artist info, add "Artist Unknown" to path
        else {
            new_path += "Artist_Unknown/";
        }

        // add album name to path (if exists)
        if (!track.album.empty()) {
            new_path += (track.album + "/");
        }
        // if no album info, add "Album Unknown" to path
        else {
            new_path += "Album_Unknown/"; 
        }
    }

    std::string file_name;
    // make file name = track title (if has title)
    if (!track.title.empty()) {
        file_name = track.title + old_path.extension().string();
    }
    // OR keep file name
    else {
        file_name = old_path.filename().string();
    }

    // add file name to path
    new_path += file_name;

    // return new file path
    return new_path;
}

void FileManager::move_file(const std::string& current_path, const std::string& new_path) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(current_path) || !filesystem::is_regular_file(current_path)) {
        std::string error_message = "Tried to move file that doesn't exist or isn't a regular file";
        error_message += "\nTried to move '" + current_path + "' to '" + new_path + "'";
        throw std::runtime_error(error_message);
    }

    // make new folders (if needed)
    filesystem::path parent_folder_path = filesystem::path(new_path).parent_path();
    filesystem::create_directories(parent_folder_path); // make folders (if needed)

    // move file from current path -> new path
    filesystem::rename(current_path, new_path);

    // delete folders (from old path) that might be empty now
    delete_empty_parent_folders(current_path);
}

std::string FileManager::rename_file(const std::string& file_path, const std::string& new_file_name) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("Tried to rename file that doesn't exist or isn't a regular file: " + file_path);
    }
    
    // construct new path
    std::string new_path = filesystem::path(file_path).parent_path().string(); // get parent path
    new_path += "/" + new_file_name; // append new file name to parent path

    // rename file
    filesystem::rename(file_path, new_path); 

    // return new path
    return new_path;
}

// get just file name (without full path)
std::string FileManager::get_file_name(const std::string& file_path) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("Tried to get name of file that doesn't exist or isn't a regular file: " + file_path);
    }

    // return file name
    return filesystem::path(file_path).filename().string();
}

void FileManager::delete_file(const std::string& file_path) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("Tried to delete file that doesn't exist or isn't a regular file: " + file_path);
    }

    // delete file
    filesystem::remove(file_path);

    // delete folders that might be empty now
    delete_empty_parent_folders(filesystem::path(file_path).parent_path());
}

//--------------------------------------------------------------------------------
//                              FOLDER FUNCTIONS
//--------------------------------------------------------------------------------
std::vector<std::string> FileManager::get_files_from_folder(const std::string& folder_path) {
    // check if folder doesn't exist or isn't a folder
    if (!filesystem::exists(folder_path) || !filesystem::is_directory(folder_path)) {
        throw std::runtime_error("Tried adding files from invalid folder path: " + folder_path);
    }
    
    std::vector<std::string> file_paths;
    for (const auto& file : filesystem::directory_iterator(folder_path)) {
        // check if it's a regular file & doesn't start with '.' (hidden file)
        if (filesystem::is_regular_file(file) && file.path().filename().string()[0] != '.') {
            // add file path to vector
            file_paths.push_back(file.path().string());
        }

        // check if it's a folder (to grab nested files)
        if (filesystem::is_directory(file)) {
            // grab all files in that folder, append to vector
            std::vector<std::string> new_files = get_files_from_folder(file.path().string());
            file_paths.insert(file_paths.end(), new_files.begin(), new_files.end());
        }
    }

    return file_paths;
}

bool FileManager::is_folder_empty(const std::string& folder_path) {
    // check if folder doesn't exist or isn't a folder
    if (!filesystem::exists(folder_path) || !filesystem::is_directory(folder_path)) {
        throw std::runtime_error("Tried to check if folder is empty with invalid path: " + folder_path);
    }

    // return if folder is empty 
    return filesystem::directory_iterator(folder_path) == filesystem::directory_iterator();
}

void FileManager::delete_empty_parent_folders(filesystem::path starting_path) {
    // get parent folder if it's a file or doesn't exist (meaning it was deleted)
    if (filesystem::is_regular_file(starting_path) || !filesystem::exists(starting_path)) {
        starting_path = starting_path.parent_path();
    }
    
    // check if folder is empty
    if (is_folder_empty(starting_path)) {
        // delete folder
        filesystem::remove(starting_path);

        // recursively check & delete empty parent folders
        // stop when reaching "Music_Files" folder
        filesystem::path parent_path = filesystem::path(starting_path).parent_path();
        if (!filesystem::equivalent(parent_path, filesystem::path("../../Music_Files"))) {
            delete_empty_parent_folders(parent_path); // call this function on parent path
        }
    }
}