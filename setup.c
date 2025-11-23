#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

#define SHM_NAME "/rt_shm"
#define SEM_NAME "/rt_sem"
// 1024 width * 768 height * 3 bytes (RGB)
#define IMG_BYTES (1024 * 768 * 3) 

struct shared_memory {
    long pixel_index; // Tracks which PIXEL (not byte) we are on
    unsigned char data[IMG_BYTES];
};

int main() {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);

    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open failed"); return 1; }
    
    // Allocate full RGB size + struct overhead
    if (ftruncate(fd, sizeof(struct shared_memory)) == -1) { 
        perror("ftruncate failed"); 
        return 1; 
    }

    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) { perror("sem_open failed"); return 1; }

    printf("Setup Complete: RGB Buffer (%d bytes) created.\n", IMG_BYTES);
    return 0;
}