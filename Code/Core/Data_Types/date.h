// Samuel Sutton
#pragma once
#include <ostream>
#include <string>

class Date {
public:
    Date(int month=0, int day=0, int year=0); // Constructor

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

    // Setters
    void set_month(int month_num);
    void set_day(int day);
    void set_year(int year);

    // Conversions
    std::string to_string() const; // MM/DD/YYYY
    std::string to_long_string() const; // Month DD, YYYY
    std::optional<int64_t> to_unix() const;
    static Date from_unix(int64_t unix_time);

    // Helper functions
    bool is_leap_year() const;
    int days_in_month() const;

    // Print
    friend std::ostream& operator<<(std::ostream& os, const Date date);

private:
    // Month
    class Month {
    public:
        Month(int number); // Constructor
        Month& operator=(const Month& rhs); // Assignment operator

        // Set & get data
        void set_month(int month_number);
        int number() const;
        std::string name() const;

        // Operators
        // ==
        bool operator==(const Month& rhs) const;
        bool operator==(int rhs) const;
        bool operator==(const std::string& rhs) const;
        // !=
        bool operator!=(const Month& rhs) const;
        bool operator!=(int rhs) const;
        bool operator!=(const std::string& rhs) const;
        // <
        bool operator<(const Month& rhs) const;
        bool operator<(int rhs) const;
        bool operator<(const std::string& rhs) const;
        // >
        bool operator>(const Month& rhs) const;
        bool operator>(int rhs) const;
        bool operator>(const std::string& rhs) const;
        // <=
        bool operator<=(const Month& rhs) const;
        bool operator<=(int rhs) const;
        bool operator<=(const std::string& rhs) const;
        // >=
        bool operator>=(const Month& rhs) const;
        bool operator>=(int rhs) const;
        bool operator>=(const std::string& rhs) const;


    private:
        std::string find_month_name(int month_number) const;
        int find_month_number(const std::string& month_name) const;

        int month_number;
        std::string month_name;
    };

    // Private helper functions
    void validate_date() const; // Throws errors
    void clamp_excess_days();

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