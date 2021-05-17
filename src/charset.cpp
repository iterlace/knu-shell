
#include "charset.h"


Charset::Charset(const std::string &chars) {
    for (const char &c : chars) {
        charset.insert(c);
    }
}


Charset::Charset(const char *chars) {
    for (const char *i = chars; *i != '\0'; i++) {
        charset.insert(*i);
    }
}


Charset::Charset(const std::set<char> &chars) {
    charset = chars;
}

const std::set<char> &Charset::get() const {
    return charset;
}

bool Charset::contains(const char &c) const {
    return charset.contains(c);
}


Charset Charset::operator+(const Charset &other) const {
    std::set<char> merged;
    std::merge(charset.begin(), charset.end(),
               other.get().begin(), other.get().end(),
               std::inserter(merged, merged.begin()));
    return Charset(merged);
}

Charset Charset::operator-(const Charset &other) const {
    std::set<char> differentiated;
    std::set_difference(charset.begin(), charset.end(),
                        other.get().begin(), other.get().end(),
                        std::inserter(differentiated, differentiated.begin()));
    return Charset(differentiated);
}

bool Charset::operator==(const Charset &other) const {
    return std::equal(charset.begin(), charset.end(),
                      other.get().begin(), other.get().end());
}

Charset::operator std::string() const {
    return std::string(charset.begin(), charset.end());
}


