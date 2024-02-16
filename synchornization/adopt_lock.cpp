#include <iostream>
#include <thread>
#include <mutex>

// Shared resource
int sharedData = 0;

// Mutex to protect shared resource
std::mutex mutex;

// Function to be executed by threads
void threadFunction(int threadId) {
    // Assume ownership of the already locked mutex
    std::unique_lock<std::mutex> lock(mutex, std::adopt_lock);

    // Critical section: Access the shared resource
    sharedData++;
    std::cout << "Thread " << threadId << ": Shared Data = " << sharedData << std::endl;

    // lock is automatically released when std::unique_lock goes out of scope
}

int main() {
    // Number of threads
    const int numThreads = 3;

    // Lock the mutex before starting threads
    mutex.lock();

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

    // Mutex is already locked, explicitly unlock it
    mutex.unlock();

    return 0;
}
