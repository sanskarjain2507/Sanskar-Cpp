#include <iostream>
#include <thread>
#include <shared_mutex>
#include <mutex>
// Shared resource
int sharedData = 0;

// Shared mutex to protect shared resource

std::timed_mutex tm;

// Function to be executed by threads for writing
void writerFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Attempt to acquire a unique lock and block for 1 sec
        if (tm.try_lock_for(std::chrono::seconds(1))) {
            // Modifying the shared resource
            sharedData++;
            std::cout << "Writer Thread " << threadId << ": Modified Shared Data = " << sharedData << std::endl;

            // Release the unique lock
            tm.unlock();
        } else {
            // Couldn't acquire the unique lock, handle accordingly
            std::cout << "Writer Thread " << threadId << ": Couldn't acquire unique lock on attempt " << i + 1 << std::endl;
        }

        // Simulate some work being done by the writer thread
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    // Number of reader and writer threads
    const int numWriters = 2;


    std::thread writerThreads[numWriters];

    // Create writer threads
    for (int i = 0; i < numWriters; ++i) {
        writerThreads[i] = std::thread(writerFunction, i + 1);
    }


    // Wait for writer threads to finish
    for (int i = 0; i < numWriters; ++i) {
        writerThreads[i].join();
    }

    return 0;
}
