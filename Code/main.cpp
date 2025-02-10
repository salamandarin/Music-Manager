// Samuel Sutton - 2025
#include "core.h"
#include "print_structs.h" // TODO: DELETE
#include <vector> // TODO: DELETE (only using for printing files)
#include <taglib/fileref.h>
#include <taglib/tag.h>

void choose_test_demo(Core& core);
void add_file_demo(Core& core);
void read_metadata_demo(Core& core);
void edit_metadata_demo(Core& core);

void print_music_files(const std::vector<Track>& files);
void display_music_library_demo();

int main() {
    try {
        Core core;
        choose_test_demo(core);
    }
    catch (std::runtime_error& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
 }

void choose_test_demo(Core& core) {
    std::cout << "Enter a number to choose a demo to test: (will run last demo by default if invalid input)\n";

    std::cout << "\t[0] Demo - Add file to library\n";
    std::cout << "\t[1] Demo - Read file metadata\n";
    std::cout << "\t[2] Demo - Edit file metadata\n";

    std::cout << ">> ";
    int demo_choice;
    std::cin >> demo_choice;

    if (demo_choice == 0) {
        add_file_demo(core);
    }
    if (demo_choice == 1) {
        read_metadata_demo(core);
    }
    else {
        edit_metadata_demo(core);
    }
}

void add_file_demo(Core& core) {
    // test adding file
    std::cout << "Running 'test adding file' simulation......\n\n";
    std::cout << "You are adding 'dust bowl (demo)' to your music library :)\n\n";
    core.add_file("../../Music_Files/dust bowl (demo).mp3");
}

// test reading track data
void read_metadata_demo(Core& core) {
    std::cout << "Running 'test reading metadata' simulation......\n\n";
    std::cout << "Choose a track to read the metadata from:\n";
    // TODO: display_music_library();

    std::cout << ">> ";
    int track_choice;
    std::cin >> track_choice;
    //TODO: finish this.....
}

void edit_metadata_demo(Core& core) {
    Track dust_bowl = {"../../Music_Files/dust bowl (demo.mp3", "dust bowl (demo)"};
    Track lead_poisoning = {"../../Music_Files/Lead Poisoning.mp3", "Lead Poisoning"};
    Track nettles = {"../../Music_Files/nettles.mp3", "nettles (demo)"};
    Track pure_feeling = {"../../Music_Files/Pure Feeling.mp3", "Pure Feeling"};
    // TODO: maybe have file_manager get all tracks function??, use get_data() to fill out this?
    std::vector<Track> files = {dust_bowl, lead_poisoning, nettles, pure_feeling};


    std::cout << "Running 'test reading metadata' simulation......\n\n";
    std::cout << "Enter the number of the track you want to read the metadata from: (will read first track by default if invalid number is entered)\n";
    display_music_library_demo();
    print_music_files(files);
    
    std::cout << ">> ";
    int track_choice;
    std::cin >> track_choice;

    if (track_choice > 0 && < files.size()) {
        MetadataManager metadata_manager{files[track_choice].file_path};
        Track track_data = metadata_manager.get_data();
        std::cout << track_data;
    }
    else {
        MetadataManager metadata_manager{files[0].file_path};
        Track track_data = metadata_manager.get_data();
        std::cout << track_data;  
    }
}

void print_music_files(const std::vector<Track>& files) {
    std::cout << "----------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < files.size(); ++i) {
        std::cout << i << "\t" << file.title << "\t" << file.artist << "\t" << file.album << "\t" << file.duration << "\n";
        std::cout << "----------------------------------------------------------------------------------------------------\n";
    }
}

void display_music_library_demo() {
    std::cout << "         [0]                         [1]                         [2]                         [3]\n"
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << "*********************       *********************       *********************       *********************\n";
    std::cout << " dust bowl (demo).mp3        Lead Poisoning.mp3          nettles (demo).mp3           Pure Feeling.mp3\n\n";
}