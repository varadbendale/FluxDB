#include<storage_engine.h>


int db_open(const char *filename) {
    return open(filename, O_RDWR | O_CREAT, 0644);
}

void write(int fd, int page_num, uint8_t *buf) {
    ssize_t size = pwrite(fd, buf, PAGE_SIZE, page_num * PAGE_SIZE);
    if (size != PAGE_SIZE ){
        // error ; 
        exit(1) ; 
    }
}

void read(int fd, int page_num, uint8_t *buf) {
    ssize_t size = pread(fd, buf, PAGE_SIZE, page_num * PAGE_SIZE);
    if (size != PAGE_SIZE ){
        // error ; 
        exit(1) ; 
    }
}

void close(int fd) {
    close(fd);
}



