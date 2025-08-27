#include "Resolver.hpp"
#include <cassert>
#include "cpplox.h"

Resolver:: Resolver(Interpreter& interpreter)
: m_interpreter(interpreter)
{

}

void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& statements) {
    LOG_DEBUG("Resolver: resolve begin stmtsss");
    for (auto stmt : statements) {
        resolve(stmt);
    }
    LOG_DEBUG("Resolver: resolve end stmtsss");
}

void Resolver::resolve(std::shared_ptr<Stmt> stmt) {
    LOG_DEBUG("Resolver: resolve begin stmt");
    stmt->accept(shared_from_this());
    LOG_DEBUG("Resolver: resolve end stmt");
}

void Resolver::resolve(std::shared_ptr<Expr> expr) {
    LOG_DEBUG("Resolver: resolve begin expr");
    expr->accept(shared_from_this());
    LOG_DEBUG("Resolver: resolve end expr");
}

void Resolver::beginScope() {
    m_scopes.emplace_back(std::unordered_map<std::string, bool>());
}

void Resolver::endScope() {
    m_scopes.pop_back();
}

std::any Resolver::visitBlockStmt(std::shared_ptr<Block> stmt) {
    LOG_DEBUG("Resolver: BlockStmt begin");
    beginScope();
    resolve(*stmt->m_statements);
    endScope();
    LOG_DEBUG("Resolver: BlockStmt end");
    return nullptr;
}


std::any Resolver::visitAssignExpr(std::shared_ptr<Assign> expr) {
    LOG_DEBUG("Resolver: AssignExpr begin");
    resolve(expr->m_value);
    resolveLocal(expr, expr->m_name);
    LOG_DEBUG("Resolver: AssignExpr end");
    return nullptr;
}

std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    LOG_DEBUG("Resolver: BinaryExpr begin");
    resolve(expr->m_left);
    resolve(expr->m_right);
    LOG_DEBUG("Resolver: BinaryExpr end");
    return nullptr;
}

std::any Resolver::visitCallExpr(std::shared_ptr<Call> expr) {
    LOG_DEBUG("Resolver: Callexpr begin");
    resolve(expr->m_callee);

    for (auto argument : *expr->m_arguments) {
        resolve(argument);
    }
    LOG_DEBUG("Resolver: Callexpr end");
    return nullptr;
}

std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    LOG_DEBUG("Resolver: Grouping expr  begin");
    resolve(expr->m_expression);
    LOG_DEBUG("Resolver: Grouping expr  end");
    return nullptr;
}

std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    LOG_DEBUG("Resolver: Literral expr  begin/end");
    return nullptr;
}

std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    LOG_DEBUG("Resolver: LogicalExpr  begin");
    resolve(expr->m_left);
    resolve(expr->m_right);
    LOG_DEBUG("Resolver: LogicalExpr  end");
    return nullptr;
}

std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    LOG_DEBUG("Resolver: UnaryExpr  begin");
    resolve(expr->m_right);
    LOG_DEBUG("Resolver: UnaryExpr  end");
    return nullptr;
}

std::any Resolver::visitVariableExpr(std::shared_ptr<Variable> expr) {
    LOG_DEBUG("Resolver: visitVariable Begin, name = {}", expr->m_name->m_lexeme);
    if (!m_scopes.empty())
    {
        auto iter = m_scopes.back().find(expr->m_name->m_lexeme);
        if (iter != m_scopes.back().end() && iter->second == false) {
            cpplox::error(*expr->m_name, "Can't read local variable in its own initializer.");
        }
    }
    resolveLocal(expr, expr->m_name);
    LOG_DEBUG("Resolver: visitVariable end");
    return nullptr;
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, std::shared_ptr<Token> name) {
    LOG_DEBUG("Resolver: resolveLocal Begin, name = {}", name->m_lexeme);
    for (int i = m_scopes.size() - 1; i >= 0; i--) {
        if (m_scopes[i].count(name->m_lexeme) != 0) {
            LOG_DEBUG("Resolver: interpreter resolve name = {}, distance = {}", name->m_lexeme, m_scopes.size() - 1 - i);
            m_interpreter.resolve(expr, m_scopes.size() - 1 - i);
            break;
        }
    }
    LOG_DEBUG("Resolver: resolveLocal End");
    return;
}

void Resolver::declare(std::shared_ptr<Token> name) {
    if (m_scopes.empty()) {
        return ;
    }
    auto& scope = m_scopes.back();
    if (scope.count(name->m_lexeme) != 0) {
        cpplox::error(*name, "Already a variable with this name in this scope");
    }

    const auto& ret = scope.insert({name->m_lexeme, false}); // mark the variable "not ready"
    if (!ret.second) {
        assert("resolver define insert failed");
    }
    LOG_DEBUG("Resolver: declared varname = {}, idx = {}", name->m_lexeme, m_scopes.size() - 1);
}

void Resolver::define(std::shared_ptr<Token> name) {
    if (m_scopes.empty()) {
        return ;
    }
    auto& scope = m_scopes.back();
    // need check with end()?
    scope[name->m_lexeme] = true;

    LOG_DEBUG("Resolver: defined varname = {}, idx = {}", name->m_lexeme, m_scopes.size() - 1);
}

std::any Resolver::visitVarStmt(std::shared_ptr<Var> stmt) {
    LOG_DEBUG("Resolver: visitVarStmt begin");
    // split declare and define into two stepes
    // in order to avoid current scope's varstmt declare the same variable as the outer scope do
    declare(stmt->m_name);
    if (stmt->m_initializer != nullptr) {
        resolve(stmt->m_initializer);
    }
    define(stmt->m_name);
    LOG_DEBUG("Resolver: visitVarStmt end");
    return nullptr;
}

std::any Resolver::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    LOG_DEBUG("Resolver: expressionstmt begin");
    resolve(stmt->m_expression);
    LOG_DEBUG("Resolver: expressionstmt end");
    return nullptr;
}

std::any Resolver::visitFunctionStmt(std::shared_ptr<Function> stmt) {
    LOG_DEBUG("Resolver: functionstmt begin");
    declare(stmt->m_name);
    define(stmt->m_name); // unlike variable resolution, we define function name early so that we can do recursions
    resolveFunction(stmt, FunctionType::FUNCTION);
    LOG_DEBUG("Resolver: functionstmt end");
    return nullptr;
}

void Resolver::resolveFunction(std::shared_ptr<Function> function, FunctionType type) {
    LOG_DEBUG("Resolver: resolveFunction begin");
    beginScope();
    FunctionType enclosingFunction = m_currentFunction;
    m_currentFunction = type; 
    for (auto token : *function->m_params) {
        declare(token);
        define(token);
    }
    resolve(*function->m_body);
    endScope();
    m_currentFunction = enclosingFunction;
    LOG_DEBUG("Resolver: resolveFunction end");
}

std::any Resolver::visitIfStmt(std::shared_ptr<If> stmt) {
    LOG_DEBUG("Resolver: ifstmt begin");
    resolve(stmt->m_condition);
    resolve(stmt->m_thenBranch);
    if (stmt->m_elseBranch != nullptr) {
        resolve(stmt->m_elseBranch);
    }
    LOG_DEBUG("Resolver: ifstmt end");
    return nullptr;
}

std::any Resolver::visitPrintStmt(std::shared_ptr<Print> stmt) {
    LOG_DEBUG("Resolver: printstmt begin");
    resolve(stmt->m_expression);
    LOG_DEBUG("Resolver: printstmt end");
    return nullptr;
}

std::any Resolver::visitReturnStmt(std::shared_ptr<Return> stmt) {
    LOG_DEBUG("Resolver: reteurnstmt begin");
    if (m_currentFunction == FunctionType::NONE) {
        cpplox::error(*stmt->m_keyword, "Can't return from top-level code.");
    }

    if (stmt->m_value != nullptr) {
        resolve(stmt->m_value);
    }
    LOG_DEBUG("Resolver: reteurnstmt end");
    return nullptr;
}

std::any Resolver::visitWhileStmt(std::shared_ptr<While> stmt) {
    LOG_DEBUG("Resolver: whilestme begin");
    resolve(stmt->m_condition);
    resolve(stmt->m_body);
    LOG_DEBUG("Resolver: whilestme end");
    return nullptr;
}

std::any Resolver::visitClassStmt(std::shared_ptr<Class> stmt) {
    LOG_DEBUG("Resolver: class stmt begin");
    declare(stmt->m_name);
    define(stmt->m_name);
    LOG_DEBUG("Resolver: class stmt end");
    return nullptr;
}