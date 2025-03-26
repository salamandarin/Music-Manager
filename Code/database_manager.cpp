// Samuel Sutton - 2025
#include "database_manager.h"
#include <fstream>
#include <iostream> //TODO: DELETE

//--------------------------------------------------------------------------------
//                                  CONSTRUCTOR & DESTRUCTOR
//--------------------------------------------------------------------------------
DatabaseManager::DatabaseManager() {
    // open database
    int return_code = sqlite3_open("../Database/music_manager.db", &database);
    if (return_code){
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    // initialize tables from .sql file
    std::string file_name = "../Database/tables.sql";
    std::ifstream file{file_name};
    if (!file) {
        throw std::runtime_error("Could not open " + file_name + "\n");
    }
    std::string tables_sql{std::istreambuf_iterator<char>{file}, {}};
    
    // execute tables_sql
    char* error_message = nullptr;
    if (sqlite3_exec(database, tables_sql.c_str(), nullptr, nullptr, &error_message) != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(error_message);
        sqlite3_free(error_message);
        throw std::runtime_error(error);
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(database); // close database
}

//--------------------------------------------------------------------------------
//                                  ADD OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::add_track(const Track& track) { 
    // check if exists in DB already
    std::optional<int> track_id = get_track_id(track.title);
    if (track_id) {
        return; // return early if found
    }

    // handle adding new cascading data
    // add new artist (if not in DB)
    if (!track.artist.empty()) { // if artist is NOT empty
        Artist new_artist;
        new_artist.name = track.artist;
        add_artist(new_artist);
    }
    // add new album (if not in DB)
    if (!track.album.empty()) { // if album is NOT empty
        Album new_album;
        new_album.title = track.album;
        add_album(new_album);
    }

    // get artist & album ids
    std::optional<int> artist_id = get_artist_id(track.artist);
    std::optional<int> album_id = get_album_id(track.album);

    // prep sql
    sqlite3_stmt* sql = prepare_sql("INSERT INTO tracks (title, artist_id, album_id, duration, date, tracklist_num, file_path, image_path) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");    

    // bind input to sql
    bind_input_to_sql(sql, 1, track.title);
    bind_input_to_sql(sql, 2, artist_id); // int fk artist_id
    bind_input_to_sql(sql, 3, album_id); // int fk album_id
    bind_input_to_sql(sql, 4, track.duration.to_string());
    bind_input_to_sql(sql, 5, track.date.to_string());
    bind_input_to_sql(sql, 6, track.tracklist_num);
    bind_input_to_sql(sql, 7, track.file_path);
    bind_input_to_sql(sql, 8, track.image_path);

    // execute
    execute_sql(sql);
}
void DatabaseManager::add_album(const Album& album) {
    // check if exists in DB already
    std::optional<int> album_id = get_album_id(album.title);
    if (album_id) {
        return; // return early if found
    }

    // handle adding new cascading data
    // add new artist (if not in DB)
    if (!album.artist.empty()) { // if artist is NOT empty
        Artist new_artist;
        new_artist.name = album.artist;
        add_artist(new_artist);
    }

    // get artist & album type ids
    std::optional<int> artist_id = get_artist_id(album.artist);
    std::optional<int> type_id = get_album_type_id(album.type);

    // prep sql
    sqlite3_stmt* sql = prepare_sql("INSERT INTO albums (title, artist_id, date, type_id, image_path) VALUES (?, ?, ?, ?, ?)");    

    // bind input to sql
    bind_input_to_sql(sql, 1, album.title);
    bind_input_to_sql(sql, 2, artist_id); // int fk artist_id
    bind_input_to_sql(sql, 3, album.date.to_string());
    bind_input_to_sql(sql, 4, type_id); // int fk type_id
    bind_input_to_sql(sql, 5, album.image_path);

    // execute
    execute_sql(sql);
}
void DatabaseManager::add_artist(const Artist& artist) {
    // check if exists in DB already
    std::optional<int> artist_id = get_artist_id(artist.name);
    if (artist_id) {
        return; // return early if found
    }

    // handle adding new cascading data
    // add new person (use artist name if person name not provided)
    std::string person_name = artist.person_behind.empty() ? artist.name : artist.person_behind;
    add_person(person_name);

    // get person id
    std::optional<int> possible_person_id = get_person_id(person_name); // use person name from above

    // prep sql
    sqlite3_stmt* sql = prepare_sql("INSERT INTO artists (name, person_behind_id, image_path) VALUES (?, ?, ?)");    

    // bind input to sql
    bind_input_to_sql(sql, 1, artist.name);
    bind_input_to_sql(sql, 2, possible_person_id); // int fk person_id
    bind_input_to_sql(sql, 3, artist.image_path);

    // execute
    execute_sql(sql);
}
void DatabaseManager::add_person(const std::string& person) {
    // check if exists in DB already
    std::optional<int> person_id = get_person_id(person);
    if (person_id) {
        return; // return early if found
    }

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("INSERT INTO people (name) VALUES ( ? )");
    bind_input_to_sql(sql, 1, person);

    // execute
    execute_sql(sql);
}

//--------------------------------------------------------------------------------
//                                  REMOVE OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::remove_track(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("DELETE FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id);

    // execute
    execute_sql(sql);

    // TODO: remove cascading data (remove albums, artists, people) too if they no longer have any tracks or CREDITS?
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
}
void DatabaseManager::remove_album(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("DELETE FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id);

    // execute
    execute_sql(sql);

    // TODO: remove cascading data (remove albums, artists, people) too if they no longer have any tracks or CREDITS?
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
}
void DatabaseManager::remove_artist(int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("DELETE FROM artists WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_id);

    // execute
    execute_sql(sql);

    // TODO: remove cascading data (remove albums, artists, people) too if they no longer have any tracks or CREDITS?
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
}
void DatabaseManager::remove_person(int person_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("DELETE FROM people WHERE person_id = ?");
    bind_input_to_sql(sql, 1, person_id);

    // execute
    execute_sql(sql);

    // TODO: remove cascading data (remove albums, artists, people) too if they no longer have any tracks or CREDITS?
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
}

//--------------------------------------------------------------------------------
//                                  CHECK IF OBJECT EXISTS
//--------------------------------------------------------------------------------
bool DatabaseManager::track_exists(int track_id) {
    return object_exists("track_id", "tracks", track_id);
}
bool DatabaseManager::album_exists(int album_id) {
    return object_exists("album_id", "albums", album_id);
}
bool DatabaseManager::artist_exists(int artist_id) {
    return object_exists("artist_id", "artists", artist_id);
}
bool DatabaseManager::person_exists(int person_id) {
    return object_exists("person_id", "people", person_id);
}

//--------------------------------------------------------------------------------
//                                  GET COMPLETE TABLES
//--------------------------------------------------------------------------------
std::vector<Track> DatabaseManager::get_all_tracks() {
    std::vector<Track> tracks; // make vector

    // prep sql
    sqlite3_stmt* sql = prepare_sql("SELECT track_id, title, artist_id, album_id, duration, date, tracklist_num, file_path, image_path FROM tracks");

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Track track;
        
        // track.id = extract_int(sql, 0); // track_id (0)
        // track.title = extract_string(sql, 1); // title (1)

        // int artist_id = sqlite3_column_int(sql, 2); // artist_id (2)
        // if (artist_id) {
        //     std::optional<Artist> artist = get_artist(artist_id);
        //     if (artist) {
        //         track.artist = artist->name;
        //     }
        // }

        // // Extract album_id and get album title if available
        // int album_id = sqlite3_column_int(sql, 3);
        // if (album_id) {
        //     std::optional<Album> album = get_album(album_id);
        //     if (album) {
        //         track.album = album->title;
        //     }
        // }

        // // duration
        // const char* duration_str = reinterpret_cast<const char*>(sqlite3_column_text(sql, 4));
        // if (duration_str) {
        //     track.duration = Duration::from_string(duration_str);
        // }

        // // date
        // const char* date_str = reinterpret_cast<const char*>(sqlite3_column_text(sql, 5));
        // if (date_str) {
        //     track.date = Date::from_string(date_str);
        // }

        // // tracklist num
        // track.tracklist_num = sqlite3_column_int(sql, 6);

        // // file path
        // const char* file_path = reinterpret_cast<const char*>(sqlite3_column_text(sql, 7));
        // if (file_path) {
        //     track.file_path = file_path;
        // }

        // // image path
        // const char* image_path = reinterpret_cast<const char*>(sqlite3_column_text(sql, 8));
        // if (image_path) {
        //     track.image_path = image_path;
        // }

        tracks.push_back(track); // add to vector
    }


    sqlite3_finalize(sql); // clean up sql statement
    return tracks;
}
std::vector<Album> DatabaseManager::get_all_albums() {
    std::vector<Album> albums;

    // TODO: CODE

    return albums;
}
std::vector<Artist> DatabaseManager::get_all_artists() {
    std::vector<Artist> artists;

    // TODO: CODE

    return artists;
}
std::vector<std::string> DatabaseManager::get_all_people() {
    std::vector<std::string> people;

    // TODO: CODE

    return people;
}

//--------------------------------------------------------------------------------
//                                  GET ENTIRE OBJECTS
//--------------------------------------------------------------------------------
std::optional<Track> DatabaseManager::get_track(int track_id) {
    // if track exists in db
    if (track_exists(track_id)) {
        // prep & bind sql
        sqlite3_stmt* sql = prepare_sql(R"(
            SELECT tracks.title, 
                    artists.name AS artist, 
                    albums.title AS album,
                    tracks.duration,
                    tracks.date,
                    tracks.tracklist_num,
                    tracks.file_path,
                    tracks.image_path
            FROM tracks
            LEFT JOIN artists ON tracks.artist_id = artists.artist_id
            LEFT JOIN albums ON tracks.album_id = albums.album_id
            WHERE tracks.track_id = ?
        )");
        bind_input_to_sql(sql, 1, track_id);

        // execute
        if (sqlite3_step(sql) == SQLITE_ROW) {
            Track track;

            track.id = track_id; // id
            track.title = extract_string(sql, 0).value_or(""); // title - 0
            track.artist = extract_string(sql, 1).value_or(""); // artist NAME - 1
            track.album = extract_string(sql, 2).value_or(""); // album TITLE - 2
            // track.duration = extract_string(sql, 3).value_or(""); // duration - 3 // TODO: HANDLE STRING/INT -> DURATION
            // track.date = extract_string(sql, 4).value_or(""); // date - 4 // TODO: HANDLE STRING -> DATE
            track.tracklist_num = extract_int(sql, 5).value_or(0); // tracklist_num - 5
            track.file_path = extract_string(sql, 6).value_or(""); // file_path - 6
            track.image_path= extract_string(sql, 7).value_or(""); // image_path - 7

            sqlite3_finalize(sql);
            return track;
        }
        // if track NOT found, return null
        else {
            sqlite3_finalize(sql); // clean up sql statement
            return std::nullopt;
        }
    }
}

std::optional<Album> DatabaseManager::get_album(int album_id) {
    if (album_exists(album_id)) { // make sure it's in db
        Album album;
        album.id = album_id;

        // TODO: FINISH!!!!

        return album;
    }
    // return null if not found
    else {
        return std::nullopt;
    }
}
std::optional<Artist> DatabaseManager::get_artist(int artist_id) {
    if (artist_exists(artist_id)) { // make sure it's in db
        Artist artist;
        artist.id = artist_id; // id

        // TODO: FINISH!!!!

        return artist;
    }
    // return null if not found
    else {
        return std::nullopt;
    }
}

// ------------------------ GET OBJECTS BY NAME ------------------------
std::optional<Album> DatabaseManager::get_album(const std::string& album_title) {
    // get id from title
    std::optional<int> album_id = get_album_id(album_title);

    // return album if exists
    if (album_id) {
        return get_album(*album_id);
    }
    // return null if no album
    else {
        return std::nullopt;
    }
}
std::optional<Artist> DatabaseManager::get_artist(const std::string& artist_name) {
    // get id from name
    std::optional<int> artist_id = get_artist_id(artist_name);

    // return artist if exists
    if (artist_id) {
        return get_artist(*artist_id);
    }
    // return null if no artist
    else {
        return std::nullopt;
    }
}

//--------------------------------------------------------------------------------
//                                  GET SPECIFIC TRACK DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_track_title(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT title FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}
std::optional<Artist> DatabaseManager::get_track_artist(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT artist_id FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // get artist id
    std::optional<int> artist_id = query_sql<int>(sql, extract_int);

    // return artist if exists
    if (artist_id) {
        return get_artist(*artist_id);
    }
    // return null if no artist
    else {
        return std::nullopt;
    }
}
std::optional<Album> DatabaseManager::get_track_album(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT album_id FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // get album id
    std::optional<int> album_id = query_sql<int>(sql, extract_int);

    // return album if exists
    if (album_id) {
        return get_album(*album_id);
    }
    // return null if no album
    else {
        return std::nullopt;
    }
}
std::optional<Duration> DatabaseManager::get_track_duration(int track_id) {
    // TODO: CONVERT OPTIONAL STRING TO OPTIONAL DURATION (preserve nullopt)
    return std::nullopt;
}
std::optional<Date> DatabaseManager::get_track_date(int track_id) {
    // TODO: CONVERT OPTIONAL STRING TO OPTIONAL DATE (preserve nullopt)
    return std::nullopt;
}
std::optional<int> DatabaseManager::get_track_tracklist_num(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT tracklist_num FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}
std::optional<std::string> DatabaseManager::get_track_file_path(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT file_path FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}
std::optional<std::string> DatabaseManager::get_track_image_path(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT image_path FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}

//--------------------------------------------------------------------------------
//                                  GET SPECIFIC ALBUM DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_album_title(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT title FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}

//--------------------------------------------------------------------------------
//                                  GET SPECIFIC ARTIST DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_artist_name(int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT name FROM artists WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}

// ------------------------- GET ID BY NAME -------------------------
std::optional<int> DatabaseManager::get_track_id(const std::string& track_title) {
    return get_id_by_name("track_id", "tracks", "title", track_title);
}
std::optional<int> DatabaseManager::get_album_id(const std::string& album_title) {
    return get_id_by_name("album_id", "albums", "title", album_title);
}
std::optional<int> DatabaseManager::get_album_type_id(const std::string& album_type) {
    return get_id_by_name("type_id", "album_types", "name", album_type);
}
std::optional<int> DatabaseManager::get_artist_id(const std::string& artist_name) {
    return get_id_by_name("artist_id", "artists", "name", artist_name);
}
std::optional<int> DatabaseManager::get_person_id(const std::string& person_name) {
    return get_id_by_name("person_id", "people", "name", person_name);
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET TRACK DATA ------------------------------
// set track title
void DatabaseManager::set_track_title(int track_id, const std::string& title) {
    set_object_value("tracks", "title", title, "track_id", track_id);
}
// set track artist
void DatabaseManager::set_track_artist(int track_id, const std::string& artist_name) {
    // TODO: check if artist exists, or make new artist
    // TODO: grab the artist id
    // TODO: set track artist id

    // TODO: possibly overload to also take in Artist type too, use that to set person?
}
// set track album
void DatabaseManager::set_track_album(int track_id, const std::string& album_title) {
    // TODO: check if album exists, or make new album
    // TODO: grab the album id
    // TODO: set track album id

    // TODO: possibly overload to also take in Album type too, use that info too?
}
// set track date
void DatabaseManager::set_track_date(int track_id, const Date& date) {
    set_object_value("tracks", "date", date.to_string(), "track_id", track_id);
}
// set track tracklist number
void DatabaseManager::set_track_tracklist_num(int track_id, int tracklist_num) {
    set_object_value("tracks", "tracklist_num", std::to_string(tracklist_num), "track_id", track_id);
}
// set track file path
void DatabaseManager::set_track_file_path(int track_id, const std::string& file_path) {
    set_object_value("tracks", "file_path", file_path, "track_id", track_id);
}
// set track image path
void DatabaseManager::set_track_image_path(int track_id, const std::string& image_path) {
    set_object_value("tracks", "image_path", image_path, "track_id", track_id);
}

// ------------------------------ SET ALBUM DATA ------------------------------
// set album title
void DatabaseManager::set_album_title(int album_id, const std::string& title) {
    set_object_value("albums", "title", title, "album_id", album_id);
}
// set album artist
void DatabaseManager::set_album_artist(int album_id, const std::string& artist_name) {
    // TODO: check if artist exists, or make new artist
    // TODO: grab the artist id
    // TODO: set album artist id

    // TODO: possibly overload to also take in Artist type too, use that to set person?
}
// set album date
void DatabaseManager::set_album_date(int album_id, const Date& date) {
    set_object_value("albums", "date", date.to_string(), "album_id", album_id);
}
// set album type
void DatabaseManager::set_album_type(int album_id, const std::string& album_type) {
    // TODO: grab album type id, return error if doesn't exist
    // TODO: set album type id
}
// set album image path
void DatabaseManager::set_album_image_path(int album_id, const std::string& image_path) {
    set_object_value("albums", "image_path", image_path, "album_id", album_id);
}

// ------------------------------ SET ARTIST DATA ------------------------------
// set artist name
void DatabaseManager::set_artist_name(int artist_id, const std::string& name) {
    set_object_value("artists", "name", name, "artist_id", artist_id);
}
// set artist person behind
void DatabaseManager::set_artist_person_behind(int artist_id, const std::string& person_behind) {
    // TODO: check if person exists, or make new person
    // TODO: grab the person id
    // TODO: set artist person behind id
}
// set artist image path
void DatabaseManager::set_artist_image_path(int artist_id, const std::string& image_path) {
    set_object_value("artists", "image_path", image_path, "artist_id", artist_id);
}

// ------------------------------ SET PERSON DATA ------------------------------
// set person name
void DatabaseManager::set_person_name(int person_id, const std::string& name) {
    set_object_value("people", "name", name, "person_id", person_id);
}

//--------------------------------------------------------------------------------
//                  PRIVATE FUNCTIONS - PREP/BIND/EXECUTE SQL
//--------------------------------------------------------------------------------
sqlite3_stmt* DatabaseManager::prepare_sql(const std::string& sql_to_prepare) {
    // prepare sql
    sqlite3_stmt* new_sql;
    int return_code = sqlite3_prepare_v2(database, sql_to_prepare.c_str(), -1, &new_sql, nullptr);
    if (return_code != SQLITE_OK) {
        sqlite3_finalize(new_sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    return new_sql;
}

// ------------------------- BIND SQL -------------------------
// optional string
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<std::string>& input_value) {
    if (input_value) {
        bind_input_to_sql(sql, index, *input_value);
    }
    else {
        if (sqlite3_bind_null(sql, index) != SQLITE_OK) {
            sqlite3_finalize(sql);  // clean up if failed
            throw std::runtime_error(sqlite3_errmsg(database));
        }
    }
}
// optional int
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<int>& input_value) {
    if (input_value) {
        bind_input_to_sql(sql, index, *input_value);
    }
    else {
        if (sqlite3_bind_null(sql, index) != SQLITE_OK) {
            sqlite3_finalize(sql);  // clean up if failed
            throw std::runtime_error(sqlite3_errmsg(database));
        }
    }
}
// string
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, const std::string& input_value) {
    // check if empty string, bind null if so
    if (input_value.empty()) {
        if (sqlite3_bind_null(sql, index) != SQLITE_OK) {
            sqlite3_finalize(sql);  // clean up if failed
            throw std::runtime_error(sqlite3_errmsg(database));
        }
        return;
    }
    // bind non-empty strings
    if (sqlite3_bind_text(sql, index, input_value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}
// int
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, int input_value) {
    if (sqlite3_bind_int(sql, index, input_value) != SQLITE_OK) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}

//--------------------------------------------------------------------------------
//                      PRIVATE FUNCTIONS - QUERY
//--------------------------------------------------------------------------------

// execute sql queries that don't return anything
void DatabaseManager::execute_sql(sqlite3_stmt* sql) {
    // execute
    if (sqlite3_step(sql) != SQLITE_DONE) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
    sqlite3_finalize(sql); // clean up sql statement
}


// get id by name
std::optional<int> DatabaseManager::get_id_by_name(const std::string& id_label,
                                                   const std::string& table,
                                                   const std::string& name_label,
                                                   const std::string& name_to_search) {
    // if no name provided
    if (name_to_search.empty()) {
        return std::nullopt;
    }

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT " + id_label + " FROM " + table + " WHERE " + name_label + " = ?");
    bind_input_to_sql(sql, 1, name_to_search);

    // if result exists, return data
    if (sqlite3_step(sql) == SQLITE_ROW) {
        std::optional<int> id = extract_int(sql); // extract handles if null
        sqlite3_finalize(sql); // clean up sql statement
        return id;
    }
    // if result NOT found, return null
    else {
        sqlite3_finalize(sql); // clean up sql statement
        return std::nullopt;
    }                                        
}

// set object data
void DatabaseManager::set_object_value(const std::string& table,
                                       const std::string& value_label,
                                       const std::string& value,
                                       const std::string& id_label,
                                       int id) {
    // make sure object exists
    if (!object_exists(id_label, table, id)) { // if object DOESN'T exist
        // make error message
        std::string error_message = "Tried to set " + value_label + " to '" + value;
        error_message += "' for object in " + table + ", but object with ";
        error_message += id_label + " '" + std::to_string(id) + "' was not found";
        // throw error
        throw std::runtime_error(error_message);
    }

    // prep sql
    sqlite3_stmt* sql = prepare_sql("UPDATE " + table + " SET " + value_label + " = ? WHERE " + id_label + "= ?");

    // bind input to sql
    bind_input_to_sql(sql, 1, value);
    bind_input_to_sql(sql, 2, id);

    // execute
    execute_sql(sql);
}

// check if object exists
bool DatabaseManager::object_exists(const std::string& id_label, const std::string& table, int id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT " + id_label + " FROM " + table + " WHERE " + id_label + " = ?");
    bind_input_to_sql(sql, 1, id); // bind id

    // execute & get result
    bool exists = (sqlite3_step(sql) == SQLITE_ROW);

    sqlite3_finalize(sql); // clean up sql statement
    return exists;
}

//--------------------------------------------------------------------------------
//                      PRIVATE FUNCTIONS - EXTRACT TYPES
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::extract_string(sqlite3_stmt* sql, int column) {
    // return nullopt if null
    if (sqlite3_column_type(sql, column) == SQLITE_NULL) {
        return std::nullopt;
    }
    // extract data
    const unsigned char* text = sqlite3_column_text(sql, column);
    return reinterpret_cast<const char*>(text);   
}
std::optional<int> DatabaseManager::extract_int(sqlite3_stmt* sql, int column) {
    // return nullopt if null
    if (sqlite3_column_type(sql, column) == SQLITE_NULL) {
        return std::nullopt;
    }
    // extract data
    return sqlite3_column_int(sql, column); 
}
std::optional<bool> DatabaseManager::extract_bool(sqlite3_stmt* sql, int column) {
    // return nullopt if null
    if (sqlite3_column_type(sql, column) == SQLITE_NULL) {
        return std::nullopt;
    }
    // extract data
    return sqlite3_column_int(sql, column) != 0;
}