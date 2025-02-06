// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include <string>

struct Album {
    std::string title;
    std::string artist;
    std::optional<Date> date;
    std::string image_path;
    std::string type;
};