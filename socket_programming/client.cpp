#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Set timeout for connecting
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;

    if (setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1 ||
        setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("Error setting timeout");
        close(clientSocket);
        return -1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    serverAddress.sin_port = htons(8890); // Server port

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
            // Connection is still in progress (non-blocking socket), handle timeout or continue as needed
            std::cerr << "Connection timeout or in progress." << std::endl;
        } else {
            perror("Error connecting to server");
            close(clientSocket);
            return -1;
        }
    } else {
        // Connected successfully
        std::cout << "Connected to the server." << std::endl;

        // Continue with sending and receiving data as needed
        char buffer[1024] = {0};
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead == -1) {
            perror("Error receiving data");
            close(clientSocket);
            return -1;
        }

    std::cout << "Client  received from Server : " << buffer << std::endl;
        sleep(10);
        send(clientSocket , "bye.." , strlen("bye..") , 0 ); 
        memset(&buffer[0],0,sizeof(buffer));
         bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
         if (bytesRead == -1) {
            perror("Error receiving data");
            close(clientSocket);
            return -1;
        }
        std::cout << "Client  received from Server : " << buffer << std::endl;
        // Close the client socket when done
        close(clientSocket);
    }

    return 0;
}
