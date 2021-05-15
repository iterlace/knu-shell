#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define private public
#include "../src/shell.h"
#undef private


//TEST(ShellTest, EchoSimpleVariableTest) {
//    std::stringstream o;
//    Shell shell = Shell(o, o, {}, {});
//    shell.set({"A", "\"123\""});
//    shell.echo({"${A}"});
//    ASSERT_EQ(o.str(), "123");
//}
//
//TEST(ShellTest, EchoComplexVariableTest) {
//    std::stringstream o;
//    Shell shell = Shell(o, o, {}, {});
//    shell.set({"A", ""});
//    shell.echo({"${A}"});
//    ASSERT_EQ(o.str(), "123");
//}
