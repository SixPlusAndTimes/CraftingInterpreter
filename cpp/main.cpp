#include <iostream>
#include <stdio.h>
#include <string>
#include <string_view>
#include <fstream>
#include <format>
#include <memory>
#include "AstPrinter.h"
#include "Expr.h"
#include "token.h"
#include "utils.h"
#include "cpplox.h"
void testprinter() {
    // Expr::Expr* expression = new Expr::Binary(
    //         new Expr::Unary(new Token(TokenType::MINUS, "-", nullptr, 1), new Expr::Literal(new Object(123.0))),
    //         new Token(TokenType::STAR, "*", nullptr, 1),
    //         new Expr::Grouping(new Expr::Literal(new Object(45.67))));

    auto unary = std::make_shared<Expr::Unary>(std::make_shared<Token>(TokenType::MINUS, "-", nullptr, 1), std::make_shared<Expr::Literal>(std::make_shared<Object>(123.0)));
    auto token = std::make_shared<Token>(TokenType::STAR, "*", nullptr, 1);
    auto grouping = std::make_shared<Expr::Grouping>(std::make_shared<Expr::Literal>(std::make_shared<Object>(55.67)));
    auto expression = std::make_shared<Expr::Binary>(unary, token, grouping);
    
    auto printer = std::make_shared<AstPrinter>();
    std::cout << printer->print(expression) << std::endl;
}
int main(int argc, char* argv[]) {
    testprinter();

    // if (argc > 3) {
    //     printf("Usage: clox [script]\n");
    // } else if (argc == 2) {
    //     cpplox::RunFile(argv[1]);
    // } else {
    //    cpplox::RunPrompt(); 
    // }
}