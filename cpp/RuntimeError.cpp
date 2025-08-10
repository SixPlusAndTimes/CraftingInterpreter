#include <memory>
#include "RuntimeError.h"

RuntimeError::RuntimeError(const Token& token, const std::string& msg) 
: std::runtime_error("RuntimeError: " + msg)
, m_token(token)
{

}

ReturnException::ReturnException(Object value) 
: std::runtime_error("Return")
, m_value(value)
{

}