// Samuel Sutton - 2025
#include "core.h"
#include "print_structs.h" // TODO: DELETE
#include <taglib/fileref.h>
#include <taglib/tag.h>

int main() {
    try {
        Core core;

        // test reading track data
        std::cout << "Choose a track to read the metadata from:\n";
        // display_music_library();
        int track_choice;
        std::cin >> track_choice;
        //TODO: finish this.....



        Track test_track{"test track", "", "test album"};
        std::cout << test_track;


        // test adding file
        std::cout << "Running 'test adding file' simulation......\n";
        std::cout << "You are adding 'dust bowl (demo)' to your music library :)\n\n";
        core.add_file("../../Music_Files/dust bowl (demo).mp3");

    }
    catch (std::runtime_error& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
 }