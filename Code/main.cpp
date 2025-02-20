// Samuel Sutton - 2025
#include "core.h"
#include "print_structs.h" // TODO: DELETE
#include "metadata_manager.h" // TODO: DELETE
#include <vector> // TODO: DELETE (only using for printing files)
#include <taglib/fileref.h>
#include <taglib/tag.h>

void choose_test_demo(Core& core, std::vector<std::string>& file_names);
Track choose_track(const std::vector<std::string>& file_names);

void add_file_demo(Core& core);
void read_metadata_demo(Core& core, const std::vector<std::string>& file_names);
void edit_metadata_demo(Core& core, std::vector<std::string>& file_names);

void print_music_files(const std::vector<std::string>& file_names);

int main() {
    try {
        std::string dust_bowl = "../../Music_Files/dust bowl (demo).mp3";
        std::string lead_poisoning = "../../Music_Files/Lead Poisoning.mp3";
        std::string nettles = "../../Music_Files/nettles (demo).mp3";
        std::string pure_feeling = "../../Music_Files/Pure Feeling.mp3";
        std::vector<std::string> file_names = {dust_bowl, lead_poisoning, nettles, pure_feeling};

        Core core;
        choose_test_demo(core, file_names);
    }
    catch (std::runtime_error& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
 }

void choose_test_demo(Core& core, std::vector<std::string>& file_names) {
    std::cout << "Enter a number to choose a demo to test: (will run 'view files' default if invalid input)\n";
    std::cout << "\t[0] Add file to library [DEMO]\n";
    std::cout << "\t[1] Read file metadata [DEMO]\n";
    std::cout << "\t[2] Edit file metadata [DEMO]\n";
    std::cout << "\t[3] View files [DEMO]\n";

    std::cout << ">> ";
    int demo_choice;
    std::cin >> demo_choice;

    if (demo_choice == 0) {
        add_file_demo(core);
    }
    else if (demo_choice == 1) {
        read_metadata_demo(core, file_names);
    }
    else if (demo_choice == 2) {
        edit_metadata_demo(core, file_names);
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

void add_file_demo(Core& core) {
    // test adding file
    std::cout << "Running 'test adding file' simulation......\n\n";
    std::cout << "You are adding 'dust bowl (demo)' to your music library :)\n\n";
    core.add_file("../../Music_Files/dust bowl (demo).mp3");
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

void print_music_files(const std::vector<std::string>& file_names) {
    std::cout << " #\t\tTitle\t\t\t\tArtist\t\t\t\t\tAlbum\t\t\t\t\tDuration\n";
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < file_names.size(); ++i) {
        MetadataManager metadata_manager{file_names[i]};
        Track track = metadata_manager.get_data();

        std::cout << " " << i << "\t" << track.title << "\t\t\t" << track.artist << "\t\t\t" << track.album << "\t\t\t" << track.duration << "\n";
        std::cout << "----------------------------------------------------------------------------------------------------\n";
    }
}