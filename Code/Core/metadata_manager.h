// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include <string>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>

class MetadataManager {
public:
    // constructor
    MetadataManager(const std::string& file_name);

    // ---------- GET DATA ----------
    Track get_data();

    // simple getters
    std::string get_track_title();
    std::string get_artist();
    std::string get_album();
    int get_tracklist_num();
    Duration get_duration();

    // get cover art
    std::string save_cover_art(); // include alts if id3v2

    
    // ---------- SET DATA ----------
    void set_track_title(TagLib::String new_track_title);
    void set_artist(TagLib::String new_artist);
    void set_album(TagLib::String new_album);
    void set_tracklist_num(unsigned int new_tracklist_num);

    // set cover art
    void set_cover_art(const std::string& image_path);
    // remove cover art
    void remove_cover_art();

private:
    // helper functions (for cover art)
    std::string save_id3_cover_art(TagLib::ID3v2::Tag* id3v2_tag);
    std::string get_image_extension(const std::string& mime_type);
    std::string write_image_to_file(const TagLib::ByteVector& image_data, const std::string& mime_type);

    const std::string file_path;
    TagLib::FileRef file_ref;
};