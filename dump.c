#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/rt_shm"
#define BYTES_TOTAL (1024 * 768 * 3)

struct shared_memory {
    long pixel_index;
    unsigned char data[BYTES_TOTAL];
};

int main() {
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }

    struct shared_memory *shm = mmap(0, sizeof(*shm), PROT_READ, MAP_SHARED, fd, 0);
    
    // 1. Read the header from the first input image
    FILE *header_src = fopen("imagem1.bmp", "rb");
    if (!header_src) {
        printf("Error: Could not open imagem1.bmp to copy header.\n");
        return 1;
    }
    unsigned char header[54];
    fread(header, 1, 54, header_src);
    fclose(header_src);

    // 2. Write Output
    FILE *fp = fopen("resultado_final.bmp", "wb");
    
    // A. Write Header
    fwrite(header, 1, 54, fp);
    // B. Write Shared Memory Data
    fwrite(shm->data, 1, BYTES_TOTAL, fp);
    
    fclose(fp);

    printf("Success! Saved 'resultado_final.bmp'.\n");
    return 0;
}