#pragma once
#include <iostream>
#include <pthread.h>

namespace ns_task
{
    class Task
    {
    private:
        int _x;
        int _y;
        char _op; //表示+-*/%
    public:
        Task() //无参构造，为了拿任务，不需要参数列表
        {
        }
        //进行函数重载
        Task(int x, int y, char op) //有参构造，制造任务
            : _x(x), _y(y), _op(op)
        {
        }
        ~Task()
        {
        }
        int Run()
        {
            int res = 0;
            switch (_op)
            {
            case '+':
                res = _x + _y;
                break;
            case '-':
                res = _x - _y;
                break;
            case '*':
                res = _x * _y;
                break;
            case '/':
                res = _x / _y;
                break;
            case '%':
                res = _x % _y;
                break;
            default:
                std::cout << "bug?" << std::endl;
                break;
            }
            std::cout << "当前任务正在被：" << pthread_self() << "处理:" << _x << _op << _y << "=" << res << std::endl;
            return res;
        }
        Task operator=(Task &s)
        {
            if (this != &s)
            {
                _x = s._x;
                _y = s._y;
                _op = s._op;
            }
            return *this;
        }
        int operator()()//重载一个函数
        {
            return Run();
        }
    };
}