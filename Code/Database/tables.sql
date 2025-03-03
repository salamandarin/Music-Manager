-- Create Table: person
CREATE TABLE IF NOT EXISTS people
(
    person_id         INTEGER PRIMARY KEY,
    name              TEXT NOT NULL
);

-- Create Table: artists
CREATE TABLE IF NOT EXISTS artists
(
    artist_id         INTEGER PRIMARY KEY,
    name              TEXT NOT NULL,
    person_behind_id  INTEGER,
    image_path        TEXT,
    FOREIGN KEY (person_behind_id) REFERENCES people (person_id)
);

-- Create Table: album_types
CREATE TABLE IF NOT EXISTS album_types
(
    album_type_id     INTEGER PRIMARY KEY,
    name              TEXT NOT NULL
);

-- Create Table: albums
CREATE TABLE IF NOT EXISTS albums
(
    album_id          INTEGER PRIMARY KEY,
    title             TEXT NOT NULL,
    artist_id         INTEGER,
    date              TEXT,
    type_id           INTEGER,
    image_path        TEXT,
    FOREIGN KEY (artist_id) REFERENCES artists (artist_id),
    FOREIGN KEY (type_id) REFERENCES album_types (album_type_id)
);

-- Create Table: tracks
CREATE TABLE IF NOT EXISTS tracks
(
    track_id          INTEGER PRIMARY KEY,
    title             TEXT,
    artist_id         INTEGER,
    album_id          INTEGER,
    duration          INTEGER,
    date              TEXT,
    tracklist_num     INTEGER,
    file_path         TEXT,
    image_path        TEXT,
    FOREIGN KEY (artist_id) REFERENCES artists (artist_id),
    FOREIGN KEY (album_id) REFERENCES albums (album_id)
);

INSERT INTO album_types (name)
VALUES
    ('Album'),
    ('EP'),
    ('Single')