#ifndef ASTPRINTER_H
#define ASTPRINTER_H
#include <string>
#include <memory>
#include "Expr.h"
class AstPrinter : public Expr::Visitor, public std::enable_shared_from_this<AstPrinter> {
public:
    std::string print(std::shared_ptr<Expr::Expr> expr);
	std::string parenthesize(const std::string& name, std::vector<std::shared_ptr<Expr::Expr>>& exprs);
	std::any visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Expr::Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override;
	~AstPrinter() {};
};

#endif // ASTPRINTER_H