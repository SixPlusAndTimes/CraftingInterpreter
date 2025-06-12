#include <any>
#include <stdexcept>
#include "Interpreter.h"
#include "utils.h"


std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    Object left = evaluate(expr->m_left);
    Object right = evaluate(expr->m_right);

    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::GREATER:
            return std::get<double>(left) > std::get<double>(right);
        case TokenType::GREATER_EQUAL:
            return std::get<double>(left) >= std::get<double>(right);
        case TokenType::LESS:
            return std::get<double>(left) < std::get<double>(right);
        case TokenType::LESS_EQUAL:
            return std::get<double>(left) <= std::get<double>(right);
        case TokenType::MINUS:
           return std::get<double>(left) - std::get<double>(right);
        case TokenType::PLUS: 
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return std::get<double>(left) + std::get<double>(right);
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)){
                return std::get<std::string>(left) + std::get<std::string>(right);
            } else {
                LOG_ERROR("PLUS operator, but unsuppoted types");
                break;
            }
        case TokenType::SLASH:
            return std::get<double>(left) / std::get<double>(right);
        case TokenType::STAR:
            return std::get<double>(left) * std::get<double>(right);
        default:
            break;
    }

    return nullptr;
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return  evaluate(expr->m_expression);
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return expr->m_value;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    Object right = evaluate(expr->m_right);
    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::MINUS: {
            return -1 * std::get<double>(right); // handle exceptiion? or just let it burn...
        }
        case TokenType::BANG: {
            return !isTruthy(right);
        }
        default:
            break;
    }

    return nullptr;
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    return std::any_cast<Object>(expr->accept(shared_from_this()));
}

bool Interpreter::isTruthy(const Object& object) {
    if (const auto doublePtr = std::get_if<double>(&object)) {
        return true;
    } else if (const auto boolPtr = std::get_if<bool>(&object)) {
        return *boolPtr;
    } else if (const auto stringPtr = std::get_if<std::string>(&object)) {
        return true;
    } else {
        return false;
    }
}