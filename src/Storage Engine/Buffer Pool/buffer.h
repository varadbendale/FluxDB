#define frames_in_buf 1024 
#include <time.h> 
#include <time.h>
#include <stdint.h>
#define frames_in_buf 1024
#define page_size 4096
#define times 3 
#define wait_time 15

typedef enum {
    force = 200 ,
    normal 
} frame_retrieve_type;

typedef struct details{
    int valid ; 
    int dirty ; 
    uint32_t page_id ; 
    int use_count ; 
    int clock_treated_bit ; 
}details ; 



typedef struct map_out_frames{
    int *pages_hash ; 
    int *frame_hash ; 
    uint8_t bitmaps[frames_in_buf/8] ; 
    int free_bits ; 
}map_out_frames ; 



typedef struct frames{
    char frames[frames_in_buf][4096] ; 
    details * dt[frames_in_buf] ; 
    map_out_frames * mp ; 
}frames ; 








