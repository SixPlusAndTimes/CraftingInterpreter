#ifndef RESOLVER_H
#define RESOLVER_H
#include "Stmt.h"
#include "Expr.h"

enum class FunctionType {
    NONE,
    FUNCTION,
    METHOD,
    UNKNOWN
};

enum class ClassType {
    NONE,
    CLASS,
    UNKNOWN
};

class Resolver : public Expr::Visitor, public Stmt::Visitor , public std::enable_shared_from_this<Resolver>{
    public:
        Resolver(Interpreter& interpreter);
        std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
		std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
		std::any visitCallExpr(std::shared_ptr<Call> expr) override;
		std::any visitGetExpr(std::shared_ptr<Get> expr) override;
		std::any visitSetExpr(std::shared_ptr<Set> expr) override;
		std::any visitThisExpr(std::shared_ptr<This> expr) override;
		std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
		std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
		std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
		std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
		std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;

        std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
		std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
		std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override;
		std::any visitIfStmt(std::shared_ptr<If> stmt) override;
		std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
		std::any visitReturnStmt(std::shared_ptr<Return> stmt) override;
		std::any visitWhileStmt(std::shared_ptr<While> stmt) override;
		std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
	    std::any visitClassStmt(std::shared_ptr<Class> stmt) override;


        void resolve(const std::vector<std::shared_ptr<Stmt>>& statements);
        void resolve(std::shared_ptr<Stmt>);
        void resolve(std::shared_ptr<Expr>);
        void beginScope();
        void endScope();
        void declare(std::shared_ptr<Token> name); 
        void define(std::shared_ptr<Token> name);
        void resolveLocal(std::shared_ptr<Expr> expr, std::shared_ptr<Token> name);
        void resolveFunction(std::shared_ptr<Function> stmt, FunctionType type);
    private:
        Interpreter&                                        m_interpreter;
        std::vector<std::unordered_map<std::string, bool>>  m_scopes;
        FunctionType                                        m_currentFunction = FunctionType::NONE;
        ClassType                                           m_currentClass = ClassType::NONE;        
};


#endif // RESOLVER_H