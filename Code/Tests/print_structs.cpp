// Samuel Sutton - 2025
#include "print_structs.h"

std::ostream& operator<<(std::ostream& os, const Track& track) {
    os << "\nTRACK DATA\n";
    os << "--------------------\n";
    os << "ID: " << track.id << "\n";
    os << "Title: " << track.title << "\n";
    os << "Artist: " << track.artist << "\n";
    os << "Album: " << track.album << "\n";
    os << "# in Album Tracklist: " << track.tracklist_num << "\n";
    os << "Duration: " << track.duration.to_string() << "\n";
    os << "Date: " << track.date.to_string() << "\n";
    os << "File Path: " << track.file_path << "\n";

    os << "\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Album& album) {
    os << "\nALBUM DATA\n";
    os << "--------------------\n";
    os << "ID: " << album.id << "\n";
    os << "Title: " << album.title << "\n";
    os << "Artist: " << album.artist << "\n";
    os << "Date: " << album.date.to_string() << "\n";
    os << "Image Path: " << album.image_path << "\n";
    os << "Type: " << album.type << "\n";

    os << "\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Artist& artist) {
    os << "\nARTIST DATA\n";
    os << "--------------------\n";
    os << "ID: " << artist.id << "\n";
    os << "Name: " << artist.name << "\n";
    os << "Person Behind: " << artist.person_behind << "\n";
    os << "Image Path: " << artist.image_path << "\n";

    os << "\n";
    return os;
}