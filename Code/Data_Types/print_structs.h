// Samuel Sutton - 2025
#pragma once
#include "track.h"
#include "album.h"
#include "artist.h"
#include <ostream>

std::ostream& operator<<(std::ostream& os, const Track& track);
std::ostream& operator<<(std::ostream& os, const Album& album);
std::ostream& operator<<(std::ostream& os, const Artist& artist);

