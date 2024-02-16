// subscriber.c
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include "zhelpers.h"
int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_SUB);

    // Connect to both publishers
    zmq_connect(socket, "tcp://localhost:6555");
    zmq_connect(socket, "tcp://localhost:6556");

    // Subscribe to all messages
    int rc=zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
     assert(rc == 0);
    while (1) {
        char buffer[256];
        zmq_recv(socket, buffer, sizeof(buffer)-1, 0);
        buffer[sizeof(buffer)-1] = '\0';
        printf("Received: %s\n", buffer);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}
