#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>

// Shared resource
int sharedData = 0;

// Shared mutex to protect shared resource
std::shared_mutex sharedMutex;

// Function to be executed by threads for reading
void readerFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Shared lock for reading
        std::shared_lock<std::shared_mutex> lock(sharedMutex);
        // Reading the shared resource
        std::cout << "Reader Thread " << threadId << ": Shared Data = " << sharedData << std::endl;

        // Release the shared lock automatically when std::shared_lock goes out of scope
    }
}

// Function to be executed by threads for writing
void writerFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Upgrade to unique lock for writing
        std::unique_lock<std::shared_mutex> lock(sharedMutex);

        // Modifying the shared resource
        sharedData++;
        std::cout << "Writer Thread " << threadId << ": Modified Shared Data = " << sharedData << std::endl;

        // Release the unique lock automatically when std::unique_lock goes out of scope
    }
}

int main() {
    // Number of reader and writer threads
    const int numReaders = 5;
    const int numWriters = 5;

    // Thread objects for readers and writers
    std::thread readerThreads[numReaders];
    std::thread writerThreads[numWriters];

    // Create reader threads
    for (int i = 0; i < numReaders; ++i) {
        readerThreads[i] = std::thread(readerFunction, i + 1);
    }

    // Create writer threads
    for (int i = 0; i < numWriters; ++i) {
        writerThreads[i] = std::thread(writerFunction, i + 1);
    }

    // Wait for reader threads to finish
    for (int i = 0; i < numReaders; ++i) {
        readerThreads[i].join();
    }

    // Wait for writer threads to finish
    for (int i = 0; i < numWriters; ++i) {
        writerThreads[i].join();
    }

    return 0;
}
