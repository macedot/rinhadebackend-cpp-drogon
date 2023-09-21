#pragma once

#include <string>

#include <drogon/drogon.h>

void logInfo(std::string_view message) { std::cout << message << "\n"; }

void logException(std::string_view message) { std::cerr << "ERROR:" << message << "\n"; }

constexpr int MAX_VALID_YR = 9999;
constexpr int MIN_VALID_YR = 1800;

bool isLeap(int year) { return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); }

bool isDateValid(const std::string& date)
{
    std::istringstream is(date);
    int                y, m, d;
    char               delimiter;

    if (is >> y >> delimiter >> m >> delimiter >> d) {
        if (y > MAX_VALID_YR || y < MIN_VALID_YR) {
            return false;
        }
        if (m < 1 || m > 12) {
            return false;
        }
        if (d < 1 || d > 31) {
            return false;
        }
        if (m == 4 || m == 6 || m == 9 || m == 11) {
            return (d <= 30);
        }
        if (m == 2) {
            return (d <= (28 + isLeap(y)));
        }
        return true;
    }

    return false;
}

// function expects the string in format yyyy-mm-dd:
// bool extractDate(const std::string& s, int& y, int& m, int& d)
// {
//     std::istringstream is(s);
//     char               delimiter;
//     if (is >> y >> delimiter >> m >> delimiter >> d) {
//         struct tm t;
//         t.tm_sec    = 0;        /* Seconds.	[0-60] (1 leap second) */
//         t.tm_min    = 0;        /* Minutes.	[0-59] */
//         t.tm_hour   = 0;        /* Hours.	[0-23] */
//         t.tm_mday   = d;        /* Day.		[1-31] */
//         t.tm_mon    = m - 1;    /* Month.	[0-11] */
//         t.tm_year   = y - 1900; /* Year	- 1900.  */
//         t.tm_wday   = 0;        /* Day of week.	[0-6] */
//         t.tm_yday   = 0;        /* Days in year.[0-365]	*/
//         t.tm_isdst  = -1;       /* DST.		[-1/0/1]*/
//         t.tm_gmtoff = 0;        /* Seconds east of UTC.  */
//         t.tm_zone   = nullptr;  /* Timezone abbreviation.  */
//         // normalize:
//         time_t           when = mktime(&t);
//         const struct tm* norm = localtime(&when);
//         // the actual date would be:
//         // m = norm->tm_mon + 1;
//         // d = norm->tm_mday;
//         // y = norm->tm_year;
//         // e.g. 29/02/2013 would become 01/03/2013
//         // validate (is the normalized date still the same?):
//         return (norm->tm_mday == d && norm->tm_mon == m - 1 && norm->tm_year == y - 1900);
//     }
//     return false;
// }

// bool isDateValid2(const std::string& date)
// {
//     int y, m, d;
//     return extractDate(date, y, m, d);
// }

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   // static_cast<int(*)(int)>(std::tolower)         // wrong
                   // [](int c){ return std::tolower(c); }           // wrong
                   // [](char c){ return std::tolower(c); }          // wrong
                   [](unsigned char c) { return std::tolower(c); } // correct
    );
    return s;
}

auto splitValues(Json::Value& stack, const std::string& bufer)
{
    std::stringstream stackStream(bufer);
    std::string       substr;
    while (getline(stackStream, substr, ',')) {
        stack.append(substr);
    }
};