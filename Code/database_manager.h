// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include "artist.h"
#include "date.h"
#include <string>

class DatabaseManager {
public:
    DatabaseManager() = default;

    // add new objects to DB
    void log_new_track(const Track& track_data); // add new track and its data to DB
    void log_new_album(const Album& album_data);
    void log_new_artist(const Artist& artist_data);

    // get data
    std::optional<std::string> get_file_path(const int& track_id);

    // set data
    void set_track_date(const int& track_id, const Date& new_date);

    // search if info exists in DB
    std::optional<Album> find_album(const std::string& album_title); // look for album title matches, return Album if found
    std::optional<Artist> find_artist(const std::string& artist_name); // look for artist name matches, return Artist if found

};