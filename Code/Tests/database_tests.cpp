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

// get all objects
TEST_CASE("database get all objects") {
    SUBCASE("get all tracks") {
        std::vector<Track> tracks = database.get_all_tracks();
        CHECK(tracks.size() != 0);

        std::cout << "ALL TRACKS\n";
        std::cout << "-----------------------------\n";
        for (const Track& track: tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all albums") {
        std::vector<Album> albums = database.get_all_albums();
        CHECK(albums.size() != 0);

        std::cout << "ALL ALBUMS\n";
        std::cout << "-----------------------------\n";
        for (const Album& album: albums) {
            std::cout << album.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all artists") {
        std::vector<Artist> artists = database.get_all_artists();
        CHECK(artists.size() != 0);

        std::cout << "ALL ARTISTS\n";
        std::cout << "-----------------------------\n";
        for (const Artist& artist: artists) {
            std::cout << artist.name << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all people") {
        std::vector<std::string> people = database.get_all_people();
        CHECK(people.size() != 0);

        std::cout << "ALL PEOPLE\n";
        std::cout << "-----------------------------\n";
        for (const std::string& person: people) {
            std::cout << person << "\n";
        }
        std::cout << "\n\n";
    }
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

// get & set track info
TEST_CASE("database get & set track info") {
    SUBCASE("get & set track title") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_title(1);

        // test
        database.set_track_title(1, "new title");
        std::optional<std::string> new_value = database.get_track_title(1);
        CHECK(*new_value == "new title");

        // set back to old value
        database.set_track_title(1, old_value.value_or(""));
    }
    SUBCASE("get & set track artist") {
        // grab old value to reset later
        std::optional<Artist> old_value = database.get_track_artist(1);

        // test
        database.set_track_artist(1, "new artist");
        std::optional<Artist> new_value = database.get_track_artist(1);
        CHECK(new_value->name == "new artist");

        // set back to old value
        old_value ? database.set_track_artist(1, old_value->name) : database.set_track_artist(1, "");
    }
    SUBCASE("get & set track album") {
        // grab old value to reset later
        std::optional<Album> old_value = database.get_track_album(1);

        // test
        database.set_track_album(1, "new album");
        std::optional<Album> new_value = database.get_track_album(1);
        CHECK(new_value->title == "new album");

        // set back to old value
        old_value ? database.set_track_album(1, old_value->title) : database.set_track_album(1, "");
    }
    SUBCASE("get & set track date") {
        // grab old value to reset later
        std::optional<Date> old_value = database.get_track_date(10);

        // test
        database.set_track_date(1, Date{1,1,2025});
        std::optional<Date> new_value = database.get_track_date(1);
        CHECK(*new_value == Date{1,1,2025});

        // set back to old value
        old_value ? database.set_track_date(1, *old_value) : database.set_track_date(1, Date{});
    }
    SUBCASE("get & set track tracklist_num") {
        // grab old value to reset later
        std::optional<int> old_value = database.get_track_tracklist_num(1);

        // test
        database.set_track_tracklist_num(1, 7);
        std::optional<int> new_value = database.get_track_tracklist_num(1);
        CHECK(*new_value == 7);

        // set back to old value
        database.set_track_tracklist_num(1, old_value.value_or(0));
    }
    SUBCASE("get & set track file_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_file_path(1);

        // test
        database.set_track_file_path(1, "new path");
        std::optional<std::string> new_value = database.get_track_file_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        database.set_track_file_path(1, old_value.value_or(""));
    }
    SUBCASE("get & set track image_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_track_image_path(1);

        // test
        database.set_track_image_path(1, "new path");
        std::optional<std::string> new_value = database.get_track_image_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        database.set_track_image_path(1, old_value.value_or(""));
    }
}

// get & set album info
TEST_CASE("database get & set album info") {
    SUBCASE("get & set album title") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_album_title(1);

        // test
        database.set_album_title(1, "new title");
        std::optional<std::string> new_value = database.get_album_title(1);
        CHECK(*new_value == "new title");

        // set back to old value
        database.set_album_title(1, *old_value);
    }
    SUBCASE("get & set album artist") {
        std::optional<Artist> old_value = database.get_album_artist(1);

        // test
        database.set_album_artist(1, "new artist");
        std::optional<Artist> new_value = database.get_album_artist(1);
        CHECK(new_value->name == "new artist");

        // set back to old value
        old_value ? database.set_album_artist(1, old_value->name) : database.set_album_artist(1, "");
    }
    SUBCASE("get & set album date") {
        // grab old value to reset later
        std::optional<Date> old_value = database.get_album_date(10);

        // test
        database.set_album_date(1, Date{1,1,2025});
        std::optional<Date> new_value = database.get_album_date(1);
        CHECK(*new_value == Date{1,1,2025});

        // set back to old value
        old_value ? database.set_album_date(1, *old_value) : database.set_album_date(1, Date{});
    }
    SUBCASE("get & set album type") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_album_type(1);

        // test
        database.set_album_type(1, "EP");
        std::optional<std::string> new_value = database.get_album_type(1);
        CHECK(*new_value == "EP");

        // set back to old value
        database.set_album_type(1, old_value.value_or(""));
    }
    SUBCASE("trying to set empty album type") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_album_type(1);

        // test
        database.set_album_type(1, ""); // try to set empty album type
        std::optional<std::string> new_value = database.get_album_type(1);
        CHECK(new_value.value_or("") == "");

        // set back to old value
        database.set_album_type(1, old_value.value_or(""));
    }
    SUBCASE("trying to set non-existent album type") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_album_type(1);

        // test
        database.set_album_type(1, "double lp"); // try to set non-existent album type
        std::optional<std::string> new_value = database.get_album_type(1);
        CHECK(new_value.value_or("") == "");

        // set back to old value
        database.set_album_type(1, old_value.value_or(""));
    }
    SUBCASE("get & set album image_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_album_image_path(1);

        // test
        database.set_album_image_path(1, "new path");
        std::optional<std::string> new_value = database.get_album_image_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        database.set_album_image_path(1, old_value.value_or(""));
    }
}

// get & set artist info
TEST_CASE("database artist info") {
    SUBCASE("get & set artist name") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_artist_name(1);

        // test
        database.set_artist_name(1, "new name");
        std::optional<std::string> new_value = database.get_artist_name(1);
        CHECK(*new_value == "new name");

        // set back to old value
        database.set_artist_name(1, old_value.value_or(""));
    }
    SUBCASE("get & set artist person_behind") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_artist_person_behind(1);

        // test
        database.set_artist_person_behind(1, "new person");
        std::optional<std::string> new_value = database.get_artist_person_behind(1);
        CHECK(*new_value == "new person");

        // set back to old value
        database.set_artist_person_behind(1, old_value.value_or(""));
    }
    SUBCASE("get & set artist image_path") {
        // grab old value to reset later
        std::optional<std::string> old_value = database.get_artist_image_path(1);

        // test
        database.set_artist_image_path(1, "new path");
        std::optional<std::string> new_value = database.get_artist_image_path(1);
        CHECK(*new_value == "new path");

        // set back to old value
        database.set_artist_image_path(1, old_value.value_or(""));
    }
}