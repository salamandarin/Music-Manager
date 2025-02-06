// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include "duration.h"
#include <string>

struct Track {
    std::string title;
    std::string artist;
    std::string album;
    Duration duration;
    std::optional<Date> Date;
    std::string file_path;
};