#pragma once
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

inline std::string trim_quotes(const std::string& s){
    if (s.size() >= 2 && s.front()=='"' && s.back()=='"') return s.substr(1, s.size()-2);
    return s;
}
inline void strip_quotes_inplace(std::string& s){
    if (!s.empty() && s.front()=='"' ) s.erase(s.begin());
    if (!s.empty() && s.back()=='"' ) s.pop_back();
}
inline std::string remove_quotes(const std::string& s){
    std::string r; r.reserve(s.size());
    for(char c: s){ if(c!='"') r.push_back(c); }
    return r;
}
inline std::string normalize_code(const std::string& code){
    // remove leading zeros
    size_t i=0;
    while(i<code.size() && code[i]=='0') ++i;
    std::string out = code.substr(i);
    if(out.empty()) out = "0";
    return out;
}
inline std::string format_thousands(long long v){
    bool neg = v<0;
    unsigned long long n = neg? -v : v;
    std::string s = std::to_string(n);
    std::string out;
    int cnt=0;
    for(int i=(int)s.size()-1;i>=0;--i){
        out.push_back(s[i]);
        cnt++;
        if(cnt==3 && i>0){ out.push_back('.'); cnt=0; }
    }
    if(neg) out.push_back('-');
    std::reverse(out.begin(), out.end());
    return out;
}
inline std::string format_percent(double x){
    std::ostringstream oss;
    oss<<std::fixed<<std::setprecision(2)<<x;
    std::string s = oss.str();
    std::replace(s.begin(), s.end(), '.', ',');
    return s;
}