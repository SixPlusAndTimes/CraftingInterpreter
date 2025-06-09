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

class cpplox {
public:
    static bool hadError;
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
        
        // scan sourcefile
        Scanner scaner(fileContent);
        auto tokens = scaner.scanTokens();

        // parse tokens
        Parser parser((std::move(tokens)));
        std::shared_ptr<Expr> expression = parser.parse();

        if (hadError) return;

        auto printer = std::make_shared<AstPrinter>();
        std::cout << printer->print(expression) << std::endl;
        // for (const auto& toke : tokens) {
        //     std::cout << toke.toString()  << std::endl;
        // }
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
    static void report(int line, const std::string& where, std::string_view message) {
        std::cout << std::format("[line {}] Error {}: {}", line, where, message);
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
};

#endif