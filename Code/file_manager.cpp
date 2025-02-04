// Samuel Sutton - 2025
#include "file_manager.h"
#include "metadata_manager.h"
#include "database_manager.h"
#include "Data_Types/artist.h"

void FileManager::move_file(const std::string& current_path, const std::string& new_path){
    // TODO: WRITE CODE
}

std::string FileManager::add_file(const std::string& file_path, const Track& track_data) {
    std::string new_path = "Music_Files/" // base path for music files

    // gather artist real name to add that + artist name to path
    DatabaseManager database_manager;
    Artist artist = database_manager.find_artist(track_data.artist)
    if (!artist) { // if artist isn't in database already
        // TODO: check if right + scope
        new_path += (track_data.artist + "/" + track_data.artist + "/"); // artist name x2 cuz person_name/artist_name
    }
    else { // person_behind/artist_name
        // TODO: check if right + scope
        new_path += (artist.person_behind + "/" + artist.artist_name + "/");
    }

    // add album name to path (if exists)
    if (track_data.album) {
        new_path += (track_data.album + "/");
    }

    // move file to new directory
    move_file(file_path, new_path);

    // return new file_path
    return new_path;
}
