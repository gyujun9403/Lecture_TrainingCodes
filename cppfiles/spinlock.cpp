#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

class Spinlock
{
private:
    //bool locked_ = false;
    // volatile :c++에서는 단순히 컴파일러에게 최적화를 하지 말라고 하는 키워드.
    // C#이나 java에서는 추가적인 기능들을 더 포함한다.
    //volatile bool locked_ = false;
    //locked_를 atomic
    std::atomic<bool> locked_ = false;
public:
    void lock()
    {
        // 릴리즈 모드에서는 온갖 최적화가 포함되게 되므로, 
        // locked_가 volatile이 아니면 반복문시 컴파일러가 임의로 조건문을 변경할수 있음.
        //lock확인과, lock설정이 2개의 코드로 따로 설정되어 있다. -> 원자화 필요.
        //  1. lock여부를 확인하는 locked_를 atomic으로 선언한다.
        //  2. 아래의 두 코드를 묶어서 atomical하게 동작하게 하는 std::atomic::compare_exchange_strong으로 대체한다.
        //while (this->locked_  == true);
        //this->locked_ = true; <- atmoic하지 않아서 문제가 발생하는 spinlock
        bool expected = false;
        bool desired = true;
        // CAS Compare And Swap -> 검색필요, https://popcorntree.tistory.com/22?category=813523
        /* compare_exchange_strong는 아래와 같은 동작을 atomic하게 수행한다.
        if (locked_ == expected)
        {
            expected = locked_;
            locked_ = desired;
            return true;
        }
        else
        {
            expected = locked_;
            return false;
        }
        */
        // 일반적으로 compare_exchange_strong는 반복문을 통해 성공할때까지 반복한다.
        if (locked_.compare_exchange_strong(expected, desired))
        {
            // 내부적으로 참조에 의해 expected값이 변경되므로, 
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
        //std::lock_guard<std::mutex> guard(m);
        std::lock_guard<Spinlock> guard(spinLock);
        sum++;
    }
}

void Sub()
{
    for(int i = 0; i < 1000; i++)
    {
        //std::lock_guard<std::mutex> guard(m);
        std::lock_guard<Spinlock> guard(spinLock);
        sum--;
    }
}

int main()
{
    
}