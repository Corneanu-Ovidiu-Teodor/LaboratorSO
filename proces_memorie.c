#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_NAME "/shared_memory_example"
#define SEM_NAME "/semaphore_example"
#define MAX_COUNT 1000

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *shared_count = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    if (fork() == 0) { 
        while (*shared_count < MAX_COUNT) {
            sem_wait(semaphore);
            if (rand() % 2 == 1) {
                (*shared_count)++;
                printf("Child: %d\n", *shared_count);
            }
            sem_post(semaphore);
            usleep(100);
        }
    } else { 
        while (*shared_count < MAX_COUNT) {
            sem_wait(semaphore);
            if (rand() % 2 == 1) {
                (*shared_count)++;
                printf("Parent: %d\n", *shared_count);
            }
            sem_post(semaphore);
            usleep(100);
        }
        wait(NULL);
        munmap(shared_count, sizeof(int));
        shm_unlink(SHM_NAME);
        sem_close(semaphore);
        sem_unlink(SEM_NAME);
    }
    return 0;
}

