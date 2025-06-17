#include <memory>
#include "RuntimeError.h"

RuntimeError::RuntimeError(std::shared_ptr<Token> token, const std::string& msg):std::runtime_error(msg), m_token(token) {
}