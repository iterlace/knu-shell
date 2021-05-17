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


TEST(InputParserTest, AssignmentTest) {
    auto p = InputParser("A=\"${B}\"");
    Command c = p.run();
}


TEST(InputParserTest, EchoTest) {
    auto p = InputParser("echo \"Hello, world! Your name is ${USERNAME}!!!\"   ");
    Command c = p.run();
}


