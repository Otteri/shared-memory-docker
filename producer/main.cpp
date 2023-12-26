#include <iostream>
#include <chrono>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

// Implement block memory version

#define SHM_KEY 0x1234
#define BUF_SIZE 1024
struct shmseg {
   int cnt;
   int complete;
   char buf[BUF_SIZE];
};

int fill_buffer(char * bufptr, int size) {
   static char ch = 'A';
   int filled_count;
   
   //printf("size is %d\n", size);
   memset(bufptr, ch, size - 1);
   bufptr[size-1] = '\0';
   if (ch > 122)
   ch = 65;
   if ( (ch >= 65) && (ch <= 122) ) {
      if ( (ch >= 91) && (ch <= 96) ) {
         ch = 65;
      }
   }
   filled_count = strlen(bufptr);

   ch++;
   return filled_count;
}


int main(int argc, char *argv[])
{
    key_t key;
    int shmid;
    struct shmseg *shmp;

    /* make the key: */
    if ((key = ftok("/dev/shm/hello.txt", 'R')) == -1) /* Here the file must exist */ 
    {
        perror("ftok");
        exit(1);
    }
    std::cout << "key: " << key << std::endl;

    /*  create the segment: */
    if ((shmid = shmget(key, BUF_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    if ((shmp = (shmseg*)shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }


    #define BUF_SIZE 1024
    char *bufptr;
    int spaceavailable;
    int numtimes;
    //struct shmseg *shmp;
    /* Transfer blocks of data from buffer to shared memory */
    bufptr = shmp->buf;
    spaceavailable = BUF_SIZE;
    for (numtimes = 0; numtimes < 1000; numtimes++) {
        shmp->cnt = fill_buffer(bufptr, spaceavailable);
        shmp->complete = 0;
        printf("Writing Process: Shared Memory Write: Wrote %d bytes\n", shmp->cnt);
        bufptr = shmp->buf;
        spaceavailable = BUF_SIZE;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    printf("Writing Process: Wrote %d times\n", numtimes);
    shmp->complete = 1;



    /* detach from the segment: */
    if (shmdt(shmp) == -1) {
        perror("shmdt");
        exit(1);
    }

    // TODO: delete the segment?

    return 0;
}
