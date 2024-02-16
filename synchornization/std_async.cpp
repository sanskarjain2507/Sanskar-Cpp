#include <iostream>
#include <future>
#include <thread>

int someFunction() {
    // Simulate some time-consuming operation
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main() {
    // Launch someFunction asynchronously
    std::future<int> resultFuture = std::async(std::launch::async, someFunction);

    // Do other work concurrently

    // Wait for the result and retrieve it
    int result = resultFuture.get();

    // Use the result
    std::cout << "Result: " << result << std::endl;

    return 0;
}
