#include <iostream>
#include <string>
#include <cstdio>

#include "shell.h"

int main(int argc, char *argv[], char *envp[]) {
    Shell s = Shell(std::cin, std::cout, argv, envp);
    s.run();
    return 0;
}


