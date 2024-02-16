#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>

using namespace std;

int main() {
    // Ensure unique key for reader 2
    key_t key = ftok("shmfile1", 66);

    // Check for errors in key generation
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666);

    // Check for errors in shared memory creation
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // shmat to attach to shared memory
    char* str = (char*)shmat(shmid, nullptr, 0);

    // Check for errors in attaching to shared memory
    if (str == (char*)-1) {
        perror("shmat");
        return 1;
    }

    // Reader 2
    cout << "Reader 2 - Data read from memory: " << str << endl;

    // Detach from shared memory
    if (shmdt(str) == -1) {
        perror("shmdt");
        return 1;
    }

    return 0;
}