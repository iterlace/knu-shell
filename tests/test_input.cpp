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


TEST(InputTest, ParseTest) {
//    // test basic usage
//    ASSERT_EQ(Input::parse("echo \"Hello, World\""),
//              Command("echo", {"\"Hello, World\""}));
//
//    // test no arguments
//    ASSERT_EQ(Input::parse("argv"),
//              Command("argv", {}));
//
//    // test invalid command
//    ASSERT_THROW(Input::parse("1argv"), InvalidCommandError);
//
//    // test assign
//    ASSERT_EQ(Input::parse("A=1"),
//              Command("set", {"A", "1"}));
//
//    // test assign no value
//    ASSERT_EQ(Input::parse("A="),
//              Command("set", {"A", ""}));
//
//    // test assign multiple values
//    ASSERT_THROW(Input::parse("A=1 2"), InvalidCommandError);
}

TEST(InputTest, CommandEqualityTest) {
    Command c1{{
                       std::make_shared<CommandToken>("echo"),
                       std::make_shared<StringToken>(
                               StringToken({std::make_shared<TextToken>("a")})
                       )
               }};
    Command c2{{
                       std::make_shared<CommandToken>("echo"),
                       std::make_shared<StringToken>(
                               StringToken({std::make_shared<TextToken>("a")})
                       )
               }};
    Command c3{{
                       std::make_shared<CommandToken>("argc")
               }};
    Command c4{{
                       std::make_shared<VariableToken>("SOME_VARIABLE"),
                       std::make_shared<AssignmentToken>(),
                       std::make_shared<StringToken>(
                               StringToken({std::make_shared<TextToken>("a")})
                       )
               }};

    ASSERT_TRUE(c1 == c2);
    ASSERT_TRUE(c1 != c3);
    ASSERT_TRUE(c1 != c4);

}
