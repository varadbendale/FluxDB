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
    map->frame_page_hash[thing] = -1 ; 
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
        page *pg = (page *)fra->frames[ind];
        fra->dt[ind]->dirty = 0 ; 
        write_to_disk( pg->header.page_id , ind ,fra ) ; 
    }

    fra->dt[ind]->valid = 0  ; 
    fra->dt[ind]->dirty = 0 ; 
    fra->dt[ind]->page_id = /* invalid page id need to put that num */; ;  
    fra->dt[ind]->use_count = 0  ; 
    fra->dt[ind]->clock_treated_bit = 0  ; 
    make_it_free(fra->mp , ind) ; 
    return ind ;
}


void read_from_disk( uint32_t page_id, int frame_index, frames *fra) {
    int fd = open(/* normal page file */, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        return ;
    }
    lseek(fd, page_id * PAGE_SIZE, SEEK_SET);
    read(fd, fra->frames[frame_index], PAGE_SIZE);
}

void write_to_disk( uint32_t page_id, int frame_index, frames *fra) {
    int fd = open(/* normal page file */, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        return ;
    }
    lseek(fd, page_id * PAGE_SIZE, SEEK_SET);
    write(fd, fra->frames[frame_index], PAGE_SIZE);
}


int get_the_page_in_the_frame(frames * fra  , int page_num ){
    if (fra->mp->frame_page_hash[page_num] != -1 ){
        fra->dt[where_to_put_frame]->use_count++ ;
        fra->dt[where_to_put_frame]->clock_treated_bit++ ;
        return fra->mp->frame_page_hash[page_num]  ; 
    }
    else { 
        int where_to_put_frame = get_which_is_free(fra, normal);
        if (where_to_put_frame == -1) {
            return -1 ;  
        }
        read_from_disk( page_id , where_to_put_frame , fra);
        fra->dt[where_to_put_frame]->page_id = page_id;
        fra->dt[where_to_put_frame]->valid = 1;
        fra->dt[where_to_put_frame]->dirty = 0;
        fra->dt[where_to_put_frame]->use_count = 1;
        fra->dt[where_to_put_frame]->clock_treated_bit = 1;
        fra->mp->frame_page_hash[page_id] = where_to_put_frame;
        return where_to_put_frame ; 
    }
}


void not_in_use_get_out(int changed , int frame_id , frames * fra ){
    fra->dt[frame_id]->use_count-- ; 
    fra->dt[frame_id]->dirty = 0 ;
    if (changed == 1 ){
        fra->dt[frame_id]->dirty = 1 ;
    }
    if (fra->dt[frame_id]->use_count == 0 ){
        fra->dt[frame_id]->valid = 0 ;
    }
}

void insert_the_thing( page * pg  , frames * fra ){
    int frame_num  = get_which_is_free( fra  ,  normal  ) ; 
    fra->dt[frame_num]->page_id = pg->header.page_num;
    fra->dt[frame_num]->valid = 1 ;
    fra->dt[frame_num]->dirty = 0 ;
    fra->dt[frame_num]->use_count = 0 ; 
    fra->dt[frame_num]->clock_treated_bit = 0 ;
    memcpy(fra->frames[frame_num], pg, v);
    fra->mp->frame_page_hash[page_id] = frame_num ;
    write_to_disk(pg , -1 ) ; 
}

void delete_the_page(int page_id , frames * fra  ){
        char zeros[page_size];
        memset(zeros, 0, page_size);
        lseek(fd, page_id * page_size, SEEK_SET);
        write(fd, zeros, page_size) ;
        fra->dt[fra->mp->frame_page_hash[page_num] ]->page_id = pg->header.page_num;
        fra->dt[fra->mp->frame_page_hash[page_num] ]->valid = 1 ;
        fra->dt[fra->mp->frame_page_hash[page_num] ]->dirty = 0 ;
        fra->dt[fra->mp->frame_page_hash[page_num] ]->use_count = 0 ; 
        fra->dt[fra->mp->frame_page_hash[page_num] ]->clock_treated_bit = 0 ;
}



void delete_the_page(int fd, uint32_t page_id, frames *fra) {
    int frame_index = fra->mp->frame_page_hash[page_id];
    if (fra->dt[frame_index]->use_count > 0){
        return ;
    }
    char zeros[page_size];
    memset(zeros, 0, page_size);
    lseek(fd, page_id * page_size, SEEK_SET);
    write(fd, zeros, page_size);
    fra->dt[frame_index]->valid = 0 ;
    fra->dt[frame_index]->dirty = 0 ;
    fra->dt[frame_index]->page_id = INVALID_PAGE_ID ;
    fra->dt[frame_index]->use_count = 0 ;
    fra->dt[frame_index]->clock_treated_bit = 0 ;
    fra->mp->frame_page_hash[page_id] = -1;
    make_it_free(fra->mp, frame_index);
}






