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
    Core(); // constructor

    // settings
    bool get_is_nested();
    void set_is_nested(bool new_value);
    
    bool get_copy_music_files();
    void set_copy_music_files(bool new_value);

    // ------------------------- ADD OBJECTS -------------------------
    // add tracks
    void add_track(std::string file_path); // track w/ file
    void add_track(Track track); // track w/ or w/o file
    void add_tracks_from_folder(const std::string& folder_path);

    // add other objects
    void add_album(const Album& album);
    void add_artist(const Artist& artist);
    void add_person(const std::string& person);

    // ------------------------- REMOVE DATA -------------------------
    // remove objects
    void remove_track(int track_id);
    void remove_album(int album_id);
    void remove_artist(int artist_id);
    void remove_person(int person_id);

    // remove files / images
    void remove_track_file(int track_id);
    void remove_track_image(int track_id);
    void remove_album_image(int album_id);
    void remove_artist_image(int artist_id);

    // ------------------------- SET DATA -------------------------
    // set track data (in both database & metadata if applicable)
    void set_track_title(int track_id, const std::string& track_title);

    void set_track_artist(int track_id, const std::string& artist_name);
    void set_track_artist_id(int track_id, int artist_id);

    void set_track_album(int track_id, const std::string& album_title);
    void set_track_album_id(int track_id, int album_id);

        // skip duration - do not manually set
    void set_track_date(int track_id, const Date& track_date);
    void set_track_tracklist_num(int track_id, int tracklist_num);
    void set_track_file(int track_id, std::string file_path);
    void set_track_image(int track_id, std::string image_path);


    // set album data
    void set_album_title(int album_id, const std::string& album_title);

    void set_album_artist(int album_id, const std::string& artist_name);
    void set_album_artist_id(int album_id, int artist_id);

    void set_album_date(int album_id, const Date& album_date);

    bool set_album_type(int album_id, const std::string& type_name); // ALBUM'S type (returns if set or not)
    void set_album_type_id(int track_id, int type_id); // ALBUM'S type

    void set_album_image(int album_id, std::string image_path);


    // set artist data
    void set_artist_name(int artist_id, const std::string& artist_name);
    
    void set_artist_person_behind(int artist_id, const std::string& person_behind);
    void set_artist_person_behind_id(int artist_id, int person_id);

    void set_artist_image(int artist_id, std::string image_path);


    // set person data
    void set_person_name(int person_id, const std::string& person_name);


    // ------------------------- GET OBJECTS -------------------------
    // get ALL objects
    std::vector<Track> get_all_tracks();
    std::vector<Album> get_all_albums();
    std::vector<Artist> get_all_artists();
    std::vector<std::string> get_all_people();

    // get objects by category
    std::vector<Track> get_album_tracks(int album_id); // ordered by tracklist_num
    std::vector<Track> get_artist_tracks(int artist_id);
    std::vector<Album> get_artist_albums(int artist_id);
    std::vector<Artist> get_person_artists(int person_id);
    
    // get object
    Track get_track(int track_id);
    Album get_album(int track_id);
    Artist get_artist(int track_id);
    std::string get_person(int person_id); // just name
    std::string get_type(int album_type_id); // just name (album_type, NOT album's album_type)

    // ---------- WARNING: EXTREMELY DANGEROUS!!! ----------
    // DELETE all music files, images, entire database file, and another folder too if provided
    void delete_entire_library(); // WARNING: EXTREMELY DANGEROUS!!!

private:
    void update_file_structure();
    std::string set_file_title(const std::string& file_path); // make track title & file name match
    
    void set_track_file_artist(int track_id, const Track& track_data);
    void set_track_file_album(int track_id, const Track& track_data);

    DatabaseManager database;

    // settings
    bool is_nested;
    bool copy_music_files;
};