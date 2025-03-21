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
    // close database
    sqlite3_close(database);
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
    const char* sql_to_prep = "INSERT INTO tracks (title, artist_id, album_id, duration, date, tracklist_num, file_path, image_path) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);    

    // bind input to sql
    bind_input_to_sql(sql, 1, track.title);
    bind_input_to_sql(sql, 2, artist_id); // int fk artist_id
    bind_input_to_sql(sql, 3, album_id); // int fk album_id
    // bind_input_to_sql(sql, 4, album.duration); // TODO: MAKE STRING
    // bind_input_to_sql(sql, 5, album.date); // TODO: MAKE STRING
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
    const char* sql_to_prep = "INSERT INTO albums (title, artist_id, date, type_id, image_path) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);    

    // bind input to sql
    bind_input_to_sql(sql, 1, album.title);
    bind_input_to_sql(sql, 2, artist_id); // int fk artist_id
    // bind_input_to_sql(sql, 3, album.date); // TODO: MAKE STRING
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
    const char* sql_to_prep = "INSERT INTO artists (name, person_behind_id, image_path) VALUES (?, ?, ?)";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);    

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
    const char* sql_to_prep = "INSERT INTO people (name) VALUES ( ? )";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, person);

    // execute
    execute_sql(sql);
}

//--------------------------------------------------------------------------------
//                                  REMOVE OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::remove_track(int track_id) {
    
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
    
    // prep & bind sql
    const char* sql_to_prep = "DELETE FROM tracks WHERE track_id = ?";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, track_id);

    // execute
    execute_sql(sql);
}
void DatabaseManager::remove_album(int album_id) {
    
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
    
    // prep & bind sql
    const char* sql_to_prep = "DELETE FROM albums WHERE album_id = ?";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, album_id);

    // execute
    execute_sql(sql);
}
void DatabaseManager::remove_artist(int artist_id) {
    
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)
    
    // prep & bind sql
    const char* sql_to_prep = "DELETE FROM artists WHERE artist_id = ?";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, artist_id);

    // execute
    execute_sql(sql);
}
void DatabaseManager::remove_person(int person_id) {
    
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
    // TODO: OR JUST FLAG INSTEAD OF REMOVING STUFF (might still want)

    // prep & bind sql
    const char* sql_to_prep = "DELETE FROM people WHERE person_id = ?";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, person_id);

    // execute
    execute_sql(sql);
}


//--------------------------------------------------------------------------------
//                                  GET ENTIRE OBJECTS
//--------------------------------------------------------------------------------

// look for album title matches, return Album if found
std::optional<Album> DatabaseManager::get_album(const std::string& album_title) {
    //TODO: CODE

    // return album found (or nullopt)
    return std::nullopt;
}

// look for artist name matches, return Artist if found
std::optional<Artist> DatabaseManager::get_artist(const std::string& artist_name) {
    //TODO: CODE

    // return artist found (or nullopt)
    return std::nullopt;
}

//--------------------------------------------------------------------------------
//                                  GET SPECIFIC DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_file_path(int track_id) {
    // TODO: WRITE CODE to return file path IF exists
    return std::nullopt;
}

// ------------------------- GET ID BY NAME -------------------------
std::optional<int> DatabaseManager::get_id_by_name(const std::string& name_to_search,
                                                   const std::string& table,
                                                   const std::string& id_label,
                                                   const std::string& name_label) {
    // if no name provided
    if (name_to_search.empty()) {
        return std::nullopt;
    }

    std::string sql_to_prep = "SELECT " + id_label + " FROM " + table + " WHERE " + name_label + " = ?;";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep.c_str());
    bind_input_to_sql(sql, 1, name_to_search);

    int result = sqlite3_step(sql);
    // if result is found
    if (result == SQLITE_ROW) {
        int id = sqlite3_column_int(sql, 0);
        sqlite3_finalize(sql); // clean up sql statement
        return id;
    }
    // if result is NOT found
    else {
        sqlite3_finalize(sql); // clean up sql statement
        return std::nullopt; 
    }                                        
}
// ------------------------- GET ID BY NAME WRAPPERS -------------------------
std::optional<int> DatabaseManager::get_track_id(const std::string& track_title) {
    return get_id_by_name(track_title, "tracks", "track_id", "title");
}
std::optional<int> DatabaseManager::get_album_id(const std::string& album_title) {
    return get_id_by_name(album_title, "albums", "album_id", "title");
}
std::optional<int> DatabaseManager::get_album_type_id(const std::string& album_type) {
    return get_id_by_name(album_type, "album_types", "type_id", "name");
}
std::optional<int> DatabaseManager::get_artist_id(const std::string& artist_name) {
    return get_id_by_name(artist_name, "artists", "artist_id", "name");
}
std::optional<int> DatabaseManager::get_person_id(const std::string& person_name) {
    return get_id_by_name(person_name, "people", "person_id", "name");
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------
void DatabaseManager::set_track_date(int track_id, const Date& new_date) {
    // TODO: CODE
}

//--------------------------------------------------------------------------------
//                                  PRIVATE FUNCTIONS
//--------------------------------------------------------------------------------
sqlite3_stmt* DatabaseManager::prepare_sql(const char* sql_to_prepare) {
    // prepare sql
    sqlite3_stmt* new_sql;
    int return_code = sqlite3_prepare_v2(database, sql_to_prepare, -1, &new_sql, nullptr);
    if (return_code != SQLITE_OK) {
        sqlite3_finalize(new_sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    return new_sql;
}

void DatabaseManager::execute_sql(sqlite3_stmt* sql) {
    // execute
    if (sqlite3_step(sql) != SQLITE_DONE) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    sqlite3_finalize(sql); // clean up sql statement
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