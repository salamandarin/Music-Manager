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

    //TODO: CHECK FOR OTHER FILE / MIME TYPES !!!
}

//--------------------------------------------------------------------------------
//                                  GET DATA
//--------------------------------------------------------------------------------
Track& MetadataManager::get_data() {
    Track track;

    track.title = get_track_title(); // TODO: check on going from TagLib::String -> string
    track.artist = get_artist(); // TODO: check on going from TagLib::String -> string
    track.album = get_album(); // TODO: check on going from TagLib::String -> string
    track.tracklist_num = get_tracklist_num(); // TODO: check on going from unsigned int -> int
    track.duration = get_duration();
    // file doesn't hold date - must be manually added to DB
    track_data.file_path = file_name; // TODO: either keep same, or use tag->complexProperties->GENERALOBJECT->fileName

    //TODO: add more fields?

    return track_data;
}



// ---------- Simple Data Getters ----------
// TODO: Possibly do data type conversions before returning??? (depending on data types using)
TagLib::String MetadataManager::get_track_title() {
    return tag->title();
}
TagLib::String MetadataManager::get_artist() {
    return tag->artist();
}
TagLib::String MetadataManager::get_album() {
    return tag->album();
}
unsigned int MetadataManager::get_tracklist_num() {
    return tag->track();
}


// ---------- Complex Data Getters ----------
// TODO: Possibly do data type conversions before returning??? (depending on data types using)
Duration MetadataManager::get_duration() {
    // TODO: CODE
    Duration duration;
    return duration;
}
std::string MetadataManager::get_cover_art() { // include alts if id3v2 // TODO: RETURN TYPE?????
    // TODO: CODE
    return ".......cover art path placeholder..........";
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
void MetadataManager::set_tracklist_num(unsigned int new_tracklist_num) {
    tag->setTrack(new_tracklist_num);
    file.save();
}











