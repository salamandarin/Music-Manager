// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include <string>

struct Album {
    std::string title;
    Date date;
    std::string artist;
    std::string image_path;
    std::string type;
};