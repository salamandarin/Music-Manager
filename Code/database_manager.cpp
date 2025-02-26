// Samuel Sutton - 2025
#include "database_manager.h"
#include <fstream>
#include <iostream> //TODO: DELETE

// TODO: CUT DOWN ON DUPLICATED CODE !!!


//--------------------------------------------------------------------------------
//                                  CONSTRUCTOR & DESTRUCTOR
//--------------------------------------------------------------------------------
DatabaseManager::DatabaseManager() {
    // open database
    int return_code = sqlite3_open("../Database/music_manager.db", &database);
    if (return_code){
        throw std::runtime_error(sqlite3_errmsg(database));
    }
    else {// TODO: DELETE
        std::cout << "Opened database successfully\n";
    }

    // initialize tables from .sql file
    std::string file_name = "../Database/tables.sql";
    std::ifstream file{file_name};
    if (!file) {
        throw std::runtime_error("Could not open " + file_name + "\n");
    }
    std::string tables_sql{std::istreambuf_iterator<char>{file}, {}};
    
    execute_sql(tables_sql);
}

DatabaseManager::~DatabaseManager() {
    // close database
    sqlite3_close(database);
}
//--------------------------------------------------------------------------------
//                                  ADD OBJECTS
//--------------------------------------------------------------------------------
// fetch file data, add to DB
void DatabaseManager::add_track(const Track& track_data) {
    // TODO: WRITE REAL CODE

    // TODO: check if already exists & return if so ??? or do that before calling this function???

    // check if need to add new album to DB, make new if so

    // check if need to add new artist to DB, make new if so

    // add new track entry to database, log all info


    std::cout << "Logging new file info to database.......\n";
    std::cout << "Track added to database! [not actually though]\n\n";
}

void DatabaseManager::add_album(const Album& album_data) {
    // TODO: CODE

    // TODO: check if already exists & return if so ??? or do that before calling this function???


}
void DatabaseManager::add_artist(const Artist& artist_data) {
    // TODO: CODE

    // TODO: check if already exists & return if so ??? or do that before calling this function???

    // TODO: if no artist exists, make artist.person_behind = artist.name (may need to make new person too idk)
    
}
void DatabaseManager::add_person(const std::string& person_name) {

    // TODO: check if already exists & return if so ??? or do that before calling this function???

    // prepare sql
    const char* sql = "INSERT INTO people (name) VALUES ( ? )";
    sqlite3_stmt* new_sql = prepare_sql(sql, person_name);

    // execute
    if (sqlite3_step(new_sql) != SQLITE_DONE) {
        sqlite3_finalize(new_sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    sqlite3_finalize(new_sql); // finalize

    std::cout << "Added new person: '" << person_name << "'\n";
}

//--------------------------------------------------------------------------------
//                                  REMOVE OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::remove_track(const int& track_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}
void DatabaseManager::remove_album(const int& album_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)

}
void DatabaseManager::remove_artist(const int& artist_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}
void DatabaseManager::remove_person(const int& person_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}


//--------------------------------------------------------------------------------
//                                  GET DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_file_path(const int& track_id) {
    // TODO: WRITE CODE to return file path IF exists
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------
void DatabaseManager::set_track_date(const int& track_id, const Date& new_date) {
    // TODO: CODE
}

//--------------------------------------------------------------------------------
//                                  SEARCH TOOLS
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


//--------------------------------------------------------------------------------
//                                  PRIVATE FUNCTIONS
//--------------------------------------------------------------------------------
void DatabaseManager::execute_sql(const std::string& sql) { // only used in constructor
    char* error_message = nullptr;
    int return_code = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &error_message);
    if (return_code != SQLITE_OK) {
        std::string error = "SQL error: " + std::string(error_message);
        sqlite3_free(error_message);
        throw std::runtime_error(error);
    }
}

sqlite3_stmt* DatabaseManager::prepare_sql(const char* sql_to_prepare, const std::string& input_value) {
    // prepare sql
    sqlite3_stmt* new_sql;
    int return_code = sqlite3_prepare_v2(database, sql_to_prepare, -1, &new_sql, nullptr);
    if (return_code != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    // bind sql to input
    if (sqlite3_bind_text(new_sql, 1, input_value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(new_sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    return new_sql;
}