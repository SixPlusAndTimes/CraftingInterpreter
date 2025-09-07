# 说明
本项目是对[这个项目](https://craftinginterpreters.com/contents.html)的part1和2的复刻（算是用C++翻译java的版本吧）

总结自己的代码就是写成了一坨，应该有很多东西可以重构，但目前懒得搞了。

为什么做这个？因为有意思，也想了解一下编译器的原理，但是做的这部分只相当于一个解释器，之后的东西再说吧。

为什么用C++做？因为工作使用的语言是C++，但是目前还没有怎么用C++写过项目，只是背熟了八股文，实战是写不了一点的。但通过这个解释器的学习捣鼓，对C++的使用有较大进步。


# 总结

写这个解释器，目前我体会到的C++最难的一点就是：手动地进行生命周期的管理。尽管有shared_ptr、unique_ptr，但是还是有很多可坑，踩了之后才知道。我记录了这些坑，以后当做实战记录发在博客上。

第一，回顾整个解释器，我做的最错误的决定应该就是无节制得使用shared_ptr, 导致我后期的代码也必须使用shared_ptr(深刻地体会了shared_ptr的传染性)。

第二，原书是使用java写的，java天生地有 Object 这个类可以表示运行过程中的所有类。但C++没有这样的Object，我的处理方式取巧了，使用了std::variant, 并使用Object这个别名
~~~c++
using Object = std::variant<nullptr_t, double, bool, std::string, 
                            std::shared_ptr<CppLoxCallable>, std::shared_ptr<CppLoxInstance>>;
~~~
也许我应该自己写一个Object类处理上述的几个Lox类型。而且Object必须非得向上面那样定义吗，尤其是那两个sharedptr


- 但是其中的CppLoxCallable能做成std::unique_ptr<CppLoxCallable> 吗 —— 可能不行，因为闭包的存在，可能存在共享的场景，为了简化起见，直接使用了shared_ptr

- 能使用CppLoxCallable吗，可能不行，因为CppLoxCallable是需要动态语义的，不能用值语义存储它

- 那么能直接使用指针吗？ 好像可以，但是生命周期管理太费劲了, 尤其是涉及闭包的场景，我该如何释放某个闭包？ 整个程序结束之后吗？ 那么就要在某个东西下面存储所有的闭包，然后当这个东西销毁时，顺带将闭包全部销毁。
    - 那么这个东西是什么？ 全局environment吗？尝试过，但是这有引入了额外的复杂性。因为每个logcal env 必须存储global env的指针或者引用，然后在适当的时机将闭包的局部变量存储在global env中。更复杂的是，当执行一个闭包并进行变量resolve、define和assign时，需要考虑global env。
