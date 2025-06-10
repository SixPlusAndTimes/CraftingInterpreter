#include <any>
#include "Interpreter.h"

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return {"not implemented"};
}
std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return  evaluate(expr->m_expression);
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return expr->m_value;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return {"not implemented"};
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(shared_from_this());
}