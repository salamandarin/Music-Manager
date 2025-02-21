// Samuel Sutton
#pragma once
#include <ostream>
#include <string>

class Duration {
public:
    // constructors
    Duration(); // default constructor
    Duration(int minutes, int seconds);
    Duration(int duration_in_seconds);


    int get_duration_in_seconds() const;
    int get_minutes() const;
    int get_seconds() const;

    // equality
    bool operator==(const Duration& rhs) const;
    bool operator!=(const Duration& rhs) const;

    bool operator<(const Duration& rhs) const;
    bool operator>(const Duration& rhs) const;

    bool operator<=(const Duration& rhs) const;
    bool operator>=(const Duration& rhs) const;


    friend std::ostream& operator<<(std::ostream& os, const Duration duration);

private:
    int calc_duration_in_seconds() const;
    int calc_minutes_from_seconds(int duration_in_seconds) const;
    int calc_seconds_leftover(int duration_in_seconds) const;

    int minutes;
    int seconds;
    int duration_in_seconds;
};

