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
    bool album_exists(int album_id);
    bool artist_exists(int artist_id);
    bool person_exists(int person_id);

    // ------------------------- GET DATA -------------------------
    // get entire tables
    std::vector<Track> get_all_tracks();
    std::vector<Album> get_all_albums();
    std::vector<Artist> get_all_artists();
    std::vector<std::string> get_all_people();

    // get data for entire objects
    std::optional<Track> get_track(int track_id);
    std::optional<Album> get_album(int album_id);
    std::optional<Artist> get_artist(int artist_id);

    // get entire objects by name (not id)
    std::optional<Album> get_album(const std::string& album_title);
    std::optional<Artist> get_artist(const std::string& artist_name);

    // get specific track data
    std::optional<std::string> get_track_title(int track_id);
    std::optional<Artist> get_track_artist(int track_id);
    std::optional<Album> get_track_album(int track_id);
    std::optional<Duration> get_track_duration(int track_id);
    std::optional<Date> get_track_date(int track_id);
    std::optional<int> get_track_tracklist_num(int track_id);
    std::optional<std::string> get_track_file_path(int track_id);
    std::optional<std::string> get_track_image_path(int track_id);

    // get specific album data
    std::optional<std::string> get_album_title(int album_id);

    // get specific artist data
    std::optional<std::string> get_artist_name(int artist_id);

    // get id from name
    std::optional<int> get_track_id(const std::string& track_title);
    std::optional<int> get_album_id(const std::string& album_title);
    std::optional<int> get_album_type_id(const std::string& album_type);
    std::optional<int> get_artist_id(const std::string& artist_name);
    std::optional<int> get_person_id(const std::string& person_name);

    // ------------------------- SET DATA -------------------------
    // set track data
    void set_track_title(int track_id, const std::string& title);
    void set_track_artist(int track_id, const std::string& artist_name);
    void set_track_album(int track_id, const std::string& album_title);
    void set_track_date(int track_id, const Date& date);
    void set_track_tracklist_num(int track_id, int tracklist_num);
    void set_track_file_path(int track_id, const std::string& file_path);
    void set_track_image_path(int track_id, const std::string& image_path);

    // set album data
    void set_album_title(int album_id, const std::string& title);
    void set_album_artist(int album_id, const std::string& artist_name);
    void set_album_date(int album_id, const Date& date);
    void set_album_type(int album_id, const std::string& album_type);
    void set_album_image_path(int album_id, const std::string& image_path);

    void set_artist_name(int artist_id, const std::string& name);
    void set_artist_person_behind(int artist_id, const std::string& person_behind);
    void set_artist_image_path(int artist_id, const std::string& image_path);

    // set person data
    void set_person_name(int person_id, const std::string& name);

private:
    sqlite3_stmt* prepare_sql(const std::string& sql_to_prepare);

    // bind input to sql statement
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<std::string>& input_value); // optional string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::optional<int>& input_value); // optional int
    void bind_input_to_sql(sqlite3_stmt* sql, int index, const std::string& input_value); // string
    void bind_input_to_sql(sqlite3_stmt* sql, int index, int input_value); // int

    // ---------- backend of other functions ---------- 

    // execute sql queries
    void execute_sql(sqlite3_stmt* sql); // queries with no return
    template<typename T> // queries with return
    std::optional<T> query_sql(sqlite3_stmt* sql, std::optional<T> (*result_extractor)(sqlite3_stmt*, int), int column=0);

    // type extractors
    static std::optional<std::string> extract_string(sqlite3_stmt* sql, int column=0);
    static std::optional<int> extract_int(sqlite3_stmt* sql, int column=0);
    static std::optional<bool> extract_bool(sqlite3_stmt* sql, int column=0);

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