#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mutex;
std::condition_variable cv;
bool isReady = false;

void worker_thread() {
    // Work...
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        isReady = true;
    } // lock goes out of scope, releasing the associated mutex
    
    cv.notify_one(); // Notify one waiting thread
}

int main() {
    std::thread t(worker_thread);

    {
        std::unique_lock<std::mutex> lock(mutex);
        auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(2);
        if (cv.wait_until(lock, timeout, [] { return isReady; })) {
            std::cout << "Condition met within 2 seconds.\n";
        } else {
            std::cout << "Timeout reached.\n";
        }
    }

    // Continue with the main thread...

    t.join();
    return 0;
}
