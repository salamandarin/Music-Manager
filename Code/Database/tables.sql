-- Create Table: person
CREATE TABLE IF NOT EXISTS people
(
    person_id      INTEGER PRIMARY KEY,
    name           TEXT NOT NULL
);

-- Create Table: artists
CREATE TABLE IF NOT EXISTS artists
(
    artist_id      INTEGER PRIMARY KEY,
    artist_name    TEXT NOT NULL,
    person_behind  TEXT NOT NULL,
    image_path     TEXT,
    FOREIGN KEY (person_behind) REFERENCES people (person_id)
);

-- Create Table: album_types
CREATE TABLE IF NOT EXISTS album_types
(
    album_type_id  INTEGER PRIMARY KEY,
    name           TEXT NOT NULL
);

-- Create Table: albums
CREATE TABLE IF NOT EXISTS albums
(
    album_id       INTEGER PRIMARY KEY,
    title          TEXT NOT NULL,
    artist         INTEGER,
    date           TEXT,
    type           TEXT,
    image_path     TEXT,
    FOREIGN KEY (artist) REFERENCES artists (artist_id),
    FOREIGN KEY (type) REFERENCES album_types (album_type_id)
);

-- Create Table: tracks
CREATE TABLE IF NOT EXISTS tracks
(
    track_id       INTEGER PRIMARY KEY,
    title          TEXT NOT NULL,
    artist         INTEGER,
    album          INTEGER,
    duration       INTEGER,
    date           TEXT,
    tracklist_num  INTEGER,
    file_path      TEXT,
    image_path     TEXT,
    FOREIGN KEY (album) REFERENCES albums (album_id),
    FOREIGN KEY (artist) REFERENCES artists (artist_id)
);