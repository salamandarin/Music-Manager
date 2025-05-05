// Samuel Sutton - 2025
#include "core.h"
#include "print_structs.h"
#include <doctest.h>
#include <iostream>

Core core;

// constructor
TEST_CASE("core constructor") {
    CHECK_NOTHROW(Core core);
}

// get all objects
TEST_CASE("core get all objects") {
    SUBCASE("get all tracks") {
        std::vector<Track> tracks = core.get_all_tracks();
        CHECK(tracks.size() != 0);

        std::cout << "ALL TRACKS\n";
        std::cout << "-----------------------------\n";
        for (const Track& track: tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all albums") {
        std::vector<Album> albums = core.get_all_albums();
        CHECK(albums.size() != 0);

        std::cout << "ALL ALBUMS\n";
        std::cout << "-----------------------------\n";
        for (const Album& album: albums) {
            std::cout << album.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all artists") {
        std::vector<Artist> artists = core.get_all_artists();
        CHECK(artists.size() != 0);

        std::cout << "ALL ARTISTS\n";
        std::cout << "-----------------------------\n";
        for (const Artist& artist: artists) {
            std::cout << artist.name << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get all people") {
        std::vector<std::string> people = core.get_all_people();
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
TEST_CASE("core get objects by category") {
    SUBCASE("get album tracks") {
        std::vector<Track> tracks = core.get_album_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "TRACKS IN ALBUM 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist tracks") {
        std::vector<Track> tracks = core.get_artist_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "\n\nTRACKS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist albums") {
        std::vector<Album> albums = core.get_artist_albums(1);
        CHECK(albums.size() != 0);

        std::cout << "\n\nALBUMS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Album& album : albums) {
            std::cout << album.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get person artists") {
        std::vector<Artist> artists = core.get_person_artists(1);
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
TEST_CASE("core get object") {
    SUBCASE("get track") {
        Track track = core.get_track(1);
        CHECK(track.title != "");
        CHECK(track.artist != "");

        std::cout << "TRACK 1: " << track << "\n";
    }
    SUBCASE("get album") {
        Album album = core.get_album(1);
        CHECK(album.title != "");
        CHECK(album.artist != "");

        std::cout << "ALBUM 1: " << album << "\n";
    }
    SUBCASE("get artist") {
        Artist artist = core.get_artist(1);
        CHECK(artist.name != "");
        CHECK(artist.person_behind != "");

        std::cout << "ARTIST 1: " << artist << "\n\n";
    }
}

// get & set track info
TEST_CASE("core get & set track info") {
    Track track_old = core.get_track(1); // get old track info to reset later

    SUBCASE("get & set track title") {
        // test
        core.set_track_title(1, "new title");
        Track new_track = core.get_track(1);
        CHECK(new_track.title == "new title");

        // set back to old value
        core.set_track_title(1, track_old.title);
    }
    SUBCASE("get & set track artist") {
        // test
        core.set_track_artist(1, "new artist");
        Track new_track = core.get_track(1);
        CHECK(new_track.artist == "new artist");

        // set back to old value
        core.set_track_artist(1, track_old.artist);
    }
    SUBCASE("get & set track album") {
        // test
        core.set_track_album(1, "new album");
        Track new_track = core.get_track(1);
        CHECK(new_track.album == "new album");

        // set back to old value
        core.set_track_album(1, track_old.album);
    }
    SUBCASE("get & set track date") {
        // test
        core.set_track_date(1, Date{1,1,2025});
        Track new_track = core.get_track(1);
        CHECK(new_track.date == Date{1,1,2025});

        // set back to old value
        core.set_track_date(1, track_old.date);
    }
    SUBCASE("get & set track tracklist_num") {
        // test
        core.set_track_tracklist_num(1, 7);
        Track new_track = core.get_track(1);
        CHECK(new_track.tracklist_num == 7);

        // set back to old value
        core.set_track_tracklist_num(1, track_old.tracklist_num);
    }
}

// get & set album info
TEST_CASE("core get & set album info") {
    Album album_old = core.get_album(1); // get old artist info to reset later

    SUBCASE("get & set album title") {
        // test
        core.set_album_title(1, "new title");
        Album new_album = core.get_album(1);
        CHECK(new_album.title == "new title");

        // set back to old value
        core.set_album_title(1, album_old.title);
    }
    SUBCASE("get & set album artist") {
        // test
        core.set_album_artist(1, "new artist");
        Album new_album = core.get_album(1);
        CHECK(new_album.artist == "new artist");

        // set back to old value
        core.set_album_artist(1, album_old.artist);
    }
    SUBCASE("get & set album date") {
        // test
        core.set_album_date(1, Date{1,1,2025});
        Album new_album = core.get_album(1);
        CHECK(new_album.date == Date{1,1,2025});

        // set back to old value
        core.set_album_date(1, album_old.date);
    }
    SUBCASE("get & set album type") {
        // test
        core.set_album_type(1, "EP");
        Album new_album = core.get_album(1);
        CHECK(new_album.type == "EP");

        // set back to old value
        core.set_album_type(1, album_old.type);
    }
    SUBCASE("trying to set empty album type") {
        // test
        core.set_album_type(1, ""); // try to set empty album type
        Album new_album = core.get_album(1);
        CHECK(new_album.type == "");

        // set back to old value
        core.set_album_type(1, album_old.type);
    }
    SUBCASE("trying to set non-existent album type") {
        // test
        core.set_album_type(1, "double lp"); // try to set non-existent album type
        Album new_album = core.get_album(1);
        CHECK(new_album.type == "");

        // set back to old value
        core.set_album_type(1, album_old.type);
    }
}

// get & set artist info
TEST_CASE("core artist info") {
    Artist artist_old = core.get_artist(1); // get old artist info to reset later

    SUBCASE("get & set artist name") {
        // test
        core.set_artist_name(1, "new name");
        Artist new_artist = core.get_artist(1);
        CHECK(new_artist.name == "new name");

        // set back to old value
        core.set_artist_name(1, artist_old.name);
    }
    SUBCASE("get & set artist person_behind") {
        // test
        core.set_artist_person_behind(1, "new person");
        Artist new_artist = core.get_artist(1);
        CHECK(new_artist.person_behind == "new person");

        // set back to old value
        core.set_artist_person_behind(1, artist_old.person_behind);
    }
}