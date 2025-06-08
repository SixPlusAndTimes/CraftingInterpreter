#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include "Expr.h"
#include "token.h"
class ParseError : public std::runtime_error {
public :
    ParseError(const std::string& errorMsg):std::runtime_error(errorMsg) {
    }
};

class Parser {
public:
    Parser(const std::vector<Token>&) ;
    Parser(std::vector<Token>&&) ;
    std::shared_ptr<Expr> parse();
private:
    std::vector<Token> m_tokens;
    // point to the next token that we wnat parse
    int m_current = 0;

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType>);
    // check current token is the given type
    bool check(TokenType);
    // comsumes the current token and return it 
    Token advance();
    bool isAtEnd();
    // return current token
    Token peek();
    // return previous token
    Token previous();
    // does the same thing as match() does, and throw exceptrion if not matched 
    Token consume(TokenType tokenType, const std::string& errorMsg);

    ParseError error(Token token, const std::string& errorMsg);

    // if parser encounter a error, use thsi functiong to process the error
    // and continue to parse the leat codes
    void synchronize();
};

#endif // PARSER_H