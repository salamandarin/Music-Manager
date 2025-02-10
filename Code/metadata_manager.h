// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include <string>
#include <taglib/fileref.h>

class MetadataManager {
public:
    MetadataManager(const std::string& file_path); // constructor

    // ---------- GET DATA ----------
    Track& get_data();

    // simple getters
    TagLib::String get_track_title();
    TagLib::String get_artist();
    TagLib::String get_album();
    unsigned int get_tracklist_num();

    // complex getters
    Duration get_duration();
    std::string get_cover_art(); // include alts if id3v2 // TODO: RETURN TYPE?????

    // super complex getters
//    TagLib::String get_file_type();
//    TagLib::String get_file_size();
//    TagLib::String get_file_path();
//    Date get_created_date();
//    Date get_modified_date();
//    TagLib::String get_source_link();
//    TagLib::String get_sample_rate();
//    TagLib::String get_bit_rate();
//    TagLib::String get_channels();
//    TagLib::String get_encoded_with();
//    TagLib::String get_format();


    // ---------- SET DATA ----------
    void set_track_title(TagLib::String new_track_title);
    void set_artist(TagLib::String new_artist);
    void set_album(TagLib::String new_album);
    void set_tracklist_num(unsigned int new_tracklist_num);


private:
    const TagLib::FileName file_name;
    TagLib::FileRef file;
    TagLib::Tag *tag;
};