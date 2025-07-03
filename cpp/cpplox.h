#ifndef CPP_LOX_H
#define CPP_LOX_H
#include <string_view>
#include <string>
#include <iostream>
#include <format>
#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "utils.h"
#include "Token.h"
#include "RuntimeError.h"
#include "Interpreter.h"

class cpplox {
public:
    static bool hadError;
    static bool hadRuntimeError;
    static std::shared_ptr<Interpreter> interpreterPtr;
    static void  RunFile(std::string_view path) {
        // read out sourcefile
        std::string fileContent;
        if (ReadAllBytesFromFile(path, fileContent) == CLoxResult::SUCCESS){
            DumpBuffer(fileContent);
        }else {
            std::cerr << "ReadFileFailed!\n";
        }

        if (hadError) {
            exit(65);
        }

        if (hadRuntimeError) {
            exit(70); 
        }
        // scan sourcefile
        Scanner scaner(fileContent);
        auto tokens = scaner.scanTokens();

        // parse tokens
        Parser parser((std::move(tokens)));
        std::vector<std::shared_ptr<Stmt>> expression = parser.parse();

        if (hadError) return;

        interpreterPtr->interpreter(expression);
        auto printer = std::make_shared<AstPrinter>();
        // std::cout << printer->print(expression) << std::endl;
        // for (const auto& toke : tokens) {
        //     std::cout << toke.toString()  << std::endl;
        // }
    }

    static void RunPromptLine(std::string_view line) {
        if (!line.empty()) std::cout << line << "\n";
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

    static void report(int line, const std::string& where, std::string_view message) {
        std::cout << std::format("[line {}] Error {}: {}", line, where, message) << std::endl;
        hadError = true;
    }

    static void error(int line, std::string_view message) {
        report(line, "", message);
    }

    static void error(Token token, const std::string& errorMsg) {
        if (token.m_tokenType == TokenType::EOF_TOKEN) {
            report(token.m_line, " at end", errorMsg);
        } else {
            report(token.m_line, std::format(" at '{}'", token.m_lexeme), errorMsg);
        }
    }

    static void runtimeError(RuntimeError& runtimeError) {
       std::cout << runtimeError.what(); 
       hadRuntimeError = true;
    }
};

#endif