// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "date.h"
#include <string>

class Core {
public:
    Core() = default;

    // add & remove tracks
    void add_track(const std::string& file_path); // track w/ file
    void add_track(const Track& track); // track w/o file
    void remove_track(const int& track_id);

    // set track data (in both DB & metadata if applicable)
    void set_track_title(const int& track_id, const std::string& new_track_title);
    void set_track_artist(const int& track_id, const std::string& new_artist);
    void set_track_album(const int& track_id, const std::string& new_album);

    // set track data (in just metadata)
    void set_track_tracklist_num(const int& track_id, const int& new_tracklist_num);

    // set track data (in just DB)
    void set_track_date(const int& track_id, const Date& new_date);

    // get track data
    Track get_track_data(const int& track_id);
};