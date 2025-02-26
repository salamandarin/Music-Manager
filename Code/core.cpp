// Samuel Sutton - 2025
#include "core.h"
#include "metadata_manager.h"
#include "file_manager.h"
#include "database_manager.h"

//--------------------------------------------------------------------------------
//                                  ADD TRACK
//--------------------------------------------------------------------------------

// ---------- Add Track w/ File ----------
void Core::add_track(const std::string& file_path) {
    // 1. gather metadata from track
    MetadataManager metadata_manager{file_path};
    Track track_data = metadata_manager.get_data();

    // 2. move file to correct location
    FileManager file_manager;
    std::string new_file_path = file_manager.add_file(file_path, track_data);
    track_data.file_path = new_file_path; // update track_data with new file path

    // 3. log info to database
    DatabaseManager database_manager;
    database_manager.add_track(track_data);
}

// ---------- Add Track w/o File ----------
void Core::add_track(const Track& track) {
    DatabaseManager database_manager;
    database_manager.add_track(track);
}

//--------------------------------------------------------------------------------
//                                  REMOVE TRACK
//--------------------------------------------------------------------------------
void Core::remove_track(const int& track_id) {
    // TODO: WRITE CODE

    // remove actual file (IF exists)
    
    // remove in database
}

//--------------------------------------------------------------------------------
//                                  EDIT TRACK DATA
//--------------------------------------------------------------------------------

// ---------- INFO IN BOTH DATABASE & METADATA ----------
void Core::set_track_title(const int& track_id, const std::string& new_track_title) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE

    // check if it has file attached
    DatabaseManager database_manager;
    std::optional<std::string> possible_file_path = database_manager.get_file_path(track_id);
    if (possible_file_path) {
        // update title in metadata
        std::string file_path = *possible_file_path;
        MetadataManager file_metadata{file_path};
        file_metadata.set_track_title(new_track_title);
    }

    // update file name
    // TODO: CODE

    // update title in DB
    // TODO: CODE
}
void Core::set_track_artist(const int& track_id, const std::string& new_artist) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}
void Core::set_track_album(const int& track_id, const std::string& new_album) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}

// ---------- METADATA ONLY INFO ----------
void Core::set_track_tracklist_num(const int& track_id, const int& new_tracklist_num) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}

// ---------- DATABASE ONLY INFO ----------
void Core::set_track_date(const int& track_id, const Date& new_date) {
    DatabaseManager database_manager;
    database_manager.set_track_date(track_id, new_date);
}

//--------------------------------------------------------------------------------
//                                  GET TRACK DATA
//--------------------------------------------------------------------------------
Track Core::get_track_data(const int& track_id) {
    // TODO: CODE

    // info to fetch from DB

    // info to fetch from file metadata (IF exists) OR JUST HAVE EVERYTHING IN DB

    // return track
}


