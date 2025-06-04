#ifndef EXPR_H
#define EXPR_H
#include <vector>
#include <any>
#include"token.h"
namespace Expr {
class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor {
public:
	virtual std::any visitBinaryExpr(Binary* Expr) = 0;
	virtual std::any visitGroupingExpr(Grouping* Expr) = 0;
	virtual std::any visitLiteralExpr(Literal* Expr) = 0;
	virtual std::any visitUnaryExpr(Unary* Expr) = 0;
	virtual ~Visitor() {};
};

class Expr {

public:
	virtual std::any accept(Visitor* visitor);
};

class Binary : public Expr {
public:
    Binary(Expr* left, Token* operater, Expr* right) {
      this->m_left = left;
      this->m_operater = operater;
      this->m_right = right;
    }
		std::any accept (Visitor* visitor) override{
			return visitor->visitBinaryExpr(this);
		}

    Expr* m_left;
    Token* m_operater;
    Expr* m_right;

};
class Grouping : public Expr {
public:
    Grouping(Expr* expression) {
      this->m_expression = expression;
    }
		std::any accept (Visitor* visitor) override{
			return visitor->visitGroupingExpr(this);
		}

    Expr* m_expression;

};
class Literal : public Expr {
public:
    Literal(Object* value) {
      this->m_value = value;
    }
		std::any accept (Visitor* visitor) override{
			return visitor->visitLiteralExpr(this);
		}

    Object* m_value;

};
class Unary : public Expr {
public:
    Unary(Token* operater, Expr* right) {
      this->m_operater = operater;
      this->m_right = right;
    }
		std::any accept (Visitor* visitor) override{
			return visitor->visitUnaryExpr(this);
		}

    Token* m_operater;
    Expr* m_right;

};
} //namespace Expr
#endif
