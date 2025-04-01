# TinyWebServer学习的一些收获

## 单例类的使用(属于日志记录模块)
> 本项目中, __日志记录__ 模块使用到了单例模式,使用的是懒汉式构造,C++11之后,懒汉式构造是天然的线程安全,不需要使用互斥锁保证线程安全.

**对于单例模式,有以下一些注意事项:**
- 构造私有
- 禁止拷贝构造和赋值操作
> 这两个操作都是为了保证单例的唯一性,可靠性.

``` C++
class Log
{
public:
    //C++11以后,使用局部变量懒汉不用加锁
    static Log *get_instance()
    {
        static Log instance;
        return &instance;
    }

    // ...原文的其他代码不予展示,这里只是研究单例模式

private:
	// 怎么没有删除拷贝和赋值?
	/*
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
	*/

    Log();
    virtual ~Log();

private:
	// 成员变量....
};

```

上述代码中,get_instance是个静态函数,而单例指针instance也是静态变量,不存在于堆区或者栈区,而是存在于全局静态区,保证了单例模式的唯一性和加载速度.
