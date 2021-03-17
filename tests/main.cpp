#include "gtest/gtest.h"

int main(int argc, char **argv) {
    printf("tests/main.cpp");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}