// Implement block memory version

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
struct shmseg {
   int cnt;
   int complete;
   char buf[SHM_SIZE];
};

int main(int argc, char *argv[])
{
    key_t key;
    int shmid;
    int mode;
    struct shmseg *shmp;


    /* make the key: */
    // if ((key = ftok("/dev/shm/hello.txt", 'R')) == -1) /* Here the file must exist */ 
    // {
    //     perror("ftok");
    //     exit(1);
    // }
    // std::cout << "key: " << key << std::endl;

    /*  create the segment: */
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {  // kley, 0644
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    if ((shmp = (shmseg*)shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    /* Transfer blocks of data from shared memory to stdout*/
    while (shmp->complete != 1) {
        printf("segment contains : \n\"%s\"\n", shmp->buf);
        if (shmp->cnt == -1) {
            perror("read");
            return 1;
        }
        printf("Reading Process: Shared Memory: Read %d bytes\n", shmp->cnt);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    printf("Reading Process: Complete\n");

    /* detach from the segment: */
    if (shmdt(shmp) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
