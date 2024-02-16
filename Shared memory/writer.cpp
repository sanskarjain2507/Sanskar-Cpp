#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include<unistd.h>

using namespace std;

int main() {
    // Ensure unique keys by using distinct files
    key_t key = ftok("shmfile", 65);
    key_t key1 = ftok("shmfile1", 66);

    // Check for errors in key generation
    if (key == -1 || key1 == -1) {
        perror("ftok");
        return 1;
    }

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    int shmid1 = shmget(key1, 1024, 0666 | IPC_CREAT);

    // Check for errors in shared memory creation
    if (shmid == -1 || shmid1 == -1) {
        perror("shmget");
        return 1;
    }

    // shmat to attach to shared memory
    char* str = (char*)shmat(shmid, nullptr, 0);
    char* str1 = (char*)shmat(shmid1, nullptr, 0);

    // Check for errors in attaching to shared memory
    if (str == (char*)-1 || str1 == (char*)-1) {
        perror("shmat");
        return 1;
    }

    cout << "Write Data for reader1 : ";
    cin.getline(str, 1024);

    cout << "Data written in memory for reader1: " << str << endl;

    cout << "Write Data for reader2 : ";
    cin.getline(str1, 1024);

    cout << "Data written in memory for reader2: " << str1 << endl;
    sleep(100);
    // Detach from shared memory
    if (shmdt(str) == -1 || shmdt(str1) == -1) {
        perror("shmdt");
        return 1;
    }

    // destroy the shared memory
    if (shmctl(shmid, IPC_RMID, nullptr) == -1 || shmctl(shmid1, IPC_RMID, nullptr) == -1) {
        perror("shmctl");
        return 1;
    }

    return 0;
}