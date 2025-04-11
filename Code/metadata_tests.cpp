// Samuel Sutton - 2025
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include "metadata_manager.h"

const std::string TEST_FILE_PATH = "Music_Files/dust bowl (demo).mp3";

TEST_CASE("metadata constructor testing") {
    CHECK_NOTHROW(MetadataManager metadata_manager{TEST_FILE_PATH});
}

TEST_CASE("metadata getters testing") {
    MetadataManager metadata_manager{TEST_FILE_PATH};

    SUBCASE("metadata get title testing") {
        std::string title = metadata_manager.get_track_title();
        CHECK(title == "dust bowl (demo)");
    }

    SUBCASE("metadata get artist testing") {
        std::string artist = metadata_manager.get_artist();
        CHECK(artist == "Ethel Cain");
    }

    SUBCASE("metadata get album testing") {
        std::string album = metadata_manager.get_album();
        CHECK(album == "Test Album");
    }

    SUBCASE("metadata get track number testing") {
        int track_num = metadata_manager.get_tracklist_num();
        CHECK(track_num == 0);
    }
}

TEST_CASE("metadata setters testing") {
    MetadataManager metadata_manager{TEST_FILE_PATH};

    SUBCASE("metadata set title testing") {
        std::string old_title = metadata_manager.get_track_title(); // grab old title

        metadata_manager.set_track_title("test title");
        std::string changed_title = metadata_manager.get_track_title();
        CHECK(changed_title == "test title");

        metadata_manager.set_track_title(old_title); // change title back
    }   

    SUBCASE("metadata set artist testing") {
        std::string old_artist = metadata_manager.get_artist(); // grab old artist

        metadata_manager.set_artist("test artist");
        std::string changed_title = metadata_manager.get_artist();
        CHECK(changed_title == "test artist");

        metadata_manager.set_artist(old_artist); // change artist back
    }

    SUBCASE("metadata set album testing") {
        std::string old_album = metadata_manager.get_album(); // grab old album

        metadata_manager.set_album("test album");
        std::string changed_title = metadata_manager.get_album();
        CHECK(changed_title == "test album");

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




