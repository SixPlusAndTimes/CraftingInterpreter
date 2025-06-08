#include <any>
#include <iostream>
#include "AstPrinter.h"

std::string AstPrinter::parenthesize(const std::string& name, std::vector<std::shared_ptr<Expr>>& exprs) {
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

std::any AstPrinter::visitBinaryExpr(std::shared_ptr<Binary> expr){
    std::vector<std::shared_ptr<Expr>> exprs{expr->m_left, expr->m_right};
    return std::any_cast<std::string>(parenthesize(expr->m_operater->m_lexeme, exprs));
}

std::any AstPrinter::visitGroupingExpr(std::shared_ptr<Grouping> expr)  {
    std::vector<std::shared_ptr<Expr>> exprs{expr->m_expression};
    return std::any_cast<std::string>(parenthesize("group", exprs));
}

std::any AstPrinter::visitUnaryExpr(std::shared_ptr<Unary> expr)  {
    std::vector<std::shared_ptr<Expr>> exprs{expr->m_right};
    return std::any_cast<std::string>(parenthesize(expr->m_operater->m_lexeme, exprs));
}

std::any AstPrinter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
   return LoxLiteralTyeToString(*expr->m_value);
}