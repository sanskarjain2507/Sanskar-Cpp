#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

// Shared resource
int sharedData = 0;

// Mutex to protect shared resource
std::mutex mutex;

// Function to be executed by threads
void threadFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // std::unique_lock is used with deferred locking strategy
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);

        // Perform some other work before acquiring the lock
        // ...

        // Acquire the lock explicitly
        lock.lock();

        // Critical section: Access the shared resource
        sharedData++;
        std::cout << "Thread " << threadId << ": Shared Data = " << sharedData << std::endl;

        // Unlock the mutex manually
        lock.unlock();  //if this is not given then lock will be automatically unlocked when its scope ends

        // Perform some other work after releasing the lock
        // ...

        // Simulate some work being done by the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Number of threads
    const int numThreads = 3;

    // Thread objects
    std::thread threads[numThreads];

    // Create threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(threadFunction, i + 1);
    }

    // Wait for threads to finish
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    return 0;
}
