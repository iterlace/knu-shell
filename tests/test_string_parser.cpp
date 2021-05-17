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


TEST(StringParserTest, BasicTest) {
    auto p = StringParser("Hello, ${world}!!!");
    StringToken s = p.run();
}


