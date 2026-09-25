int get_which_is_free(frames *fra , int type ){
    int where_to_put  = -1 ; 
    int i = 0 ; 
    int temp = 0 ; 
    while (i < frames_in_buf / 8) {
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
    if (where_to_put != -1 ){
        make_it_occupied(fra->mp, where_to_put) ;
        return where_to_put ;
    }
    else { 
        if (type == force ){
            where_to_put = give_the_frame_through_clock(fra, force);
            if(where_to_put == -1 ){
                return -1 ; 
            }
            make_it_occupied(fra->mp, where_to_put) ;
            return where_to_put;
        }
        else { 
            for (int r = 0; r < times; r++) {
                where_to_put = give_the_frame_through_clock(fra , normal );
                if (where_to_put != -1) {
                    make_it_occupied(fra->mp, where_to_put) ;
                    return where_to_put ; 
                } 
                wait_for_the_next(wait_time);  
            }
            if (where_to_put == -1){
                where_to_put = give_the_frame_through_clock(fra , force );
                if (where_to_put != -1 ){
                    make_it_occupied(fra->mp, where_to_put) ;
                    return where_to_put ; 
                }
                else { 
                    return -1 ; 
                }
            }
        }
    }

    return -1  ; 
}

void make_it_free(map_out_frames *map, int thing) {
    int which_num = thing / 8;
    int bit_in_num = thing % 8;
    map->bitmaps[which_num] &= ~(1 << bit_in_num);
}


void in_use(map_out_frames *map, int thing) {  
    int which_num  = thing / 8;
    int bit_in_num = thing % 8;
    map->bitmaps[which_num] |= (1 << bit_in_num);
}


void wait_for_the_next(int time) {
    struct timespec ts;
    ts.tv_sec  = time / 1000;
    ts.tv_nsec = (time % 1000) * 1000000;
    nanosleep(&ts, NULL);
}


int give_the_frame_through_clock(frames *fra  , int type ){
    int num = INT_MAX ; 
    int ind = -1 ; 
    int count = 0 ; 
    if (type == force ){
        for (int i = 0 ; i < frames_in_buf ; i++ ){
            if (fra->dt[i]->valid == 1 ){
                if ( fra->dt[i]->use_count == 0 ){
                    fra->dt[i]->clock_treated_bit-- ; 
                    if (fra->dt[i]->clock_treated_bit < num  ){
                        num = fra->dt[i]->clock_treated_bit  ; 
                        ind = i ; 
                        count = 1 ; 
                    }
                    else if (fra->dt[i]->clock_treated_bit == num ){
                        count++ ; 
                    }
                }
            }
        }
    }

    else {
        for (int i = 0 ; i < frames_in_buf ; i++ ){
            if (fra->dt[i]->valid == 1 ){
                if ( fra->dt[i]->use_count == 0 ){
                    fra->dt[i]->clock_treated_bit-- ; 
                    if (fra->dt[i]->clock_treated_bit == 0  ){
                        ind = i ; 
                        break ; 
                    }
                }
            }
        }
    }


    if (ind == -1 ){
        return -1 ; 
    }

    if( fra->dt[ind]->dirty == 1 ){
        // update the thing in the file 
        fra->dt[ind]->valid = 0  ; 
        fra->dt[ind]->dirty = 0 ; 
        fra->dt[ind]->page_id = /* invalid page id need to put that num */;  
        fra->dt[ind]->use_count = 0  ; 
        fra->dt[ind]->clock_treated_bit = 0  ; 
        make_it_free(fra->mp , ind) ; 
    }
    else{
        fra->dt[ind]->valid = 0  ; 
        fra->dt[ind]->dirty = 0 ; 
        fra->dt[ind]->page_id = /* invalid page id need to put that num */; ;  
        fra->dt[ind]->use_count = 0  ; 
        fra->dt[ind]->clock_treated_bit = 0  ; 
        make_it_free(fra->mp , ind) ; 
    }
    return ind ;
}









