// Samuel Sutton - 2025
#include "metadata_manager.h"
#include "file_manager.h"
#include <taglib/tag.h>
#include <filesystem>

MetadataManager::MetadataManager(const std::string& file_path)
    // save file path, open file with TagLib
    :file_path{file_path}, file(this->file_path.c_str()), tag{file.tag()} {
    // check for errors opening file
    if (file.isNull()) {
        throw std::runtime_error("Could not find/open file: " + file_path);
    }
    if (!file.tag()) {
        throw std::runtime_error("Could not access tags from file: " + file_path);
    }

    // make track title = file name if no track title
    if (get_track_title().empty()) {
        set_track_title(FileManager::get_file_name(file_path));
    }

    //TODO: CHECK FOR OTHER FILE / MIME TYPES !!!

    // TODO: save cover art to a file (it if has any)
}

//--------------------------------------------------------------------------------
//                                  GET DATA
//--------------------------------------------------------------------------------
Track MetadataManager::get_data() {
    Track track;

    track.title = get_track_title();
    track.artist = get_artist();
    track.album = get_album();
    track.tracklist_num = get_tracklist_num();
    track.duration = get_duration();
    // file doesn't hold date - must be manually added to DB
    track.file_path = file_path; // TODO: either keep same, or use tag->complexProperties->GENERALOBJECT->fileName

    //TODO: add more fields?

    return track;
}



// ---------- Simple Data Getters ----------
std::string MetadataManager::get_track_title() {
    std::string title_string = tag->title().to8Bit();
    return title_string;
}
std::string MetadataManager::get_artist() {
    std::string artist_string = tag->artist().to8Bit();
    return artist_string;
}
std::string MetadataManager::get_album() {
    std::string album_string = tag->album().to8Bit();
    return album_string;
}
int MetadataManager::get_tracklist_num() {
    if (tag->track() > INT_MAX) {
        throw std::out_of_range("Tried to get unsigned int value of tracklist number that exceeds INT_MAX");
    }
    return static_cast<int>(tag->track());
}


// ---------- Complex Data Getters ----------
// TODO: Possibly do data type conversions before returning??? (depending on data types using)
Duration MetadataManager::get_duration() {
    // return default duration if no audio properties
    if (!file.audioProperties()) {
        return Duration{};
    }
    
    // return duration
    Duration duration = file.audioProperties()->length();
    return duration;
}
std::string MetadataManager::get_cover_art() { // include alts if id3v2 // TODO: RETURN TYPE?????
    // TODO: CODE
    return ".......cover art path placeholder..........";
} 


// ---------- File Info ----------



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











