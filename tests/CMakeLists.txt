
cmake_minimum_required(VERSION 3.17)
project(shell_tests)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest)
enable_testing()

include_directories(${gtest_SOURCE_DIR}/include)
link_libraries(shell_lib)

add_executable(shell_test main.cpp test_input.cpp test_shell.cpp)
target_link_libraries(shell_test gtest gtest_main)