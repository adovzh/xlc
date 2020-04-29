#include <iostream>
#include <thread>
#include <blocking_queue.h>

void consumer(xlc::blocking_queue<int>& queue, std::string name)
{
    int x;

    while (queue.poll(x, 100)) {
        std::cout << name << ": " << x << std::endl;
    }

    std::cout << name << " finished." << std::endl;
}

int main()
{
    using namespace xlc;

    try {
        blocking_queue<int> q(222);

        std::cout << "Capacity: " << q.capacity() << std::endl;
        for (int i = 1; i < 100; i++) {
            std::cout << std::boolalpha << "Offer " << i << ": " << q.offer(i, 100) << std::endl;
        }

        std::thread t1(consumer, std::ref(q), std::move(std::string("Thread 1")));
        std::thread t2(consumer, std::ref(q), std::move(std::string("Thread 2")));

        t1.join();
        t2.join();
    }
    catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error!" << std::endl;
    }


    return 0;
}




