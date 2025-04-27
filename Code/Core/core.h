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
    Core(bool is_nested=true); // constructor

    // handle settings
    bool get_is_nested();
    void toggle_nested();

    // ------------------------- ADD OBJECTS -------------------------
    // add tracks
    void add_track(const std::string& file_path); // track w/ file
    void add_track(Track& track); // track w/ or w/o file
    void add_tracks_from_folder(const std::string& folder_path);

    // add other objects
    void add_album(const Album& album);
    void add_artist(const Artist& artist);
    void add_person(const std::string& person);

    // ------------------------- REMOVE OBJECTS -------------------------
    // remove objects
    void remove_track(int track_id);
    void remove_album(int album_id);
    void remove_artist(int artist_id);
    void remove_person(int person_id);

    // ------------------------- SET DATA -------------------------
    // set track data (in both DB & metadata if applicable)
    void set_track_title(int track_id, const std::string& new_track_title);
    void set_track_artist(int track_id, const std::string& new_artist_name);
    void set_track_album(int track_id, const std::string& new_album_title);
    void set_track_date(int track_id, const Date& new_date);
    void set_track_tracklist_num(int track_id, int new_tracklist_num);
    void set_track_image(int track_id, const std::string& new_image_path);

    // ------------------------- GET OBJECTS -------------------------
    // get ALL objects
    std::vector<Track> get_all_tracks();
    std::vector<Album> get_all_albums();
    std::vector<Artist> get_all_artists();

    // get objects by category
    std::vector<Track> get_album_tracks(int album_id);
    std::vector<Track> get_artist_tracks(int artist_id);
    std::vector<Album> get_artist_albums(int artist_id);
    std::vector<Artist> get_person_artists(int person_id);
    
    // get object
    Track get_track(int track_id);
    Album get_album(int track_id);
    Artist get_artist(int track_id);

    // ---------- WARNING: EXTREMELY DANGEROUS!!! ----------
    // DELETE all music files, images, entire database file, and another folder too if provided
    void delete_entire_library(const std::string& extra_folder_to_delete=""); // WARNING: EXTREMELY DANGEROUS!!!

private:
    void update_file_structure();
    std::string set_file_title(const std::string& file_path); // make track title & file name match

    DatabaseManager database;

    // settings
    bool is_nested;
};