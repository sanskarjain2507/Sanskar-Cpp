#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    // Create a pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork to create a child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) { // Parent process
        // Close the read end of the pipe as it won't be used by the parent
        close(pipe_fd[0]);

        // Message to be sent to the child process
        std::string message = "Hello from the parent!";

        // Write the message to the pipe
        // sleep(10);
        while(1)
        {
            cout<<"writing..\n";
            if (write(pipe_fd[1], message.c_str(), message.size()) == -1) {  
                perror("write");
                return 1;
            }
        }
         

        // Close the write end of the pipe after writing
        close(pipe_fd[1]);
    } else { // Child process
        // Close the write end of the pipe as it won't be used by the child
        close(pipe_fd[1]);

        // Buffer to read the message
        char buffer[256];

        // Read from the pipe
        while(1)
        {
        cout<<"waiting for message..\n";
        sleep(1);
        ssize_t bytesRead = read(pipe_fd[0], buffer, sizeof(buffer)); //read will be blocked until received some msg

        if (bytesRead == -1) {
            perror("read");
            return 1;
        }

        // Null-terminate the received message
        buffer[bytesRead] = '\0';

        // Print the message received from the parent
        std::cout << "Child process received: " << buffer << std::endl;
        }
        // Close the read end of the pipe after reading
        close(pipe_fd[0]);
    }

    return 0;
}
