#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include "Expr.h"
#include "Token.h"
#include "Stmt.h"
class ParseError : public std::logic_error {
public :
    ParseError(const std::string& errorMsg):std::logic_error(errorMsg) {
    }
};

class Parser {
public:
    Parser(const std::vector<Token>&) ;
    Parser(std::vector<Token>&&) ;
    std::vector<std::shared_ptr<Stmt>>      parse();
private:
    std::vector<Token> m_tokens;
    // point to the next token that we wnat parse
    int m_current = 0;

    std::shared_ptr<Expr>                   assignment();
    std::shared_ptr<Expr>                   expression();
    std::shared_ptr<Expr>                   equality();
    std::shared_ptr<Expr>                   comparison();
    std::shared_ptr<Expr>                   term();
    std::shared_ptr<Expr>                   factor();
    std::shared_ptr<Expr>                   unary();
    std::shared_ptr<Expr>                   primary();
    std::shared_ptr<Stmt>                   statement();
    std::shared_ptr<Stmt>                   declaration();
    std::shared_ptr<Stmt>                   varDeclaration();
    std::shared_ptr<Stmt>                   printStatement();
    std::shared_ptr<Stmt>                   expressionStatement();
    std::shared_ptr<std::vector<std::shared_ptr<Stmt>>>      block();
    bool                                    match(std::initializer_list<TokenType>);
    // check current token is the given type
    bool                                    check(TokenType);
    // comsumes the current token and return it 
    std::shared_ptr<Token>                  advance();
    bool isAtEnd();
    // return current token
    std::shared_ptr<Token>                  peek();
    // return previous token
    std::shared_ptr<Token>                  previous();
    // do the same thing as match() do, and throw exceptrion if not matched 
    std::shared_ptr<Token>                  consume(TokenType tokenType, const std::string& errorMsg);

    ParseError                              error(std::shared_ptr<Token> token, const std::string& errorMsg);

    // if parser encounter a error, use this function to process the error
    // and continue to parse the remianing codes
    void                                    synchronize();
};

#endif // PARSER_H