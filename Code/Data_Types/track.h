// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include "duration.h"
#include <string>

struct Track {
    int id=0;
    std::string title;
    std::string artist;
    std::string album;
    Duration duration;
    Date date;
    int tracklist_num=0;
    std::string file_path;
    std::string image_path;
};