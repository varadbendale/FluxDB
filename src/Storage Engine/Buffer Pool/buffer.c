int get_which_is_free(frames *fra){
    int where_to_put  = -1 ; 
    int i = 0 ; 
    int temp = 0 ; 
    while (i < frames_in_buf) {
        temp = fra->bitmaps[i];
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
        where_to_put = clock_it_up(fra) ; 
    }
    return where_to_put ; 
}

void make_it_free(map_out_frames *map, int thing) {
    int which_num = thing / 8;
    int bit_in_num = thing % 8;
    map->bitmaps[which_num] &= ~(1 << bit_in_num);
}

void clock_it_up(frames *fra){
    int num = INT_MAX ; 
    int ind = 0 ; 
    int max_count = 0 ; 
    for (int i = 0 ; i < frames_in_buf ; i++ ){
        if ( fra->dt[i]->use_count == 0 ){
            fra->dt[i]->clock_treated_bit-- ; 
            if (fra->dt[i]->clock_treated_bit < num  ){
                num = fra->dt[i]->clock_treated_bit  ; 
                ind = i ; 
                count = 1 ; 
            }
            else if (fra->dt[i]->clock_treated_bit == num ){
                max_count++ ; 
            }
        }
    }



}


