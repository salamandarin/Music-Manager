// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include "artist.h"
#include "date.h"
#include <string>
#include <sqlite3.h>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    // add new objects to DB
    void add_track(const Track& track_data); // add new track and its data to DB
    void add_album(const Album& album_data);
    void add_artist(const Artist& artist_data);
    void add_person(const std::string& person_name);

    // remove objects from DB
    void remove_track(const int& track_id);
    void remove_album(const int& album_id);
    void remove_artist(const int& artist_id);
    void remove_person(const int& person_id);

    // get data
    std::optional<std::string> get_file_path(const int& track_id);

    // set data
    void set_track_date(const int& track_id, const Date& new_date);

    // search if info exists in DB
    std::optional<Album> find_album(const std::string& album_title); // look for album title matches, return Album if found
    std::optional<Artist> find_artist(const std::string& artist_name); // look for artist name matches, return Artist if found

private:
    void execute_sql(const std::string& sql); // only used in constructor
    sqlite3_stmt* prepare_sql(const char* sql_to_prepare, const std::string& input_value);
    
    sqlite3* database;
};