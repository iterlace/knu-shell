#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <regex>

#define private public

#include "string_parser.h"

#undef private

using std::make_shared;


TEST(StringParserTest, BasicTest) {
    auto p = StringParser("Hello, ${username}!!!");
    StringToken s = p.run();

    StringToken expected{{
        make_shared<TextToken>("Hello, "),
        make_shared<LinkToken>("username"),
        make_shared<TextToken>("!!!"),
    }};

    ASSERT_TRUE(s == expected);
}


