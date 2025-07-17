#ifndef STMT_H
#define STMT_H
#include <vector>
#include <any>
#include <memory>
#include"Token.h"
class Block;
class Expression;
class If;
class Print;
class While;
class Var;
class Expr;

class Stmt {
public:
	class Visitor {
	public:
		virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
		virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
		virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
		virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
		virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;
		virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
		virtual ~Visitor() {};
};
	virtual std::any accept(std::shared_ptr<Visitor> visitor) = 0;
};

class Block : public Stmt, public std::enable_shared_from_this<Block> {
public:
    Block(std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> statements) {
      this->m_statements = statements;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitBlockStmt(shared_from_this());
		}

    std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> m_statements;
};

class Expression : public Stmt, public std::enable_shared_from_this<Expression> {
public:
    Expression(std::shared_ptr<Expr> expression) {
      this->m_expression = expression;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitExpressionStmt(shared_from_this());
		}

    std::shared_ptr<Expr> m_expression;
};

class If : public Stmt, public std::enable_shared_from_this<If> {
public:
    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch) {
      this->m_condition = condition;
      this->m_thenBranch = thenBranch;
      this->m_elseBranch = elseBranch;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitIfStmt(shared_from_this());
		}

    std::shared_ptr<Expr> m_condition;
    std::shared_ptr<Stmt> m_thenBranch;
    std::shared_ptr<Stmt> m_elseBranch;
};

class Print : public Stmt, public std::enable_shared_from_this<Print> {
public:
    Print(std::shared_ptr<Expr> expression) {
      this->m_expression = expression;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitPrintStmt(shared_from_this());
		}

    std::shared_ptr<Expr> m_expression;
};

class While : public Stmt, public std::enable_shared_from_this<While> {
public:
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) {
      this->m_condition = condition;
      this->m_body = body;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitWhileStmt(shared_from_this());
		}

    std::shared_ptr<Expr> m_condition;
    std::shared_ptr<Stmt> m_body;
};

class Var : public Stmt, public std::enable_shared_from_this<Var> {
public:
    Var(std::shared_ptr<Token> name, std::shared_ptr<Expr> initializer) {
      this->m_name = name;
      this->m_initializer = initializer;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitVarStmt(shared_from_this());
		}

    std::shared_ptr<Token> m_name;
    std::shared_ptr<Expr> m_initializer;
};

#endif
