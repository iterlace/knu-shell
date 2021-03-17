#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <regex>

#define private public
#include "input.h"
#undef private


TEST(InputTest, StripTest) {
    ASSERT_EQ(Input::strip("   echo 1   "), "echo 1");
    ASSERT_EQ(Input::strip("   echo 1"), "echo 1");
    ASSERT_EQ(Input::strip("echo 1   "), "echo 1");
    ASSERT_EQ(Input::strip("echo 1"), "echo 1");
}


TEST(InputTest, ParseArgsTest) {
    typedef std::vector<std::string> result;

    ASSERT_EQ(Input::parseArgs("/var/log/a\\ b\\ c.txt"),
              result({"/var/log/a b c.txt"}));

    ASSERT_EQ(Input::parseArgs("/var/log/a\\\\ b\\\\ c.txt"),
              result({"/var/log/a\\ b\\ c.txt"}));

    ASSERT_EQ(Input::parseArgs("/var/log/a\\\\\\ b\\\\\\ c.txt"),
              result({"/var/log/a\\\\ b\\\\ c.txt"}));

    // test escape char at the beginning
    ASSERT_EQ(Input::parseArgs("\\ c.txt"),
              result({" c.txt"}));

    // test space inside double quotes
    ASSERT_EQ(Input::parseArgs("\"Hello, World!\""),
              result({"\"Hello, World!\""}));

    ASSERT_EQ(Input::parseArgs("main.cpp --log-level=DEBUG -v 1 -i \"/var/log/abc.txt\" -o /var/log/a\\\\ b\\\\ c.txt"),
              result({"main.cpp", "--log-level=DEBUG", "-v", "1", "-i", "\"/var/log/abc.txt\"",
                           "-o", "/var/log/a\\ b\\ c.txt"}));
}


TEST(InputTest, ParseTest) {
    // test basic usage
    ASSERT_EQ(Input::parse("echo \"Hello, World\""),
              Command("echo", {"\"Hello, World\""}));

    // test no arguments
    ASSERT_EQ(Input::parse("argv"),
              Command("argv", {}));

    // test invalid command
    ASSERT_THROW(Input::parse("1argv"), InvalidCommandError);

    // test assign
    ASSERT_EQ(Input::parse("A=1"),
              Command("set", {"A", "1"}));

    // test assign no value
    ASSERT_EQ(Input::parse("A="),
              Command("set", {"A", ""}));

    // test assign multiple values
    ASSERT_THROW(Input::parse("A=1 2"), InvalidCommandError);


}
