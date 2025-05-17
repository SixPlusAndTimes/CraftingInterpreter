#include <string>
#include <unordered_map>
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

    {TokenType::EOF_TOKEN, "EOF_TOKEN"},
};

template <typename LiteralType>
std::string LoxLiteralTyeToString(LiteralType literal) 
{
    if constexpr (std::is_same_v<LiteralType, std::string>) {
        return literal;
    } else if constexpr (std::is_integral_v<LiteralType>) {
        return std::to_string(literal);
    } else if constexpr (std::is_same_v<LiteralType, float> || std::is_same_v<LiteralType, double>)  {
        return std::to_string(literal);
    }
    else {
        static_assert(std::is_same_v<LiteralType, void>, "Unsupported type for to_string_custom.");
        return "";
    }
}
template <typename LiteralType>
class Token
{
private:
    TokenType   m_tokenType;
    std::string m_lexeme;
    LiteralType m_literal;
    int         m_line;
public:
    Token(TokenType type, const std::string& lexeme, LiteralType literal, int line):
    m_tokenType(type),
    m_lexeme(lexeme),
    m_literal(literal),
    m_line(line)
    {

    }
    Token();
    std::string toString() {
        // return TokenTypeToString.at(m_tokenType) ;
        return TokenTypeToString.at(m_tokenType) +  " " + m_lexeme + " " + LoxLiteralTypeToString<LiteralType>(m_literal);
    }
};
