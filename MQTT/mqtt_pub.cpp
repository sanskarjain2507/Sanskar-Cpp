#include <iostream>
#include <cstring>
#include <mosquitto.h>

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

    // Connect to the MQTT broker
    if (mosquitto_connect(mosq, host, port, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Error: Unable to connect to the MQTT broker" << std::endl;
        return 1;
    }

    // Publish a message
    const char *topic = "your_topic";
    const char *message = "Hello from MQTT Publisher!";
    mosquitto_publish(mosq, NULL, topic, strlen(message), message, 0, false);

    // Disconnect
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
