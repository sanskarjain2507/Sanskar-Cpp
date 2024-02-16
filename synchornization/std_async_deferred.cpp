#include <iostream>
#include <future>

int someFunction() {
    std::cout << "Executing someFunction" << std::endl;
    return 42;
}

int main() {
    // Launch someFunction with std::launch::deferred
    std::future<int> resultFuture = std::async(std::launch::deferred, someFunction);

    // Other work can be done here

    // Wait for the result and retrieve it
    int result = resultFuture.get();

    // Use the result
    std::cout << "Result: " << result << std::endl;

    return 0;
}
