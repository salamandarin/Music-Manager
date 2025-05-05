// Samuel Sutton - 2025
#include "database_manager.h"
#include <fstream>

//--------------------------------------------------------------------------------
//                                   BIG FIVE
//--------------------------------------------------------------------------------
DatabaseManager::DatabaseManager() {
    // open database
    int return_code = sqlite3_open("Code/Core/Database/music_manager.db", &database);
    if (return_code){
        throw std::runtime_error(sqlite3_errmsg(database));
    }

    // check if database is new
    sqlite3_stmt* get_version_sql = prepare_sql("PRAGMA user_version;");
    int version = query_sql<int>(get_version_sql, extract_int).value_or(0); // returns 0 if null

    if (version == 0) {
        // initialize tables from .sql file (if new database)
        std::string file_name = "Code/Core/Database/tables.sql";
        std::ifstream file{file_name};
        if (!file) {
            throw std::runtime_error("Could not open " + file_name + "\n");
        }
        std::string tables_sql{std::istreambuf_iterator<char>{file}, {}};

        // execute tables.sql
        char* error_message = nullptr;
        if (sqlite3_exec(database, tables_sql.c_str(), nullptr, nullptr, &error_message) != SQLITE_OK) {
            std::string error = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            throw std::runtime_error(error);
        }        
        
        // set version to 1
        sqlite3_stmt* set_version_sql = prepare_sql("PRAGMA user_version = 1");
        execute_sql(set_version_sql);
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(database); // close database
}

// move constructor
DatabaseManager::DatabaseManager(DatabaseManager&& other) noexcept
    :database{other.database} {
        other.database = nullptr;
}

// move assignment
DatabaseManager& DatabaseManager::operator=(DatabaseManager&& other) noexcept {
    std::swap(database, other.database);
    return *this;
}


//--------------------------------------------------------------------------------
//                                  ADD OBJECTS
//--------------------------------------------------------------------------------
void DatabaseManager::add_track(const Track& track) { 
    // handle adding new cascading data
    // add new artist (if not in db)
    if (!track.artist.empty()) { // if artist is NOT empty
        add_artist(track.artist);
    }
    // add new album (if not in db)
    if (!track.album.empty()) { // if album is NOT empty
        add_album_from_track(track);
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
    bind_input_to_sql(sql, 4, track.duration.get_duration_in_seconds());
    bind_input_to_sql(sql, 5, track.date.to_unix());
    bind_input_to_sql(sql, 6, track.tracklist_num);
    bind_input_to_sql(sql, 7, track.file_path);
    bind_input_to_sql(sql, 8, track.image_path);

    // execute
    execute_sql(sql);
}
void DatabaseManager::add_album(const Album& album) {
    // check if title exists in db already
    std::optional<int> album_id = get_album_id(album.title);
    if (album_id) {
        return; // return early if found
    }

    // handle adding new cascading data
    // add new artist (if not in db)
    if (!album.artist.empty()) { // if artist is NOT empty
        add_artist(album.artist);
    }
    // doesn't add new album type cuz presets only

    // get artist & album type ids
    std::optional<int> artist_id = get_artist_id(album.artist);
    std::optional<int> type_id = get_type_id(album.type);

    // prep sql
    sqlite3_stmt* sql = prepare_sql("INSERT INTO albums (title, artist_id, date, type_id, image_path) VALUES (?, ?, ?, ?, ?)");    

    // bind input to sql
    bind_input_to_sql(sql, 1, album.title);
    bind_input_to_sql(sql, 2, artist_id); // int fk artist_id
    bind_input_to_sql(sql, 3, album.date.to_unix());
    bind_input_to_sql(sql, 4, type_id); // int fk type_id
    bind_input_to_sql(sql, 5, album.image_path);

    // execute
    execute_sql(sql);
}
// wrapper function
void DatabaseManager::add_album_from_track(const Track& track) {
    Album new_album;
    new_album.title = track.album;
    new_album.artist = track.artist; // TODO: figure out what if track has multiple artists?
    new_album.image_path = track.image_path;
    new_album.date = track.date; // TODO: keep or delete?
    add_album(new_album); // calls other function
}
void DatabaseManager::add_artist(const Artist& artist) {
    // check if name exists in db already
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
// wrapper function
void DatabaseManager::add_artist(const std::string& artist_name) {
    Artist new_artist;
    new_artist.name = artist_name;
    add_artist(new_artist); // calls other function
}
void DatabaseManager::add_person(const std::string& person) {
    // check if name exists in db already
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
//                                  GET COMPLETE TABLES
//--------------------------------------------------------------------------------
std::vector<Track> DatabaseManager::get_all_tracks() {
    std::vector<Track> tracks; // make vector
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT tracks.track_id,
                tracks.title, 
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
        ORDER BY artist DESC, album, tracklist_num
    )"); // TODO: make it not descending?

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Track track = get_track_row(sql);
        tracks.push_back(track); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return tracks;
}

std::vector<Album> DatabaseManager::get_all_albums() {
    std::vector<Album> albums; // make vector
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT albums.album_id,
                albums.title,
                artists.name AS artist,
                albums.date,
                album_types.name AS type,
                albums.image_path
        FROM albums
        LEFT JOIN artists ON albums.artist_id = artists.artist_id
        LEFT JOIN album_types ON albums.type_id = album_types.album_type_id
    )");

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Album album = get_album_row(sql);
        albums.push_back(album); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return albums;
}
std::vector<Artist> DatabaseManager::get_all_artists() {
    std::vector<Artist> artists; // make vector
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT artists.artist_id,
                artists.name,
                people.name AS person_behind,
                artists.image_path
        FROM artists
        LEFT JOIN people ON artists.person_behind_id = people.person_id
    )");

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Artist artist = get_artist_row(sql);
        artists.push_back(artist); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return artists;
}
std::vector<std::string> DatabaseManager::get_all_people() {
    std::vector<std::string> people; // make vector
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql("SELECT name FROM people");

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        std::string person_name = extract_string(sql).value_or("");
        people.push_back(person_name); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return people;
}
std::unordered_map<std::string, bool> DatabaseManager::get_all_settings() {
    std::unordered_map<std::string, bool> settings; // make map
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql("SELECT name, value FROM settings");

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        std::optional<std::string> setting_name = extract_string(sql); // column 0
        std::optional<bool> setting_value = extract_bool(sql, 1); // column 1
        if (!setting_name || !setting_value) {
            std::string name = setting_name.value_or("");
            throw std::runtime_error("Found null setting/value in database:\nSetting: '" + name + "'");
        }
        settings[*setting_name] = *setting_value; // set map
    }

    sqlite3_finalize(sql); // clean up sql statement
    return settings;
}

//--------------------------------------------------------------------------------
//                            GET OBJECTS BY CATEGORY
//--------------------------------------------------------------------------------
// get all tracks in album - ORDERED BY TRACKLIST_NUM
std::vector<Track> DatabaseManager::get_album_tracks(int album_id) {
    std::vector<Track> tracks; // make vector

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT tracks.track_id,
                tracks.title, 
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
        WHERE tracks.album_id = ?
        ORDER BY tracklist_num
    )");
    bind_input_to_sql(sql, 1, album_id);

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Track track = get_track_row(sql);
        tracks.push_back(track); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return tracks;
}
std::vector<Track> DatabaseManager::get_artist_tracks(int artist_id) {
    std::vector<Track> tracks; // make vector

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT tracks.track_id,
                tracks.title, 
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
        WHERE tracks.artist_id = ?
    )");
    bind_input_to_sql(sql, 1, artist_id);

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Track track = get_track_row(sql);
        tracks.push_back(track); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return tracks;
}
std::vector<Album> DatabaseManager::get_artist_albums(int artist_id) {
    std::vector<Album> albums; // make vector
    
    // prep sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT albums.album_id,
                albums.title,
                artists.name AS artist,
                albums.date,
                album_types.name AS type,
                albums.image_path
        FROM albums
        LEFT JOIN artists ON albums.artist_id = artists.artist_id
        LEFT JOIN album_types ON albums.type_id = album_types.album_type_id
        WHERE albums.artist_id = ?
    )");
    bind_input_to_sql(sql, 1, artist_id);

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Album album = get_album_row(sql);
        albums.push_back(album); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return albums;
}
std::vector<Artist> DatabaseManager::get_person_artists(int person_id) {
    std::vector<Artist> artists; // make vector

    // prep sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT artists.artist_id,
                artists.name,
                people.name AS person_behind,
                artists.image_path
        FROM artists
        LEFT JOIN people ON artists.person_behind_id = people.person_id
        WHERE artists.person_behind_id = ?
    )");
    bind_input_to_sql(sql, 1, person_id);

    // execute & grab all data for each row
    while (sqlite3_step(sql) == SQLITE_ROW) {
        Artist artist = get_artist_row(sql);
        artists.push_back(artist); // add to vector
    }

    sqlite3_finalize(sql); // clean up sql statement
    return artists;
}
//--------------------------------------------------------------------------------
//                               GET ENTIRE OBJECT
//--------------------------------------------------------------------------------
Track DatabaseManager::get_track(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT tracks.track_id,
                tracks.title, 
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

    // make sure track exists in db
    if (sqlite3_step(sql) == SQLITE_ROW) {
        // execute & return row
        Track track = get_track_row(sql);
        sqlite3_finalize(sql); // clean up sql statement
        return track;
    }
    // if track NOT in db, throw error
    else {
        sqlite3_finalize(sql); // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}

Album DatabaseManager::get_album(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT albums.album_id,
                albums.title,
                artists.name AS artist,
                albums.date,
                album_types.name AS type,
                albums.image_path
        FROM albums
        LEFT JOIN artists ON albums.artist_id = artists.artist_id
        LEFT JOIN album_types ON albums.type_id = album_types.album_type_id
        WHERE albums.album_id = ?
    )");
    bind_input_to_sql(sql, 1, album_id);

    // make sure album exists in db
    if (sqlite3_step(sql) == SQLITE_ROW) {
        // execute & return row
        Album album = get_album_row(sql);
        sqlite3_finalize(sql);  // clean up sql statement
        return album;
    }
    // if album NOT in db, throw error
    else {
        sqlite3_finalize(sql); // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}

Artist DatabaseManager::get_artist(int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql(R"(
        SELECT artists.artist_id,
                artists.name,
                people.name AS person_behind,
                artists.image_path
        FROM artists
        LEFT JOIN people ON artists.person_behind_id = people.person_id
        WHERE artists.artist_id = ?
    )");
    bind_input_to_sql(sql, 1, artist_id);

    // make sure artist exists in db
    if (sqlite3_step(sql) == SQLITE_ROW) {
        // execute & return row
        Artist artist = get_artist_row(sql);
        sqlite3_finalize(sql); // clean up sql statement
        return artist;
    }
    // if artist NOT in db, throw error
    else {
        sqlite3_finalize(sql); // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}

std::string DatabaseManager::get_person(int person_id) {
    // doesn't return id
    return get_person_name(person_id);
}
// album_type (NOT album's album_type)
std::string DatabaseManager::get_type(int album_type_id) { // non-optional
    // doesn't return id
    return get_type_name(album_type_id);
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
//                            GET SPECIFIC TRACK DATA
//--------------------------------------------------------------------------------
std::optional<std::string> DatabaseManager::get_track_title(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT title FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}

std::optional<Artist> DatabaseManager::get_track_artist(int track_id) {
    // get artist id
    std::optional<int> artist_id = get_track_artist_id(track_id);

    // return artist if exists
    if (artist_id) {
        return get_artist(*artist_id);
    }
    // return null if no artist
    else {
        return std::nullopt;
    }
}
std::optional<int> DatabaseManager::get_track_artist_id(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT artist_id FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

std::optional<Album> DatabaseManager::get_track_album(int track_id) {
    // get album id
    std::optional<int> album_id = get_track_album_id(track_id);

    // return album if exists
    if (album_id) {
        return get_album(*album_id);
    }
    // return null if no album
    else {
        return std::nullopt;
    }
}
std::optional<int> DatabaseManager::get_track_album_id(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT album_id FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

std::optional<Duration> DatabaseManager::get_track_duration(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT duration FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<Duration>(sql, extract_duration);
}
std::optional<Date> DatabaseManager::get_track_date(int track_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT date FROM tracks WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_id); // bind id

    // execute & return result
    return query_sql<Date>(sql, extract_date);
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
//                           GET SPECIFIC ALBUM DATA
//--------------------------------------------------------------------------------
std::string DatabaseManager::get_album_title(int album_id) { // non-optional
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT title FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    std::optional<std::string> album_title = query_sql<std::string>(sql, extract_string);
    if (!album_title) { // throw error if failed since album title can't be null
        throw std::runtime_error("Could not find title for album with ID: " + std::to_string(album_id));
    }
    return *album_title;
}

std::optional<Artist> DatabaseManager::get_album_artist(int album_id) {
    // get artist id
    std::optional<int> artist_id = get_album_artist_id(album_id);

    // return artist if exists
    if (artist_id) {
        return get_artist(*artist_id);
    }
    // return null if no artist
    else {
        return std::nullopt;
    }
}
std::optional<int> DatabaseManager::get_album_artist_id(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT artist_id FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

std::optional<Date> DatabaseManager::get_album_date(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT date FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    return query_sql<Date>(sql, extract_date);
}

// ALBUM'S type
std::optional<std::string> DatabaseManager::get_album_type(int album_id) {
    // get ALBUM'S type id
    std::optional<int> type_id = get_album_type_id(album_id);

    // return type if exists
    if (type_id) {
        return get_type(*type_id); // get type name
    }
    // return null if no type
    else {
        return std::nullopt;
    }
}
// ALBUM'S type
std::optional<int> DatabaseManager::get_album_type_id(int album_id) { 
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT type_id FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

std::optional<std::string> DatabaseManager::get_album_image_path(int album_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT image_path FROM albums WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}

//--------------------------------------------------------------------------------
//                             GET SPECIFIC ARTIST DATA
//--------------------------------------------------------------------------------
std::string DatabaseManager::get_artist_name(int artist_id) { // non-optional
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT name FROM artists WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // bind id

    // execute & return result
    std::optional<std::string> artist_name = query_sql<std::string>(sql, extract_string);
    if (!artist_name) { // throw error if failed since artist name can't be null
        throw std::runtime_error("Could not find name for artist with ID: " + std::to_string(artist_id));
    }
    return *artist_name;
}

std::optional<std::string> DatabaseManager::get_artist_person_behind(int artist_id) {
    // get person id
    std::optional<int> person_id = get_artist_person_behind_id(artist_id);

    // return person if exists
    if (person_id) {
        return get_person(*person_id);
    }
    // return null if no person
    else {
        return std::nullopt;
    }
}
std::optional<int> DatabaseManager::get_artist_person_behind_id(int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT person_behind_id FROM artists WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // bind id

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

std::optional<std::string> DatabaseManager::get_artist_image_path(int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT image_path FROM artists WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // bind id

    // execute & return result
    return query_sql<std::string>(sql, extract_string);
}
//--------------------------------------------------------------------------------
//                            GET OTHER SPECIFIC OBJECT DATA
//--------------------------------------------------------------------------------
// get person name
std::string DatabaseManager::get_person_name(int person_id) { // non-optional
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT name FROM people WHERE person_id = ?");
    bind_input_to_sql(sql, 1, person_id); // bind id

    // execute & return result
    std::optional<std::string> person_name = query_sql<std::string>(sql, extract_string);
    if (!person_name) { // throw error if failed since person name can't be null
        throw std::runtime_error("Could not find name for person with ID: " + std::to_string(person_id));
    }
    return *person_name;
}

// album_type (NOT album's album_type)
std::string DatabaseManager::get_type_name(int album_type_id) { // non-optional
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT name FROM album_types WHERE album_type_id = ?");
    bind_input_to_sql(sql, 1, album_type_id); // bind id

    // execute & return result
    std::optional<std::string> type_name = query_sql<std::string>(sql, extract_string);
    if (!type_name) { // throw error if failed since type name can't be null
        throw std::runtime_error("Could not find name for album_type with ID: " + std::to_string(album_type_id));
    }
    return *type_name;
}

//--------------------------------------------------------------------------------
//                                  GET ID FROM DATA
//--------------------------------------------------------------------------------
// ------------------------- GET ID FROM NAME -------------------------
std::optional<int> DatabaseManager::get_track_id(const std::string& track_title) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT track_id FROM tracks WHERE title = ?");
    bind_input_to_sql(sql, 1, track_title); // bind title

    // execute & return result
    return query_sql<int>(sql, extract_int);
}
std::optional<int> DatabaseManager::get_album_id(const std::string& album_title) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT album_id FROM albums WHERE title = ?");
    bind_input_to_sql(sql, 1, album_title); // bind title

    // execute & return result
    return query_sql<int>(sql, extract_int);
}
// album_type (NOT album's album_type)
std::optional<int> DatabaseManager::get_type_id(const std::string& album_type_name) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT album_type_id FROM album_types WHERE name = ?");
    bind_input_to_sql(sql, 1, album_type_name); // bind album_type_name

    // execute & return result
    return query_sql<int>(sql, extract_int);
}
std::optional<int> DatabaseManager::get_artist_id(const std::string& artist_name) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT artist_id FROM artists WHERE name = ?");
    bind_input_to_sql(sql, 1, artist_name); // bind name

    // execute & return result
    return query_sql<int>(sql, extract_int);
}
std::optional<int> DatabaseManager::get_person_id(const std::string& person_name) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT person_id FROM people WHERE name = ?");
    bind_input_to_sql(sql, 1, person_name); // bind name

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

// ------------------------- GET ID FROM FILE PATH -------------------------
std::optional<int> DatabaseManager::get_track_id_from_file(const std::string& file_path) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("SELECT track_id FROM tracks WHERE file_path = ?");
    bind_input_to_sql(sql, 1, file_path); // bind file path

    // execute & return result
    return query_sql<int>(sql, extract_int);
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------

// ------------------------------ SET TRACK DATA ------------------------------
// set track title
void DatabaseManager::set_track_title(int track_id, const std::string& track_title) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET title = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_title); // track_title
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);
}

// set track artist
void DatabaseManager::set_track_artist(int track_id, const std::string& artist_name) {
    // save old artist to possibly delete later (if empty)
    std::optional<int> old_artist_id = get_track_artist_id(track_id);

    // add new artist ONLY IF name given
    if (!artist_name.empty()) {
        add_artist(artist_name);
    }

    // get possible artist_id
    std::optional<int> artist_id = get_artist_id(artist_name);

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET artist_id = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // artist_id (binds null if empty)
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);

    // delete old artist if had artist before & that artist now has no songs
    if (old_artist_id) { // check if had valid old artist
        if (get_artist_tracks(*old_artist_id).size() == 0) { // check if artist has no tracks
            remove_artist(*old_artist_id);
        }
    }
}
// set track artist (with given artist_id)
void DatabaseManager::set_track_artist_id(int track_id, int artist_id) {
    // save old artist to possibly delete later (if empty)
    std::optional<int> old_artist_id = get_track_artist_id(track_id);
    
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET artist_id = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // artist_id
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);

    // delete old artist if had artist before & that artist now has no songs
    if (old_artist_id) { // check if had valid old artist
        if (get_artist_tracks(*old_artist_id).size() == 0) { // check if artist has no tracks
            remove_artist(*old_artist_id);
        }
    }
}

// set track album
void DatabaseManager::set_track_album(int track_id, const std::string& album_title) {
    Track track = get_track(track_id); // get track info
    
    // save old album to possibly delete later (if empty)
    std::string old_album = track.album;

    // add new album ONLY IF title given
    if (!album_title.empty()) { // TODO: change to checking album existing HERE to prevent uneeded get_track()
        track.album = album_title; // use NEW album
        add_album_from_track(track);
    }

    // get possible album_id
    std::optional<int> album_id = get_album_id(album_title);

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET album_id = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, album_id); // album_id (binds null if empty)
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);

    // delete old album if had album before & that album is now empty
    std::optional<int> old_album_id = get_album_id(old_album);
    if (old_album_id) { // check if had valid old album
        if (get_album_tracks(*old_album_id).size() == 0) { // check if album is empty
            remove_album(*old_album_id);
        }
    }
}
// set track album (with given album_id)
void DatabaseManager::set_track_album_id(int track_id, int album_id) {
    // save old album to possibly delete later (if empty)
    std::optional<int> old_album_id = get_track_album_id(track_id);
    
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET album_id = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, album_id); // album_id
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);

    // delete old album if had album before & that album is now empty
    if (old_album_id) { // check if had valid old album
        if (get_album_tracks(*old_album_id).size() == 0) { // check if album is empty
            remove_album(*old_album_id);
        }
    }
}

// set track date
void DatabaseManager::set_track_date(int track_id, const Date& track_date) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET date = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, track_date.to_unix()); // track_date
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);
}
// set track tracklist number
void DatabaseManager::set_track_tracklist_num(int track_id, int tracklist_num) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET tracklist_num = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, tracklist_num); // tracklist_num
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);
}
// set track file path
void DatabaseManager::set_track_file_path(int track_id, const std::string& file_path) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET file_path = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, file_path); // file_path
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);
}
// set track image path
void DatabaseManager::set_track_image_path(int track_id, const std::string& image_path) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE tracks SET image_path = ? WHERE track_id = ?");
    bind_input_to_sql(sql, 1, image_path); // image_path
    bind_input_to_sql(sql, 2, track_id); // track_id

    // execute
    execute_sql(sql);
}

// ------------------------------ SET ALBUM DATA ------------------------------
// set album title
void DatabaseManager::set_album_title(int album_id, const std::string& album_title) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET title = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_title); // album_title
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}

// set album artist
void DatabaseManager::set_album_artist(int album_id, const std::string& artist_name) {
    // add new artist ONLY IF name given
    if (!artist_name.empty()) {
        add_artist(artist_name);
    }

    // get possible artist_id
    std::optional<int> artist_id = get_artist_id(artist_name);

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET artist_id = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // artist_id (binds null if empty)
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}
// set album artist (with given artist_id)
void DatabaseManager::set_album_artist_id(int album_id, int artist_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET artist_id = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, artist_id); // artist_id
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}

// set album date
void DatabaseManager::set_album_date(int album_id, const Date& album_date) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET date = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, album_date.to_unix()); // album_date
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}

// set ALBUM'S type
bool DatabaseManager::set_album_type(int album_id, const std::string& type_name) {
    // get (possible) id of given type (doesn't make new type cuz presets only)
    std::optional<int> type_id = get_type_id(type_name);

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET type_id = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, type_id); // type_id (binds null if empty)
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);

    // return bool if actually set or not (aka if valid id cuz doesn't set if invalid type)
    return type_id.has_value();
}
// set ALBUM'S type (with given type_id)
void DatabaseManager::set_album_type_id(int album_id, int type_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET type_id = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, type_id); // type_id
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}

// set album image path
void DatabaseManager::set_album_image_path(int album_id, const std::string& image_path) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE albums SET image_path = ? WHERE album_id = ?");
    bind_input_to_sql(sql, 1, image_path); // image_path
    bind_input_to_sql(sql, 2, album_id); // album_id

    // execute
    execute_sql(sql);
}

// ------------------------------ SET ARTIST DATA ------------------------------
// set artist name
void DatabaseManager::set_artist_name(int artist_id, const std::string& artist_name) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE artists SET name = ? WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, artist_name); // artist_name
    bind_input_to_sql(sql, 2, artist_id); // artist_id

    // execute
    execute_sql(sql);
}

// set artist person_behind
void DatabaseManager::set_artist_person_behind(int artist_id, const std::string& person_behind) {
    // add new person ONLY IF name given
    if (!person_behind.empty()) {
        add_person(person_behind);
    }

    // get possible person_id
    std::optional<int> person_id = get_person_id(person_behind);

    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE artists SET person_behind_id = ? WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, person_id); // person_id (binds null if empty)
    bind_input_to_sql(sql, 2, artist_id); // artist_id

    // execute
    execute_sql(sql);
}
// set artist person_behind (with given person_id)
void DatabaseManager::set_artist_person_behind_id(int artist_id, int person_id) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE artists SET person_behind_id = ? WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, person_id); // person_id
    bind_input_to_sql(sql, 2, artist_id); // artist_id

    // execute
    execute_sql(sql);
}

// set artist image path
void DatabaseManager::set_artist_image_path(int artist_id, const std::string& image_path) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE artists SET image_path = ? WHERE artist_id = ?");
    bind_input_to_sql(sql, 1, image_path); // image_path
    bind_input_to_sql(sql, 2, artist_id); // artist_id

    // execute
    execute_sql(sql);
}

// ------------------------------ SET OTHER DATA ------------------------------
// set person name
void DatabaseManager::set_person_name(int person_id, const std::string& person_name) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE people SET name = ? WHERE person_id = ?");
    bind_input_to_sql(sql, 1, person_name); // person_name
    bind_input_to_sql(sql, 2, person_id); // person_id

    // execute
    execute_sql(sql);
}

// set setting value
void DatabaseManager::set_setting_value(const std::string& setting_name, bool setting_value) {
    // prep & bind sql
    sqlite3_stmt* sql = prepare_sql("UPDATE settings SET value = ? WHERE name = ?");
    bind_input_to_sql(sql, 1, setting_value); // setting_value
    bind_input_to_sql(sql, 2, setting_name); // setting_name

    // execute
    execute_sql(sql);
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
// int64_t
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, int64_t input_value) {
    if (sqlite3_bind_int64(sql, index, input_value) != SQLITE_OK) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}
// bool
void DatabaseManager::bind_input_to_sql(sqlite3_stmt* sql, int index, bool input_value) {
    int bool_value = input_value; // convert bool to int
    if (sqlite3_bind_int(sql, index, bool_value) != SQLITE_OK) {
        sqlite3_finalize(sql);  // clean up if failed
        throw std::runtime_error(sqlite3_errmsg(database));
    }
}
// null
void DatabaseManager::bind_null_to_sql(sqlite3_stmt* sql, int index) {
    if (sqlite3_bind_null(sql, index) != SQLITE_OK) {
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
std::optional<Duration> DatabaseManager::extract_duration(sqlite3_stmt* sql, int column) {
    // return nullopt if null
    if (sqlite3_column_type(sql, column) == SQLITE_NULL) {
        return std::nullopt;
    }
    // extract data
    return Duration{sqlite3_column_int(sql, column)}; 
}
std::optional<Date> DatabaseManager::extract_date(sqlite3_stmt* sql, int column) {
    // return nullopt if null
    if (sqlite3_column_type(sql, column) == SQLITE_NULL) {
        return std::nullopt;
    }
    // extract data
    return Date::from_unix(sqlite3_column_int64(sql, column)); 
}

//--------------------------------------------------------------------------------
//                      PRIVATE FUNCTIONS - GET ENTIRE OBJECT ROW
//--------------------------------------------------------------------------------
Track DatabaseManager::get_track_row(sqlite3_stmt* sql) {
    // SHOULD STEP & CHECK IF EXISTS IN DB BEFORE CALLING THIS FUNCTION!!!
    Track track;

    track.id = extract_int(sql, 0).value_or(0); // id - 0
    track.title = extract_string(sql, 1).value_or(""); // title - 1
    track.artist = extract_string(sql, 2).value_or(""); // artist (NAME) - 2
    track.album = extract_string(sql, 3).value_or(""); // album (TITLE) - 3
    track.duration = extract_duration(sql, 4).value_or(Duration{}); // duration - 4
    track.date = extract_date(sql, 5).value_or(Date{}); // date - 5
    track.tracklist_num = extract_int(sql, 6).value_or(0); // tracklist_num - 6
    track.file_path = extract_string(sql, 7).value_or(""); // file_path - 7
    track.image_path= extract_string(sql, 8).value_or(""); // image_path - 8

    return track;
}

Album DatabaseManager::get_album_row(sqlite3_stmt* sql) {
    // SHOULD STEP & CHECK IF EXISTS IN DB BEFORE CALLING THIS FUNCTION!!!
    Album album;

    album.id = extract_int(sql, 0).value_or(0); // id - 0
    album.title = extract_string(sql, 1).value_or(""); // title - 1
    album.artist = extract_string(sql, 2).value_or(""); // artist (NAME) - 2
    album.date = extract_date(sql, 3).value_or(Date{}); // date - 3
    album.type = extract_string(sql, 4).value_or(""); // type (NAME) - 4
    album.image_path= extract_string(sql, 5).value_or(""); // image_path - 5

    return album;
}

Artist DatabaseManager::get_artist_row(sqlite3_stmt* sql) {
    // SHOULD STEP & CHECK IF EXISTS IN DB BEFORE CALLING THIS FUNCTION!!!
    Artist artist;

    artist.id = extract_int(sql, 0).value_or(0); // id - 0
    artist.name = extract_string(sql, 1).value_or(""); // name - 1
    artist.person_behind = extract_string(sql, 2).value_or(""); // person_behind (NAME) - 2
    artist.image_path = extract_string(sql, 3).value_or(""); // image_path - 3

    return artist;
}
