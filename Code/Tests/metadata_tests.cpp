// Samuel Sutton - 2025
#include "metadata_manager.h"
#include <doctest.h>

const std::string TEST_FILE_PATH = "Music_Files/TEST TRACK.mp3";

// constructor
TEST_CASE("metadata constructor testing") {
    CHECK_NOTHROW(MetadataManager metadata_manager{TEST_FILE_PATH});
}

// getters
TEST_CASE("metadata getters testing") {
    MetadataManager metadata_manager{TEST_FILE_PATH};

    metadata_manager.set_track_title("TEST TRACK");
    metadata_manager.set_artist("TEST ARTIST");
    metadata_manager.set_album("TEST ALBUM");
    metadata_manager.set_tracklist_num(7);

    SUBCASE("metadata get title testing") {
        std::string title = metadata_manager.get_track_title();
        CHECK(title == "TEST TRACK");
    }

    SUBCASE("metadata get artist testing") {
        std::string artist = metadata_manager.get_artist();
        CHECK(artist == "TEST ARTIST");
    }

    SUBCASE("metadata get album testing") {
        std::string album = metadata_manager.get_album();
        CHECK(album == "TEST ALBUM");
    }

    SUBCASE("metadata get track number testing") {
        int track_num = metadata_manager.get_tracklist_num();
        CHECK(track_num == 7);
    }
}

// setters
TEST_CASE("metadata setters testing") {
    MetadataManager metadata_manager{TEST_FILE_PATH};

    SUBCASE("metadata set title testing") {
        std::string old_title = metadata_manager.get_track_title(); // grab old title

        metadata_manager.set_track_title("new title");
        std::string changed_title = metadata_manager.get_track_title();
        CHECK(changed_title == "new title");

        metadata_manager.set_track_title(old_title); // change title back
    }

    SUBCASE("metadata set artist testing") {
        std::string old_artist = metadata_manager.get_artist(); // grab old artist

        metadata_manager.set_artist("new artist");
        std::string changed_title = metadata_manager.get_artist();
        CHECK(changed_title == "new artist");

        metadata_manager.set_artist(old_artist); // change artist back
    }

    SUBCASE("metadata set album testing") {
        std::string old_album = metadata_manager.get_album(); // grab old album

        metadata_manager.set_album("new album");
        std::string changed_title = metadata_manager.get_album();
        CHECK(changed_title == "new album");

        metadata_manager.set_album(old_album); // change album back
    }

    SUBCASE("metadata set tracklist number testing") {
        int old_tracklist_num = metadata_manager.get_tracklist_num(); // grab old tracklist_num

        metadata_manager.set_tracklist_num(77);
        int changed_title = metadata_manager.get_tracklist_num();
        CHECK(changed_title == 77);

        metadata_manager.set_tracklist_num(old_tracklist_num); // change tracklist_num back
    }
}
