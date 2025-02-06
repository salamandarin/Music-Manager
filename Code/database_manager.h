// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include "artist.h"
#include <string>

class DatabaseManager {
public:
    DatabaseManager() = default;

    void log_new_track(const Track& track_data); // add new track and its data to DB

    void log_new_album(const Album& album_data);
    void log_new_artist(const Artist& artist_data);


    std::optional<Album> find_album(const std::string& album_title); // look for album title matches, return Album if found
    std::optional<Artist> find_artist(const std::string& artist_name); // look for artist name matches, return Artist if found

};