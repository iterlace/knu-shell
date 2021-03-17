#include <iostream>
#include <string>
#include <cstdio>

#include "src/shell.h"

int main(int argc, char *argv[], char *envp[]) {
    Shell s = Shell(argv, envp);
    return s.run();
}


