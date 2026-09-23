int get_which_is_free(map_out_frames *map){
    int where_to_put  = -1 ; 
    int i = 0 ; 
    while (i < frames_in_buf) {
        temp = map->bitmaps[i];
        for (int j = 7; j >= 0; j--) {
            int bit = (temp >> j) & 1;
            if (bit == 0) {
                where_to_put = (i * 8) + j;
                break;
            }
        }
        if (where_to_put != -1) {
            break;
        }
        i++ ; 
    }
    if (where_to_put == -1 ){
        clock_it_up(map , where_to_put) ; 
    }
    return where_to_put ; 
}


