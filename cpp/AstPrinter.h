#ifndef ASTPRINTER_H
#define ASTPRINTER_H
#include <string>
#include <memory>
#include "Expr.h"
#include "Stmt.h"
class AstPrinter : public Expr::Visitor, public Stmt::Visitor, public std::enable_shared_from_this<AstPrinter> {
public:
    std::string print(std::shared_ptr<Expr> expr);
	std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);
	std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
	std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
	std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
	std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
	std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
};

#endif // ASTPRINTER_H