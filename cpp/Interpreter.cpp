#include <any>
#include <stdexcept>
#include <cassert>
#include <unordered_map>
#include "Interpreter.h"
#include "utils.h"
#include "RuntimeError.h"
#include "cpplox.h"
#include "CppLoxCallable.h"
#include "LoxFunction.h"
#include "CppLoxClass.h"
#include "CppLoxInstance.h"
Interpreter::Interpreter()
: m_globalEnvironment(std::make_shared<Environment>())
, m_environment(m_globalEnvironment)
{
    // std::cout << "globalEnv addr = " << m_environment << "\n";
    // give it a try
    class ClockFunction : public CppLoxCallable
    {
        public:
            ClockFunction() = default;
            ~ClockFunction() = default;
            size_t arity() const override {
                return 0;
            }

            Object call(Interpreter& interpreter, std::vector<Object>& arguments) override {
                // TODU
                return 11212.222f;
            }
            
            std::string toString() const override{
                return "<native fn : clock>";
            }
    };
    // LOG_DEBUG("Global define");
    // std::cout << "globalEnv addr = " << m_environment << "\n";
    m_globalEnvironment->define("clock", std::make_shared<ClockFunction>());
}

void Interpreter::interpreter(const std::vector<std::shared_ptr<Stmt>>& statements) {
    LOG_DEBUG("Interpreter begin, statements.size() = {}", statements.size());
    try {
        for (auto stmt : statements)
        {
            execute(stmt);
        }
    }catch (RuntimeError& error) {
        LOG_DEBUG("catch runtime error");
        cpplox::runtimeError(error);
    }
    LOG_DEBUG("Interpreter end");
}

void Interpreter::execute(std::shared_ptr<Stmt> stmt) {
    stmt->accept(shared_from_this());
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth) {
    m_locals.insert({expr.get(), depth});
}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    LOG_DEBUG("Interpreter visitLogicalExpr begin") ;
    Object value = evaluate(expr->m_left);   
    // short circuit here
    if (expr->m_operater->m_tokenType == TokenType::OR) {
        if (isTruthy(value)) {
            return value;
        }
    }else {
        if (!isTruthy(value)) {
            return value;
        }
    }
    return evaluate(expr->m_right);
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    LOG_DEBUG("Interpreter visitAssignExpr begin") ;
    // LOG_DEBUG("visit Assign expr, name[{}], value[{}]", (expr->m_name)->m_lexeme.c_str(), LoxLiteralTyeToString(value));
    Object value = evaluate(expr->m_value);
    
    auto iter = m_locals.find(expr.get());
    if (iter != m_locals.end()) {
        int distance = iter->second;
        m_environment->assignAt(distance, *expr->m_name, value);
    }else {
        m_globalEnvironment->assign(*expr->m_name, value);
    }
    return value;
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    LOG_DEBUG("Interpreter visitBinaryExpr begin") ;
    Object left = evaluate(expr->m_left);
    Object right = evaluate(expr->m_right);

    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::GREATER:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) > std::get<double>(right));
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) >= std::get<double>(right));
        case TokenType::LESS:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) < std::get<double>(right));
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) <= std::get<double>(right));
        case TokenType::MINUS:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) - std::get<double>(right));
        case TokenType::BANG_EQUAL: return Object(!isEqual(left, right));
        case TokenType::EQUAL_EQUAL: return Object(isEqual(left, right));
        case TokenType::PLUS: 
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
                return Object(std::get<double>(left) + std::get<double>(right));
            } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)){
                return Object(std::get<std::string>(left) + std::get<std::string>(right));
            } else {
                LOG_ERROR("Operands must be two numbers or two strings.");
                throw RuntimeError(*expr->m_operater, "Operands must be two numbers or two strings.");
            }
        case TokenType::SLASH:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) / std::get<double>(right));
        case TokenType::STAR:
            checkNumberOperands(expr->m_operater, left, right);
            return Object(std::get<double>(left) * std::get<double>(right));
        default:
            break;
    }
    return nullptr;
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    LOG_DEBUG("Interpreter visitGroupingExpr begin") ;
    return  evaluate(expr->m_expression);
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    LOG_DEBUG("Interpreter visitLiteralExpr begin") ;
    return *(expr->m_value);
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    LOG_DEBUG("Interpreter visitUnaryExpr begin") ;
    Object right = evaluate(expr->m_right);
    switch (expr->m_operater->m_tokenType)
    {
        case TokenType::MINUS: {
            checkNumberOperand(expr->m_operater, right);
            return Object(-1 * std::get<double>(right)); 
        }
        case TokenType::BANG: {
            return Object(!isTruthy(right));
        }
        default:
            break;
    }
    LOG_DEBUG("Interpreter visitUnaryExpr end") ;
    return nullptr;
}

std::any Interpreter::visitGetExpr(std::shared_ptr<Get> expr) {
    Object object = evaluate(expr->m_object);
    if (std::holds_alternative<std::shared_ptr<CppLoxInstance>>(object)) {
        std::shared_ptr<CppLoxInstance> instancePtr = std::get<std::shared_ptr<CppLoxInstance>>(object);
        return instancePtr->get(expr->m_name);
    }
    throw RuntimeError(*expr->m_name, "Only instances have fields.");
}

std::any Interpreter::visitSetExpr(std::shared_ptr<Set> expr) {
    Object object = evaluate(expr->m_object);
    if (!std::holds_alternative<std::shared_ptr<CppLoxInstance>>(object)) {
        throw RuntimeError(*expr->m_name, "Only instances have fields.");
    }
    Object value = evaluate(expr->m_value);

    std::shared_ptr<CppLoxInstance> instancePtr = std::get<std::shared_ptr<CppLoxInstance>>(object);
    instancePtr->set(expr->m_name, value);

    return value;
}

std::any Interpreter::visitThisExpr(std::shared_ptr<This> expr) {
    return lookUpVariable(expr->m_keyword, expr);
}

std::any Interpreter::visitSuperExpr(std::shared_ptr<Super> expr) {
    assert(m_locals.count(expr.get()) != 0);
    int distance = m_locals[expr.get()];

    Object object = m_environment->getAt(distance, "super");
    if (!std::holds_alternative<std::shared_ptr<CppLoxCallable>>(object)) {
        assert("Object not hold a CppLoxCallable");
    }
    if (std::dynamic_pointer_cast<CppLoxClass>(std::get<std::shared_ptr<CppLoxCallable>>(object)) == nullptr) {
        assert("CppLoxCallable cannot cast to CppLoxClass");
    }
    std::shared_ptr<CppLoxClass> superclass = std::dynamic_pointer_cast<CppLoxClass>(std::get<std::shared_ptr<CppLoxCallable>>(object));

    object = m_environment->getAt(distance - 1, "this");
    if (!std::holds_alternative<std::shared_ptr<CppLoxInstance>>(object)) {
        assert("Object not hold a CppLoxInstance");
    }
    std::shared_ptr<CppLoxInstance> instance = std::get<std::shared_ptr<CppLoxInstance>>(object);

    std::shared_ptr<LoxFunction> method = superclass->findMethod(expr->m_method->m_lexeme);
    if (!method) {
        throw RuntimeError(*expr->m_method, "Undefined property '" + expr->m_method->m_lexeme + "'.");
    }

    return static_cast<Object>(method->bind(instance)); // shoudl do static_cast here, otherwise will throw std::bad_anycast exception
}

std::any Interpreter::visitCallExpr(std::shared_ptr<Call> expr) {
    LOG_DEBUG("visit function call begin");

    Object callee = evaluate(expr->m_callee);
    std::vector<Object> arguments;
    for (auto& argument : *(expr->m_arguments)) {
        arguments.push_back(evaluate(argument));
    }

    auto calleePtr = std::get_if<std::shared_ptr<CppLoxCallable>>(&callee);
    if (calleePtr == nullptr) {
      throw RuntimeError(*expr->m_paren,
          "Can only call functions and classes.");
    }

    assert(calleePtr != nullptr);
    std::shared_ptr<CppLoxCallable> function = *calleePtr;
    assert(function != nullptr);
    if (arguments.size() != function->arity())
    {
        throw RuntimeError(*expr->m_paren, "Expected " +
          std::to_string(function->arity()) + " arguments but got " +
          std::to_string(arguments.size()) + ".");
    }
    return function->call(*this, arguments);
}

Object Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    LOG_DEBUG("evaluate start");
    return std::any_cast<Object>(expr->accept(shared_from_this()));
}

bool Interpreter::isTruthy(const Object& object) {
    if (const auto doublePtr = std::get_if<double>(&object)) {
        return true;
    } else if (const auto boolPtr = std::get_if<bool>(&object)) {
        return *boolPtr;
    } else if (const auto stringPtr = std::get_if<std::string>(&object)) {
        return true;
    } else {
        return false;
    }
}

bool Interpreter::isEqual(const Object& left, const Object& right) {

    return ObjectEquals(left, right);
}

void Interpreter::checkNumberOperand(std::shared_ptr<Token> operater, const Object& operand) {
    if (std::holds_alternative<double>(operand)) {
        return;
    }
    LOG_ERROR("Operand Must be a number.");
    throw RuntimeError(*operater, "Operand Must be a number.");
}

void Interpreter::checkNumberOperands(std::shared_ptr<Token> operater, const Object& operandLeft, const Object& operandRight) {

    if (std::holds_alternative<double>(operandLeft) && std::holds_alternative<double>(operandRight)) {
        return;
    }
    LOG_ERROR("Operands Must be a number.");
    throw RuntimeError(*operater, "Operands Must be a number.");
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    LOG_DEBUG("visit expression statement begin");
    evaluate(stmt->m_expression);
    LOG_DEBUG("visit expression statement end");
    return nullptr;
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    LOG_DEBUG("visit print statement begin");
    Object ret = evaluate(stmt->m_expression);
    std::cout << stringfy(ret) << std::endl;
    LOG_DEBUG("visit print statement end");
    return nullptr;
}

Object Interpreter::lookUpVariable(std::shared_ptr<Token> name, std::shared_ptr<Expr> expr) {
    LOG_DEBUG("Interpreter: lookUpVariable begin, varname = {}", name->m_lexeme);
    auto iter = m_locals.find(expr.get());
    if (iter != m_locals.end()) {
        int distance = iter->second;
        LOG_DEBUG("Interpreter: lookUpVariable distance = {}, varname = {}", distance, name->m_lexeme);
        return m_environment->getAt(distance, name->m_lexeme);
    }else {
        LOG_DEBUG("Interpreter: lookUpVariable no distance, will find in global env");
        return m_globalEnvironment->get(*name);
    }
    LOG_DEBUG("Interpreter: lookUpVariable end", name->m_lexeme);
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    LOG_DEBUG("visit varexpression begin, var name = [{}]", expr->m_name->m_lexeme);

    return lookUpVariable(expr->m_name, expr);

}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
    LOG_DEBUG("visit varstatement begin");
    Object value = nullptr;
    if (stmt->m_initializer != nullptr) {
        value = evaluate(stmt->m_initializer);
    }
    m_environment->define(stmt->m_name->m_lexeme, value);
    LOG_DEBUG("visit varstatement end");
    return nullptr;
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    std::shared_ptr<Environment> childEnvironment = std::make_shared<Environment>(m_environment);
    executeBlock(stmt->m_statements, childEnvironment) ;
    return nullptr;
}

std::any Interpreter::visitIfStmt(std::shared_ptr<If> stmt) {
    if (isTruthy(evaluate(stmt->m_condition))) {
        execute(stmt->m_thenBranch) ;
    } else if (stmt->m_elseBranch) {
        execute(stmt->m_elseBranch);
    }
    return nullptr;
}

std::any Interpreter::visitWhileStmt(std::shared_ptr<While> stmt) {

    while (isTruthy(evaluate(stmt->m_condition))) {
        execute(stmt->m_body);
    }

    return nullptr;
}

std::any Interpreter::visitFunctionStmt(std::shared_ptr<Function> stmt) {
    LOG_DEBUG("Visit Function declartion begin");
    std::shared_ptr<CppLoxCallable> function = std::make_shared<LoxFunction>(stmt.get(), m_environment, false);
    LOG_DEBUG("Degine funcion name = [{}]", static_cast<LoxFunction*>(function.get())->toString());
    m_environment->define(stmt->m_name->m_lexeme, function);
    LOG_DEBUG("Visit Function declartion end");
    return nullptr;   
}

std::any Interpreter::visitReturnStmt(std::shared_ptr<Return> stmt) {
    Object value = nullptr;
    LOG_DEBUG("visitReturnStmt begin");
    if (stmt->m_value != nullptr) {
        LOG_DEBUG("m_value != nullptr evaluating");
        value = evaluate(stmt->m_value);
        LOG_DEBUG("evaluate done value = {}", LoxLiteralTyeToString(value));
    }else {
        LOG_DEBUG("m_value == nullptr, return nil");
    }

    LOG_DEBUG("visitReturnStmt end, value = {}", LoxLiteralTyeToString(value));
    // use exception to return 
    throw ReturnException(value);
}

void Interpreter::executeBlock(std::shared_ptr<std::vector<std::shared_ptr<Stmt>>> stmtVecPtr, std::shared_ptr<Environment> environmentChild) {
    std::shared_ptr<Environment> parentEnv = m_environment;
    m_environment = environmentChild;
    try
    {
        for (auto stmt : *stmtVecPtr) {
            execute(stmt);
        }
    }
    catch(const std::exception& e)
    {
        m_environment = parentEnv; 
        // LOG_ERROR(e.what());
        throw ; // throw again, let the caller handle the exeception
    }
        m_environment = parentEnv; 
}


std::any Interpreter::visitClassStmt(std::shared_ptr<Class> stmt) {
    LOG_DEBUG("visitClassStmt begin");
    Object superClass = nullptr;    
    std::shared_ptr<CppLoxClass> superClassPtr = nullptr;
    if (stmt->m_superclass) {
        superClass = evaluate(stmt->m_superclass);
        // first check if object holds cpploxcallable ptr , if it does then check if the ptr is cpploxccalss ptr
        // since CppLoxClass is inherited from CppLoxCallable, we can not put CppLoxClass in Object's declaration again (it's so tedious, I admit)
        if (!std::holds_alternative<std::shared_ptr<CppLoxCallable>>(superClass) 
            || std::dynamic_pointer_cast<CppLoxClass>(std::get<std::shared_ptr<CppLoxCallable>>(superClass)) == nullptr) {
            throw RuntimeError(*stmt->m_superclass->m_name, "SuperClass must be a class");
        }
        superClassPtr = std::static_pointer_cast<CppLoxClass>(std::get<std::shared_ptr<CppLoxCallable>>(superClass));
        LOG_DEBUG("    visitClassStmt has superclass");
    }

    m_environment->define(stmt->m_name->m_lexeme, nullptr);

    // handling superkeyword
    if (stmt->m_superclass) {
        m_environment = std::make_shared<Environment>(m_environment);
        m_environment->define("super", superClass);
    }

    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    
    for (auto& method : *stmt->m_methods) {
        std::shared_ptr<LoxFunction> function = std::make_shared<LoxFunction>(method.get(), m_environment, method->m_name->m_lexeme == "init");
        methods[method->m_name->m_lexeme] = function;
    }

    std::shared_ptr<CppLoxClass> klass = std::make_shared<CppLoxClass>(stmt->m_name->m_lexeme, superClassPtr, methods);

    // handling super keyword
    if (stmt->m_superclass) {
        m_environment = m_environment->getEnclosing();
    }

    m_environment->assign(*stmt->m_name, klass);
    LOG_DEBUG("visitClassStmt end");
    return nullptr;
}

// util functions , maybe put in utils.h
std::string Interpreter::stringfy(const Object& object) {
    if (std::holds_alternative<std::nullptr_t>(object)) {
        return "nil";
    }else if (std::holds_alternative<double>(object)) {
        auto ret = std::format("{:.3f}", std::get<double>(object));
        // if the number end with ".000", cpplox perceive it as an interger, remove the fractional part
        if (ret.size() >= 4 && ret.substr(ret.size() - 4) == ".000") 
        {
            ret = ret.substr(0, ret.size() - 4);
        }
        return ret; 
    }else if (std::holds_alternative<bool>(object)) {
        bool boolean = std::get<bool>(object);
        return boolean ? "true" : "false";
    }else if (std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }else if (std::holds_alternative<std::shared_ptr<CppLoxCallable>>(object)) {
        return std::get<std::shared_ptr<CppLoxCallable>>(object)->toString();
    }else if (std::holds_alternative<std::shared_ptr<CppLoxInstance>>(object)) {
        return std::get<std::shared_ptr<CppLoxInstance>>(object)->toString();
    }
    return std::string{"UnKnownType!"};
}