
#ifndef SHELL_UTIL_H
#define SHELL_UTIL_H

#include <string>


std::string format(const std::string& format, ...);
void sprint(std::ostream& out, const std::string& format, ...);

/**
 * https://stackoverflow.com/a/50067142/9158268
 */
#define CLASS std::remove_reference<decltype(classMacroImpl(this))>::type
template<class T> T& classMacroImpl(const T* t);

#endif //SHELL_UTIL_H
