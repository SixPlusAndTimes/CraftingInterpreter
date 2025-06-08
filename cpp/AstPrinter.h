#ifndef ASTPRINTER_H
#define ASTPRINTER_H
#include <string>
#include <memory>
#include "Expr.h"
class AstPrinter : public Visitor, public std::enable_shared_from_this<AstPrinter> {
public:
    std::string print(std::shared_ptr<Expr> expr);
	std::string parenthesize(const std::string& name, std::vector<std::shared_ptr<Expr>>& exprs);
	std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
	~AstPrinter() {};
};

#endif // ASTPRINTER_H