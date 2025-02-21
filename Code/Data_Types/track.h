// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include "duration.h"
#include <string>

struct Track {
    int id;
    std::string file_path;
    std::string title;
    std::string artist;
    std::string album;
    int tracklist_num;
    Duration duration;
    std::optional<Date> date;
};