// Samuel Sutton - 2025
#include "core.h"
#include "metadata_manager.h"
#include "file_manager.h"
#include "database_manager.h"
#include "track.h"

void Core::add_file(const std::string& file_path) {
    // 1. gather metadata from track
    MetadataManager metadata_manager{file_path};
    Track track_data = metadata_manager.get_data();

    // 2. move file to correct location
    FileManager file_manager;
    std::string new_file_path = file_manager.add_file(file_path, track_data);
    track_data.file_path = new_file_path; // update track_data with new file path

    // 3. log info to database
    DatabaseManager database_manager;
    database_manager.log_new_track(track_data);
}


