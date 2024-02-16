// publisher2.c
#include <zmq.h>
#include <string.h>
#include <unistd.h>
#include "zhelpers.h"

int main() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_PUB);
    int rc=zmq_bind(socket, "tcp://*:6556");
    assert(rc == 0);
    while (1) {
        const char *message = "Message from Publisher 2";
        zmq_send(socket, message, strlen(message), 0);

        sleep(2);
    }

    zmq_close(socket);
    zmq_ctx_destroy(context);

    return 0;
}
