-- Create Table: settings
CREATE TABLE IF NOT EXISTS settings
(
    name              TEXT PRIMARY KEY NOT NULL,
    value             INTEGER NOT NULL
);

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
    date              INTEGER,
    type_id           INTEGER,
    image_path        TEXT,
    FOREIGN KEY (artist_id) REFERENCES artists (artist_id),
    FOREIGN KEY (type_id) REFERENCES album_types (album_type_id)
);

-- Create Table: tracks
CREATE TABLE IF NOT EXISTS tracks
(
    track_id          INTEGER PRIMARY KEY,
    title             TEXT, -- TODO: make not null? (make getter non-optional too)
    artist_id         INTEGER,
    album_id          INTEGER,
    duration          INTEGER,
    date              INTEGER,
    tracklist_num     INTEGER,
    file_path         TEXT,
    image_path        TEXT,
    FOREIGN KEY (artist_id) REFERENCES artists (artist_id),
    FOREIGN KEY (album_id) REFERENCES albums (album_id)
);


--------------- INSERT DEFAULT DATA ---------------
-- Insert: default settings values
INSERT OR IGNORE INTO settings (name, value)
VALUES
    ('is_nested', 1), -- true
    ('copy_music_files', 1); -- true

-- Insert: album types
INSERT OR IGNORE INTO album_types (name)
VALUES
    ('Album'),
    ('LP'), -- TODO: have EITHER 'Album' or 'LP'
    ('EP'),
    ('Single');