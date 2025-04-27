// Samuel Sutton - 2025
#include "database_manager.h"
#include "print_structs.h"
#include <doctest.h>
#include <iostream>

DatabaseManager database_manager;

// constructor
TEST_CASE("database constructor") {
    CHECK_NOTHROW(DatabaseManager database);
}

// getters
TEST_CASE("database get objects by category") {
    SUBCASE("get album tracks") {
        std::vector<Track> tracks = database_manager.get_album_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "TRACKS IN ALBUM 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist tracks") {
        std::vector<Track> tracks = database_manager.get_artist_tracks(1);
        CHECK(tracks.size() != 0);

        std::cout << "\n\nTRACKS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Track& track : tracks) {
            std::cout << track.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get artist albums") {
        std::vector<Album> albums = database_manager.get_artist_albums(1);
        CHECK(albums.size() != 0);

        std::cout << "\n\nALBUMS BY ARTIST 1\n";
        std::cout << "-----------------------------\n";
        for (const Album& album : albums) {
            std::cout << album.title << "\n";
        }
        std::cout << "\n\n";
    }
    SUBCASE("get person artists") {
        std::vector<Artist> artists = database_manager.get_person_artists(1);
        CHECK(artists.size() != 0);

        std::cout << "\n\nARTIST NAMES UNDER PERSON 1\n";
        std::cout << "-----------------------------\n";
        for (const Artist& artist : artists) {
            std::cout << artist.name << "\n";
        }
        std::cout << "\n\n";
    }
}

TEST_CASE("database get object") {
    SUBCASE("get track") {
        Track track = database_manager.get_track(1);
        CHECK(track.title != "");
        CHECK(track.artist != "");

        std::cout << "TRACK 1: " << track << "\n";
    }
    SUBCASE("get album") {
        Album album = database_manager.get_album(1);
        CHECK(album.title != "");
        CHECK(album.artist != "");

        std::cout << "ALBUM 1: " << album << "\n";
    }
    SUBCASE("get artist") {
        Artist artist = database_manager.get_artist(1);
        CHECK(artist.name != "");
        CHECK(artist.person_behind != "");

        std::cout << "ARTIST 1: " << artist << "\n\n";
    }
}