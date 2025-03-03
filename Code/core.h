// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include "date.h"
#include "database_manager.h"
#include "file_manager.h"
#include <string>

class Core {
public:
    Core() = default;

    // add tracks
    void add_track(const std::string& file_path); // track w/ file
    void add_track(const Track& track); // track w/ or w/o file
    void add_tracks_from_folder(const std::string& folder_path);

    // add other objects
    void add_album(const Album& album);
    void add_artist(const Artist& artist);
    void add_person(const std::string& person);

    // remove objects
    void remove_track(int track_id);
    void remove_album(int album_id);
    void remove_artist(int artist_id);
    void remove_person(int person_id);

    // set track data (in both DB & metadata if applicable)
    void set_track_title(int track_id, const std::string& new_track_title);
    void set_track_artist(int track_id, const std::string& new_artist);
    void set_track_album(int track_id, const std::string& new_album);

    // set track data (in just metadata)
    void set_track_tracklist_num(int track_id, int new_tracklist_num);

    // set track data (in just DB)
    void set_track_date(int track_id, const Date& new_date);

    // get track data
    Track get_track_data(int track_id);

private:
    DatabaseManager database;
    FileManager file_manager;
};