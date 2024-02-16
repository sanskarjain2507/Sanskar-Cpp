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
        // Try to lock both mutexes without blocking
        if (std::try_lock(mutex1,mutex2,mutex3) == -1) {
            // Successfully locked both mutexes
            // Critical section: Access the shared resources
            resource1++;
            resource2++;

            
            std::cout << "Thread " << threadId << ": Resource1 = " << resource1
                      << ", Resource2 = " << resource2 << std::endl;

            // Unlock both mutexes
            mutex1.unlock();
            mutex2.unlock();
            mutex3.unlock();
        } else {
            // Couldn't lock both mutexes, handle accordingly
            mutex3.lock();
            std::cout << "Thread " << threadId << ": Couldn't lock both mutexes on attempt " << i + 1 << std::endl;
            mutex3.unlock();
        }

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
