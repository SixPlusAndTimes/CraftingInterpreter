#include <vector>
#include "cpplox.h"
#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens) : 
    m_tokens(tokens),
    m_current(0)  { }

Parser::Parser(std::vector<Token>&& tokens) :
    m_tokens(std::move(tokens)),
    m_current(0)  { }

std::shared_ptr<Expr> Parser::parse() {
    try
    {
        return expression();
    }
    catch(ParseError& error)
    {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token operatorToken = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, std::shared_ptr<Token>(&operatorToken), right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token operatorToken = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, std::shared_ptr<Token>(&operatorToken), right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token operatorToken = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, std::shared_ptr<Token>(&operatorToken), right);
    }

    return expr;
}


std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token operatorToken = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, std::shared_ptr<Token>(&operatorToken), right);
    }

    return expr;

}
std::shared_ptr<Expr> Parser::unary() {
    // to handl !!  --  etc 
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token operatorToken = previous();
        std::shared_ptr<Expr> right = unary(); 
        return std::make_shared<Unary>(std::shared_ptr<Token>(&operatorToken), right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    // handeling literals 
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(std::make_shared<Object>(false));
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(std::make_shared<Object>(true));
    if (match({TokenType::NIL})) return std::make_shared<Literal>(std::make_shared<Object>(nullptr));
    if (match({TokenType::NUMBER, TokenType::STRING})) {
       return std::make_shared<Literal>(std::make_shared<Object>(previous().m_literal));  
    }

    // handling grouping
    if (match({TokenType::LEFT_PAREN})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    /// TODO here
    throw error(peek(), "Expect expression.");
}

Token Parser::consume(TokenType tokenType, const std::string& errorMsg) {
    if (check(tokenType)) return advance();

    throw error(peek(), errorMsg);
}

ParseError Parser::error(Token token, const std::string& errorMsg) {
    cpplox::error(token, errorMsg);
    return ParseError{errorMsg};
}

bool Parser::match(std::initializer_list<TokenType> tokentypes) {
    for (TokenType type : tokentypes) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().m_tokenType == type;
}

Token Parser::advance() {
    if (!isAtEnd()) ++m_current;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().m_tokenType == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return m_tokens.at(m_current);
}

Token Parser::previous() {
    return m_tokens.at(m_current - 1);
}

void Parser::synchronize() {
    // advance();

    // while (!isAtEnd()) {
    //   if (previous().m_tokenType == TokenType::SEMICOLON) return;

    //   switch (peek().m_tokenType) {
    //     case TokenType::CLASS:
    //     case TokenType::FUN:
    //     case TokenType::VAR:
    //     case TokenType::FOR:
    //     case TokenType::IF:
    //     case TokenType::WHILE:
    //     case TokenType::PRINT:
    //     case TokenType::RETURN:
    //       return;
    //   }

    //   advance();
    // }
}