#include <iostream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <fstream>
#include <format>
#include <memory>
#include "Expr.h"
#include "Token.h"
#include "utils.h"
#include "cpplox.h"
void testprinter() {
    // Expr* expression = new Binary(
    //         new Unary(new Token(TokenType::MINUS, "-", nullptr, 1), new Literal(new Object(123.0))),
    //         new Token(TokenType::STAR, "*", nullptr, 1),
    //         new Grouping(new Literal(new Object(45.67))));

    // auto unary = std::make_shared<Unary>(std::make_shared<Token>(TokenType::MINUS, "-", nullptr, 1), std::make_shared<Literal>(std::make_shared<Object>(123.0)));
    // auto token = std::make_shared<Token>(TokenType::STAR, "*", nullptr, 1);
    // auto grouping = std::make_shared<Grouping>(std::make_shared<Literal>(std::make_shared<Object>(55.67)));
    // auto expression = std::make_shared<Binary>(unary, token, grouping);
    
    // auto printer = std::make_shared<AstPrinter>();
    // std::cout << printer->print(expression) << std::endl;
}
int main(int argc, char* argv[]) {
    setLogLevel(readLogLevelFromConfig("./clox.conf"));
    if (argc > 3) {
        printf("Usage: clox [script]\n");
    } else if (argc == 2) {
        cpplox::RunFile(argv[1]);
    } else {
       cpplox::RunPrompt(); 
    }
}