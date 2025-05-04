// Samuel Sutton - 2025
#include "core.h"
#include "metadata_manager.h"
#include "file_manager.h"

//--------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//--------------------------------------------------------------------------------
Core::Core()
    :database{} {
        // grab settings from database
        std::unordered_map<std::string, bool> settings = database.get_all_settings();
        is_nested = settings.at("is_nested");
        copy_music_files = settings.at("copy_music_files");
    }

//--------------------------------------------------------------------------------
//                                  HANDLE SETTINGS
//--------------------------------------------------------------------------------
// is_nested
bool Core::get_is_nested() {
    return is_nested;
}
void Core::set_is_nested(bool new_value) {
    if (is_nested == new_value) return; // don't do anything if same

    // update value
    is_nested = new_value;
    database.set_setting_value("is_nested", new_value);

    // rearrange file structure (to either nested or flat)
    update_file_structure();
}

// copy_music_files
bool Core::get_copy_music_files() {
    return copy_music_files;
}
void Core::set_copy_music_files(bool new_value) {
    if (copy_music_files == new_value) return; // don't do anything if same

    // update value
    copy_music_files = new_value;
    database.set_setting_value("copy_music_files", new_value);
}

//--------------------------------------------------------------------------------
//                                  ADD TRACKS
//--------------------------------------------------------------------------------

// ---------- Add Track w/ File ----------
void Core::add_track(std::string file_path) {
    // make track title & file name match
    file_path = set_file_title(file_path);

    // gather metadata from track 
    MetadataManager metadata_manager{file_path};
    Track track = metadata_manager.get_data();
    
    // TODO: MAKE GET / EXTRACT INFO FROM FILE FUNCTION (that gets file info too) (+ maybe calls save cover art)

    // save cover art from metadata (if exists)
    track.image_path = metadata_manager.save_cover_art();

    // save music file to correct location
    track.file_path = FileManager::save_music_file(track.file_path, track, is_nested, copy_music_files);

    // log info to database
    database.add_track(track);
}

// ---------- Add Track w/ or w/o File ----------
void Core::add_track(Track track) {
    // if no file attached 
    if (track.file_path.empty()) { 
        // grab image file if included, save new path
        if (!track.image_path.empty()) {
            track.image_path = FileManager::save_image_file(track.image_path, track.title);
        }

        // add track to database
        database.add_track(track);
    }
    // divert to other function if has file
    else {
        add_track(track.file_path);
    }
}

void Core::add_tracks_from_folder(const std::string& folder_path) {
    std::vector<std::string> file_paths = FileManager::get_files_from_folder(folder_path);
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
        FileManager::delete_file(*possible_file_path, MUSIC_FOLDER); // delete file
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
//                                  SET TRACK DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET TRACK TITLE ------------------------------
void Core::set_track_title(int track_id, const std::string& track_title) {
    // make sure isn't same as old title
    std::optional<std::string> current_title = database.get_track_title(track_id);
    if (current_title) {
        if (*current_title == track_title) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_title(track_id, track_title);

    // if file exists: update metadata + file name + database file path
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager metadata{*possible_file_path};
        metadata.set_track_title(track_title);

        // update file name to match
        std::string new_path = FileManager::rename_file(*possible_file_path, track_title);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }

    // if image file exists: rename that too
    std::optional<std::string> possible_image_path = database.get_track_image_path(track_id);
    if (possible_image_path) { // check if image file exists
        // update image file name to match
        std::string new_image_path = FileManager::rename_file(*possible_image_path, track_title);

        // update image path in database
        database.set_track_image_path(track_id, new_image_path);
    }
}

// ------------------------------ SET TRACK ARTIST ------------------------------
void Core::set_track_artist(int track_id, const std::string& artist_name) {
    // make sure isn't same as old artist
    std::optional<Artist> current_artist = database.get_track_artist(track_id);
    if (current_artist) {
        if (current_artist->name == artist_name) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_artist(track_id, artist_name);

    // update file info (if file exists)
    set_track_file_artist(track_id, artist_name);
}
void Core::set_track_artist_id(int track_id, int artist_id) {
    // make sure isn't same as old artist
    std::optional<int> current_artist_id = database.get_track_artist_id(track_id);
    if (current_artist_id) {
        if (current_artist_id == artist_id) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_artist_id(track_id, artist_id);
    
    // update file info (if file exists)
    set_track_file_artist(track_id, database.get_artist_name(artist_id));
}
// private helper function
void Core::set_track_file_artist(int track_id, const std::string& artist_name) {
    // if file exists: update metadata + file name + database file path
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager metadata{*possible_file_path};
        metadata.set_artist(artist_name);

        // update file path
        Track track_data = get_track(track_id); // TODO: should this call core or db get track??
        std::string new_path = FileManager::make_music_file_path(*possible_file_path, track_data, is_nested);
        new_path = FileManager::move_file(*possible_file_path, new_path, MUSIC_FOLDER);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }
}

// ------------------------------ SET TRACK ALBUM ------------------------------
void Core::set_track_album(int track_id, const std::string& album_title) {
    // make sure isn't same as old album
    std::optional<Album> current_album = database.get_track_album(track_id);
    if (current_album) {
        if (current_album->title == album_title) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_album(track_id, album_title);

    // update file info (if file exists)
    set_track_file_album(track_id, album_title);
}
void Core::set_track_album_id(int track_id, int album_id) {
    // make sure isn't same as old album
    std::optional<int> current_album_id = database.get_track_album_id(track_id);
    if (current_album_id) {
        if (current_album_id == album_id) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_album_id(track_id, album_id);
    
    // update file info (if file exists)
    set_track_file_album(track_id, database.get_album_title(album_id));
}
// private helper function
void Core::set_track_file_album(int track_id, const std::string& album_title) {
    // update in metadata + file path (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager metadata{*possible_file_path};
        metadata.set_album(album_title);

        // update file path
        Track track_data = get_track(track_id); // TODO: should this call core or db get track??
        std::string new_path = FileManager::make_music_file_path(*possible_file_path, track_data, is_nested);
        new_path = FileManager::move_file(*possible_file_path, new_path, MUSIC_FOLDER);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }
}

// ------------------------------ SET TRACK DATE ------------------------------
void Core::set_track_date(int track_id, const Date& track_date) {
    // make sure isn't same as old date
    std::optional<Date> current_date = database.get_track_date(track_id);
    if (current_date) {
        if (*current_date == track_date) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_date(track_id, track_date);

    // TODO: check if date is in metadata, set if so
}
// ------------------------------ SET TRACK TRACKLIST NUM ------------------------------
void Core::set_track_tracklist_num(int track_id, int tracklist_num) {
    // make sure isn't same as old tracklist num
    std::optional<int> current_tracklist_num = database.get_track_tracklist_num(track_id);
    if (current_tracklist_num) {
        if (*current_tracklist_num == tracklist_num) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_track_tracklist_num(track_id, tracklist_num);

    // update in metadata (if there is file)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        // update in metadata
        MetadataManager metadata{*possible_file_path};
        metadata.set_tracklist_num(tracklist_num);
    }
}

// ------------------------------ SET TRACK FILE ------------------------------
void Core::set_track_file(int track_id, const std::string& file_path) {
    // TODO: CODE
    // TODO: EITHER COPY OR MOVE DEPENDING ON "copy_music_files" bool (do same as add_track())
    // TODO: IF NO FILE - ADD NEW FILE TO PROGRAM, ATTACHING TO THIS TRACK
    // TODO: IF HAS FILE - REPLACE THAT FILE, ADD THIS ONE
    // TODO: put new path in database
}
// ------------------------------ SET TRACK IMAGE ------------------------------
void Core::set_track_image(int track_id, const std::string& image_path) {
    // TODO: CODE - IF HAS FILE
        // TODO: get file_path from id, error if no file (or make this function take in file path directly)

        // TODO: call MetadataManager with file_path (returns image_path)

        // TODO: put returned image_path into database

    // TODO: CODE - IF NO FILE
        // TODO: move image directly to correct folder, return image_path
        

    // TODO: put returned image_path into database
}

//--------------------------------------------------------------------------------
//                               SET ALBUM DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET ALBUM TITLE ------------------------------
void Core::set_album_title(int album_id, const std::string& album_title) {
    // make sure isn't same as old title
    std::optional<std::string> current_title = database.get_album_title(album_id);
    if (current_title) {
        if (*current_title == album_title) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_title(album_id, album_title);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ALBUM ARTIST ------------------------------
void Core::set_album_artist(int album_id, const std::string& artist_name) {
    // make sure isn't same as old artist
    std::optional<Artist> current_artist = database.get_album_artist(album_id);
    if (current_artist) {
        if (current_artist->name == artist_name) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_artist(album_id, artist_name);

    // TODO: FILE / METADATA CODE
}
void Core::set_album_artist_id(int album_id, int artist_id) {
    // make sure isn't same as old artist_id
    std::optional<int> current_artist_id = database.get_album_artist_id(album_id);
    if (current_artist_id) {
        if (*current_artist_id == artist_id) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_artist_id(album_id, artist_id);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ALBUM DATE ------------------------------
void Core::set_album_date(int album_id, const Date& album_date) {
    // make sure isn't same as old date
    std::optional<Date> current_date = database.get_album_date(album_id);
    if (current_date) {
        if (*current_date == album_date) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_date(album_id, album_date);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ALBUM TYPE ------------------------------
void Core::set_album_type(int album_id, const std::string& album_type) {
    // make sure isn't same as old album_type
    std::optional<std::string> current_album_type = database.get_album_album_type(album_id);
    if (current_album_type) {
        if (*current_album_type == album_type) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_type(album_id, album_type);

    // TODO: FILE / METADATA CODE
}
void Core::set_album_type_id(int album_id, int album_type_id) {
    // make sure isn't same as old album_type
    std::optional<int> current_album_type_id = database.get_album_album_type_id(album_id);
    if (current_album_type_id) {
        if (*current_album_type_id == album_type_id) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_album_type_id(album_id, album_type_id);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ALBUM IMAGE ------------------------------
void Core::set_album_image(int album_id, const std::string& image_path) {
    // TODO: CODE
    // TODO: NOTE IT'S "set IMAGE" - NOT PATH - MEANS EDIT FILE
    // TODO: NOTE IT'S "set IMAGE" - NOT PATH - MEANS EDIT FILE
    // TODO: update in database
}

//--------------------------------------------------------------------------------
//                               SET ARTIST DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET ARTIST NAME ------------------------------
void Core::set_artist_name(int artist_id, const std::string& artist_name) {
    // make sure isn't same as old name
    std::optional<std::string> current_name = database.get_artist_name(artist_id);
    if (current_name) {
        if (*current_name == artist_name) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_artist_name(artist_id, artist_name);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ARTIST PERSON BEHIND ------------------------------
void Core::set_artist_person_behind(int artist_id, const std::string& person_behind) {
    // make sure isn't same as old person_behind
    std::optional<std::string> current_person_behind = database.get_artist_person_behind(artist_id);
    if (current_person_behind) {
        if (*current_person_behind == person_behind) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_artist_person_behind(artist_id, person_behind);

    // TODO: FILE / METADATA CODE
}
void Core::set_artist_person_behind_id(int artist_id, int person_id) {
    // make sure isn't same as old person_id
    std::optional<int> current_person_id = database.get_artist_person_behind_id(artist_id);
    if (current_person_id) {
        if (*current_person_id == person_id) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_artist_person_behind_id(artist_id, person_id);

    // TODO: FILE / METADATA CODE
}

// ------------------------------ SET ARTIST IMAGE ------------------------------
void Core::set_artist_image(int artist_id, const std::string& image_path) {
    // TODO: CODE
    // TODO: NOTE IT'S "set IMAGE" - NOT PATH - MEANS EDIT FILE
    // TODO: NOTE IT'S "set IMAGE" - NOT PATH - MEANS EDIT FILE
    // TODO: update in database
}

//--------------------------------------------------------------------------------
//                               SET PERSON DATA
//--------------------------------------------------------------------------------
void Core::set_person_name(int person_id, const std::string& person_name) {
    // make sure isn't same as old name
    std::optional<std::string> current_name = database.get_person_name(person_id);
    if (current_name) {
        if (*current_name == person_name) {
            return; // return if same as old
        }
    }

    // update in database
    database.set_person_name(person_id, person_name);

    // TODO: FILE / METADATA CODE
}


//--------------------------------------------------------------------------------
//                               GET -ALL- OBJECTS
//--------------------------------------------------------------------------------
std::vector<Track> Core::get_all_tracks() {
    // get database info
    std::vector<Track> tracks = database.get_all_tracks();

    // TODO: figure out if need file info, & possibly use Core::get_track() for that ???

    return tracks;
}
std::vector<Album> Core::get_all_albums() {
    return database.get_all_albums(); // get database info
}
std::vector<Artist> Core::get_all_artists() {
    return database.get_all_artists(); // get database info
}

//--------------------------------------------------------------------------------
//                            GET OBJECTS BY CATEGORY
//--------------------------------------------------------------------------------
std::vector<Track> Core::get_album_tracks(int album_id) {
    // get database info
    std::vector<Track> tracks = database.get_album_tracks(album_id);

    // TODO: figure out if need file info, & possibly use Core::get_track() for that ???

    return tracks;
}
std::vector<Track> Core::get_artist_tracks(int artist_id) {
    // get database info
    std::vector<Track> tracks = database.get_artist_tracks(artist_id);

    // TODO: figure out if need file info, & possibly use Core::get_track() for that ???

    return tracks;
}
std::vector<Album> Core::get_artist_albums(int artist_id) {
    return database.get_artist_albums(artist_id); // get database info
}
std::vector<Artist> Core::get_person_artists(int person_id) {
    return database.get_person_artists(person_id); // get database info
}

//--------------------------------------------------------------------------------
//                                  GET OBJECT
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
//                            DELETE ENTIRE LIBRARY
//--------------------------------------------------------------------------------
// DELETE all music files, images, entire database file, and another folder too if provided
void Core::delete_entire_library() {
    // ---------- WARNING: EXTREMELY DANGEROUS!!! ----------

    // TODO: PUT EXTRA STEPS IN PLACE TO ENSURE SAFETY WHEN CALLING !!!

    // delete & remake entire database file
    FileManager::delete_file("Code/Core/Database/music_manager.db"); // must match path in database_manager.h
    database = DatabaseManager{}; // remake database file

    // delete entire music files folder (if exists)
    if (FileManager::exists(MUSIC_FOLDER)) {
        FileManager::delete_folder(MUSIC_FOLDER);
    }
    // delete entire images folder (if exists)
    if (FileManager::exists(IMAGES_FOLDER)) {
        FileManager::delete_folder(IMAGES_FOLDER);
    }
}

//--------------------------------------------------------------------------------
//                                 PRIVATE HELPER FUNCTIONS
//--------------------------------------------------------------------------------
void Core::update_file_structure() {
    // grab all tracks with file paths from database
    std::vector<Track> tracks = get_all_tracks();
    for (Track& track : tracks) {
        if (track.file_path != "") {
            // move file to new path
            std::string new_path = FileManager::make_music_file_path(track.file_path, track, is_nested);
            new_path = FileManager::move_file(track.file_path, new_path, MUSIC_FOLDER);

            // update path in database
            database.set_track_file_path(track.id, new_path);
        }
    }
}

// make track title & file name match
std::string Core::set_file_title(const std::string& file_path) {
    MetadataManager metadata_manager{file_path};
    std::string track_title = metadata_manager.get_track_title();

    // make title = file name
    if (track_title.empty()) { // if no track title
        std::string file_name = FileManager::get_file_name(file_path).string();
        metadata_manager.set_track_title(file_name);
        return file_path; // return unchanged file path
    }
    // make file name = title
    else { // if there is track title
        return FileManager::rename_file(file_path, track_title); // rename + return new path
    }
}
