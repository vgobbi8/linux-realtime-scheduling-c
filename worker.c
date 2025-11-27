#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <sched.h> 

#define SHM_NAME "/rt_shm"
#define SEM_NAME "/rt_sem"
#define WIDTH 1024
#define HEIGHT 768
#define PIXELS_TOTAL (WIDTH * HEIGHT)
#define BYTES_TOTAL (PIXELS_TOTAL * 3) // RGB

struct shared_memory {
    long pixel_index;
    unsigned char data[BYTES_TOTAL];
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <ID> <BURST_PIXELS> ...\n", argv[0]);
        return 1;
    }
    int id = atoi(argv[1]);
    int burst_pixels = atoi(argv[2]);

    // load BMP file
    char filename[32];
    sprintf(filename, "imagem%d.bmp", id);

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Task %d Error: Cannot open %s\n", id, filename);
        return 1;
    }

    // Allocate local memory for the source image
    unsigned char *local_buffer = malloc(BYTES_TOTAL);
    if (!local_buffer) { perror("malloc"); return 1; }

    // Skip the 54-byte BMP Header
    fseek(fp, 54, SEEK_SET);
    // Read the pixel data
    fread(local_buffer, 1, BYTES_TOTAL, fp);
    fclose(fp);
    
    printf("Task %d: Loaded %s into memory.\n", id, filename);

    //Connect to Shared Memory
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }
    
    struct shared_memory *shm = mmap(0, sizeof(*shm), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) { perror("mmap"); return 1; }

    sem_t *sem = sem_open(SEM_NAME, 0);

    // Real-Time Processing Loop
    while (1) {
        sem_wait(sem); 
        
        if (shm->pixel_index >= PIXELS_TOTAL) {
            sem_post(sem);
            break; 
        }

        long start_pixel = shm->pixel_index;
        long end_pixel = start_pixel + burst_pixels;
        if (end_pixel > PIXELS_TOTAL) end_pixel = PIXELS_TOTAL;
        
        long byte_offset = start_pixel * 3;
        long byte_count = (end_pixel - start_pixel) * 3;

        memcpy(&shm->data[byte_offset], &local_buffer[byte_offset], byte_count);

        shm->pixel_index = end_pixel;

        sem_post(sem);
        sched_yield();
    }
    
    free(local_buffer);
    printf("Task %d Finished.\n", id);
    return 0;
}