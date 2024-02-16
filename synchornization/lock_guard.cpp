#include <iostream>
#include <thread>
#include <mutex>

// Shared resource
int sharedData = 0;

// Mutex to protect shared resource
std::mutex mutex;

// Function to be executed by threads
void threadFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // std::lock_guard is used to automatically lock and unlock the mutex
        std::lock_guard<std::mutex> lock(mutex);

        // Critical section: Access the shared resource
        sharedData++;
        std::cout << "Thread " << threadId << ": Shared Data = " << sharedData << std::endl;

        // lock is automatically released when std::lock_guard goes out of scope
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
