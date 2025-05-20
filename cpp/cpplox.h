#ifndef CPP_LOX_H
#define CPP_LOX_H
#include <string_view>
#include <string>
#include <iostream>
#include <format>
#include "utils.h"

class cpplox {
public:
    static bool hadError;
    static void  RunFile(std::string_view path) {
        std::string fileContent;
        if (ReadAllBytesFromFile(path, fileContent) == CLoxResult::SUCCESS){
            DumpBuffer(fileContent);
        }else {
            std::cerr << "ReadFileFailed!\n";
        }
        if (hadError) {
            exit(65);
        }
    }
    static void RunPromptLine(std::string_view line) {
        if (!line.empty()) std::cout << line << "\n";
        
        // Scanner scanner = new Scanner(source);
        // List<Token> tokens = scanner.scanTokens();

        // // For now, just print the tokens.
        // for (Token token : tokens) {
        //   System.out.println(token);
        // }
    }
    static void RunPrompt() {
        std::cout << "runPrompt\n";
        while (true) {
            if (std::cin.eof()) {
                std::cout << "\n";
                break;
            }
            std::cout << "> ";
            std::string line;
            getline(std::cin, line);
            RunPromptLine(line);
            hadError = false;
        }
    }
    static void report(int line, std::string_view where, std::string_view message) {
        std::cout << std::format("[line {}] Error {}: {}", line, where, message);
        hadError = true;
    }

    static void error(int line, std::string_view message) {
        report(line, "", message);
    }
};

#endif