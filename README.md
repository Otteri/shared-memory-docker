# shared-memory-docker

Producer writes every second and consumer reads every two seconds.
So, if the program runs smoothly, then you are likely to see consumer printing only every other number.


Build command:
docker compose up --build producer consumer

Assumes that you are working on Linux system

* Easy to test and minimal

* Realize that there is limited amount of RAM installed to the system
  and we need to share the host system RAM to the docker containers.


* We use the 'older' shm system call, because it integrates well with docker.
  shm essentially creates a file to `/dev/shm` (RAM memory) and we can easily
  bind mount /dev/shm from host to the docker containers.

## Deps
* docker
* docker-compose

## Used inside container
* CMake


Reference: https://datawookie.dev/blog/2021/11/shared-memory-docker/


* https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_shared_memory.htm
* https://stackoverflow.com/questions/5656530/how-to-use-shared-memory-with-linux-in-c