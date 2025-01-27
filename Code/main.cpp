// Samuel Sutton - 2025
#include "tag.h"
#include <iostream>

int main() {
    try {
        TagLib::FileRef f("dust_bowl");
        TagLib::String artist = f.tag()->artist();

        std::cout << artist << "\n";
    }
    catch (std::runtime_error& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}