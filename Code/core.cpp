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
        // just add to database
        database.add_track(track);
    } else {
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
    // delete file (if exists)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        file_manager.delete_file(*possible_file_path); // delete file
    }

    // remove in database (must do last or else can't get file path)
    database.remove_track(track_id);
}
void Core::remove_album(int album_id) {
    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)

    // remove in database
    database.remove_album(album_id);
}
void Core::remove_artist(int artist_id) {
    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)

    // remove in database
    database.remove_artist(artist_id);
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
    // make sure isn't same as old title
    std::optional<std::string> current_title = database.get_track_title(track_id);
    if (current_title) {
        if (*current_title == new_track_title) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_title(track_id, new_track_title);

    // update in metadata + file name + path (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager file_metadata{*possible_file_path};
        file_metadata.set_track_title(new_track_title);

        // update file name to match
        file_manager.rename_file(*possible_file_path, new_track_title);

        // TODO: Update file path in db to, OR make priv function that does all??
    }
}
void Core::set_track_artist(int track_id, const std::string& new_artist_name) {
    // make sure isn't same as old artist
    std::optional<Artist> current_artist = database.get_track_artist(track_id);
    if (current_artist) {
        if (current_artist->name == new_artist_name) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_artist(track_id, new_artist_name);

    // update in metadata + file path (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager file_metadata{*possible_file_path};
        file_metadata.set_artist(new_artist_name);

        // TODO: UPDATE FILE PATH, MOVE TO NEW FOLDER!!!!!!!!!!
        // TODO: Update file path in db to, OR make priv function that does all??
    }

    // TODO: possibly overload to also take in Artist type too, use that to set person?
}
void Core::set_track_album(int track_id, const std::string& new_album_title) {
    // make sure isn't same as old album
    std::optional<Album> current_album = database.get_track_album(track_id);
    if (current_album) {
        if (current_album->title == new_album_title) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_album(track_id, new_album_title);

    // update in metadata + file path (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager file_metadata{*possible_file_path};
        file_metadata.set_album(new_album_title);

        // TODO: UPDATE FILE PATH
        // TODO: Update file path in db to, OR make priv function that does all??
    }

    // TODO: possibly overload to also take in Album type too, use that info too?
}
void Core::set_track_date(int track_id, const Date& new_date) {
    // make sure isn't same as old date
    std::optional<Date> current_date = database.get_track_date(track_id);
    if (current_date) {
        if (*current_date == new_date) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_date(track_id, new_date);

    // TODO: check if date is in metadata, set if so
}
void Core::set_track_tracklist_num(int track_id, int new_tracklist_num) {
    // make sure isn't same as old tracklist num
    std::optional<int> current_tracklist_num = database.get_track_tracklist_num(track_id);
    if (current_tracklist_num) {
        if (*current_tracklist_num == new_tracklist_num) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_tracklist_num(track_id, new_tracklist_num);

    // update in metadata (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager file_metadata{*possible_file_path};
        file_metadata.set_tracklist_num(new_tracklist_num);
    }
}

//--------------------------------------------------------------------------------
//                                  GET OBJECTS
//--------------------------------------------------------------------------------
Track Core::get_track(int track_id) {
    // get database info
    Track track = database.get_track(track_id);

    // TODO: info to fetch from file metadata (IF exists) OR JUST HAVE EVERYTHING IN DB ??

    return track;
}
Album Core::get_album(int album_id) {
    // get database info
    Album album = database.get_album(album_id);
    return album;
}
Artist Core::get_artist(int artist_id) {
    // get database info
    Artist artist = database.get_artist(artist_id);
    return artist;
}

//--------------------------------------------------------------------------------
//                                  GET -ALL- OBJECTS
//--------------------------------------------------------------------------------
std::vector<Track> Core::get_all_tracks() {
    // get database info
    std::vector<Track> tracks = database.get_all_tracks();

    // TODO: figure out if need file info, & possibly use Core::get_track() for that ???

    return tracks;
}
std::vector<Album> Core::get_all_albums() {
    // get database info
    std::vector<Album> albums = database.get_all_albums();
    return albums;
}
std::vector<Artist> Core::get_all_artists() {
    // get database info
    std::vector<Artist> artists = database.get_all_artists();
    return artists;
}

//--------------------------------------------------------------------------------
//                              PRIVATE FUNCTIONS
//--------------------------------------------------------------------------------
void move_file(int track_id, const std::string new_file_path) {
    
    // TODO: CODE

    // TODO: actually move file, + update in db
    // TODO: use THIS function everywhere (including core::add_track())
}

