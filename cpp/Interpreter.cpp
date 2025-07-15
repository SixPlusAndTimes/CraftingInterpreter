#include <any>
#include <stdexcept>
#include "Interpreter.h"
#include "utils.h"
#include "RuntimeError.h"
#include "cpplox.h"
Interpreter::Interpreter()
:m_environment(std::make_unique<Environment>())
{}

void Interpreter::interpreter(const std::vector<std::shared_ptr<Stmt>>& statements) {
    LOG_DEBUG("Interpreter begin, statements.size() = {}", statements.size());
    try {
        for (auto stmt : statements)
        {
            execute(stmt);
        }
    }catch (RuntimeError& error) {
        LOG_DEBUG("catch runtime error");
        cpplox::runtimeError(error);
    }
    LOG_DEBUG("Interpreter end");
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    Object value = evaluate(expr->m_left);   
    // short circuit here
    if (expr->m_operater->m_tokenType == TokenType::OR) {
        if (isTruthy(value)) {
            return value;
        }
    }else {
        if (!isTruthy(value)) {
            return value;
        }
    }
    return evaluate(expr->m_right);
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    Object value = evaluate(expr->m_value);
    m_environment->assign(*expr->m_name, value);
    LOG_DEBUG("visit Assign expr, name[{}], value[{}]", (expr->m_name)->m_lexeme.c_str(), LoxLiteralTyeToString(value));
    return value;
}

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
                throw new RuntimeError(*expr->m_operater, "Operands must be two numbers or two strings.");
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

bool Interpreter::isEqual(const Object& left, const Object& right) {

    return ObjectEquals(left, right);
}

void Interpreter::checkNumberOperand(std::shared_ptr<Token> operater, const Object& operand) {
    if (std::holds_alternative<double>(operand)) {
        return;
    }
    LOG_ERROR("Operand Must be a number.");
    throw new RuntimeError(*operater, "Operand Must be a number.");
}

void Interpreter::checkNumberOperands(std::shared_ptr<Token> operater, const Object& operandLeft, const Object& operandRight) {

    if (std::holds_alternative<double>(operandLeft) && std::holds_alternative<double>(operandRight)) {
        return;
    }
    LOG_ERROR("Operands Must be a number.");
    throw new RuntimeError(*operater, "Operands Must be a number.");
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    LOG_DEBUG("visit expression statement begin");
    evaluate(stmt->m_expression);
    LOG_DEBUG("visit expression statement end");
    return nullptr;
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    LOG_DEBUG("visit print statement begin");
    Object ret = evaluate(stmt->m_expression);
    std::cout << stringfy(ret) << std::endl;
    LOG_DEBUG("visit print statement end");
    return nullptr;
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {

    return m_environment->get(*expr->m_name);
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
    LOG_DEBUG("visit varstatement begin");
    Object value = nullptr;
    if (stmt->m_initializer != nullptr) {
        value = evaluate(stmt->m_initializer);
    }
    m_environment->define(stmt->m_name->m_lexeme, value);
    LOG_DEBUG("visit varstatement end");
    return nullptr;
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    executeBlock(stmt->m_statements, std::make_unique<Environment>(this->m_environment.get())) ;
    return nullptr;
}

std::any Interpreter::visitIfStmt(std::shared_ptr<If> stmt) {
    if (isTruthy(evaluate(stmt->m_condition))) {
        execute(stmt->m_thenBranch) ;
    } else if (stmt->m_elseBranch) {
        execute(stmt->m_elseBranch);
    }
    return nullptr;
}

std::any Interpreter::visitWhileStmt(std::shared_ptr<While> stmt) {

    while (isTruthy(evaluate(stmt->m_condition))) {
        execute(stmt->m_body);
    }

    return nullptr;
}

void Interpreter::executeBlock(std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> stmtVecPtr, std::unique_ptr<Environment> environmentChild) {
    std::unique_ptr<Environment> parentEnv = std::move(this->m_environment);
    try
    {
        this->m_environment = std::move(environmentChild);
        for (auto stmt : *stmtVecPtr) {
            execute(stmt);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    this->m_environment = std::move(parentEnv);
}
// util functions , maybe in utils.h
std::string Interpreter::stringfy(const Object& object) {
    if (std::holds_alternative<nullptr_t>(object)) {
        return "nil";
    }else if (std::holds_alternative<double>(object)) {
        auto ret = std::format("{:.3f}", std::get<double>(object));
        // // 如果以 ".000" 结尾，说明是整数，去掉小数点及后面的部分
        if (ret.size() >= 4 && ret.substr(ret.size() - 4) == ".000") 
        {
            ret = ret.substr(0, ret.size() - 4);
        }
        return ret; 
    }else if (std::holds_alternative<bool>(object)) {
        bool boolean = std::get<bool>(object);
        return boolean ? "true" : "false";
    }else if (std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }
    return std::string{"UnKnownType!"};
}