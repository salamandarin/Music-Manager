// Samuel Sutton - 2025
#include "file_manager.h"
#include "metadata_manager.h"
#include "database_manager.h"
#include "artist.h"
#include <iostream> //TODO: DELETE

void FileManager::move_file(const std::string& current_path, const std::string& new_path){
    // TODO: WRITE CODE

    //TODO: DELETE THIS
    std::cout << "Pretending to move file......\n";
    std::cout << "File moving from:\n";
    std::cout << "\t\t" << current_path << "\n";
    std::cout << "to: \t" << new_path << "\n\n";
}

std::string FileManager::add_file(const std::string& file_path, const Track& track_data) {
    std::string new_path = "../../Music_Files/"; // base path for music files

    // gather artist real name to add that + artist name to path
    if (!track_data.artist.empty()) { // if there is artist info
        DatabaseManager database_manager;
        std::optional<Artist> possible_artist = database_manager.get_artist(track_data.artist);
        Artist artist;
        if (possible_artist) { // if artist is in database already
            artist = *possible_artist; 
        } // TODO: possibly add file to db first so person is always known??? or not
        else { // if artist isn't in database already (therefore person behind isn't known)
            artist.name = track_data.artist;
            artist.person_behind = track_data.artist; // just use artist name as person name
        }
        // add person_behind + artist to path
        new_path += (artist.person_behind + "/" + artist.name + "/");
    }
    // if no artist info, add "Artist Unknown" to path
    else {
        new_path += "Artist_Unknown/";
    }

    // add album name to path (if exists)
    if (!track_data.album.empty()) {
        new_path += (track_data.album + "/");
    }
    // if no album info, add "Album Unknown" to path
    else {
        new_path += "Album_Unknown/";
    }

    // move file to new directory
    move_file(file_path, new_path);

    // return new file_path
    return new_path;
}
