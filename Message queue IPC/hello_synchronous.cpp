#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 256

// Structure for the message
struct message {
    long mtype; // Message type (must be greater than 0)
    char mtext[MAX_MESSAGE_SIZE]; // Message content
};

// Function to send a message
void sendMessage(int msgid, const char *messageText) {
    struct message msg;
    msg.mtype = 1; // Message type, can be any positive integer

    strncpy(msg.mtext, messageText, sizeof(msg.mtext));
    sleep(5);
    // Send the message
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Process: Sent message - %s\n", messageText);
}

// Function to receive a message
void receiveMessage(int msgid) {
    struct message msg;
    printf("in child\n");
    // Receive the message
    // sleep(5);
    while (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT) == -1) {
        perror("msgrcv");
        sleep(1);
        // exit(EXIT_FAILURE);
    }

    printf("Process: Received message - %s\n", msg.mtext);
}

int main() {
    key_t key = ftok("/tmp", 'A'); // Generate a key for the message queue

    int msgid = msgget(key, 0666 | IPC_CREAT); // Create or open the message queue

    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // Create a child process

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process

        sendMessage(msgid, "Hello from Parent!");

        // Wait for the child process to finish
        wait(NULL);

        // Remove the message queue
        if (msgctl(msgid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }
    } else {
        // Child process
        receiveMessage(msgid);
        exit(EXIT_SUCCESS); // Exit after receiving the message
    }

    return 0;
}
