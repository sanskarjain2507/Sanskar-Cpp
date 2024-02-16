#include <iostream>
#include <cstring>
#include <mosquitto.h>

// Callback function for handling incoming messages
void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if(message->payloadlen){
        std::cout << "Received message: " << (char *)message->payload << " on topic: " << message->topic << std::endl;
    } else {
        std::cout << "Empty message received on topic: " << message->topic << std::endl;
    }
}

int main() {
    const char *host = "localhost";
    int port = 1883;  // Default MQTT port

    mosquitto_lib_init();

    // Create a new Mosquitto instance
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        std::cerr << "Error: Unable to initialize Mosquitto library" << std::endl;
        return 1;
    }

    // Set the callback function for incoming messages
    mosquitto_message_callback_set(mosq, message_callback);

    // Connect to the MQTT broker
    if (mosquitto_connect(mosq, host, port, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: Unable to connect to the MQTT broker" << std::endl;
        return 1;
    }

    // Subscribe to a topic
    const char *topic = "your_topic";
    if (mosquitto_subscribe(mosq, NULL, topic, 0) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: Unable to subscribe to the topic" << std::endl;
        return 1;
    }

    // Enter the Mosquitto message loop
    mosquitto_loop_start(mosq);

    // Wait for a key press to exit
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    // Cleanup and disconnect
    mosquitto_disconnect(mosq);
    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
