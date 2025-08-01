#include <any>
#include <iostream>
#include "AstPrinter.h"

std::string AstPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::string res{"(" + name};
    for (auto& expr : exprs) {
        res += " ";
        res += std::any_cast<std::string>(expr->accept(shared_from_this()));
    }
    res += ")";
    return res;
}

std::string AstPrinter::print(std::shared_ptr<Expr> expr) {
    if (!expr) {
        std::cout << "expr is null!!!" << std::endl;
        return {"expr is null!!!"};
    }
    return std::any_cast<std::string>(expr->accept(shared_from_this()));
}

std::any AstPrinter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    return nullptr;
}
std::any AstPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr){
    return parenthesize(expr->m_operater->m_lexeme, {expr->m_left, expr->m_right});
}

std::any AstPrinter::visitGroupingExpr(std::shared_ptr<Grouping> expr)  {
    return parenthesize("group", {expr->m_expression});
}

std::any AstPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr)  {
    return parenthesize(expr->m_operater->m_lexeme, {expr->m_right});
}

std::any AstPrinter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
   return LoxLiteralTyeToString(*expr->m_value);
}

std::any AstPrinter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    // return parenthesize(";")
    return nullptr;
}

std::any AstPrinter::visitPrintStmt(std::shared_ptr<Print> stmt) {

    return nullptr;
}

std::any AstPrinter::visitVariableExpr(std::shared_ptr<Variable> expr) {

    return nullptr;
}

std::any AstPrinter::visitVarStmt(std::shared_ptr<Var> stmt) {

    return nullptr;
}

std::any AstPrinter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    return nullptr;
}

std::any AstPrinter::visitIfStmt(std::shared_ptr<If> stmt)
{
    return nullptr;
}

std::any AstPrinter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    return nullptr;
}

std::any AstPrinter::visitWhileStmt(std::shared_ptr<While> stmt) {
    return nullptr;
}