// Samuel Sutton
#pragma once
#include <ostream>
#include <string>

class Date {
public:
    Date(int month, int day, int year); // Constructor

    // Equality operators
    bool operator==(const Date& rhs) const;
    bool operator!=(const Date& rhs) const;

    bool operator<(const Date& rhs) const;
    bool operator>(const Date& rhs) const;

    bool operator<=(const Date& rhs) const;
    bool operator>=(const Date& rhs) const;

    // Assignment operator
    Date& operator=(const Date& rhs);

    // Increment / decrement by 1 day
    void operator++();
    void operator--();

    // Get date forwards / backwards in time
    Date add_time(int days, int months, int years) const;
    Date subtract_time(int days, int months, int years) const;

    // Getters
    int get_month_num() const;
    std::string get_month_name() const;
    int get_day() const;
    int get_year() const;

    // Helper functions
    bool is_leap_year() const;
    int days_in_month() const;

    // Print
    friend std::ostream& operator<<(std::ostream& os, const Date date);

private:
    // Month
    struct Month {
        Month(int number);

        int number;
        std::string name;

        std::string find_month_name(const int& month_number) const;
        int find_month_number(const std::string& month_name) const;

        std::string set_month(int month_number);

        bool operator==(const Month& rhs) const;
        bool operator==(const int& rhs) const;
        bool operator==(const std::string& rhs) const;

        bool operator<(const Month& rhs) const;
        bool operator<(const int& rhs) const;
        bool operator<(const std::string& rhs) const;

        bool operator>(const Month& rhs) const;
        bool operator>(const int& rhs) const;
        bool operator>(const std::string& rhs) const;
    };

    // Private member functions
    void validate_days_in_month();

    void increment_year();
    void decrement_year();

    void increment_month();
    void decrement_month();

    // Data
    Month month;
    int day;
    int year;
};

std::string to_lowercase(std::string string);