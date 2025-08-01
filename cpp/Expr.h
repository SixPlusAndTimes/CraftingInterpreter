#ifndef EXPR_H
#define EXPR_H
#include <vector>
#include <any>
#include <memory>
#include"Token.h"
class Assign;
class Binary;
class Grouping;
class Literal;
class Logical;
class Unary;
class Variable;

class Expr {
public:
	class Visitor {
	public:
		virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
		virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
		virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
		virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
		virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
		virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
		virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
		virtual ~Visitor() {};
};
	virtual std::any accept(std::shared_ptr<Visitor> visitor) = 0;
};


class Assign : public Expr, public std::enable_shared_from_this<Assign> {
public:
    Assign(std::shared_ptr<Token> name, std::shared_ptr<Expr> value) {
      this->m_name = name;
      this->m_value = value;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitAssignExpr(shared_from_this());
		}

    std::shared_ptr<Token> m_name;
    std::shared_ptr<Expr> m_value;
};

class Binary : public Expr, public std::enable_shared_from_this<Binary> {
public:
    Binary(std::shared_ptr<Expr> left, std::shared_ptr<Token> operater, std::shared_ptr<Expr> right) {
      this->m_left = left;
      this->m_operater = operater;
      this->m_right = right;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitBinaryExpr(shared_from_this());
		}

    std::shared_ptr<Expr> m_left;
    std::shared_ptr<Token> m_operater;
    std::shared_ptr<Expr> m_right;
};

class Grouping : public Expr, public std::enable_shared_from_this<Grouping> {
public:
    Grouping(std::shared_ptr<Expr> expression) {
      this->m_expression = expression;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitGroupingExpr(shared_from_this());
		}

    std::shared_ptr<Expr> m_expression;
};

class Literal : public Expr, public std::enable_shared_from_this<Literal> {
public:
    Literal(std::shared_ptr<Object> value) {
      this->m_value = value;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitLiteralExpr(shared_from_this());
		}

    std::shared_ptr<Object> m_value;
};

class Logical : public Expr, public std::enable_shared_from_this<Logical> {
public:
    Logical(std::shared_ptr<Expr> left, std::shared_ptr<Token> operater, std::shared_ptr<Expr> right) {
      this->m_left = left;
      this->m_operater = operater;
      this->m_right = right;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitLogicalExpr(shared_from_this());
		}

    std::shared_ptr<Expr> m_left;
    std::shared_ptr<Token> m_operater;
    std::shared_ptr<Expr> m_right;
};

class Unary : public Expr, public std::enable_shared_from_this<Unary> {
public:
    Unary(std::shared_ptr<Token> operater, std::shared_ptr<Expr> right) {
      this->m_operater = operater;
      this->m_right = right;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitUnaryExpr(shared_from_this());
		}

    std::shared_ptr<Token> m_operater;
    std::shared_ptr<Expr> m_right;
};

class Variable : public Expr, public std::enable_shared_from_this<Variable> {
public:
    Variable(std::shared_ptr<Token> name) {
      this->m_name = name;
    }
		std::any accept (std::shared_ptr<Visitor> visitor) override{
			return visitor->visitVariableExpr(shared_from_this());
		}

    std::shared_ptr<Token> m_name;
};

#endif
