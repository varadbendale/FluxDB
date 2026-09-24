#define frames_in_buf 1024 

typedef struct frames{
    char frames[frames_in_buf][4096] ; 
    details *dt[frames_in_buf] ; 
}frames ; 


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
    uint8_t bitmaps[frames_in_buf] ; 
    int free_bits ; 
}map_out_frames ; 