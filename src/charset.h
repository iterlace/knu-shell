
#ifndef SHELL_CHARSET_H
#define SHELL_CHARSET_H

#include <stack>
#include <vector>
#include <optional>
#include <set>


class Charset {
public:
    Charset(const std::string &chars);
    Charset(const char *chars);
    Charset(const std::set<char> &chars);

    Charset operator+(const Charset &other) const;
    Charset operator-(const Charset &other) const;
    bool operator==(const Charset &other) const;
    operator std::string() const;

    const std::set<char> &get() const;
    bool contains(const char &c) const;

protected:
    std::set<char> charset;
};


static const Charset LETTERS{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
static const Charset DIGITS{"0123456789"};
static const Charset SYMBOLS{"|!#$%&()*+,-./:};>=<?@[\\]^_`{}~\"\'"};
static const Charset CHARS{LETTERS + DIGITS + SYMBOLS};
static const Charset SPACES{"\t\r "};
static const Charset LINE_END{"\n"};
static const Charset ANY_CHAR{""};


#endif //SHELL_CHARSET_H
