#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
// Shared resource
int sharedData = 0;

// Mutex to protect shared resource
std::mutex m;

// Function to be executed by threads
void threadFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Lock the mutex before accessing the shared resource
        m.lock();
        // Critical section: Access the shared resource
        sharedData++;
        std::cout << "Thread " << threadId << ": Shared Data = " << sharedData << std::endl;
        // Unlock the mutex after accessing the shared resource (lock_guard does this automatically)
        m.unlock();
        sleep(1);
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
