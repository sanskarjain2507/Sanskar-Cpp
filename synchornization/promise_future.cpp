#include <iostream>
#include <thread>
#include <future>

void setValue(std::promise<int>& p) {
    // Set a value in the promise
    p.set_value(42);
}

int main() {
    std::promise<int> myPromise;
    std::future<int> myFuture = myPromise.get_future();

    std::thread t(setValue, std::ref(myPromise));

    // Get the value from the future
    int result = myFuture.get();

    std::cout << "Received from future: " << result << std::endl;

    t.join();
    return 0;
}
