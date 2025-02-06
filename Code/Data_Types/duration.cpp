// Samuel Sutton
#include "duration.h"
#include <stdexcept>
#include <string>

Duration::Duration(int minutes, int seconds)
   :minutes{minutes}, seconds{seconds}, duration_in_seconds{calc_duration_in_seconds()} {
       if (seconds > 60) {
           throw std::runtime_error("Cannot have more than 60 seconds");
       }
       if (minutes < 0 || seconds < 0) {
           throw std::runtime_error("Cannot have negative time");
       }
   }
Duration::Duration(int duration_in_seconds)
   :minutes{calc_minutes_from_seconds(duration_in_seconds)}, seconds{calc_seconds_leftover(duration_in_seconds)},
    duration_in_seconds{duration_in_seconds} {
       if (seconds > 60) {
           throw std::runtime_error("Cannot have more than 60 seconds");
       }
       if (minutes < 0 || seconds < 0) {
           throw std::runtime_error("Cannot have negative time");
       }
    }



int Duration::get_duration_in_seconds() const {
    return duration_in_seconds;
}
int Duration::get_minutes() const {
    return minutes;
}
int Duration::get_seconds() const {
    return seconds;
}


bool Duration::operator==(const Duration& rhs) const {
    return duration_in_seconds == rhs.get_duration_in_seconds();
}
bool Duration::operator!=(const Duration& rhs) const {
    return !(*this == rhs);
}
bool Duration::operator<(const Duration& rhs) const {
    return duration_in_seconds < rhs.get_duration_in_seconds();
}
bool Duration::operator>(const Duration& rhs) const {
    return duration_in_seconds > rhs.get_duration_in_seconds();
}
bool Duration::operator<=(const Duration& rhs) const {
    return *this < rhs || *this == rhs;
}
bool Duration::operator>=(const Duration& rhs) const {
    return *this > rhs || *this == rhs;
}


int Duration::calc_duration_in_seconds() const {
    return ((minutes * 60) + seconds);
}
int Duration::calc_minutes_from_seconds(int duration_in_seconds) const {
    return (duration_in_seconds / 60);
}
int Duration::calc_seconds_leftover(int duration_in_seconds) const {
    return (duration_in_seconds % 60);
}


std::ostream& operator<<(std::ostream& os, const Duration duration) {
    os << std::to_string(duration.calc_duration_in_seconds());
    return os;
}
