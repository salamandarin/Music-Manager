// Samuel Sutton - 2025
#include "core.h"
#include "metadata_manager.h"
#include "file_manager.h"

//--------------------------------------------------------------------------------
//                                  ADD TRACKS
//--------------------------------------------------------------------------------

// ---------- Add Track w/ File ----------
void Core::add_track(const std::string& file_path) {
    // 1. gather metadata from track
    MetadataManager metadata_manager{file_path};
    Track track_data = metadata_manager.get_data();

    // 2. move file to correct location
    std::string new_file_path = file_manager.create_new_path(file_path, track_data);
    track_data.file_path = new_file_path; // update track_data with new file path

    // 3. log info to database
    database.add_track(track_data);
}

// ---------- Add Track w/ or w/o File ----------
void Core::add_track(const Track& track) {
    if (track.file_path.empty()) {
        database.add_track(track);
    }
    else {
        // divert to other function if has file
        add_track(track.file_path);
    }
}

void Core::add_tracks_from_folder(const std::string& folder_path) {
    std::vector<std::string> file_paths = file_manager.get_files_from_folder(folder_path);
    for (const std::string& file_path : file_paths) {
        add_track(file_path);
    }
}

//--------------------------------------------------------------------------------
//                                  ADD OTHER OBJECTS
//--------------------------------------------------------------------------------


void Core::add_album(const Album& album) {
    database.add_album(album);
}
void Core::add_artist(const Artist& artist) {
    database.add_artist(artist);
}
void Core::add_person(const std::string& person) {
    database.add_person(person);
}

//--------------------------------------------------------------------------------
//                                  REMOVE OBJECTS
//--------------------------------------------------------------------------------
void Core::remove_track(int track_id) {
    // TODO: Remove actual file !!!! (ONLY IF exists) !!!!!

    // remove in database
    database.remove_track(track_id);
}
void Core::remove_album(int album_id) {
    // remove in database
    database.remove_album(album_id);

    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)
}
void Core::remove_artist(int artist_id) {
    // remove in database
    database.remove_artist(artist_id);

    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)
}
void Core::remove_person(int person_id) {
    // remove in database
    database.remove_person(person_id);
}

//--------------------------------------------------------------------------------
//                                  EDIT TRACK DATA
//--------------------------------------------------------------------------------

// ---------- INFO IN BOTH DATABASE & METADATA ----------
void Core::set_track_title(int track_id, const std::string& new_track_title) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE

    // check if it has file attached
    std::optional<std::string> possible_file_path = database.get_file_path(track_id);
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
void Core::set_track_artist(int track_id, const std::string& new_artist) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}
void Core::set_track_album(int track_id, const std::string& new_album) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}

// ---------- METADATA ONLY INFO ----------
void Core::set_track_tracklist_num(int track_id, int new_tracklist_num) {
    // TODO: CODE - MAY OR MAY NOT HAVE FILE
}

// ---------- DATABASE ONLY INFO ----------
void Core::set_track_date(int track_id, const Date& new_date) {
    database.set_track_date(track_id, new_date);
}

//--------------------------------------------------------------------------------
//                                  GET TRACK DATA
//--------------------------------------------------------------------------------
Track Core::get_track_data(int track_id) {
    // TODO: CODE

    // info to fetch from DB

    // info to fetch from file metadata (IF exists) OR JUST HAVE EVERYTHING IN DB

    // return track
}


