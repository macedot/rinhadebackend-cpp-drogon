#pragma once

#include <string>

#include <drogon/drogon.h>

void logInfo(std::string_view message) { std::cout << message << "\n"; }

void logException(std::string_view message) { std::cerr << "error:" << message << "\n"; }

// function expects the string in format yyyy-mm-dd:
bool extractDate(const std::string& s, int& y, int& m, int& d)
{
    std::istringstream is(s);
    char               delimiter;

    if (is >> y >> delimiter >> m >> delimiter >> d) {
        struct tm t = {0};
        t.tm_mday   = d;
        t.tm_mon    = m - 1;
        t.tm_year   = y - 1900;
        t.tm_isdst  = -1;

        // normalize:
        time_t           when = mktime(&t);
        const struct tm* norm = localtime(&when);
        // the actual date would be:
        // m = norm->tm_mon + 1;
        // d = norm->tm_mday;
        // y = norm->tm_year;
        // e.g. 29/02/2013 would become 01/03/2013

        // validate (is the normalized date still the same?):
        return (norm->tm_mday == d && norm->tm_mon == m - 1 && norm->tm_year == y - 1900);
    }

    return false;
}

bool isDateValid(const std::string& date)
{
    if (date.size() != 10) {
        return false;
    }

    int y, m, d;
    return extractDate(date, y, m, d);
}

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