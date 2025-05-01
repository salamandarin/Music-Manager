// Samuel Sutton - 2025
#include "database_manager.h"
#include "print_structs.h"
#include <doctest.h>
#include <iostream>

DatabaseManager database;

// constructor
TEST_CASE("database constructor") {
    CHECK_NOTHROW(DatabaseManager database);
}

// get objects by category
TEST_CASE("database get objects by category") {
    SUBCASE("get album tracks") {
        std::vector<Track> tracks = database.get_album_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "TRACKS IN ALBUM 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist tracks") {
        std::vector<Track> tracks = database.get_artist_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "\n\nTRACKS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist albums") {
        std::vector<Album> albums = database.get_artist_albums(1);
        CHECK(albums.size() != 0);

        std::cout << "\n\nALBUMS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Album& album : albums) {
            std::cout << album.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get person artists") {
        std::vector<Artist> artists = database.get_person_artists(1);
        CHECK(artists.size() != 0);

        std::cout << "\n\nARTIST NAMES UNDER PERSON 1\n";
        std::cout << "-----------------------------\n";
        for (const Artist& artist : artists) {
            std::cout << artist.name << "\n";
        }
        std::cout << "\n\n";
    }
}

// get entire objects
TEST_CASE("database get object") {
    SUBCASE("get track") {
        Track track = database.get_track(1);
        CHECK(track.title != "");
        CHECK(track.artist != "");

        std::cout << "TRACK 1: " << track << "\n";
    }
    SUBCASE("get album") {
        Album album = database.get_album(1);
        CHECK(album.title != "");
        CHECK(album.artist != "");

        std::cout << "ALBUM 1: " << album << "\n";
    }
    SUBCASE("get artist") {
        Artist artist = database.get_artist(1);
        CHECK(artist.name != "");
        CHECK(artist.person_behind != "");

        std::cout << "ARTIST 1: " << artist << "\n\n";
    }
}

// get track info
TEST_CASE("database track info") {
    SUBCASE("get title") {
        std::optional<std::string> title = database.get_track_title(1);
        if (title) {
            CHECK(*title != "");

            std::cout << "\nTITLE OF TRACK 1: " << *title << "\n";
        }
    }
    SUBCASE("get duration") {
        std::optional<Duration> duration = database.get_track_duration(1);
        if (duration) {
            CHECK(*duration != 0);

            std::cout << "DURATION OF TRACK 1: " << *duration << "\n";
        }
    }
}

// get album info
TEST_CASE("database album info") {
    SUBCASE("get title") {
        std::string title = database.get_album_title(1);
        CHECK(title != "");

        std::cout << "\nTITLE OF ALBUM 1: " << title << "\n";
    }
    SUBCASE("get artist") {
        std::optional<Artist> artist = database.get_album_artist(1);
        if (artist) {
            CHECK(artist->name != "");

            std::cout << "ARTIST OF ALBUM 1: " << artist->name << "\n";
        }
    }
    SUBCASE("get date") {
        std::optional<Date> date = database.get_album_date(1);
        if (date) {
            // CHECK(*date != ?); TODO: finish

            std::cout << "DATE OF ALBUM 1: " << *date << "\n";
        }
    }
    SUBCASE("get type") {
        std::optional<std::string> album_type = database.get_album_album_type(1);
        if (album_type) {
             CHECK(*album_type != "");

            std::cout << "TYPE OF ALBUM 1: " << *album_type << "\n";
        }
    }
    SUBCASE("get image_path") {
        std::optional<std::string> image_path = database.get_album_image_path(1);
        if (image_path) {
            CHECK(*image_path != "");

            std::cout << "IMAGE_PATH OF ALBUM 1: " << *image_path << "\n";
        }
    }
}

// get artist info
TEST_CASE("database artist info") {
    SUBCASE("get name") {
        std::string name = database.get_artist_name(1);
        CHECK(name != "");

        std::cout << "\nNAME OF ARTIST 1: " << name << "\n";
    }
    SUBCASE("get person_behind") {
        std::optional<std::string> person_behind = database.get_artist_person_behind(1);
        if (person_behind) {
            CHECK(*person_behind != "");

            std::cout << "PERSON_BEHIND ARTIST 1: " << *person_behind << "\n";
        }
    }
    SUBCASE("get image_path") {
        std::optional<std::string> image_path = database.get_artist_image_path(1);
        if (image_path) {
            CHECK(*image_path != "");

            std::cout << "IMAGE_PATH OF ARTIST 1: " << *image_path << "\n";
        }
    }
}

// set track info
TEST_CASE("database set track info") {
    SUBCASE("set track title") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_title(1);

        // test
        database.set_track_title(1, "new title");
        std::optional<std::string> new_value = database.get_track_title(1);
        CHECK(*new_value == "new title");

        // set back to old value
        database.set_track_title(1, *old_value);
    }
    SUBCASE("set track artist") {
        // grab old value to reset later
        std::optional<Artist> old_value = database.get_track_artist(1);

        // test
        database.set_track_artist(1, "new artist");
        std::optional<Artist> new_value = database.get_track_artist(1);
        CHECK(new_value->name == "new artist");

        // set back to old value
        old_value ? database.set_track_artist(1, old_value->name) : database.set_track_artist(1, "");
    }
    SUBCASE("set track album") {
        // grab old value to reset later
        std::optional<Album> old_value = database.get_track_album(1);

        // test
        database.set_track_album(1, "new album");
        std::optional<Album> new_value = database.get_track_album(1);
        CHECK(new_value->title == "new album");

        // set back to old value
        old_value ? database.set_track_album(1, old_value->title) : database.set_track_album(1, "");
    }
    SUBCASE("set track date") {
        // grab old value to reset later
        std::optional<Date> old_value = database.get_track_date(10);

        // test
        database.set_track_date(1, Date{1,1,2025});
        std::optional<Date> new_value = database.get_track_date(1);
        CHECK(*new_value == Date{1,1,2025});

        // set back to old value
        old_value ? database.set_track_date(1, *old_value) : database.set_track_date(1, Date{});
    }
    SUBCASE("set track tracklist_num") {
        // grab old value to reset later
        std::optional<int> old_value = database.get_track_tracklist_num(1);

        // test
        database.set_track_tracklist_num(1, 7);
        std::optional<int> new_value = database.get_track_tracklist_num(1);
        CHECK(*new_value == 7);

        // set back to old value
        old_value ? database.set_track_tracklist_num(1, *old_value) : database.set_track_tracklist_num(1, 0);
    }
    SUBCASE("set track file_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_file_path(1);

        // test
        database.set_track_file_path(1, "new path");
        std::optional<std::string> new_value = database.get_track_file_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        old_value ? database.set_track_file_path(1, *old_value) : database.set_track_file_path(1, "");
    }
    SUBCASE("set track image_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_image_path(1);

        // test
        database.set_track_image_path(1, "new path");
        std::optional<std::string> new_value = database.get_track_image_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        old_value ? database.set_track_image_path(1, *old_value) : database.set_track_image_path(1, "");
    }
}