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
        LOG_DEBUG("decalration begin");
        if (match({TokenType::FUN})) {
            return function("function");
        }

        if (match({TokenType::VAR})) {
            return varDeclaration();
        }
        LOG_DEBUG("decalration end");
        return statement();
    }catch(ParseError& error) {
        LOG_ERROR("decalration ERROR");
        synchronize();
        return nullptr;
    }
}

std::shared_ptr<Stmt> Parser::function(std::string kind) {
    // get function name
    std::shared_ptr<Token> functionName = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after     " + kind + " name.");
    // parse function parameters
    std::shared_ptr<std::vector<std::shared_ptr<Token>>> parameterVecPtr = std::make_shared<std::vector<std::shared_ptr<Token>>>();
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameterVecPtr->size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameterVecPtr->push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        }while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    
    // parse function body 
    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> body = block(); 
    
    LOG_DEBUG("parse function declaration done, functionname:{} \n", functionName->m_lexeme);
    return std::make_shared<Function>(functionName, parameterVecPtr, body);
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

std::shared_ptr<Stmt> Parser::forStateMent() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    // parse  the initializer、condition、increment and body statement
    std::shared_ptr<Stmt> initializer = nullptr;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::VAR})) {
        initializer = varDeclaration(); 
    } else {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
      increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
    
    std::shared_ptr<Stmt> body = statement();
    
    // construct while statement
    if (increment) {
        auto vecPtr = std::vector<std::shared_ptr<Stmt>>{body, std::make_shared<Expression>(increment)};
        auto stmtVecPtr = std::make_shared<std::vector<std::shared_ptr<Stmt>>>(vecPtr);
        body = std::make_shared<Block>(stmtVecPtr);
    }

    if (condition == nullptr) {
        condition = std::make_shared<Literal>(std::make_shared<Object>(true));
    }
    body = std::make_shared<While>(condition, body);

    if (initializer != nullptr) {
        auto vecPtr = std::vector<std::shared_ptr<Stmt>>{initializer, body};
        auto stmtVecPtr = std::make_shared<std::vector<std::shared_ptr<Stmt>>>(vecPtr);
        body = std::make_shared<Block>(stmtVecPtr);
    }
    return body;
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::FOR})){
        return forStateMent();
    }

    if (match({TokenType::IF}))
    {
        return ifStatement();
    }
    
    if (match({TokenType::PRINT})){
        return printStatement();
    }

    if (match({TokenType::WHILE})) {
        return whileStatement();
    }

    if (match({TokenType::LEFT_BRACE})) {
        return std::make_shared<Block>(block()); 
    }
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();

    return std::make_shared<While>(condition, body);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition."); 

    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
      elseBranch = statement();
    }

    return std::make_shared<If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> Parser::block() {
    LOG_DEBUG("parse block begin");
    auto stmtVecPtr = std::make_shared<std::vector<std::shared_ptr<Stmt>>>();
    while (!isAtEnd() && !check(TokenType::RIGHT_BRACE)) {
        stmtVecPtr->push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    LOG_DEBUG("parse block end");
    return stmtVecPtr;
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    LOG_DEBUG("Make a print statement");
    return std::make_shared<Print>(expr);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    LOG_DEBUG("Make a expression statement statement");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = logicalOr();

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

std::shared_ptr<Expr> Parser::logicalOr() {
    std::shared_ptr<Expr> expr = logicalAnd(); 
    while (match({TokenType::OR}))
    {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Logical>(expr, operatorToken, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logicalAnd() {
    std::shared_ptr<Expr> expr = equality(); 
    while (match({TokenType::AND}))
    {
        std::shared_ptr<Token> operatorToken = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, operatorToken, right);
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
    return call();
}

std::shared_ptr<Expr> Parser::call() {

    std::shared_ptr<Expr> expr = primary();
    bool functinoCallParsed = false;
    while (true)
    {
        if (match({TokenType::LEFT_PAREN}))
        {
            functinoCallParsed = true;
            expr = finishCall(expr);
        }
        else 
        {
            break;
        }
    }
    
    if (functinoCallParsed)
    {
        LOG_DEBUG("parse function call done name");
    }

    return expr;
}


std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {
    auto argumentVecPtr = std::make_shared<std::vector<std::shared_ptr<Expr>>>();
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            // restrict function call arguments less than 255
            if (argumentVecPtr->size() >= 255){
                error(peek(), "Can't have more than 255 arguments.");
            }
            argumentVecPtr->push_back(expression());
        } while(match({TokenType::COMMA}));
    }
    std::shared_ptr<Token> paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_shared<Call>(callee, paren, argumentVecPtr);
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