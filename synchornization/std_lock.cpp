#include <iostream>
#include <thread>
#include <mutex>
// Shared resources
int resource1 = 0;
int resource2 = 0;

// Mutexes to protect shared resources
std::mutex mutex1;
std::mutex mutex2;
std::mutex mutex3;
// Function to be executed by threads
void threadFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
        std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
        std::unique_lock<std::mutex> lock3(mutex3, std::defer_lock);
        // Try to lock both mutexes without blocking
        std::lock(lock1,lock2,lock3);
        // Successfully locked both mutexes
        // Critical section: Access the shared resources
        resource1++;
        resource2++;

        
        std::cout << "Thread " << threadId << ": Resource1 = " << resource1
                    << ", Resource2 = " << resource2 << std::endl;


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
