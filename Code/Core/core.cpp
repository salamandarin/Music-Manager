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
    Track track = database.get_track(track_id); // get track data

    // delete file (if exists)
    if (!track.file_path.empty()) { // check if file exists
        FileManager::delete_file(track.file_path, MUSIC_FOLDER); // delete file
    }

    // delete image file (if exists)
    if (!track.image_path.empty()) { // check if file exists
        FileManager::delete_file(track.image_path, IMAGES_FOLDER); // delete file
    }

    // remove in database (must do last)
    database.remove_track(track_id);
}
void Core::remove_album(int album_id) {
    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)

    // delete image file (if exists)
    remove_album_image(album_id); // TODO: keep this function if removing from image tracks too

    // remove in database
    database.remove_album(album_id);
}
void Core::remove_artist(int artist_id) {
    // TODO: remove from all track metadata (or refresh??) (do 1st or 2nd??)

    // delete image file (if exists)
    std::optional<std::string> possible_image_path = database.get_artist_image_path(artist_id);
    if (possible_image_path) { // check if file exists
        FileManager::delete_file(*possible_image_path, IMAGES_FOLDER); // delete file
    }

    // remove in database
    database.remove_artist(artist_id);
}
void Core::remove_person(int person_id) {
    // remove in database
    database.remove_person(person_id);
}

//--------------------------------------------------------------------------------
//                                  REMOVE FILES / IMAGES
//--------------------------------------------------------------------------------
// ------------------------------ REMOVE TRACK FILE ------------------------------
void Core::remove_track_file(int track_id) {
    // delete file (if exists)
    std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
    if (possible_file_path) { // check if file exists
        FileManager::delete_file(*possible_file_path, MUSIC_FOLDER); // delete file
    }

    // update in database
    database.set_track_file_path(track_id, "");
}

// ------------------------------ REMOVE TRACK IMAGE ------------------------------
// TODO: make function to remove ALL track images (plural)? + remove a SPECIFIC image
void Core::remove_track_image(int track_id) {
    Track track = database.get_track(track_id); // get track data

    // delete image file (if exists)
    if (!track.image_path.empty()) { // check if file exists
        FileManager::delete_file(track.image_path, IMAGES_FOLDER); // delete file
    }

    // remove cover art metadata (if has file)
    if (!track.file_path.empty()) { // check if file exists
        MetadataManager metadata{track.file_path};
        metadata.remove_cover_art(); // remove cover art
    }

    // update in database
    database.set_track_image_path(track_id, "");
}
// ------------------------------ REMOVE ALBUM IMAGE ------------------------------
void Core::remove_album_image(int album_id) {
    // delete image file (if exists)
    std::optional<std::string> possible_file_path = database.get_album_image_path(album_id);
    if (possible_file_path) { // check if file exists
        FileManager::delete_file(*possible_file_path, IMAGES_FOLDER); // delete file
    }

    // update in database
    database.set_album_image_path(album_id, "");

    // TODO: remove this image from all tracks too?????? OR NOT???
    // TODO: figure out what if multiple Metadata cover arts?? (remove_cover_art() probably nukes ALL!!)
    // TODO: call ---CORE::remove_track_image(THIS_SPECIFIC_IMAGE)----
}
// ------------------------------ REMOVE ARTIST IMAGE ------------------------------
void Core::remove_artist_image(int artist_id) {
    // delete image file (if exists)
    std::optional<std::string> possible_file_path = database.get_artist_image_path(artist_id);
    if (possible_file_path) { // check if file exists
        FileManager::delete_file(*possible_file_path, IMAGES_FOLDER); // delete file
    }

    // update in database
    database.set_artist_image_path(artist_id, "");
}

//--------------------------------------------------------------------------------
//                                  SET TRACK DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET TRACK TITLE ------------------------------
void Core::set_track_title(int track_id, const std::string& track_title) {
    // make sure isn't same as old title
    Track track_current = database.get_track(track_id); // grab track info
    if (track_current.title == track_title) return; // return if same as old

    // update in database
    database.set_track_title(track_id, track_title);

    // if file exists: update metadata + file name + database file path
    if (!track_current.file_path.empty()) { // check if file exists
        // update in metadata
        MetadataManager metadata{track_current.file_path};
        metadata.set_track_title(track_title);

        // update file name to match
        std::string new_path = FileManager::rename_file(track_current.file_path, track_title);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }

    // if image file exists: rename that too
    if (!track_current.image_path.empty()) { // check if image file exists
        // update image file name to match
        std::string new_image_path = FileManager::rename_file(track_current.image_path, track_title);

        // update image path in database
        database.set_track_image_path(track_id, new_image_path);
    }
}

// ------------------------------ SET TRACK ARTIST ------------------------------
void Core::set_track_artist(int track_id, const std::string& artist_name) {
    // make sure isn't same as old artist
    Track track_current = get_track(track_id); // grab track info
    if (track_current.artist == artist_name) return; // return if same as old

    // update in database
    database.set_track_artist(track_id, artist_name);

    // update file info (if file exists)
    set_track_file_artist(track_id, track_current);
}
void Core::set_track_artist_id(int track_id, int artist_id) {
    // make sure isn't same as old artist
    Track track_current = get_track(track_id); // grab track info
    // if (track_current.artist_id == artist_id) return; // return if same as old // TODO: make get_track() return IDs + uncomment this

    // update in database
    database.set_track_artist_id(track_id, artist_id);
    
    // update file info (if file exists)
    set_track_file_artist(track_id, track_current);
}
// private helper function
void Core::set_track_file_artist(int track_id, const Track& track_data) {
    // if file exists: update metadata + file name + database file path
    if (!track_data.file_path.empty()) { // check if file exists
        // update in metadata
        MetadataManager metadata{track_data.file_path};
        metadata.set_artist(track_data.artist);

        // update file path
        std::string new_path = FileManager::make_music_file_path(track_data.file_path, track_data, is_nested);
        new_path = FileManager::move_file(track_data.file_path, new_path, MUSIC_FOLDER);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }
}

// ------------------------------ SET TRACK ALBUM ------------------------------
void Core::set_track_album(int track_id, const std::string& album_title) {
    // make sure isn't same as old album
    Track track_current = database.get_track(track_id); // grab track info
    if (track_current.album == album_title) return; // return if same as old

    // update in database
    database.set_track_album(track_id, album_title);

    // update file info (if file exists)
    set_track_file_album(track_id, track_current);
}
void Core::set_track_album_id(int track_id, int album_id) {
    // make sure isn't same as old album
    Track track_current = database.get_track(track_id); // grab track info
    // if (track_current.album_id == album_id) return; // return if same as old // TODO: make get_track() return IDs + uncomment this

    // update in database
    database.set_track_album_id(track_id, album_id);
    
    // update file info (if file exists)
    set_track_file_album(track_id, track_current);
}
// private helper function
void Core::set_track_file_album(int track_id, const Track& track_data) {
    // update in metadata + file path (if there is file)
    if (!track_data.file_path.empty()) { // check if file exists
        // update in metadata
        MetadataManager metadata{track_data.file_path};
        metadata.set_album(track_data.album);

        // update file path
        std::string new_path = FileManager::make_music_file_path(track_data.file_path, track_data, is_nested);
        new_path = FileManager::move_file(track_data.file_path, new_path, MUSIC_FOLDER);

        // update file path in database
        database.set_track_file_path(track_id, new_path);
    }
}

// ------------------------------ SET TRACK DATE ------------------------------
void Core::set_track_date(int track_id, const Date& track_date) {
    // make sure isn't same as old date
    Track track_current = database.get_track(track_id); // grab track info
    if (track_current.date == track_date) return; // return if same as old

    // update in database
    database.set_track_date(track_id, track_date);

    // TODO: check if date is in metadata, set if so
}
// ------------------------------ SET TRACK TRACKLIST NUM ------------------------------
void Core::set_track_tracklist_num(int track_id, int tracklist_num) {
    // make sure isn't same as old tracklist num
    Track track_current = database.get_track(track_id); // grab track info
    if (track_current.tracklist_num == tracklist_num) return; // return if same as old

    // update in database
    database.set_track_tracklist_num(track_id, tracklist_num);

    // update in metadata (if there is file)
    if (!track_current.file_path.empty()) { // check if file exists
        // update in metadata
        MetadataManager metadata{track_current.file_path};
        metadata.set_tracklist_num(tracklist_num);
    }
}

// ------------------------------ SET TRACK FILE ------------------------------
void Core::set_track_file(int track_id, std::string file_path) {
    Track track_current = database.get_track(track_id); // grab track info

    // delete old file if exists
    if (!track_current.image_path.empty()) {
        FileManager::delete_file(track_current.image_path, IMAGES_FOLDER);
    }
    // -------------------- ADD FILE --------------------
    // make track title & file name match
    file_path = set_file_title(file_path);

    // gather metadata from track 
    MetadataManager metadata_manager{file_path};
    Track new_track = metadata_manager.get_data();

    // save cover art from metadata (if exists)
    new_track.image_path = metadata_manager.save_cover_art();

    // save music file to correct location
    file_path = FileManager::save_music_file(track_current.file_path, track_current, is_nested, copy_music_files);

    // update database with new file path
    database.set_track_file_path(track_id, file_path);
    // TODO: make database.set_track(int track_id, const Track& new_track)????? OR NO & HAVE ALL OLD INFO TAKE PRECEDENCE??? OR ask to import metadata??? or only take if old is empty????

    // -------------------- ADD IMAGE --------------------
    // if track HAS cover art already
    if (!track_current.image_path.empty()) {
        // if new file HAS cover art (& track had image) - OVERRIDE old image
        if (!new_track.image_path.empty()) {
            FileManager::delete_file(track_current.image_path, IMAGES_FOLDER); // delete old image
            // TODO: figure out plan here (new images is already in files from metadata.save_cover_art(), BUT probably has a number cuz duplicate name
        }
        // if new file has NO cover art (& track had image)
        else {
            // set new file metadata cover art to track image
            metadata_manager.set_cover_art(track_current.image_path);
            new_track.image_path = track_current.image_path;
        }
    }

    // update database with image path
    database.set_track_file_path(track_id, new_track.image_path);

    // TODO: careful if implementing album art - check if image_path == album_image_path
}

// ------------------------------ SET TRACK IMAGE ------------------------------
void Core::set_track_image(int track_id, std::string image_path) {
    // grab track info - make sure isn't same as old path
    Track track_current = database.get_track(track_id); // grab track info
    if (track_current.image_path == image_path) {
        return; // return if same as old
    }

    // delete old file if exists
    if (!track_current.image_path.empty()) {
        FileManager::delete_file(track_current.image_path, IMAGES_FOLDER);
    }
    
    // save image to files
    image_path = FileManager::save_image_file(image_path, track_current.title);

    // update in database
    database.set_track_image_path(track_id, image_path);

    // if music file exists: set file cover art in metadata
    if (!track_current.file_path.empty()) { // check if file exists
        // set metadata cover art
        MetadataManager metadata{track_current.file_path};
        metadata.set_cover_art(image_path);
    }
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
}

// ------------------------------ SET ALBUM'S TYPE ------------------------------
// set ALBUM'S type
bool Core::set_album_type(int album_id, const std::string& type_name) {
    // update in database + return if set
    return database.set_album_type(album_id, type_name); // returns if was set (doesn't set if invalid type)
}
// set ALBUM'S type
void Core::set_album_type_id(int album_id, int type_id) {
    // update in database
    database.set_album_type_id(album_id, type_id);
}

// ------------------------------ SET / REMOVE ALBUM IMAGE ------------------------------
void Core::set_album_image(int album_id, std::string image_path) {
    // grab album info - make sure isn't same as old path
    Album album_current = database.get_album(album_id);
    if (album_current.image_path == image_path) {
        return; // return if same as old
    }

    // delete old file if exists
    if (!album_current.image_path.empty()) {
        FileManager::delete_file(album_current.image_path, IMAGES_FOLDER);
    }

    // save image to files
    image_path = FileManager::save_image_file(image_path, album_current.title);

    // update in database
    database.set_album_image_path(album_id, image_path);

    // TODO: CASCADING TRACK / TRACK FILE INFO??
    // TODO: call ---CORE::set_track_image()---- (for metadata) OR make set_track_ALBUM_image()
    // TODO: if track has no image - then album_image = track_image & metadata cover art, else it's SECONDARY
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
}

// ------------------------------ SET ARTIST IMAGE ------------------------------
void Core::set_artist_image(int artist_id, std::string image_path) {
    // grab artist info - make sure isn't same as old path
    Artist artist_current = database.get_artist(artist_id);
    if (artist_current.image_path == image_path) {
        return; // return if same as old
    }

    // delete old file if exists
    if (!artist_current.image_path.empty()) {
        FileManager::delete_file(artist_current.image_path, IMAGES_FOLDER);
    }

    // save image to files
    image_path = FileManager::save_image_file(image_path, artist_current.name);

    // update in database
    database.set_artist_image_path(artist_id, image_path);
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

    // TODO: FINISH
    // TODO: FILE / METADATA CODE
    // TODO: CASCADING FILE INFO??
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
std::vector<std::string> Core::get_all_people() {
    return database.get_all_people(); // get database info
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
    return database.get_album(album_id); // get database info
}
Artist Core::get_artist(int artist_id) {
    return database.get_artist(artist_id); // get database info
}
std::string Core::get_person(int person_id) { // just gets name
    return database.get_person(person_id); // get database info
}
// album_type (NOT album's album_type)
std::string Core::get_type(int album_type_id) { // just gets name
    return database.get_type(album_type_id); // get database info
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
