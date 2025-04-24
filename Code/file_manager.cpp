// Samuel Sutton - 2025
#include "file_manager.h"
#include "metadata_manager.h"
#include "database_manager.h"
#include "artist.h"
#include <iostream> //TODO: DELETE

namespace filesystem = std::filesystem;

//--------------------------------------------------------------------------------
//                                  SAVE FILES
//--------------------------------------------------------------------------------
std::string FileManager::relocate_music_file(const std::string& current_path, const Track& track, bool is_nested) {
    std::string new_path = make_music_file_path(current_path, track, is_nested);
    move_file(current_path, new_path, MUSIC_FOLDER);
    // new_path = move_music_file(current_path, new_path, MUSIC_FOLDER); // TODO: update path in case something changed (like if duplicate)
    return new_path;
}

std::string FileManager::save_image_file(const std::string& current_path, std::string image_name) {
    // name it "untitled" if no name given
    if (image_name.empty()) {
        image_name = "untitled";
    }

    // make new path (with given name)
    std::string new_path = make_image_file_path(image_name, get_extension(current_path));

    // move to images folder 
    move_file(current_path, new_path, IMAGES_FOLDER);
    // new_path = move_file(current_path, new_path, IMAGES_FOLDER); // TODO: update path in case something changed (like if duplicate)

    return new_path;
}
//--------------------------------------------------------------------------------
//                               GENERATE FILE PATHS
//--------------------------------------------------------------------------------
std::string FileManager::make_image_file_path(const std::string& name, const std::string& image_extension) {
    // create images folder if needed
    if (!std::filesystem::exists(IMAGES_FOLDER)) {
        std::filesystem::create_directory(IMAGES_FOLDER);
    }

    // TODO: handle duplicates?

    // make & return image path
    std::string image_path = IMAGES_FOLDER + "/" + name + image_extension;
    return image_path;
}

std::string FileManager::make_music_file_path(const std::string& current_path, const Track& track, bool is_nested) {
    std::string new_path = MUSIC_FOLDER + "/"; // base path for music files

    // IF NESTED: add artist/album to file path
    if (is_nested) {
        // add artist
        if (!track.artist.empty()) { // add artist name to path (if given)
            new_path += track.artist + "/";
        }
        else {// if no artist info, add "Artist Unknown" to path
            new_path += "Artist_Unknown/";
        }

        // add album
        if (!track.album.empty()) { // add album name to path (if given)
            new_path += (track.album + "/");
        }
        else { // if no album info, add "Album Unknown" to path
            new_path += "Album_Unknown/"; 
        }
    }

    // TODO: handle duplicates?

    // add file name + extension
    new_path += get_file_name(current_path) + get_extension(current_path);

    return new_path; // return new path
}

//--------------------------------------------------------------------------------
//                              OTHER FILE OPERATIONS
//--------------------------------------------------------------------------------
// move file (& clean up empty parent folders up to given folder path)
void FileManager::move_file(const std::string& old_path, const std::string& new_path, const std::string& boundary_folder) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(old_path) || !filesystem::is_regular_file(old_path)) {
        std::string error_message = "Tried to move file that doesn't exist or isn't a regular file";
        error_message += "\nTried to move '" + old_path + "' to '" + new_path + "'";
        throw std::runtime_error(error_message);
    }

    // make new folders (if needed)
    filesystem::path parent_folder_path = filesystem::path(new_path).parent_path();
    filesystem::create_directories(parent_folder_path); // make folders (if needed)

    // move file from old path -> new path
    filesystem::rename(old_path, new_path);

    // cleanup: delete parent folders that might be empty now (if boundary folder is given)
    if (!boundary_folder.empty()) {
        delete_empty_parent_folders(old_path, boundary_folder); // stop at boundary folder
    }
}

// delete file (& delete empty parent folders up to given boundary folder)
void FileManager::delete_file(const std::string& file_path, const std::string& boundary_folder) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("Tried to delete file that doesn't exist or isn't a regular file: " + file_path);
    }

    // delete file
    filesystem::remove(file_path);

    // cleanup: delete parent folders that might be empty now (if boundary folder is given)
    if (!boundary_folder.empty()) { 
        delete_empty_parent_folders(filesystem::path(file_path).parent_path(), boundary_folder); // stop at boundary folder
    }
}

//--------------------------------------------------------------------------------
//                              HELPER FUNCTIONS
//--------------------------------------------------------------------------------
std::string FileManager::rename_file(const std::string& file_path, const std::string& new_file_name) {
    // check if file doesn't exist or isn't a regular file
    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("Tried to rename file that doesn't exist or isn't a regular file: " + file_path);
    }
    
    // construct new path
    std::string parent_path = get_parent_path(file_path);
    std::string extension = get_extension(file_path);
    std::string new_path = parent_path + "/" + new_file_name + extension; // construct path

    // rename file
    filesystem::rename(file_path, new_path); 

    // return new path
    return new_path;
}

// get just file name (without full path or extension)
std::string FileManager::get_file_name(const std::string& file_path) {
    return filesystem::path(file_path).stem().string();
}
// get file extension
std::string FileManager::get_extension(const std::string& file_path) {
    return filesystem::path(file_path).extension().string();
}
// get parent path (without "/" at end)
std::string FileManager::get_parent_path(const std::string& file_path) {
    return filesystem::path(file_path).parent_path().string();
}

// delete anything (file or folder), without any empty folder cleanup
void FileManager::plain_delete(const filesystem::path& path) {
    // check if path doesn't exist
    if (!filesystem::exists(path)) {
        return; // return since doesn't exist
    }

    // if folder: delete with remove_all
    if (filesystem::is_directory(path)) {
        if (!filesystem::remove_all(path)) {  // make sure actually deleted
            throw std::runtime_error("Failed to delete folder: " + path.string());
        }
        return; // return since deletion is done
    }

    // delete singular file
    if (!filesystem::remove(path)) {  // make sure actually deleted
        throw std::runtime_error("Failed to delete file: " + path.string());
    }
}

//--------------------------------------------------------------------------------
//                              GET FILES FROM FOLDER
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

//--------------------------------------------------------------------------------
//                          DELETE EMPTY PARENT FOLDERS
//--------------------------------------------------------------------------------
// delete empty parent folders UNTIL hitting boundary_folder
void FileManager::delete_empty_parent_folders(filesystem::path current_path, const filesystem::path& boundary_folder) {
    // make sure boundary_folder exists
    if (!filesystem::exists(boundary_folder)) { // throw error is boundary_folder doesn't exist
        std::string error_message = "Need valid boundary folder path to safely call delete_empty_parent_folders";
        error_message += "\nBoundary path doesn't exist: " + boundary_folder.string();
        throw std::runtime_error(error_message);
    }

    // use parent folder if it's a file or doesn't exist (meaning it was deleted)
    if (filesystem::is_regular_file(current_path) || !filesystem::exists(current_path)) {
        current_path = current_path.parent_path(); // current_path isn't const reference cuz of this 
    }
    
    // recursively check & delete empty parent folders (now that everything is validated)
    recursive_delete_empty_parent_folders(current_path, boundary_folder);
}
// private backend of delete_empty_parent_folders()
void FileManager::recursive_delete_empty_parent_folders(const filesystem::path& current_path, const filesystem::path& boundary_folder) {    
    // stop if hit the boundary
    if (filesystem::equivalent(current_path, boundary_folder)) {
        return;
    }

    // check if folder is empty
    if (is_folder_empty(current_path)) {
        // delete folder
        filesystem::remove(current_path);

        // recursively check & delete empty parent folders
        filesystem::path parent_path = filesystem::path(current_path).parent_path();
        recursive_delete_empty_parent_folders(parent_path, boundary_folder); // call this function on parent path
    }
}
