// Samuel Sutton
#include "date.h"
#include <stdexcept>
#include <cctype>
#include <ctime>

Date::Date(int month, int day, int year)
    :month{Month{month}}, day{day}, year{year} {

    validate_date();
}

void Date::validate_date() {
    std::string base_message = "Tried to construct invalid Date: ";
    base_message += std::to_string(month.number()) + "/" + std::to_string(day) + "/" + std::to_string(year) + "\n";
    if (year == 0 && (month != 0 || day != 0)) {
        throw std::runtime_error(base_message + "Cannot set month or day without valid year");
    }
    if (month == 0 && day != 0) {
        throw std::runtime_error(base_message + "Cannot set day without valid month");
    }
    if (month  < 0 || month > 12) {
        throw std::runtime_error(base_message + "Month must be between 0-12");
    }
    if (day < 0 || day > days_in_month()) {
        throw std::runtime_error(base_message + "Day cannot be negative or greater than days in month");
    }
    if (year < 0 || year > 9999) {
        throw std::runtime_error(base_message + "Year shouldn't be outside the range of 0-9999");
    }
}

// ---------- Equality Operators ----------
bool Date::operator==(const Date& rhs) const {
    return day == rhs.day && month == rhs.month && year == rhs.year;
}
bool Date::operator!=(const Date& rhs) const {
    return !(*this == rhs);
}

bool Date::operator<(const Date& rhs) const {
    if (year < rhs.year) {
        return true;
    }
    else if (year == rhs.year && *this != rhs) {
        if (month < rhs.month) {
            return true;
        }
        else if (month == rhs.month && day < rhs.day) {
            return true;
        }
        return false;
    }
    return false;
}
bool Date::operator>(const Date& rhs) const {
    if (year > rhs.year) {
        return true;
    }
    else if (*this == rhs || *this < rhs) {
        return false;
    }
    return true;
}

bool Date::operator<=(const Date& rhs) const {
    return *this < rhs || *this == rhs;
}
bool Date::operator>=(const Date& rhs) const {
    return *this > rhs || *this == rhs;
}

// Assignment operator
Date& Date::operator=(const Date& rhs) {
    if (this == &rhs) {
        return *this;
    }
    year = rhs.get_year();
    month = rhs.get_month_num();
    day = rhs.get_day();
    return *this;
}

// ---------- Increment / Decrement ----------
void Date::operator++() {
    if (day == days_in_month()) {
        day = 1;
        if (month == 12) {
            month.set_month(1);
            ++year;
        } else {
            increment_month();
        }
    } else {
        ++day;
    }
}
void Date::operator--() {
    if (day == 1) {
        day = days_in_month();
        if (month == 1) {
            --year;
            month.set_month(12);
        }
        else {
            decrement_month();
        }
    }
    else {
        --day;
    }
}

void Date::increment_year() {
    ++year;
    handle_date_overflow();
}
void Date::decrement_year() {
    --year;
    handle_date_overflow();
}

void Date::increment_month() {
    if (month == 12) {
        month.set_month(1);
        increment_year();
    }
    else {
        month.set_month(month.number() + 1);
    }
        // Clamp to last day of new month if needed
        if (original_day > days_in_month()) {
            day = days_in_month();
        }
}
void Date::decrement_month() {
    if (month == 1) {
        month.set_month(12);
        decrement_year();
    }
    else {
        month.set_month(month.number() - 1);
    }
    handle_date_overflow();
}

// ---------- Addition / Subtraction ----------
Date Date::add_time(int days, int months, int years) const {
    Date new_date = *this;

    for (int y = 0; y < years; ++y) {
        new_date.increment_year();
    }
    for (int m = 0; m < months; ++m) {
        new_date.increment_month();
    }
    for (int d = 0; d < days; ++d) {
        ++new_date;
    }

    return new_date;
}
Date Date::subtract_time(int days, int months, int years) const {
    Date new_date = *this;

    for (int y = 0; y < years; ++y) {
        new_date.decrement_year();
    }
    for (int m = 0; m < months; ++m) {
        new_date.decrement_month();
    }
    for (int d = 0; d < days; ++d) {
        --new_date;
    }

    return new_date;
}

// ---------- Getters ----------
int Date::get_month_num() const {
    return month.number();
}
std::string Date::get_month_name() const {
    return month.name();
}
int Date::get_day() const {
    return day;
}
int Date::get_year() const {
    return year;
}

// ---------- Setters ----------
void Date::set_month(int month_num) {
    month.set_month(month_num);
    validate_date();
}
void Date::set_day(int day) {
    this->day = day;
    validate_date();
}
void Date::set_year(int year) {
    this->year = year;
    validate_date();
}

// ---------- Conversions ----------
// Date -> string (MM/DD/YYYY format)
std::string Date::to_string() const {
    if (year == 0) return ""; // Empty string for null / invalid date
    
    // YYYY
    if (month == 0) {
        return std::to_string(year);
    }
    // MM/YYYY
    if (day == 0) {
        return std::to_string(month.number()) + "/" + std::to_string(year);
    }
    // MM/DD/YYYY
    return std::to_string(month.number()) + "/" + std::to_string(day) + "/" + std::to_string(year);
}
// Date -> string (Month DD, YYYY format)
std::string Date::to_long_string() const {
    if (year == 0) return ""; // Empty string for null / invalid date
    
    // YYYY
    if (month == 0) {
        return std::to_string(year);
    }
    // Month YYYY
    if (day == 0) {
        return month.name() + " " + std::to_string(year);
    }
    // Month DD, YYYY
    return month.name() + " " + std::to_string(day) + ", " + std::to_string(year);
}

std::optional<int64_t> Date::to_unix() const {
    if (year == 0) return std::nullopt; // Return null if null or invalid date

    // Throw error if too big for 32-bit time_t
    if (year >= 2038 && sizeof(std::time_t) == 4) {
        std::string message = "Date.to_unix() failed: Cannot convert dates past 2038 on this device.";
        throw std::runtime_error(message + " Need 64-bit time_t, not 32-bit");
    }

    tm time;
    time.tm_year = year - 1900; // tm counts from 1900
    time.tm_mon = month.number() - 1;
    time.tm_mday = day;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;
    
    // Convert to unix
    std::time_t unix_time = mktime(&time); // Uses local time
    if (unix_time == -1) {
        throw std::runtime_error("Failed trying to convert date to unix time");
    }
    return static_cast<int64_t>(unix_time);
}
Date Date::from_unix(int64_t unix_time) {
    // Convert int64_t to time_t (check for overflow)
    if (unix_time < static_cast<int64_t>(std::numeric_limits<std::time_t>::min()) || 
        unix_time > static_cast<int64_t>(std::numeric_limits<std::time_t>::max())) {
        throw std::runtime_error("int64_t unix time value is out of range for time_t");
    }
    std::time_t unix_converted = static_cast<std::time_t>(unix_time);

    tm time;
    if (!localtime_r(&unix_converted, &time)) { // Converts to local time (thread safe, but not cross-platform)
        throw std::runtime_error("Failed to convert unix time to date");
    }
    return Date{time.tm_mon + 1, time.tm_mday, time.tm_year + 1900};
}

// ---------- Helper Functions ----------
bool Date::is_leap_year() const {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int Date::days_in_month() const {
    if (month == 2) {
        if (is_leap_year()) {
            return 29;
        }
        return 28;
    } else if (month == 1 || month == 3 || month == 5 ||
               month == 7 || month == 8 || month == 10 ||
               month == 12) {
        return 31;
    } else {
        return 30;
    }
}

void Date::handle_date_overflow() {
    if (day > days_in_month()) {
        increment_month();
        int new_day = day - days_in_month();
        day = new_day;
    }
}

std::ostream& operator<<(std::ostream& os, const Date date) {
    os << date.month.name() << " " << date.day << ", " << date.year;
    return os;
}


//--------------------------------------------------------------------------------
//      MONTH STRUCT
//--------------------------------------------------------------------------------
// Constructor
Date::Month::Month(int month_number)
    :month_number{month_number}, month_name{find_month_name(month_number)} {}

// Assignment operator
Date::Month& Date::Month::operator=(const Month& rhs) {
    if (this == &rhs) {
        return *this;
    }
    month_number = rhs.number();
    month_name = find_month_name(month_number);
    return *this;
}

// ---------- Set & Get ----------
void Date::Month::set_month(int month_number)  {
    this->month_number = month_number;
    this->month_name = find_month_name(month_number);
}
int Date::Month::number() const {
    return month_number;
}
std::string Date::Month::name() const {
    return month_name;
}

// ---------- Operators ----------
// ==
bool Date::Month::operator==(const Month& rhs) const {
    return month_number == rhs.number();
}
bool Date::Month::operator==(int rhs) const {
    return month_number == rhs;
}
bool Date::Month::operator==(const std::string& rhs) const {
    return to_lowercase(month_name) == to_lowercase(rhs);
}
// !=
bool Date::Month::operator!=(const Month& rhs) const {
    return !(*this == rhs);
}
bool Date::Month::operator!=(int rhs) const {
    return !(*this == rhs);
}
bool Date::Month::operator!=(const std::string& rhs) const {
    return !(*this == rhs);
}

// <
bool Date::Month::operator<(const Month& rhs) const {
    return month_number < rhs.number();
}
bool Date::Month::operator<(int rhs) const {
    return month_number < rhs;
}
bool Date::Month::operator<(const std::string& rhs) const {
    return month_number < find_month_number(rhs);
}
// >
bool Date::Month::operator>(const Month& rhs) const {
    return month_number > rhs.number();
}
bool Date::Month::operator>(int rhs) const {
    return month_number > rhs;
}
bool Date::Month::operator>(const std::string& rhs) const {
    return month_number > find_month_number(rhs);
}

// <=
bool Date::Month::operator<=(const Month& rhs) const {
    return *this < rhs || *this == rhs;
}
bool Date::Month::operator<=(int rhs) const {
    return *this < rhs || *this == rhs;
}
bool Date::Month::operator<=(const std::string& rhs) const {
    return *this < rhs || *this == rhs;
}
// >=
bool Date::Month::operator>=(const Month& rhs) const {
    return *this > rhs || *this == rhs;
}
bool Date::Month::operator>=(int rhs) const {
    return *this > rhs || *this == rhs;
}
bool Date::Month::operator>=(const std::string& rhs) const {
    return *this > rhs || *this == rhs;
}

// ---------- Helpers ----------
std::string Date::Month::find_month_name(int number) const {
    if (number == 0) {
        return "";
    }
    if (number == 1) {
        return "January";
    }
    if (number == 2) {
        return "February";
    }
    if (number == 3) {
        return "March";
    }
    if (number == 4) {
        return "April";
    }
    if (number == 5) {
        return "May";
    }
    if (number == 6) {
        return "June";
    }
    if (number == 7) {
        return "July";
    }
    if (number == 8) {
        return "August";
    }
    if (number == 9) {
        return "September";
    }
    if (number == 10) {
        return "October";
    }
    if (number == 11) {
        return "November";
    }
    if (number == 12) {
        return "December";
    } else {
        throw std::runtime_error("Tried to assign name to invalid month number. Only use ints from 0-12");
    }
}
int Date::Month::find_month_number(const std::string& name) const {
    std::string lowercase_name = to_lowercase(name);
    if (lowercase_name == "") {
        return 0;
    }
    if (lowercase_name == "january") {
        return 1;
    }
    if (lowercase_name == "february") {
        return 2;
    }
    if (lowercase_name == "march") {
        return 3;
    }
    if (lowercase_name == "april") {
        return 4;
    }
    if (lowercase_name == "may") {
        return 5;
    }
    if (lowercase_name == "june") {
        return 6;
    }
    if (lowercase_name == "july") {
        return 7;
    }
    if (lowercase_name == "august") {
        return 8;
    }
    if (lowercase_name == "september") {
        return 9;
    }
    if (lowercase_name == "october") {
        return 10;
    }
    if (lowercase_name == "november") {
        return 11;
    }
    if (lowercase_name == "december") {
        return 12;
    } else {
        throw std::runtime_error("Tried to assign number to invalid month name. Please spell better");
    }
}


// Other
std::string to_lowercase(std::string string) {
    for (char& character : string) {
        character = std::tolower(character);
    }
    return string;
}
