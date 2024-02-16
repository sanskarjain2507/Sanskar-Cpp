#include <iostream>
#include <thread>
#include <semaphore>

std::counting_semaphore<> mySemaphore(1);  // Allow two threads to access the critical section simultaneously

void criticalSection(int id) {
    mySemaphore.acquire();  // Decrement semaphore value
    // Critical section
    std::cout << "Thread " << id << " is in the critical section.\n";
    // Do some work
    mySemaphore.release();  // Increment semaphore value
}

int main() {
    std::thread t1(criticalSection, 1);
    std::thread t2(criticalSection, 2);

    t1.join();
    t2.join();

    return 0;
}
