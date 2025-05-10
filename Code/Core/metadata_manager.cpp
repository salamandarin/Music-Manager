// Samuel Sutton - 2025
#include "metadata_manager.h"
#include "file_manager.h"
#include <taglib/tag.h>
// TODO: FIGURE OUT WHAT'S NEEDED
#include <filesystem>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/flacfile.h>
#include <taglib/aifffile.h>
#include <taglib/wavfile.h>
#include <taglib/mp4file.h>
#include <fstream>
#include <iostream>

MetadataManager::MetadataManager(const std::string& file_path)
    // save file path, open file with TagLib
    :file_path{file_path}, file_ref{this->file_path.c_str()} {
    
    // check for errors opening file
    if (file_ref.isNull()) {
        throw std::runtime_error("Could not find/open file: " + file_path);
    }
    if (!file_ref.tag()) {
        throw std::runtime_error("Could not access tags from file: " + file_path);
    }
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
    // metadata doesn't hold date
    track.file_path = file_path; // TODO: either keep same, or use file_ref.tag()->complexProperties->GENERALOBJECT->fileName

    // TODO: add more fields?

    return track;
}

// ---------- Simple Data Getters ----------
std::string MetadataManager::get_track_title() {
    std::string title_string = file_ref.tag()->title().to8Bit(true);
    return title_string;
}
std::string MetadataManager::get_artist() {
    std::string artist_string = file_ref.tag()->artist().to8Bit(true);
    return artist_string;
}
std::string MetadataManager::get_album() {
    std::string album_string = file_ref.tag()->album().to8Bit(true);
    return album_string;
}
int MetadataManager::get_tracklist_num() {
    // make sure not too big when converting from unsigned int -> int
    if (file_ref.tag()->track() > INT_MAX) {
        throw std::out_of_range("Tried to get tracklist num that is too big for a (signed) int");
    }
    return static_cast<int>(file_ref.tag()->track());
}
Duration MetadataManager::get_duration() {
    // return default duration if no audio properties
    if (!file_ref.audioProperties()) {
        return Duration{};
    }
    
    // return duration
    Duration duration = file_ref.audioProperties()->lengthInSeconds();
    return duration;
}

// ---------- Get Cover Art ----------
std::string MetadataManager::save_cover_art() {
    // MP3 files (w/ ID3 tag))
    if (TagLib::MPEG::File* mp3_file = dynamic_cast<TagLib::MPEG::File*>(file_ref.file())) {
        if (mp3_file->hasID3v2Tag()) {
            return save_id3_cover_art(mp3_file->ID3v2Tag());
        }
    } 
    // WAV files (w/ ID3 tag)
    else if (TagLib::RIFF::WAV::File* wav_file = dynamic_cast<TagLib::RIFF::WAV::File*>(file_ref.file())) {
        if (wav_file->hasID3v2Tag()) {
            return save_id3_cover_art(wav_file->ID3v2Tag());
        }
    }
    // AIFF files (w/ ID3 tag)
    else if (TagLib::RIFF::AIFF::File* aiff_file = dynamic_cast<TagLib::RIFF::AIFF::File*>(file_ref.file())) {
        if (aiff_file->hasID3v2Tag()) {
            return save_id3_cover_art(aiff_file->tag()); // tag() for AIFF returns ID3v2 tag
        }
    }

    // FLAC files
    else if (TagLib::FLAC::File* flac_file = dynamic_cast<TagLib::FLAC::File*>(file_ref.file())) {
        const TagLib::List<TagLib::FLAC::Picture*>& picture_list = flac_file->pictureList();
        if (!picture_list.isEmpty()) {
            TagLib::FLAC::Picture* picture = picture_list[0];

            // get image file type & extension
            std::string mime_type = picture->mimeType().to8Bit(true);
            std::string extension = get_image_extension(mime_type);

            // save image to file, return path
            return write_image_to_file(picture->data(), extension);
        }
    }

    // Files in MP4 containers (ALAC & AAC)
    else if (TagLib::MP4::File* mp4_file = dynamic_cast<TagLib::MP4::File*>(file_ref.file())) {
        if (TagLib::MP4::Tag* mp4_tag = mp4_file->tag()) {
            if (mp4_tag->itemMap().contains("covr")) {
                TagLib::MP4::CoverArtList cover_art_list= mp4_tag->itemMap()["covr"].toCoverArtList();
                if (!cover_art_list.isEmpty()) {

                    // get image file type & extension
                    std::string extension; // mp4 format is incompatible with mime -> extension function :(
                    TagLib::MP4::CoverArt::Format format = cover_art_list[0].format();
                    if (format == TagLib::MP4::CoverArt::JPEG) {
                        extension = ".jpg";
                    }
                    else if (format == TagLib::MP4::CoverArt::PNG) {
                        extension = ".png";
                    }
                    else if (format == TagLib::MP4::CoverArt::GIF) {
                        extension = ".gif"; // rare
                    }
                    else if (format == TagLib::MP4::CoverArt::BMP) {
                        extension = ".bmp"; // ultra rare
                    }
                    else { // default to jpg if unknown
                        extension = ".jpg";
                    }
                    
                    // save image to file, return path
                    return write_image_to_file(cover_art_list[0].data(), extension);
                }
            }
        }
    }

    // return empty path if not found // TODO: make std::optional<> ?
    return "";
}


//--------------------------------------------------------------------------------
//                                  SET DATA
//--------------------------------------------------------------------------------
void MetadataManager::set_track_title(TagLib::String new_track_title) {
    file_ref.tag()->setTitle(new_track_title);
    file_ref.save();
}
void MetadataManager::set_artist(TagLib::String new_artist) {
    file_ref.tag()->setArtist(new_artist);
    file_ref.save();
}
void MetadataManager::set_album(TagLib::String new_album) {
    file_ref.tag()->setAlbum(new_album);
    file_ref.save();
}
void MetadataManager::set_tracklist_num(unsigned int new_tracklist_num) {
    file_ref.tag()->setTrack(new_tracklist_num);
    file_ref.save();
}

// ---------- Set Cover Art ----------
void MetadataManager::set_cover_art(const std::string& image_path) {
    // TODO: CODE - SET COVER ART

    // TODO: save new cover art to file 

    // TODO: figure out how to handle existing cover art (+ if can handle multiple? & if has none)

    // TODO: return new image path? or no cuz probably won't use (saving og art instead)
}

// ---------- Remove Cover Art ----------
void MetadataManager::remove_cover_art() {
    // TODO: CODE - REMOVE COVER ART(s)
    // TODO: do nothing if doesn't have cover art + get ALL cover art(s)
}

//--------------------------------------------------------------------------------
//                         PRIVATE HELPER FUNCTIONS
//--------------------------------------------------------------------------------

std::string MetadataManager::save_id3_cover_art(TagLib::ID3v2::Tag* id3v2_tag) {
    TagLib::ID3v2::FrameList frames = id3v2_tag->frameList("APIC");
    if (!frames.isEmpty()) {
        // get AttachedPictureFrame (APIC)
        TagLib::ID3v2::AttachedPictureFrame* picture_frame =
            dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
        if (!picture_frame) { // TODO: do something WITHOUT cout for this error
            std::cout << "Invalid APIC frame with no Attached Picture Frame ";
            std::cout << "found in this file: " << file_path << "\n";
        }

        // get image file type & extension
        std::string mime_type = picture_frame->mimeType().to8Bit(true);
        std::string extension = get_image_extension(mime_type);

        // save image to file, return path
        return write_image_to_file(picture_frame->picture(), extension);
    }
    else {
        // return empty path if not found // TODO: make std::optional<> ?
        return "";
    }
}

// determine image extension from mime type
std::string MetadataManager::get_image_extension(const std::string& mime_type) {
    if (mime_type.find("jpeg") != std::string::npos || mime_type.find("jpg") != std::string::npos) {
        return ".jpg";
    }
    else if (mime_type.find("png") != std::string::npos) {
        return ".png";
    }
    else if (mime_type.find("gif") != std::string::npos) {
        return ".gif"; // rare
    }
    else if (mime_type.find("bmp") != std::string::npos) {
        return ".bmp"; // ultra rare
    }
    
    // default to jpg if unknown
    return ".jpg";
}

std::string MetadataManager::write_image_to_file(const TagLib::ByteVector& image_data, const std::string& extension) {
    // construct image file path (naming it after music file)
    std::string music_file_name = FileManager::get_file_name(file_path).string();
    std::string image_path = FileManager::make_image_file_path(music_file_name, extension);

    // handle duplicate file names
    image_path = FileManager::number_duplicate_files(image_path);

    // write to file
    std::ofstream output(image_path, std::ios::binary);
    if (!output) { // TODO: maybe handle in better instead of throwing error??
        throw std::runtime_error("Error trying to save cover art for this music file: " + file_path);
    }
    output.write(image_data.data(), image_data.size());
    output.close();

    // return image path
    return image_path;
}