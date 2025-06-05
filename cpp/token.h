#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>
#include <variant>
#include <format>
using Object = std::variant<nullptr_t, double, bool, std::string>;
enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EOF_TOKEN,
};
const std::unordered_map<TokenType, std::string> TokenTypeToString = {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},

    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},

    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},

    {TokenType::AND, "AND"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::FUN, "FUN"},
    {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"},
    {TokenType::NIL, "NIL"},
    {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::SUPER, "SUPER"},
    {TokenType::THIS, "THIS"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::VAR, "VAR"},
    {TokenType::WHILE, "WHILE"},

    {TokenType::EOF_TOKEN, "EOF"},
};

// class Literal{
// public:
//     std::variant<nullptr_t, double, bool, std::string> m_literalVal;
//     explicit Literal(std::variant<nullptr_t, double, bool, std::string> literalVal) {
//         m_literalVal = literalVal;
//     }
//     Literal() { }
// };

static std::string LoxLiteralTyeToString(const Object& literal) 
{
    // std::cout << "literal.type : " << 
    if (const auto doublePtr = std::get_if<double>(&literal)) {
        return std::to_string(*doublePtr);
    } else if (const auto boolPtr = std::get_if<bool>(&literal)) {
        return std::to_string(*boolPtr);
    } else if (const auto stringPtr = std::get_if<std::string>(&literal)) {
        return *stringPtr;
    }
    return {"null"};
}

class Token
{
public:
    TokenType   m_tokenType;
    std::string m_lexeme;
    Object m_literal;
    int         m_line;
    Token(TokenType type, const std::string& lexeme, Object literal, int line):
    m_tokenType(type),
    m_lexeme(lexeme),
    m_literal(literal),
    m_line(line)
    {

    }
    Token();
    std::string toString() const{
        // return TokenTypeToString.at(m_tokenType) ;
        return TokenTypeToString.at(m_tokenType) +  " " 
                + m_lexeme + " " 
                + LoxLiteralTyeToString(m_literal); 
    }
};

#endif