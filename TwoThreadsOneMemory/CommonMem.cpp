#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

static std::mutex mutex;

void func1(int* x)
{
    //std::lock_guard<std::mutex> lk(mutex);
    for (int i = 0; i < 1000; ++i)
    {
        ++(*x);
        //std::cout << "   Thread1 " << (*x)++ << std::endl;
    }
}

void func2(int* x)
{
    //std::lock_guard<std::mutex> lk(mutex);
    for (int i = 0; i < 1000; ++i)
    {
        ++(*x);
        //std::cout << "        Thread2 " << (*x)++ << std::endl;
    }
}

int main()
{
    int x = 0;
    do
    {
        x = 0;
        std::thread th1(func1, &x);
        std::thread th2(func2, &x);
    
        th1.join();
        th2.join();

        std::cout << x << std::endl;
    }
    while (x == 2000);

   std::cout << "\nResult " << x << std::endl;
}
