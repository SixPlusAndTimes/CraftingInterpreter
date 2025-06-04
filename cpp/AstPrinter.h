#ifndef ASTPRINTER_H
#define ASTPRINTER_H
#include <string>
#include "Expr.h"
class AstPrinter : public Expr::Visitor {
public:
    std::string print(Expr::Expr* expr);
	std::string parenthesize(const std::string& name,  std::vector<Expr::Expr*>& exprs);
	std::any visitBinaryExpr(Expr::Binary* Expr) override;
	std::any visitGroupingExpr(Expr::Grouping* Expr) override;
	std::any visitLiteralExpr(Expr::Literal* Expr) override;
	std::any visitUnaryExpr(Expr::Unary* Expr) override;
	~AstPrinter() {}
};

#endif // ASTPRINTER_kH