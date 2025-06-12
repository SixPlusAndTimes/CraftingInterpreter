#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "Expr.h"
#include "Token.h"
#include <memory>
class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter>{
public:
	std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    Object evaluate(std::shared_ptr<Expr> expr);

	bool isTruthy(const Object& object);
};

#endif // INTERPRETER_H