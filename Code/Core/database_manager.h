// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include "artist.h"
#include "date.h"
#include <string>
#include <sqlite3.h>
#include <unordered_map>
#include <vector>

class DatabaseManager {
public:
    // ------------------------- BIG FIVE -------------------------
    DatabaseManager(); // constructor
    ~DatabaseManager(); // destructor

    // disable copying (cuz can't copy database connections)
    DatabaseManager(const DatabaseManager&) = delete; // delete copy constructor
    DatabaseManager& operator=(const DatabaseManager&) = delete; // delete copy assignment
    
    // allow moving
    DatabaseManager(DatabaseManager&& other) noexcept; // move constructor
    DatabaseManager& operator=(DatabaseManager&& other) noexcept; // move assignment


    // ------------------------- ADD / REMOVE OBJECTS -------------------------
    // add new objects to db
    void add_track(const Track& track);

    void add_album(const Album& album);
    void add_album_from_track(const Track& track);

    void add_artist(const Artist& artist);
    void add_artist(const std::string& artist_name);

    void add_person(const std::string& person);

    // remove objects from db
    void remove_track(int track_id);
    void remove_album(int album_id);
    void remove_artist(int artist_id);
    void remove_person(int person_id);


    // ------------------------- GET OBJECTS -------------------------
    // get ALL objects in table
    std::vector<Track> get_all_tracks();
    std::vector<Album> get_all_albums();
    std::vector<Artist> get_all_artists();
    std::vector<std::string> get_all_people();
    std::unordered_map<std::string, bool> get_all_settings();

    // get objects by category
    std::vector<Track> get_album_tracks(int album_id); // ordered by tracklist_num
    std::vector<Track> get_artist_tracks(int artist_id);
    std::vector<Album> get_artist_albums(int artist_id);
    std::vector<Artist> get_person_artists(int person_id);

    // get whole objects
    Track get_track(int track_id);
    Album get_album(int album_id);
    Artist get_artist(int artist_id);
    std::string get_person(int person_id); // just name
    std::string get_type(int album_type_id); // just name (album_type, NOT album's album_type)

    // get whole objects by name (not id)
    std::optional<Album> get_album(const std::string& album_title);
    std::optional<Artist> get_artist(const std::string& artist_name);


    // ------------------------- GET DATA -------------------------
    
    // get specific track data
    std::optional<std::string> get_track_title(int track_id);

    std::optional<Artist> get_track_artist(int track_id);
    std::optional<int> get_track_artist_id(int track_id);

    std::optional<Album> get_track_album(int track_id);
    std::optional<int> get_track_album_id(int track_id);

    std::optional<Duration> get_track_duration(int track_id);
    std::optional<Date> get_track_date(int track_id);
    std::optional<int> get_track_tracklist_num(int track_id);
    std::optional<std::string> get_track_file_path(int track_id);
    std::optional<std::string> get_track_image_path(int track_id);


    // get specific album data
    std::string get_album_title(int album_id); // non-optional

    std::optional<Artist> get_album_artist(int album_id);
    std::optional<int> get_album_artist_id(int album_id);

    std::optional<Date> get_album_date(int album_id);

    std::optional<std::string> get_album_type(int album_id); // ALBUM'S type
    std::optional<int> get_album_type_id(int album_id); // ALBUM'S type id

    std::optional<std::string> get_album_image_path(int album_id);
    

    // get specific artist data
    std::string get_artist_name(int artist_id); // non-optional

    std::optional<std::string> get_artist_person_behind(int artist_id);
    std::optional<int> get_artist_person_behind_id(int artist_id);

    std::optional<std::string> get_artist_image_path(int artist_id);


    // get other specific object data
    std::string get_person_name(int person_id); // non-optional
    std::string get_type_name(int album_type_id); // non-optional (album_type, NOT album's album_type)

    // ------------------------- GET ID -------------------------
    // get id from name
    std::optional<int> get_track_id(const std::string& track_title);
    std::optional<int> get_album_id(const std::string& album_title);
    std::optional<int> get_type_id(const std::string& album_type_name); // album_type, NOT album's album_type)
    std::optional<int> get_artist_id(const std::string& artist_name);
    std::optional<int> get_person_id(const std::string& person_name);

    // get id from other values
    std::optional<int> get_track_id_from_file(const std::string& file_path);


    // ------------------------- SET DATA -------------------------
    // set track data
    void set_track_title(int track_id, const std::string& track_title);

    void set_track_artist(int track_id, const std::string& artist_name);
    void set_track_artist_id(int track_id, int artist_id);

    void set_track_album(int track_id, const std::string& album_title);
    void set_track_album_id(int track_id, int album_id);

        // skip duration - do not manually set
    void set_track_date(int track_id, const Date& track_date);
    void set_track_tracklist_num(int track_id, int tracklist_num);
    void set_track_file_path(int track_id, const std::string& file_path);
    void set_track_image_path(int track_id, const std::string& image_path);


    // set album data
    void set_album_title(int album_id, const std::string& album_title);

    void set_album_artist(int album_id, const std::string& artist_name);
    void set_album_artist_id(int album_id, int artist_id);

    void set_album_date(int album_id, const Date& album_date);

    bool set_album_type(int album_id, const std::string& type_name); // ALBUM'S type (returns if set or not)
    void set_album_type_id(int album_id, int type_id); // ALBUM'S type id

    void set_album_image_path(int album_id, const std::string& image_path);


    // set artist data
    void set_artist_name(int artist_id, const std::string& artist_name);
    
    void set_artist_person_behind(int artist_id, const std::string& person_behind);
    void set_artist_person_behind_id(int artist_id, int person_id);

    void set_artist_image_path(int artist_id, const std::string& image_path);


    // set other data
    void set_person_name(int person_id, const std::string& person_name);
    void set_setting_value(const std::string& setting_name, bool setting_value);


private:
    sqlite3_stmt* prepare_sql(const std::string& sql_to_prepare);

    // bind input to sql statement
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<std::string>& input_value); // optional string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<int>& input_value); // optional int
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::string& input_value); // string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, int input_value); // int
    void bind_input_to_sql(sqlite3_stmt* sql, int index, int64_t input_value); // int64_t
    void bind_input_to_sql(sqlite3_stmt* sql, int index, bool input_value); // bool
    void bind_null_to_sql(sqlite3_stmt* sql, int index); // null

    // --------------- backend of other functions ---------------
    // execute sql queries
    void execute_sql(sqlite3_stmt* sql); // queries with no return
    template<typename T> // queries with return
    std::optional<T> query_sql(sqlite3_stmt* sql, std::optional<T> (*result_extractor)(sqlite3_stmt*, int), int column=0);

    // type extractors
    static std::optional<std::string> extract_string(sqlite3_stmt* sql, int column=0);
    static std::optional<int> extract_int(sqlite3_stmt* sql, int column=0);
    static std::optional<bool> extract_bool(sqlite3_stmt* sql, int column=0);
    static std::optional<Duration> extract_duration(sqlite3_stmt* sql, int column=0);
    static std::optional<Date> extract_date(sqlite3_stmt* sql, int column=0);

    // get entire object rows
    Track get_track_row(sqlite3_stmt* sql);
    Album get_album_row(sqlite3_stmt* sql);
    Artist get_artist_row(sqlite3_stmt* sql);

    // data
    sqlite3* database;
};


// execute sql queries that return 1 thing
template<typename T>
    std::optional<T> DatabaseManager::query_sql(sqlite3_stmt* sql, std::optional<T> (*result_extractor)(sqlite3_stmt*, int), int column) {
    // if result exists, return data
    if (sqlite3_step(sql) == SQLITE_ROW) {
        std::optional<T> extracted_value = result_extractor(sql, column); // extractor handles if data is null
        sqlite3_finalize(sql); // clean up sql statement
        return extracted_value;
    }
    // if result NOT found, return null
    else {
        sqlite3_finalize(sql); // clean up sql statement
        return std::nullopt;
    }
}
