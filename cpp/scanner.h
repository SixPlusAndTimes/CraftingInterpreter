#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
#include <string_view>
#include <vector>
class Scanner
{
private:
    std::string_view m_source;
    std::vector<Token> m_tokens;
    int m_start; // point to first character in the lexeme being scanned
    size_t m_current; // point to character currently being considered
    int m_line; // tracks line number in source file
public:
    Scanner(const std::string& source);
    ~Scanner();
    std::vector<Token> scanTokens();
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType tokenType);
    void addToken(TokenType TokenType, Object literal);
    bool match(char expected);
    char peek();
    void extractStringLiteral();
    bool isDigit(char c);
    void number();
    char peekNext();
    bool isAlpha(char c);
    void identifier();
    bool isAlphaNumeric(char c);
};

#endif