#include <unordered_map>
#include "Scanner.h"
#include "cpplox.h"
#include "utils.h"
const static std::unordered_map<std::string, TokenType> keywords {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"if", TokenType::IF},
    {"fun", TokenType::FUN},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
};

Scanner::Scanner(const std::string& source):
    m_source(source),
    m_tokens(),
    m_start(0),
    m_current(0),
    m_line(1)
{ }

Scanner::~Scanner() { }


bool Scanner::isAtEnd() {
    return m_current >= m_source.size();
}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.emplace_back(TokenType::EOF_TOKEN, "", Object(), m_line);
    return m_tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c)
    {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!': 
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=': 
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<': 
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>': 
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {
                while(peek() != '\n' && !isAtEnd()) advance();
            }else {
                addToken(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            m_line++;
            break;
        case '"' : extractStringLiteral(); break;
        default: 
            if (isDigit(c)) {
                number();
            }
            else if(isAlpha(c)){
                identifier();
            } else {
                cpplox::error(m_line, "Unexpected Character!");
            }
    }
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    std::string text(m_source.begin() + m_start, m_source.begin() + m_current);
    if (keywords.find(text) == keywords.end()) {
        addToken(TokenType::IDENTIFIER);
    }else {
        addToken(keywords.at(text));
    }
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

void Scanner::number() {
    while (isdigit(peek())) advance();

    // look for fraction part
    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(peek())) advance();
    }
    std::string valueStr(m_source.begin() + m_start, m_source.begin() + m_current);
    LOG_DEBUG("add number {}", stod(valueStr)) ;
    addToken(TokenType::NUMBER, Object(stod(valueStr)));
}

void Scanner::extractStringLiteral() {
    while (peek() != '"' && !isAtEnd()) {
        // support multiple line strings
        if (peek() == '\n') m_line++;
        advance();
    }

    if (isAtEnd()) {
        cpplox::error(m_line, "Undetermined  String");
        return;
    }
    advance();

    std::string value(m_source.begin() + m_start + 1, m_source.begin() + m_current - 1);
    addToken(TokenType::STRING, Object(value));
}

char Scanner::peekNext() {
    if (m_current + 1 >= m_source.size()) return '\0';
    return m_source.at(m_current + 1);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::advance() {
    return m_source.at(m_current++);
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return m_source.at(m_current);
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (m_source.at(m_current) != expected) return false;

    m_current++;
    return true;
}
void Scanner::addToken(TokenType tokenType) {
    addToken(tokenType, Object());
}

void Scanner::addToken(TokenType tokenType, Object literal) {
    std::string text(m_source.begin() + m_start, m_source.begin() + m_current);
    LOG_DEBUG("addToken lexme:{}, literaltostring:{}", text.c_str(), LoxLiteralTyeToString(literal));
    m_tokens.emplace_back(tokenType, text, literal, m_line);
}