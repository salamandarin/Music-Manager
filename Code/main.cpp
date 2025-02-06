// Samuel Sutton - 2025
#include "duration.h" // TODO: DELETE
#include "date.h" // TODO: DELETE
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>

 int main() {
     try {
        Duration test_duration{11, 30};
        std::cout << "Duration test: " << test_duration << "\n";

         Date test_date{11, 29,2002};
         std::cout << "Date test: " << test_date << "\n\n\n";


         // open .mp3 file, check if null
         TagLib::FileRef file("../../Music_Files/dust bowl (demo).mp3");
         if (file.isNull()) {
             throw std::runtime_error("Could not find file");
         }

         // print title
         TagLib::String title = file.tag()->title();
         std::cout << "Title: " << title << "\n";

         // change album
         file.tag()->setAlbum("Test Album");
         file.save();

         // print artist & album
         TagLib::Tag *tag = file.tag();
         std::cout << "Artist: " << tag->artist() << "\n";
         std::cout << "Album: " << tag->album() << "\n";
     }
     catch (std::runtime_error& error) {
         std::cout << "Error: " << error.what() << "\n";
     }
 }