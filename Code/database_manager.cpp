// Samuel Sutton - 2025
#include "database_manager.h"
#include <iostream> //TODO: DELETE

// TODO: CUT DOWN ON DUPLICATED CODE !!!

//--------------------------------------------------------------------------------
//                                  LOG NEW DATA
//--------------------------------------------------------------------------------
// fetch file data, add to DB
void DatabaseManager::log_new_track(const Track& track_data) {
    // TODO: WRITE REAL CODE

    // check if need to add new album to DB, make new if so

    // check if need to add new artist to DB, make new if so

    // add new track entry to database, log all info


    std::cout << "Logging new file info to database.......\n";
    std::cout << "Track added to database! [not actually though]\n\n";
}

void log_new_album(const Album& album_data) {
    // TODO: CODE

    // TODO: either check for already existing stuff here, or do before calling function


}
void log_new_artist(const Artist& artist_data) {
    // TODO: CODE

    // TODO: either check for already existing stuff here, or do before calling function

    // TODO: if no artist exists, make artist.person_behind = artist.name (may need to make new person too idk)
}


//--------------------------------------------------------------------------------
//                                  TOOLS
//--------------------------------------------------------------------------------
// look for album title matches, return Album if found
std::optional<Album> DatabaseManager::find_album(const std::string& album_title) {
    //TODO: WRITE REAL CODE, maybe cut down duplicated code

    // return album found (or nothing)
}

// look for artist name matches, return Artist if found
std::optional<Artist> DatabaseManager::find_artist(const std::string& artist_name) {
    //TODO: WRITE REAL CODE, maybe cut down duplicated code

    // return artist found (or nothing)
}
