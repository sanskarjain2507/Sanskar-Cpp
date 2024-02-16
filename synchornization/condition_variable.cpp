#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
int balance = 0;

void worker_thread() {
    // Work...
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        balance = 500;
        std::cout<<"balance added"<<balance<<"\n";
    } // lock goes out of scope, releasing the associated mutex
    
    cv.notify_one(); // Notify one waiting thread
}

int main() {
    std::thread t(worker_thread);

    {
        std::unique_lock<std::mutex> lock(mutex);
        std::cout<<"waiting for lock..\n";
        cv.wait(lock, [] { return balance>0?true:false; }); // Wait until isReady becomes true
        std::cout<<"balance reported:"<<balance<<"\n";
    }

    // Continue with the main thread...

    t.join();
    return 0;
}
