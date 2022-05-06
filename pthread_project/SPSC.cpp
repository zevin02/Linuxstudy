#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <queue>
#include <string>
#include <stdlib.h>
#include <time.h>
using namespace std;

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
        pthread_cond_signal(&condP);
    }
    element front()
    {
        return _q.front();
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
void *produce(void *arg)
{

    while (true)
    {
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
    }
    return NULL;
}

void *consume(void *arg)
{
    while (true)
    {
        
        char *name = (char *)arg;
        element t = qt.front();
        now -= t.data;
        qt.Pop();
        
        cout << name << " consume " << t.data << "  "
             << "  products "
             << " total =" << now << endl;
    }
}

int main()
{

    srand((size_t)time(nullptr));
    pthread_t consumer, producer;
    pthread_create(&consumer, NULL, consume, (void *)"consumer");
    pthread_create(&producer, NULL, produce, (void *)"producer");
    pthread_join(consumer, NULL);
    pthread_join(producer, NULL);
    return 0;
}