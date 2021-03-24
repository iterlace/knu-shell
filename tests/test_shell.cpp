#include <gtest/gtest.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define private public
#define protected public
#include "shell.h"
#undef protected
#undef private



TEST(ShellTest, EchoSimpleVariableTest) {
    std::stringstream o;
    char **empty = new char*; empty[0] = "\0";
    Shell shell = Shell(o, o, empty, empty);

    shell.set({"path", "/etc"});
    shell.echo({"path = ${path}"});

    std::string s = o.str();
    s = s.substr(0, s.length()-2);  // skip "\n\0" in the end
    ASSERT_EQ(s, "path = /etc");
}

TEST(ShellTest, EchoEmptyVariableTest) {
    std::stringstream o;
    char **empty = new char*; empty[0] = "\0";
    Shell shell = Shell(o, o, empty, empty);

    shell.set({"A", ""});
    shell.echo({"A=${A}"});

    std::string s = o.str();
    s = s.substr(0, s.length()-2);  // skip "\n\0" in the end
    ASSERT_EQ(s, "A=");
}


TEST(FormatTreeTest, FormatTreeParseTest) {
    std::stringstream o;
    char **empty = new char*; empty[0] = "\0";
    Shell shell = Shell(o, o, empty, empty);

    shell.set({"username", "Vovan"});

    auto tree = FormatTree("You are ${username}!", &shell);
    ASSERT_EQ(tree.format(), "You are Vovan!");
}



