#include <iostream>
#include <chrono>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 0x1234
#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char *argv[])
{
    int shmid;
    char *data;

    /*  create the segment: */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    if ((data = (char*)shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    std::string str1;
    /* read or modify the segment, based on the command line: */
    for (int i = 0; i < 100; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::string str1 = "Iteration: " + std::to_string(i);
        std::cout << "[Producer] Writing: " << str1 << std::endl;
        strncpy(data, str1.c_str(), SHM_SIZE);
    }

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
