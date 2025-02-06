// Samuel Sutton - 2025
#include "metadata_manager.h"
#include <taglib/tag.h>

MetadataManager::MetadataManager(const std::string& file_path)
    // save info, open file
    :file_name{file_path.c_str()}, file(this->file_name), tag{file.tag()} { // save file path, open file with TagLib
    // check for errors opening file
    if (file.isNull()) {
        throw std::runtime_error("Could not find/open file: " + file_path);
    }
    if (!file.tag()) {
        throw std::runtime_error("Could not access tags from file: " + file_path);
    }

    //TODO: CHECK FOR OTHER FILE / MIME TYPES!!!
}

//--------------------------------------------------------------------------------
//                                  GET DATA
//--------------------------------------------------------------------------------
Track& MetadataManager::get_data() {
    Track track_data;
    // TODO: CODE

    track_data.file_path = file_name;

    return track_data;
}



TagLib::String MetadataManager::get_track_title() {
    return tag->title();
}
TagLib::String MetadataManager::get_artist() {
    return tag->artist();
}
TagLib::String MetadataManager::get_album() {
    return tag->album();
}
unsigned int MetadataManager::get_track_num() {
    return tag->track();
}

//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------
void MetadataManager::set_track_title(TagLib::String new_track_title) {
    tag->setTitle(new_track_title);
    file.save();
}
void MetadataManager::set_artist(TagLib::String new_artist) {
    tag->setArtist(new_artist);
    file.save();
}
void MetadataManager::set_album(TagLib::String new_album) {
    tag->setAlbum(new_album);
    file.save();
}
void MetadataManager::set_track_num(unsigned int new_track_num) {
    tag->setTrack(new_track_num);
    file.save();
}











