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


/*
1  →  disk I/O primitives first
         read_from_disk()
         write_to_disk()
         because everything else needs these

2  →  flush_page()
         uses write_to_disk
         clock needs this for dirty eviction

3  →  fetch_page()
         uses read_from_disk
         uses get_which_is_free
         uses flush via clock
         your main read path

4  →  unpin_page()
         simple, just decrements pin count
         marks dirty if modified

5  →  new_page()
         allocate on disk
         bring into frame immediately
         reuses fetch logic

6  →  delete_page()
         remove from buffer pool
         remove from disk

*/ 


