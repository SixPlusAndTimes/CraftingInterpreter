#include <vector>
#include <any>
#include"token.h"
namespace Expr {
class Visitor {
public:	virtual std::any visitBinaryExpr(Binary& Expr);
	virtual std::any visitGroupingExpr(Grouping& Expr);
	virtual std::any visitLiteralExpr(Literal& Expr);
	virtual std::any visitUnaryExpr(Unary& Expr);
};
class Expr {
	virtual std::any accept(Visitor& visitor);
};

class Binary : public Expr {
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
    Grouping(Expr expression) {
      this->expression = expression;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitGroupingExpr(*this);
		}

    Expr expression;

};
class Literal : public Expr {
    Literal(Object value) {
      this->value = value;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitLiteralExpr(*this);
		}

    Object value;

};
class Unary : public Expr {
    Unary(Token operater, Expr right) {
      this->operater = operater;
      this->right = right;
    }
		std::any accept (Visitor& visitor) override{
			return visitor.visitUnaryExpr(*this);
		}

    Token operater;
    Expr right;

};} //namespace Expr