char frames[1024][4096] ; 

typedef struct details{
    int valid ; 
    int dirty ; 
    uint32_t page_id ; 
    int people_count ; 
    int ref_bit ; 
}details ; 

typedef struct map_out_frames{
    int *pages ; 
    int *frame ; 
}map_out_frames ; 
