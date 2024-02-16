#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_MESSAGE_SIZE 256

volatile sig_atomic_t flag = 0;

// Structure for the message
struct message {
    long mtype; // Message type (must be greater than 0)
    char mtext[MAX_MESSAGE_SIZE]; // Message content
};

// Function to handle signals
void signalHandler(int signo) {
    if (signo == SIGUSR1) {
        flag = 1;
    }
}

// Function to send a message asynchronously
void sendMessageAsync(int msgid, const char *messageText) {
    struct message msg;
    msg.mtype = 1; // Message type, can be any positive integer

    strncpy(msg.mtext, messageText, sizeof(msg.mtext));

    // Send the message
    if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Process: Sent message asynchronously - %s\n", messageText);
}

// Function to receive a message asynchronously
void receiveMessageAsync(int msgid) {
    struct message msg;

    // Wait for the signal to receive the message
    while (!flag) {
        sleep(1);
    }

    // Receive the message
    if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Process: Received message asynchronously - %s\n", msg.mtext);
}

int main() {
    key_t key = ftok("/tmp", 'A'); // Generate a key for the message queue

    int msgid = msgget(key, 0666 | IPC_CREAT); // Create or open the message queue

    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Set up the signal handler
    signal(SIGUSR1, signalHandler);

    pid_t pid = fork(); // Create a child process

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process
        sendMessageAsync(msgid, "Hello from Parent!");

        // Send signal to the child to receive the message
        kill(pid, SIGUSR1);

        // Wait for the child process to finish
        wait(NULL);
    } else {
        // Child process
        receiveMessageAsync(msgid);
        exit(EXIT_SUCCESS); // Exit after receiving the message
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
