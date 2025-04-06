// Samuel Sutton - 2025
#include "core.h"
#include "print_structs.h" // TODO: DELETE
#include "metadata_manager.h" // TODO: DELETE
#include "database_manager.h" // TODO: DELETE
#include <vector> // TODO: DELETE (only using for printing files)
#include <taglib/fileref.h>
#include <taglib/tag.h>

void choose_test_demo(Core& core, std::vector<std::string>& file_names);
Track choose_track(const std::vector<std::string>& file_names);

void add_files_demo(Core& core,const std::vector<std::string>& file_names);
void read_metadata_demo(Core& core, const std::vector<std::string>& file_names);
void edit_metadata_demo(Core& core, std::vector<std::string>& file_names);
void add_track_nonfile_track_demo(Core& core);

void print_music_files(const std::vector<std::string>& file_names);

int main() {
    try {
        std::vector<std::string> file_names = {
            "../../Music_Files/dust bowl (demo).mp3",
            "../../Music_Files/nettles (demo).mp3",
            "../../Music_Files/Pure Feeling.mp3",
            "../../Music_Files/Child of Cain.mp3",
            "../../Music_Files/Will You Still Love Me Tomorrow (Remastered Cover).mp3",
            "../../Music_Files/Florida Heat.mp3",
            "../../Music_Files/Iron Maiden.mp3",
            "../../Music_Files/The Wake.mp3",
            "../../Music_Files/As Far as I Could Get.mp3",
            "../../Music_Files/Can't Stop Loving You (Cover).mp3",
            "../../Music_Files/Conductor.mp3",
            "../../Music_Files/south alabama (god's country demo).mp3",
            "../../Music_Files/Lead Poisoning.mp3",
        };

        Core core;

//        int track_id = 30;
//        DatabaseManager database;
//        std::optional<std::string> possible_file_path = database.get_track_file_path(track_id);
//        std::cout << "FILE PATH MAIN: " << *possible_file_path << "\n";

//        core.remove_track(track_id);

//        Track empty_track;
//        empty_track.album = "";
//        empty_track.artist = "";
//        empty_track.title = "";
//        empty_track.file_path = "";
//        empty_track.tracklist_num = 0;
//
        // DatabaseManager db;
//        db.add_track(empty_track);

//        std::optional<Artist> returned_data = db.get_artist(1);
//        std::cout << *returned_data;
//
//        std::optional<Track> returned_track = db.get_track(35);
//        std::cout << *returned_track;
//
//        std::vector<Track> tracks = db.get_all_tracks();
//        for (Track track : tracks) {
//            std::cout << track;
//        }
//
//        std::vector<Album> albums = db.get_all_albums();
//        for (Album album : albums) {
//            std::cout << album;
//        }
//
//        std::vector<Artist> artists = db.get_all_artists();
//        for (Artist artist : artists) {
//            std::cout << artist;
//        }
//
//        std::vector<std::string> people = db.get_all_people();
//        for (std::string person : people) {
//            std::cout << "\nPerson Name: " << person << "\n";
//        }

            core.add_tracks_from_folder("../../other_file_types copy");
            core.toggled_nested();
//            core.delete_entire_library("../../other_file_types copy");

//        MetadataManager data{"../../Music_Files/Conductor.mp3"};
//        Duration duration = data.get_duration();
//        std::cout << "Duration: " << duration << "\n";
//        std::cout << "Minutes: " << duration.get_minutes() << "\n";
//        std::cout << "Seconds: " << duration.get_seconds() << "\n";
//
//        std::string cover_art = data.save_cover_art();
//        std::cout << "Cover art: " << cover_art << "\n";

        // choose_test_demo(core, file_names);
    }
    catch (std::runtime_error& error) {
        std::cout << "\nError: " << error.what() << "\n";
    }
 }

void choose_test_demo(Core& core, std::vector<std::string>& file_names) {
    std::cout << "Enter a number to choose a demo to test: (will run 'view files' default if invalid input)\n";
    std::cout << "\t[0] Add all files to database [DEMO]\n";
    std::cout << "\t[1] Read file metadata [DEMO]\n";
    std::cout << "\t[2] Edit file metadata [DEMO]\n";
    std::cout << "\t[3] Add new track (w/o file) [DEMO]\n";
    std::cout << "\t[4] View files [DEMO]\n";

    std::cout << ">> ";
    int demo_choice;
    std::cin >> demo_choice;

    if (demo_choice == 0) {
        add_files_demo(core, file_names);
    }
    else if (demo_choice == 1) {
        read_metadata_demo(core, file_names);
    }
    else if (demo_choice == 2) {
        edit_metadata_demo(core, file_names);
    }
    else if (demo_choice == 3) {
        add_track_nonfile_track_demo(core);
    }
    else {
        print_music_files(file_names);
    }
}
Track choose_track(const std::vector<std::string>& file_names) {
    print_music_files(file_names);
    std::cout << "Enter a track number: (will choose track 0 if invalid number is entered)\n";
    std::cout << ">> ";
    int track_num;
    std::cin >> track_num;

    if (track_num > 0 && track_num < file_names.size()) {
        MetadataManager metadata_manager{file_names[track_num]};
        Track track_data = metadata_manager.get_data();
        return track_data;
    }
    else {
        MetadataManager metadata_manager{file_names[0]};
        Track track_data = metadata_manager.get_data();
        return track_data;
    }
}

// add all files to database
void add_files_demo(Core& core,const std::vector<std::string>& file_names) {
    std::cout << "Running 'add files to database'......\n\n";
    for (const std::string& file : file_names) {
        core.add_track(file);
    }
    std::cout << "\n\n\t\t\t\t\tAdded all files to database! Go check it!\n\n";
}

// test reading track data
void read_metadata_demo(Core& core, const std::vector<std::string>& file_names) {
    std::cout << "\n\t\t\tRunning 'test reading metadata' simulation......\n\n";
    Track track = choose_track(file_names);
    std::cout << track;
}

void edit_metadata_demo(Core& core, std::vector<std::string>& file_names) {
    std::cout << "\n\t\t\tRunning 'test editing metadata' simulation......\n\n";
    Track track = choose_track(file_names);
    MetadataManager metadata_manager{track.file_path};

    std::cout << "Enter a number to choose a field to edit: (will select last option by default if invalid input)\n";
    std::cout << "\t[0] Title\n";
    std::cout << "\t[1] Artist\n";
    std::cout << "\t[2] Album\n";
    std::cout << "\t[3] Tracklist Number\n";
    std::cout << ">> ";
    int field_choice;
    std::cin >> field_choice;

    // clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nEnter the new data:\n>> ";
    std::string new_data;
    std::getline(std::cin, new_data);

    if (field_choice == 0) {
        metadata_manager.set_track_title(new_data);
    }
    else if (field_choice == 1) {
        metadata_manager.set_artist(new_data);
    }
    else if (field_choice == 2) {
        metadata_manager.set_album(new_data);
    }
    else {
        int new_num = std::stoi(new_data);
        metadata_manager.set_tracklist_num(new_num);
    }

    std::cout << metadata_manager.get_data();
}

void add_track_nonfile_track_demo(Core& core) {
    // clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nEnter the new track title: ";
    std::string title;
    std::getline(std::cin, title);

    std::cout << "Enter track album:\n>> ";
    std::string album;
    std::getline(std::cin, album);

    std::cout << "Enter track artist:\n>> ";
    std::string artist;
    std::getline(std::cin, artist);

    Track track;
    track.title = title;
    track.album = album;
    track.artist = artist;

    core.add_track(track);

    std::cout << "\nAdded '" << title << "' to database!\n";
}

void print_music_files(const std::vector<std::string>& file_names) {
    std::cout << " #\t\tTitle\t\t\t\tArtist\t\t\t\t\tAlbum\t\t\t\t\tTrack Number\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < file_names.size(); ++i) {
        MetadataManager metadata_manager{file_names[i]};
        Track track = metadata_manager.get_data();

        std::cout << " " << i << "\t" << track.title << "\t\t\t" << track.artist << "\t\t\t" << track.album << "\t\t\t" << track.tracklist_num << "\n";
        std::cout << "----------------------------------------------------------------------------------------------------\n";
    }
}