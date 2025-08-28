#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cstdint>
#include <ctime>

namespace Utils { 
    // Função de conversão para UTF-8, idêntica à do professor.
    inline std::string iso_8859_1_to_utf8(const std::string& str) {
        std::string strOut;
        strOut.reserve(str.length());
        for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
            uint8_t ch = *it;
            if (ch < 0x80) {
                strOut.push_back(ch);
            } else {
                strOut.push_back(0b11000000 | (ch >> 6));
                strOut.push_back(0b10000000 | (ch & 0b00111111));
            }
        }
        return strOut;
    }

    class Date {
        int day, month, year;
    public:
        Date(int d, int m, int y) : day(d), month(m), year(y) {}
        Date() : day(0), month(0), year(0) {}

        static Date fromString(const std::string& dateStr) {
            std::tm tm = {};
            std::stringstream ss(dateStr);
            ss >> std::get_time(&tm, "%d/%m/%Y");
            return Date(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        }

        int getYear() const { return year; }
        int getMonth() const { return month; }
        int getDay() const { return day; }

        bool operator<(const Date& other) const {
            if (year != other.year) return year < other.year;
            if (month != other.month) return month < other.month;
            return day < other.day;
        }
    };

    inline int calculateAge(const Date& birthDate, const Date& electionDate) {
        int age = electionDate.getYear() - birthDate.getYear();
        if (electionDate.getMonth() < birthDate.getMonth() ||
            (electionDate.getMonth() == birthDate.getMonth() && electionDate.getDay() < birthDate.getDay())) {
            age--;
        }
        return age;
    }

    inline std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
            tokens.push_back(token);
        }
        return tokens;
    }
}

#endif