#include <iostream>
#include <thread>
#include <mutex>

// Shared resource
int sharedData = 0;

// Recursive mutex to protect shared resource
std::recursive_mutex recursiveMutex;

// Function to be executed by threads
void threadFunction(int threadId, int recursionDepth) {
    // Lock the recursive mutex
   recursiveMutex.lock();

    // Modify the shared resource
    sharedData++;
    std::cout << "Thread " << threadId << ": Shared Data = " << sharedData << " (Recursion Depth = " << recursionDepth << ")" << std::endl;

    // Recursive call to the same function
    if (recursionDepth > 0) {
        threadFunction(threadId, recursionDepth - 1);
    }

    // Unlock the recursive mutex
    // Note: Each lock must be matched by an equal number of unlock operations
    recursiveMutex.unlock();
}

int main() {
    // Number of threads
    const int numThreads = 3;

    // Thread objects
    std::thread threads[numThreads];

    // Create threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(threadFunction, i + 1, 2);  // Specify recursion depth as 2
    }

    // Wait for threads to finish
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    return 0;
}
