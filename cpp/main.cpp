#include <iostream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <fstream>
#include <format>
#include "token.h"
#include "utils.h"
#include "cpplox.h"

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("Usage: clox [script]\n");
    } else if (argc == 2) {
        cpplox::RunFile(argv[1]);
    } else {
       cpplox::RunPrompt(); 
    }
}