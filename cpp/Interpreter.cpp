#include <any>
#include <stdexcept>
#include "Interpreter.h"
#include "utils.h"
#include "RuntimeError.h"
#include "cpplox.h"

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    LOG_DEBUG("Interpreter visitBinaryExpr begin") ;
    Object left = evaluate(expr->m_left);
    Object right = evaluate(expr->m_right);

    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::GREATER:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) > std::get<double>(right));
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) >= std::get<double>(right));
        case TokenType::LESS:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) < std::get<double>(right));
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) <= std::get<double>(right));
        case TokenType::MINUS:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) - std::get<double>(right));
        case TokenType::BANG_EQUAL: return Object(!isEqual(left, right));
        case TokenType::EQUAL_EQUAL: return Object(isEqual(left, right));
        case TokenType::PLUS: 
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return Object(std::get<double>(left) + std::get<double>(right));
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)){
                return Object(std::get<std::string>(left) + std::get<std::string>(right));
            } else {
                LOG_ERROR("Operands must be two numbers or two strings.");
                throw new RuntimeError(expr->m_operater, "Operands must be two numbers or two strings.");
            }
        case TokenType::SLASH:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) / std::get<double>(right));
        case TokenType::STAR:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) * std::get<double>(right));
        default:
            break;
    }
    return nullptr;
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    LOG_DEBUG("Interpreter visitGroupingExpr begin") ;
    return  evaluate(expr->m_expression);
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    LOG_DEBUG("Interpreter visitLiteralExpr begin") ;
    return *(expr->m_value);
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    LOG_DEBUG("Interpreter visitUnaryExpr begin") ;
    Object right = evaluate(expr->m_right);
    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::MINUS: {
            checkNumberOperand(expr->m_operater, right);
            return Object(-1 * std::get<double>(right)); 
        }
        case TokenType::BANG: {
            return Object(!isTruthy(right));
        }
        default:
            break;
    }
    LOG_DEBUG("Interpreter visitUnaryExpr end") ;
    return nullptr;
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    LOG_DEBUG("Interpreter evaluate begin");
    // auto res = expr->accept(shared_from_this());
    return std::any_cast<Object>(expr->accept(shared_from_this()));
    // return Object();
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

bool Interpreter::isEqual(const Object& left, const Object& right) {

    return ObjectEquals(left, right);
}

void Interpreter::checkNumberOperand(std::shared_ptr<Token> operater, const Object& operand) {
    if (std::holds_alternative<double>(operand)) {
        return;
    }
    LOG_ERROR("Operand Must be a number.");
    throw new RuntimeError(operater, "Operand Must be a number.");
}

void Interpreter::checkNumberOperands(std::shared_ptr<Token> operater, const Object& operandLeft, const Object& operandRight) {

    if (std::holds_alternative<double>(operandLeft) && std::holds_alternative<double>(operandRight)) {
        return;
    }
    LOG_ERROR("Operands Must be a number.");
    throw new RuntimeError(operater, "Operands Must be a number.");
}


std::string Interpreter::stringfy(const Object& object) {
    if (std::holds_alternative<nullptr_t>(object)) {
        return "nil";
    }else if (std::holds_alternative<double>(object)) {
        return std::to_string(static_cast<int>(std::get<double>(object)));
    }else if (std::holds_alternative<bool>(object)) {
        bool boolean = std::get<bool>(object);
        return boolean ? "true" : "false";
    }else if (std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }
    return std::string{"UnKnownType!"};
}

void Interpreter::interpreter(const std::vector<std::shared_ptr<Stmt>>& statements) {
    LOG_DEBUG("Interpreter begin");
    try {
        for (auto stmt : statements)
        {
            execute(stmt);
        }
    }catch (RuntimeError& error) {
        cpplox::runtimeError(error);
    }
    LOG_DEBUG("Interpreter end test");
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    evaluate(stmt->m_expression);
    return nullptr;
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    std::any ret = evaluate(stmt->m_expression);
    std::cout << stringfy(std::any_cast<Object>(ret)) << std::endl;
    return nullptr;
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}