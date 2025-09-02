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
	std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
	std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
	std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
	std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
	std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
	std::any visitCallExpr(std::shared_ptr<Call> expr) override;
	std::any visitGetExpr(std::shared_ptr<Get> expr) override;
	std::any visitSetExpr(std::shared_ptr<Set> expr) override;
	std::any visitThisExpr(std::shared_ptr<This> expr) override;
	std::any visitSuperExpr(std::shared_ptr<Super> expr) override;
	std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
	std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
	std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
	std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
	std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
	std::any visitIfStmt(std::shared_ptr<If> stmt) override;
	std::any visitWhileStmt(std::shared_ptr<While> stmt) override;
	std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override;
	std::any visitReturnStmt(std::shared_ptr<Return> stmt) override;
	std::any visitClassStmt(std::shared_ptr<Class> stmt) override;

    Object evaluate(std::shared_ptr<Expr> expr);

	bool isTruthy(const Object& object);
	bool isEqual(const Object& left, const Object& right);
	void checkNumberOperand(std::shared_ptr<Token> operater, const Object& operand);
	void checkNumberOperands(std::shared_ptr<Token> operater, const Object& operandLeft, const Object& operandRight);

	void interpreter(const std::vector<std::shared_ptr<Stmt>>& statements);
	void execute(std::shared_ptr<Stmt> stmt);
	void resolve(std::shared_ptr<Expr>, int depth);
	void executeBlock(std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> stmtVecPtr, std::shared_ptr<Environment> environmentChild);
	Object lookUpVariable(std::shared_ptr<Token>, std::shared_ptr<Expr> expr);

	std::string stringfy(const Object&);

	std::shared_ptr<Environment> 	m_globalEnvironment;
private:
	std::shared_ptr<Environment>    m_environment;
	std::unordered_map<Expr*, int> 	m_locals;
};

#endif // INTERPRETER_H