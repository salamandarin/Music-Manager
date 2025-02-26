// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include "duration.h"
#include <string>

struct Track {
    int id;
    std::string title;
    std::string artist;
    std::string album;
    Duration duration;
    std::optional<Date> date;
    int tracklist_num;
    std::string file_path;
    std::string image_path;
};