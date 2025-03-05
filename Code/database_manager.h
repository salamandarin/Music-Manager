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
    void add_track(const Track& track);
    void add_album(const Album& album);
    void add_artist(const Artist& artist);
    void add_person(const std::string& person);

    // remove objects from DB
    void remove_track(int track_id);
    void remove_album(int album_id);
    void remove_artist(int artist_id);
    void remove_person(int person_id);

    // check if object exists
    bool track_exists(int track_id);

    // ------------------------- GET DATA -------------------------
    // get data for entire objects
    std::optional<Album> get_album(const std::string& album_title); // look for album title matches, return Album if found
    std::optional<Artist> get_artist(const std::string& artist_name); // look for artist name matches, return Artist if found

    // get specific data
    std::optional<std::string> get_file_path(int track_id);

    // get id from name
    std::optional<int> get_track_id(const std::string& track_title);
    std::optional<int> get_album_id(const std::string& album_title);
    std::optional<int> get_album_type_id(const std::string& album_type);
    std::optional<int> get_artist_id(const std::string& artist_name);
    std::optional<int> get_person_id(const std::string& person_name);

    // get title from id
    std::optional<std::string> get_track_title(int track_id);

    // ------------------------- SET DATA -------------------------
    // set track data
    void set_track_title(int track_id, const std::string& title);
    // TODO: SET TRACK ARTIST (ID)
    // TODO: SET TRACK ALBUM (ID)
    void set_track_date(int track_id, const Date& date);
    void set_track_tracklist_num(int track_id, int tracklist_num);
    void set_track_file_path(int track_id, const std::string& file_path);
    void set_track_image_path(int track_id, const std::string& image_path);

    // set album data
    void set_album_title(int album_id, const std::string& title);
    // TODO: SET ALBUM ARTIST (ID)
    void set_album_date(int album_id, const Date& date);
    // TODO: SET ALBUM TYPE (ID)
    void set_album_image_path(int album_id, const std::string& image_path);

    void set_artist_name(int artist_id, const std::string& name);
    // TODO: SET ARTIST PERSON BEHIND (ID)
    void set_artist_image_path(int artist_id, const std::string& image_path);

    // set person data
    void set_person_name(int person_id, const std::string& name);

private:
    sqlite3_stmt* prepare_sql(const char* sql_to_prepare);
    void execute_sql(sqlite3_stmt* sql);

    // bind input to sql statement
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<std::string>& input_value); // optional string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<int>& input_value); // optional int
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::string& input_value); // string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, int input_value); // int

    // ---------- backend of other functions ---------- 
    // get id by name
    std::optional<int> get_id_by_name(const std::string& id_label,
                                     const std::string& table,
                                     const std::string& name_label,
                                     const std::string& name_to_search);
    // set object value by id
    void set_object_value(const std::string& table,
                                          const std::string& value_label,
                                          const std::string& value,
                                          const std::string& id_label,
                                          int id);
    // check if data exists
    bool object_exists(const std::string& id_label, const std::string& table, int id);

        
    // data
    sqlite3* database;
};