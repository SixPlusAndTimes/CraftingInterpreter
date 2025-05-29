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
    virtual std::any visitBinaryExpr(Binary& Expr) = 0;
    virtual std::any visitGroupingExpr(Grouping& Expr) = 0;
    virtual std::any visitLiteralExpr(Literal& Expr) = 0;
    virtual std::any visitUnaryExpr(Unary& Expr) = 0;
};

class Expr {

public:
  Expr() = default;
  Expr(const Expr&) = default;
  ~Expr() = default;
	virtual std::any accept(Visitor& visitor);
};

class Binary : public Expr {
public:
    Binary(Expr left, Token operater, Expr right) {
      this->left = left;
      this->operater = operater;
      this->right = right;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitBinaryExpr(*this);
		}

    Expr left;
    Token operater;
    Expr right;

};
class Grouping : public Expr {
public:
    Grouping(Expr expression) {
      this->expression = expression;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitGroupingExpr(*this);
		}

    Expr expression;

};
class Literal : public Expr {
public:
    Literal(Object value) {
      this->value = value;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitLiteralExpr(*this);
		}

    Object value;

};
class Unary : public Expr {
public:
    Unary(Token operater, Expr right) {
      this->operater = operater;
      this->right = right;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitUnaryExpr(*this);
		}

    Token operater;
    Expr right;

};
} //namespace Expr
#endif
