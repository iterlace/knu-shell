
#ifndef SHELL_UTIL_H
#define SHELL_UTIL_H

std::string format(const std::string& format, ...);
void sprint(std::ostream& out, const std::string& format, ...);

std::string cutOuterQuotes(const std::string& s);

#endif //SHELL_UTIL_H
