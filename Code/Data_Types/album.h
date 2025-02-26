// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include <string>

struct Album {
    int id;
    std::string title;
    std::string artist;
    std::optional<Date> date;
    std::string type;
    std::string image_path;
};