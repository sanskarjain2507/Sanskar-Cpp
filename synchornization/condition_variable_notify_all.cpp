#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mutex;
std::condition_variable cv;
bool isReady = false;

void worker_thread(int id) {
    {
        while(1)
        {
             std::this_thread::sleep_for(std::chrono::seconds(2));
            std::unique_lock<std::mutex> lock(mutex);
            std::cout << "Thread " << id << " is waiting.\n";
            cv.wait(lock, [] { return isReady; });
            std::cout << "Thread " << id << " is awake.\n";
        }
    }
    // Continue with the work...
}

int main() {
    const int num_threads = 3;
    std::thread threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        threads[i] = std::thread(worker_thread, i);
    }

    // Simulate some work...
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        
        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            isReady = false;
            std::unique_lock<std::mutex> lock(mutex);
            isReady = true;
            std::cout << "Notifying all waiting thread.\n";
            cv.notify_all(); // Notify one waiting thread
        }
    }

    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

    return 0;
}
