#include <iostream>
#include <thread>
#include <mutex>
#include <bits/stdc++.h>
// Shared resource
int sharedData = 0;

// Mutex to protect shared resource
std::mutex mutex;

// Function to be executed by threads
void threadFunction(int threadId,std::string &s) 
{
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "Thread " << threadId <<": ";
        // Critical section: Access the shared resource
        int i=3;
        while(i--)
        {
            std::cout << s[sharedData] ;
            sharedData++;
            sharedData%=s.length();
            
        }
        std::cout<<"\n";
        // lock is automatically released when std::lock_guard goes out of scope
}


int main() {
    // Number of threads
    const int numThreads = 100;

    // Thread objects
    std::thread threads[numThreads];
    std::string s= "abcdefghijklmnopqrstuvwxyz";
    // Create threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(threadFunction, i + 1,std::ref(s));
    }

    // Wait for threads to finish
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    return 0;
}
