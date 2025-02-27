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
void DatabaseManager::add_track(const Track& track) {

    // fetch file data, add to DB (OR NOT CUZ NON-FILE?? + outside scope of function)
 
    // TODO: WRITE REAL CODE

    // TODO: check if already exists & return if so ??? or do that before calling this function???

    // check if need to add new album to DB, make new if so

    // check if need to add new artist to DB, make new if so

    // add new track entry to database, log all info


    std::cout << "Logging new file info to database.......\n";
    std::cout << "Track added to database! [not actually though]\n\n";
}

void DatabaseManager::add_album(const Album& album) {
    // TODO: CODE

    // TODO: check if already exists & return if so ??? or do that before calling this function???


}
void DatabaseManager::add_artist(const Artist& artist) {
    // get person id
    int person_id = 0; // TODO: fix cuz what if null?
    if (!artist.person_behind.empty()) {
        // TODO: 1. check if artist is in db already to get person_behind (OR NOT IF DONE IN PRIOR FUNCTION???)
        // TODO: 2. make artist.person_behind_id = artist.name (make new person, THEN FETCH ID)
        // TODO: 3. fetch id from new person made (OR CHANGE Artist artist.person_behind & CONTINUE TO THE FETCH STUFF BELOW)
    }
    std::optional<int> possible_person_id = get_person_id(artist.person_behind);
    if (possible_person_id) { // if person exists in db
        person_id = *possible_person_id;
    }

    // prep sql
    const char* sql_to_prep = "INSERT INTO artists (name, person_behind_id, image_path) VALUES (?, ?, ?)";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);    

    // bind input to sql
    bind_input_to_sql(sql, 1, artist.name);
    bind_input_to_sql(sql, 2, person_id); // int fk person_id
    bind_input_to_sql(sql, 3, artist.image_path);

    // execute
    if (sqlite3_step(sql) != SQLITE_DONE) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    sqlite3_finalize(sql); // clean up sql statement

    std::cout << "artist should be added!!!!";
}
void DatabaseManager::add_person(const std::string& person) {
    // prep & bind sql
    const char* sql_to_prep = "INSERT INTO people (name) VALUES ( ? )";
    sqlite3_stmt* sql = prepare_sql(sql_to_prep);
    bind_input_to_sql(sql, 1, person);

    // execute
    if (sqlite3_step(sql) != SQLITE_DONE) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    sqlite3_finalize(sql); // clean up sql statement
}

//--------------------------------------------------------------------------------
//                                  REMOVE OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::remove_track(int track_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}
void DatabaseManager::remove_album(int album_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)

}
void DatabaseManager::remove_artist(int artist_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}
void DatabaseManager::remove_person(int person_id) {
    // TODO: CODE (remove albums, artists, people too if they no longer have any tracks or CREDITS)
}


//--------------------------------------------------------------------------------
//                                  GET DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_file_path(int track_id) {
    // TODO: WRITE CODE to return file path IF exists
}
std::optional<int> DatabaseManager::get_person_id(const std::string& person_name) {
    // TODO: WRITE CODE to return id IF exists (also person_name could be "")
    return 1; // TODO: DELETE
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------
void DatabaseManager::set_track_date(int track_id, const Date& new_date) {
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

void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, const std::string& input_value) { // bind string
    if (sqlite3_bind_text(sql, index, input_value.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, int input_value) { // bind int
    if (sqlite3_bind_int(sql, index, input_value) != SQLITE_OK) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}