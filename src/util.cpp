#include <string>
#include <vector>
#include <cstdarg>

#include "util.h"


std::string format(const std::string& format, ...) {
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end(args);
    std::vector<char> vec(len + 1);
    va_start(args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end(args);
    return &vec[0];
}

void sprint(std::ostream& out, const std::string& format, ...) {
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end(args);
    std::vector<char> vec(len + 1);
    va_start(args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end(args);
    out << std::string(vec.begin(), vec.end());
}

std::string cutOuterQuotes(const std::string& s) {
    if (s.length() >= 2 && s[0] == '"' && s[s.length()-1] == '"' && s[s.length()-2] != '\\') {
        return s.substr(1, s.length()-2);
    } else {
        return s;
    }
}
