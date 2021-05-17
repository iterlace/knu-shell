#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <regex>

#define private public
#include "input_parser.h"
#undef private


TEST(InputParserTest, StripTest) {
//    auto p = InputParser("echo \"Hello, world! Your name is ${USERNAME}!!!\"   ");
    auto p = InputParser("A=\"${B}\"");
    p.run();
}
