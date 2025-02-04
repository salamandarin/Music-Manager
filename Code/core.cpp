// Samuel Sutton - 2025
#include "core.h"
#include "metadata_manager.h"
#include "file_manager.h"
#include "database_manager.h"

void Core::add_file(const std::string& file_path) {
    //TODO: FINISH FUNCTION

    // 1. gather metadata from track

    // 2. move file to correct location
    FileManager file_manager;
    std::string new_file_path = file_manager.add_file(file_path, track_data);

    // 3. log info to database
    DatabaseManager database_manager;
    database_manager.log_new_file(new_file_path);
}


