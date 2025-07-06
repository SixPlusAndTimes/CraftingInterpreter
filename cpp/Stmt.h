#ifndef STMT_H
#define STMT_H
#include <vector>
#include <any>
#include <memory>
#include"Token.h"
class Expression;
class Print;
class Var;
class Expr;

class Stmt {
public:
	class Visitor {
	public:
		virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
		virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
		virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
		virtual ~Visitor() {};
};
	virtual std::any accept(std::shared_ptr<Visitor> visitor) = 0;
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
