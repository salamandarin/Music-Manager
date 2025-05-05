// Samuel Sutton - 2025
#include "metadata_manager.h"
#include <doctest.h>

const std::string TEST_FILE_PATH = "Music_Files/TEST TRACK.mp3";

// constructor
TEST_CASE("metadata constructor") {
    CHECK_NOTHROW(MetadataManager metadata_manager{TEST_FILE_PATH});
}

// setters
TEST_CASE("metadata get & set") {
    MetadataManager metadata_manager{TEST_FILE_PATH};

    SUBCASE("get & set title") {
        std::string old_title = metadata_manager.get_track_title(); // grab old title

        metadata_manager.set_track_title("new title");
        std::string new_value = metadata_manager.get_track_title();
        CHECK(new_value == "new title");

        metadata_manager.set_track_title(old_title); // change title back
    }

    SUBCASE("get & set artist") {
        std::string old_artist = metadata_manager.get_artist(); // grab old artist

        metadata_manager.set_artist("new artist");
        std::string new_value = metadata_manager.get_artist();
        CHECK(new_value == "new artist");

        metadata_manager.set_artist(old_artist); // change artist back
    }

    SUBCASE("get & set album") {
        std::string old_album = metadata_manager.get_album(); // grab old album

        metadata_manager.set_album("new album");
        std::string new_value = metadata_manager.get_album();
        CHECK(new_value == "new album");

        metadata_manager.set_album(old_album); // change album back
    }

    SUBCASE("get & set tracklist number") {
        int old_tracklist_num = metadata_manager.get_tracklist_num(); // grab old tracklist_num

        metadata_manager.set_tracklist_num(7);
        int new_value = metadata_manager.get_tracklist_num();
        CHECK(new_value == 7);

        metadata_manager.set_tracklist_num(old_tracklist_num); // change tracklist_num back
    }
}
