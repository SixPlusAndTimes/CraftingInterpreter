#include "scanner.h"
Scanner::Scanner(const std::string& source):
    m_source(source),
    m_tokens(),
    m_start(0),
    m_current(0),
    m_line(1)
{
    
}

Scanner::~Scanner() {

}


bool Scanner::isAtEnd() {
    return m_current >= m_source.size();
}

void Scanner::scanToken() {

}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    m_tokens.emplace_back(TokenType::EOF_TOKEN, "", Literal(), m_line);
    return m_tokens;
}