# CraftingInterpreter

~~~c++
class Environment{
public:
                                                Environment();
                                                Environment(Environment* enclosing);
    virtual                                     ~Environment();
    void                                        define(const std::string& name, const Object& value);
    void                                        assign(const Token& name, const Object& value);
    void                                        assignAt(int distance, const Token& name, const Object& value);
    Object                                      get(Token name);
    Object                                      getAt(int distance, std::string name);
    std::unordered_map<std::string, Object>&    getValues();
    Environment&                                ancestor(int distance);
private:
    std::unordered_map<std::string, Object>     m_values;   
    Environment*                                m_enclosing; 
};
~~~

std::unordered_map<std::string, Object>     m_values;  中Object到底如何定义？

- 当前Object.h中，使用std::variant 作为Object

- using Object = std::variant<nullptr_t, double, bool, std::string, std::shared_ptr<CppLoxCallable>>;

- 但是其中的CppLoxCallable能做成std::unique_ptr<CppLoxCallable> 吗 —— 可能不行，因为闭包的存在，可能存在共享的场景，为了简化起见，直接使用了shared_ptr

- 能使用CppLoxCallable吗，可能不行，因为CppLoxCallable是需要动态语义的，不能用值语义存储它

- 那么能直接使用指针吗？ 好像可以，但是生命周期管理太费劲了, 尤其是涉及闭包的场景，我该如何释放某个闭包？ 整个程序结束之后吗？ 那么就要在某个东西下面存储所有的闭包，然后当这个东西销毁时，顺带将闭包全部销毁。
    - 那么这个东西是什么？ 全局environment吗？尝试过，但是这有引入了额外的复杂性。因为每个logcal env 必须存储global env的指针或者引用，然后在适当的时机将闭包的局部变量存储在global env中。更有复杂性的是，当执行一个闭包并进行变量resolve、define和assign时，需要考虑global env。
