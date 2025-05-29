#include <any>
#include "AstPrinter.h"

std::string AstPrinter::parenthesize(const std::string& name, std::vector<Expr::Expr>& exprs) {
    std::string res{"(" + name};
    for (auto& expr : exprs) {
        res += " ";
        res += std::any_cast<std::string>(expr.accept(*this));
    }
    res += ")";
    return res;
}
std::string AstPrinter::print(Expr::Expr& expr) {
    return std::any_cast<std::string>(expr.accept(*this));
}

std::any AstPrinter::visitBinaryExpr(Expr::Binary& expr){
    std::vector<Expr::Expr> exprs{expr.left, expr.right};
    return std::any_cast<std::string>(parenthesize(expr.operater.m_lexeme, exprs));
}
 std::any AstPrinter::visitGroupingExpr(Expr::Grouping& expr)  {
    std::vector<Expr::Expr> exprs{expr.expression};
    return std::any_cast<std::string>(parenthesize("group", exprs));
}
 std::any AstPrinter::visitLiteralExpr(Expr::Literal& expr) {
    return LoxLiteralTyeToString(expr.value);
}
 std::any AstPrinter::visitUnaryExpr(Expr::Unary& expr)  {
    std::vector<Expr::Expr> exprs{expr.right};
    return std::any_cast<std::string>(parenthesize(expr.operater.m_lexeme, exprs));
}