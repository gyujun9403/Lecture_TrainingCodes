#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

class Spinlock
{
private:
    
    std::atomic<bool> locked_ = false;
public:
    void lock()
    {

        bool expected = false;
        bool desired = true;
        while (locked_.compare_exchange_strong(expected, desired))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // or std::this_thread::yield();  //실행 대기상태의 동등한 우선순위 이상의 다른 스레드들에게 실행 기회를 양보하는 함수.
            // wait나 block상태로 스레드가 변경되지 않는다 -> 실행 큐 동등한 레벨의 맨 뒤로 이동됨. runable->runable. 우선순위가 더 높은게 없으면 양보 안할 수 있음.
            expected = false;
        }
    }
    void unlocked()
    {
        this->locked_ = false;
    }
};

int sum = 0;
std::mutex m;
Spinlock spinLock;

void Add()
{
    for(int i = 0; i < 1000; i++)
    {
        std::lock_guard<Spinlock> guard(spinLock);
        sum++;
    }
}

void Sub()
{
    for(int i = 0; i < 1000; i++)
    {
        std::lock_guard<Spinlock> guard(spinLock);
        sum--;
    }
}

int main()
{
    
}