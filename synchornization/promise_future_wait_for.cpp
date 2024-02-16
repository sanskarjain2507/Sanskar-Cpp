#include <iostream>
#include <future>
#include <chrono>
#include <thread>


void asyncTask(std::promise<int>& promiseObj) {
    // Simulate some asynchronous task
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Set the value in promise
    promiseObj.set_value(42);
}

int main() {
    // Create a promise and a future
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();

    // Start an asynchronous task
    std::thread(asyncTask, std::ref(promiseObj)).detach();

    // Wait for the result with a timeout using wait_for
    std::future_status status = futureObj.wait_for(std::chrono::seconds(3));

    if (status == std::future_status::ready) {
        // Result is available
        int result = futureObj.get();
        std::cout << "Result: " << result << std::endl;
    } else if (status == std::future_status::timeout) {
        std::cout << "Timeout occurred. Task not completed yet." << std::endl;
    }

    return 0;
}
