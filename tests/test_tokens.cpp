#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>

#define private public

#include "../src/tokens.h"

#undef private


TEST(TokensTest, CommandEqualityTest) {
    CommandToken c1{"echo"};
    CommandToken c2{"echo"};

    ASSERT_TRUE(c1 == c2);
    ASSERT_FALSE(c1 != c2);
}


TEST(TokensTest, TextEqualityTest) {
    TextToken t1{"hello"};
    TextToken t2{"hello"};
    TextToken t3{"hi"};

    ASSERT_TRUE(t1 == t2);
    ASSERT_TRUE(t1 != t3);
}


TEST(TokensTest, LinkEqualityTest) {
    LinkToken l1{"hello"};
    LinkToken l2{"hello"};
    LinkToken l3{"hi"};

    ASSERT_TRUE(l1 == l2);
    ASSERT_TRUE(l1 != l3);
}


TEST(TokensTest, AssignmentEqualityTest) {
    AssignmentToken a1{};
    AssignmentToken a2{};

    ASSERT_TRUE(a1 == a2);
}


TEST(TokensTest, DifferentTypesEqualityTest) {
    CommandToken c1{"echo"};
    VariableToken v1{"echo"};
    VariableToken v2{"NAME"};
    AssignmentToken a1{};

    ASSERT_TRUE(c1 != v1);
    ASSERT_TRUE(c1 != v2);
    ASSERT_TRUE(c1 != a1);
}


TEST(TokensTest, DifferentTypesEqualityByPointerTest) {
    Token *c1 = new CommandToken("echo");
    Token *v1 = new VariableToken("echo");
    Token *v2 = new VariableToken("NAME");
    Token *a1 = new AssignmentToken();

    ASSERT_TRUE(*c1 != *v1);
    ASSERT_TRUE(*c1 != *v2);
    ASSERT_TRUE(*c1 != *a1);
}


TEST(TokensTest, StringEqualityTest) {
    StringToken s1{{
                           std::make_shared<TextToken>("Oh hi, "),
                           std::make_shared<LinkToken>("name"),
                   }};
    StringToken s2{{
                           std::make_shared<TextToken>("Oh hi, "),
                           std::make_shared<LinkToken>("name"),
                   }};
    StringToken s3{{
                           std::make_shared<TextToken>("You'll die in "),
                           std::make_shared<LinkToken>("year"),
                   }};

    ASSERT_TRUE(s1 == s2);
    ASSERT_TRUE(s1 != s3);

}

