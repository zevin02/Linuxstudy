
对于被explict修饰的函数都是不支持连续构造的

如


> explicit 修饰的构造函数

~~~cpp
class a
{
    explicit a()
}
~~~

a b=2这样的初始化是不支持的