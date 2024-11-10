#include <iostream>
#include <string>
#include <curl/curl.h>

// Callback function to handle the response
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *buffer) {
    buffer->append((char *)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string response_buffer;

    // Initialize cURL
    curl = curl_easy_init();
    if (curl) {
        // Set the URL of your API
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");

        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);

        // Perform the HTTP GET request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Print the response
            std::cout << "Response: " << response_buffer << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL." << std::endl;
    }

    return 0;
}
