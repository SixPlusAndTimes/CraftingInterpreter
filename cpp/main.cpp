#include <iostream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <fstream>
#include <format>
#include "token.h"
#include "utils.h"
#include "cpplox.h"
// static bool hadError = false;
// static CLoxResult RunFile(std::string_view path) {
//     CLoxResult res{CLoxResult::SUCCESS};
//     std::string fileContent;
//     if (ReadAllBytesFromFile(path, fileContent) == CLoxResult::SUCCESS){
//         DumpBuffer(fileContent);
//     }else {
//         std::cerr << "ReadFileFailed!\n";
//     }
//     if (hadError) {
//         exit(65);
//     }
//     return res;
// }

// static void report(int line, std::string_view where, std::string_view message) {
//     std::cout << std::format("[line {}] Error {}: {}", line, where, message);
//     hadError = true;
// }

// static void error(int line, std::string_view message) {
//     report(line, "", message);
// }

// CLoxResult RunPromptLine(std::string_view line) {
//     if (!line.empty()) std::cout << line << "\n";
    
//     // Scanner scanner = new Scanner(source);
//     // List<Token> tokens = scanner.scanTokens();

//     // // For now, just print the tokens.
//     // for (Token token : tokens) {
//     //   System.out.println(token);
//     // }
//     return CLoxResult::SUCCESS;
// }
// void RunPrompt() {
//     std::cout << "runPrompt\n";
//     while (true) {
//         if (std::cin.eof()) {
//             std::cout << "\n";
//             break;
//         }
//         std::cout << "> ";
//         std::string line;
//         getline(std::cin, line);
//         RunPromptLine(line);
//         hadError = false;
//     }
// }

int main(int argc, char* argv[]) {
    if (argc > 3) {
        printf("Usage: clox [script]\n");
    } else if (argc == 2) {
        cpplox::RunFile(argv[1]);
    } else {
       cpplox::RunPrompt(); 
    }
}