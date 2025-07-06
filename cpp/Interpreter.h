#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <memory>
#include "Expr.h"
#include "Token.h"
#include "Stmt.h"
#include "Environment.h"
class Interpreter : public Expr::Visitor, public Stmt::Visitor, public std::enable_shared_from_this<Interpreter>{
public:
	Interpreter();
	std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
	std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
	std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
	std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
	std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
	std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
    Object evaluate(std::shared_ptr<Expr> expr);

	bool isTruthy(const Object& object);
	bool isEqual(const Object& left, const Object& right);
	void checkNumberOperand(std::shared_ptr<Token> operater, const Object& operand);
	void checkNumberOperands(std::shared_ptr<Token> operater, const Object& operandLeft, const Object& operandRight);

	void interpreter(const std::vector<std::shared_ptr<Stmt>>& statements);
	void execute(std::shared_ptr<Stmt> stmt);
	std::string stringfy(const Object&);

private:
	std::unique_ptr<Environment> m_environment;
};

#endif // INTERPRETER_H