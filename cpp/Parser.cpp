#include <vector>
#include "cpplox.h"
#include "Parser.h"
#include "Stmt.h"
Parser::Parser(const std::vector<Token>& tokens) : 
    m_tokens(tokens),
    m_current(0)  { }

Parser::Parser(std::vector<Token>&& tokens) :
    m_tokens(std::move(tokens)),
    m_current(0)  { }

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd())
    {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::VAR})) {
            return varDeclaration();
        }
        return statement();
    }catch(ParseError& error) {
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    std::shared_ptr<Token> name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after decalarion.");
    LOG_DEBUG("Make a declare statement");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::PRINT})){
        return printStatement();
    }
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    LOG_DEBUG("Make a print statement");
    return std::make_shared<Print>(expr);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    LOG_DEBUG("Make a expression statement statement");
    return std::make_shared<Expression >(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = equality();

    if (match({TokenType::EQUAL})) {
        std::shared_ptr<Token> equals = previous();
        std::shared_ptr<Expr> value = assignment();
        if (dynamic_cast<Variable*>(expr.get()) != nullptr) {
            std::shared_ptr<Token> name = static_cast<Variable*>(expr.get())->m_name;
            return std::make_shared<Assign>(name, value);
        }

        error(equals, "Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, operatorToken, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, operatorToken, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, operatorToken, right);
    }

    return expr;
}


std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    
    while (match({TokenType::SLASH, TokenType::STAR})) {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, operatorToken, right);
    }

    return expr;

}
std::shared_ptr<Expr> Parser::unary() {
    // to handl !!  --  etc 
    if (match({TokenType::BANG, TokenType::MINUS})) {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = unary(); 
        return std::make_shared<Unary>(operatorToken, right);
    }

    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    // handeling literals 
    if (match({TokenType::FALSE})) return std::make_shared<Literal>(std::make_shared<Object>(false));
    if (match({TokenType::TRUE})) return std::make_shared<Literal>(std::make_shared<Object>(true));
    if (match({TokenType::NIL})) return std::make_shared<Literal>(std::make_shared<Object>(nullptr));
    if (match({TokenType::NUMBER, TokenType::STRING})) return std::make_shared<Literal>(std::make_shared<Object>(previous()->m_literal));  
    // handeling identifiers
    if (match({TokenType::IDENTIFIER})) return std::make_shared<Variable>(previous());

    // handling grouping
    if (match({TokenType::LEFT_PAREN})) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    /// TODO here
    throw error(peek(), "Expect expression.");
}

std::shared_ptr<Token> Parser::consume(TokenType tokenType, const std::string& errorMsg) {
    if (check(tokenType)) return advance();

    throw error(peek(), errorMsg);
}

ParseError Parser::error(std::shared_ptr<Token> token, const std::string& errorMsg) {
    cpplox::error(*token, errorMsg);
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
    return peek()->m_tokenType == type;
}

std::shared_ptr<Token> Parser::advance() {
    if (!isAtEnd()) ++m_current;
    return previous();
}

bool Parser::isAtEnd() {
    return peek()->m_tokenType == TokenType::EOF_TOKEN;
}

std::shared_ptr<Token> Parser::peek() {
    return std::make_shared<Token>(m_tokens.at(m_current));
}

std::shared_ptr<Token> Parser::previous() {
    return std::make_shared<Token>(m_tokens.at(m_current - 1));
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
      if (previous()->m_tokenType == TokenType::SEMICOLON) return;
      switch (peek()->m_tokenType) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
            return;
        default:
            break;
      }

      advance();
    }
}