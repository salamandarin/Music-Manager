// Samuel Sutton - 2025
#pragma once
#include "date.h"
#include <string>

struct Album {
    int id=0;
    std::string title;
    std::string artist;
    Date date;
    std::string type;
    std::string image_path;
};