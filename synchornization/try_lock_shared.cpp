#include <iostream>
#include <thread>
#include <shared_mutex>

// Shared resource
int sharedData = 0;

// Shared mutex to protect shared resource
std::shared_mutex sharedMutex;

// Function to be executed by threads for reading
void readerFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Attempt to acquire a shared lock without blocking
        if (sharedMutex.try_lock_shared()) {
            // Reading the shared resource
            std::cout << "Reader Thread " << threadId << ": Shared Data = " << sharedData << std::endl;

            // Release the shared lock
            sharedMutex.unlock_shared();
        } else {
            // Couldn't acquire the shared lock, handle accordingly
            std::cout << "Reader Thread " << threadId << ": Couldn't acquire shared lock on attempt " << i + 1 << std::endl;
        }

        // Simulate some work being done by the reader thread
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Function to be executed by threads for writing
void writerFunction(int threadId) {
    for (int i = 0; i < 5; ++i) {
        // Attempt to acquire a unique lock without blocking
        if (sharedMutex.try_lock()) {
            // Modifying the shared resource
            sharedData++;
            std::cout << "Writer Thread " << threadId << ": Modified Shared Data = " << sharedData << std::endl;

            // Release the unique lock
            sharedMutex.unlock();
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
    const int numReaders = 2;
    const int numWriters = 2;

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
