#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Enable the SO_REUSEADDR option
    int option = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR |  SO_REUSEPORT, &option, sizeof(option)) == -1) {
        perror("Error setting SO_REUSEADDR option");
        close(serverSocket);
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(2511); // You can choose any available port

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    listen(serverSocket, 5);

    std::cout << "Server listening on port 2508..." << std::endl;

    // while (true) {
        // Accept a connection
        sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);

        if (clientSocket == -1) {
            perror("Error accepting connection");
            close(serverSocket);
            return -1;
        }
        std::cout<<"Accepted connection..\n";
        // Fork a new process to handle the client
        pid_t childPid = fork();

        if (childPid == -1) {
            perror("Error in fork()");
            close(clientSocket);
            close(serverSocket);
            return -1;
        }

        if (childPid == 0) {
            // This code is executed in the child process (handles client connection)
            close(serverSocket); // Child doesn't need the listening socket

            // Handle the client connection (you can put your logic here)
            const char* message = "Hello, client! You are being served by the child process.";
            send(clientSocket, message, strlen(message), 0);

            // Close the client socket
            close(clientSocket);

            // Terminate the child process
            exit(0);
        } else {
            // This code is executed in the parent process
            close(clientSocket); // Parent doesn't need the client socket
        }
    // }

    // Close the listening socket (this part won't be reached in the example)
    close(serverSocket);

    return 0;
}
