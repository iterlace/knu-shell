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

using std::make_shared;


TEST(InputParserTest, EchoTest) {
    auto p = InputParser("echo \"Hello, world! Your name is ${USERNAME}!!!\"   ");
    Command c = p.run();
    Command expected{
        {
            make_shared<CommandToken>("echo"),
            make_shared<StringToken>(
                StringToken({
                    make_shared<TextToken>("Hello, world! Your name is "),
                    make_shared<LinkToken>("USERNAME"),
                    make_shared<TextToken>("!!!"),
                })
            )
        }
    };

    ASSERT_TRUE(c == expected);
}


TEST(InputParserTest, SimpleCommandTest) {
    auto p = InputParser("echo");
    Command c = p.run();
    Command expected{
        {
            make_shared<CommandToken>("echo")
        }
    };

    ASSERT_TRUE(c == expected);
}


TEST(InputParserTest, AssignmentTextTest) {
    auto p = InputParser("A=\"1\"");
    Command c = p.run();
    Command expected{
        {
            make_shared<VariableToken>("A"),
            make_shared<AssignmentToken>(),
            make_shared<StringToken>(StringToken({
                make_shared<TextToken>("1")
            }))
        }
    };

    ASSERT_TRUE(c == expected);
}


TEST(InputParserTest, AssignmentLinkTest) {
    auto p = InputParser("A=\"${B}\"");
    Command c = p.run();
    Command expected{
        {
            make_shared<VariableToken>("A"),
            make_shared<AssignmentToken>(),
            make_shared<StringToken>(StringToken({
                make_shared<LinkToken>("B")
            }))
        }
    };

    ASSERT_TRUE(c == expected);
}


TEST(InputParserTest, AssignmentMixedTest) {
    auto p = InputParser("A=\"b=${B}!\"");
    Command c = p.run();
    Command expected{
        {
            make_shared<VariableToken>("A"),
            make_shared<AssignmentToken>(),
            make_shared<StringToken>(StringToken({
                make_shared<TextToken>("b="),
                make_shared<LinkToken>("B"),
                make_shared<TextToken>("!"),
            }))
        }
    };

    ASSERT_TRUE(c == expected);
}



