#include <vector>
#include "token.h"
class Expr {
};

class Binary : public Expr {
    Binary(Expr left, Token operater, Expr right) {
      this->left = left;
      this->operater = operater;
      this->right = right;
    }
    Expr left;
    Token operater;
    Expr right;

};
class Grouping : public Expr {
    Grouping(Expr expression) {
      this->expression = expression;
    }
    Expr expression;

};
class Literal : public Expr {
    Literal(Object value) {
      this->value = value;
    }
    Object value;

};
class Unary : public Expr {
    Unary(Token operater, Expr right) {
      this->operater = operater;
      this->right = right;
    }
    Token operater;
    Expr right;

};