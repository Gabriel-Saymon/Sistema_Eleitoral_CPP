#pragma once
#include <string>
#include <sstream>
#include <vector>

struct Date {
    int d{1}, m{1}, y{1970};
};

inline Date parseDate(const std::string& ddmmyyyy){
    Date dt;
    // expects dd/MM/yyyy
    if(ddmmyyyy.size() >= 10){
        dt.d = std::stoi(ddmmyyyy.substr(0,2));
        dt.m = std::stoi(ddmmyyyy.substr(3,2));
        dt.y = std::stoi(ddmmyyyy.substr(6,4));
    }
    return dt;
}

inline int ageOn(const Date& birth, const Date& ref){
    int age = ref.y - birth.y;
    if (ref.m < birth.m || (ref.m == birth.m && ref.d < birth.d)) age--;
    return age;
}