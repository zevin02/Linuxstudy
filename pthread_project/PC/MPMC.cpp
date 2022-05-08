
#define P 6
#define C 5

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <queue>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <cstring>
using namespace std;

pthread_mutex_t cm; // 放在生产者之间的互斥锁
pthread_mutex_t pd;
class element
{
public:
    int data;     //放进去多少元素
    long long id; //生产者or消费者名字
                  //现在缓冲区里有多少元素
};
class SPSCQueue //阻塞队列
{

public:
    explicit SPSCQueue(int capacity)
        : _capacity(capacity)
    {
        pthread_cond_init(&condC, NULL);
        pthread_cond_init(&condP, NULL);
        pthread_mutex_init(&mutex, NULL);
    }
    ~SPSCQueue()
    {

        pthread_cond_destroy(&condC);
        pthread_cond_destroy(&condP);
        pthread_mutex_destroy(&mutex);
    }
    void Push(element in)
    {
        //

        pthread_mutex_lock(&mutex);
        while (_q.size() == _capacity)
            pthread_cond_wait(&condP, &mutex);
        _q.push(in);
        pthread_mutex_unlock(&mutex);
        //在这里我们不睡眠，要出现不规则的情况
        pthread_cond_signal(&condC);
    }
    void Pop()
    {

        pthread_mutex_lock(&mutex);
        while (_q.empty())
        {
            pthread_cond_wait(&condC, &mutex);
        }

        _q.pop();

        pthread_mutex_unlock(&mutex);
        usleep(10);
        pthread_cond_signal(&condP);
    }
    element front()
    {
        return _q.front();
    }
    int capacity()
    {
        return _capacity;
    }
    bool empty()
    {
        return _q.empty();
    }

private:
    queue<element> _q;
    pthread_cond_t condC;
    pthread_cond_t condP;
    pthread_mutex_t mutex;
    size_t _capacity;
};

// template<class T>//我们把里面的类型都定义成t
int now = 0;
SPSCQueue qt(100);
pthread_cond_t condd;
void *produce(void *arg)
{

    while (true)
    {
        pthread_mutex_lock(&pd);
        while (now == qt.capacity())
        {
            pthread_cond_wait(&condd, &pd);
        }
        static int cnt = 0;
        element t;
        int x = rand() % 50 + 1;
        t.data = x;
        now += x;
        t.id = pthread_self();
        char *name = (char *)arg;

        qt.Push(t);
        cnt++;

        cout << name << " id=" << t.id << " produce " << t.data << " products"
             << " total =" << now << endl;
        pthread_mutex_unlock(&pd);
    }
    // return NULL;
}

void *consume(void *arg)
{
    while (true)
    {
        pthread_mutex_lock(&cm);
        while (qt.empty())
        {
            pthread_cond_wait(&condd, &cm);
        }
        char *name = (char *)arg;
        element t = qt.front();
        now -= t.data;
        qt.Pop();

        cout << name << " consume " << t.data << "  "
             << "  products "
             << " total =" << now << endl;
        pthread_mutex_unlock(&cm);
        pthread_cond_signal(&condd);
    }
    // return NULL;
}

int main()
{

    pthread_t p[5];
    pthread_t c[5];
    memset(p, 0, sizeof(p));
    memset(c, 0, sizeof(c));
    pthread_mutex_init(&pd, NULL);
    pthread_mutex_init(&cm, NULL);
    pthread_cond_init(&condd, NULL);
    for (int i = 0; i < 5; i++)
    {

        int ret = pthread_create(&p[i], NULL, produce, (void *)(i));
        if (ret != 0)
        {
            perror("pthread_create");
        }
    }
    for (int i = 0; i < 5; i++)
    {

        int ret = pthread_create(&c[i], NULL, consume, (void *)(i));
        if (ret != 0)
        {
            perror("pthread_create");
        }
    }

    for (int i = 0; i < 5; i++)
    {
        void *val;
        int ret = pthread_join(p[i], &val);
        if (ret != 0)
        {
            printf("return code :%d\n", *(int *)ret);
        }
    }
    cout << "1" << endl;

    for (int i = 0; i < 5; i++)
    {
        void *val;
        int ret = pthread_join(c[i], &val);
        if (ret != 0)
        {
            printf("return code :%d\n", *(int *)ret);
        }
    }
    pthread_mutex_destroy(&pd);
    pthread_mutex_destroy(&cm);
    pthread_cond_destroy(&condd);
    return 0;
}