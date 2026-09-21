get_toast_table_file_header *get_toast_table_file_header() {
    FILE *file = fopen(/*filename*/, "rb");
    if (file == NULL) {
        return NULL;
    }
    char dat[4096];
    size_t bytes_read = fread(dat, 1, 4096, file);
    fclose(file);
    if (bytes_read == 0) {
        return NULL;
    }
    get_toast_table_file_header *ans = malloc(sizeof(get_toast_table_file_header));
    if (ans == NULL) {
        return NULL;
    }
    memcpy(ans, dat, sizeof(get_toast_table_file_header));
    return ans;
}


void write_toast(toast * t, long offset) {
    FILE *file = fopen(/*filename*/, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(fp, offset, SEEK_SET);
    fwrite(t, sizeof(toast), 1, fp);
}


void put_the_chunk_in_the_list(toast_table_file_header * toast_header , int  chunk_id , int page_num , int slot_num){
    toast_header->num_of_lines_in_map++ ; 
    toast_header->map_out_toast[chund_id].chunk_id = chunk_id ; 
    toast_header->map_out_toast[chund_id].offset_of_the_same[ toast_header->map_out_toast[chund_id].offset_number++ ] = toast_header->num_of_lines_in_map ; 
    toast * tst ; 
    tst->chunk_id = chunk_id ; 
    tst->page = page_num ; 
    tst->slot_number = slot_num ; 
    write_toast( tst , toast_header->num_of_lines_in_map  ) ; 
}

void toast_tables_insert(char * data ){
    toast_table_file_header * toast_header = get_toast_table_file_header() ; 
    int a = 0 ; 
    int size = strlen(data) ; 
    toast * tst ; 
    int offset ; 
    int chunk_id = toast_header->total_num_of_chunks + 1 ; 
    while ( size > 0 ){
        if (toast_header->dead_space > 0 ){
            int page_num = -1  ; 
            int first = 0 ; 
            int second = 0 ; 
            while ( a < 128 ){
                uint8_t temp = toast_header->dead_slots[a] ;
                for (int j = 7; j >= 0; j--) {
                    int bit = (temp >> j) & 1;
                    if (bit == 0) {
                        page_num = a * 8 + j ; 
                        first = i ; 
                        second = j ; 
                        break;
                    }
                }
                if (page_num != -1 ){
                    break ; 
                }
                else { 
                    a++ ; 
                }
            }
            if (page_num == -1 ){
                // error ; 
            }
            else { 
               page * pg = get_page_from_file( page_num * 4096 , filename ) ; 
                int i = 0;
                int j = 0;
                int k = 0;
                int wonder = 0 ; 
                int temp_slot_num = pg->slot_num / 8;
                uint8_t temp;
                while (i < temp_slot_num) {
                    temp = pg->header->dead_slots[i];
                    for (int j = 7; j >= 0; j--) {
                        int bit = (temp >> j) & 1;
                        if (bit == 1) {
                            pg->slot[i*8+j].offset = wonder ; 
                            wonder = wonder + pg->slot[i*8+j].size ; 
                            i++  ; 
                        }
                        else { 
                            i++ ; 
                            continue ; 
                        }
                    }
                }
                int b = 0 ; 
                int temp_pointer = 0 ; 
                while ( b < pg->slot_num ){
                    int first = b / 8 ; 
                    int second  = 7 - (b % 8);
                    if( (pg->header->dead_slots[first] >> second) & 1  == 1 ){
                        if (pg->slot[b].offset != temp_pointer ){
                            memmove(  pg->data + temp_pointer , pg->data + pg->slot[b].offset  , pg->slot[b].size)
                        }
                        temp_pointer  = temp_pointer + pg->slot[b].size ; 
                        b++ ; 
                    }
                    else { 
                        b++
                        continue ; 
                    }
                }
                pg->free_size = pg->normal_free_size ; 
                pg->slot_num++ ; 
                pg->slot[pg->slot_num].offset = wonder ; 
                int temp_slot_num ; 
                if (size + sizeof(slot) > pg->free_size ){
                    if (size + sizeof(slot) - pg->free_size < 48 ){
                        memcpy(pg->data + wonder , data + offset  , size ) ; 
                        pg->slot[pg->slot_num].size = size ; 
                        pg->free_size = pg->free_size  - size  ; 
                        size  = 0 ; 
                        toast_header->dead_space = toast_header->dead_space - size ; 
                        temp_slot_num = pg->slot_num / 8;
                        pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                        if (pg->free_size < 612 ){
                            int index = second ;
                            int bit_pos = 7 - index;
                            temp |= (1 << bit_pos);
                            pg->header->dead_slots[k] = temp;
                            toast_header->dead_space_available[first] = temp ; 
                        } 
                        put_the_chunk_in_the_list(toast_header , chunk_id , page_num , pg->slot_num) ; 
                    }
                    else { 
                        memcpy(pg->data + wonder , data + offset  , pg->free_size  ) ; 
                        size = size - pg->free_size ; 
                        pg->slot[pg->slot_num].size = pg->free_size ; 
                        offset = offset + pg->free_size ; 
                        pg->free_size = 0  ; 
                        toast_header->dead_space = toast_header->dead_space - pg->free_size  ; 
                        int index = second ;
                        int bit_pos = 7 - index;
                        temp |= (1 << bit_pos);
                        pg->header->dead_slots[k] = temp;
                        toast_header->dead_space_available[first] = temp ; 
                        put_the_chunk_in_the_list( toast_header , chunk_id , page_num , pg->slot_num ) ; 
                    }
                }
                else{
                    memcpy(pg->data + wonder , data + offset  , size ) ; 
                    pg->slot[pg->slot_num].size = size ; 
                    pg->free_size = pg->free_size  - size  ; 
                    size  = 0 ; 
                    toast_header->dead_space = toast_header->dead_space - size ; 
                    temp_slot_num = pg->slot_num / 8;
                    pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                    if (pg->free_size < 612 ){
                        int index = second ;
                        int bit_pos = 7 - index;
                        temp |= (1 << bit_pos);
                        pg->header->dead_slots[k] = temp;
                        toast_header->dead_space_available[first] = temp ; 
                    }
                    put_the_chunk_in_the_list(toast_header , chunk_id , page_num , pg->slot_num) ; 
                }
            }
        }
        else { 
            toast_header->pages_used++ ; 
            if (size - offset > 2752){
                page * pg  = malloc(sizeof(page)); 
                pg->header->page_num = toast_header->pages_used ; 
                pg->header->page_type = toast ; 
                pg->slot_num = 0 ; 
                pg->slots[ pg->slot_num]->offset = 0 ; 
                pg->slots[ pg->slot_num]->size = 0 ; 
                pg->slot_num++
                memcpy(pg->data , data + offset , size - offset ) ; 
                toast_header->dead_space = toast_header->dead_space + ( 2752 - (size - offset ) ) ; 
                page->header->free_size = page->header->free_size + ( 2752 - (size - offset ) ) ; 
                int temp_slot_num = toast_header->pages_used / 8 ;
                toast_header->dead_space_available[temp_slot_num] = (toast_header->dead_space_available[temp_slot_num]<< 1) | 1;
                int temp_slot_num = pg->slot_num / 8;
                pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                size = 0 ; 
                put_the_chunk_in_the_list(toast_header , chunk_id , pg->header->page_num  , 0  ) ; 
            }
            else { 
                page * pg  = malloc(sizeof(page)); 
                pg->header->page_num = toast_header->pages_used ; 
                pg->header->page_type = toast ; 
                pg->slot_num = 0 ; 
                pg->slots[ pg->slot_num]->offset = 0 ; 
                pg->slots[ pg->slot_num]->size = 0 ; 
                pg->slot_num++
                memcpy(pg->data , data + offset , size - offset ) ; 
                if ( 2752 - (size - offset ) >= 612 ){
                    toast_header->dead_space = toast_header->dead_space + ( 2752 - (size - offset ) ) ; 
                    page->header->free_size = page->header->free_size + ( 2752 - (size - offset ) ) ; 
                    int temp_slot_num = toast_header->pages_used / 8 ;
                    toast_header->dead_space_available[temp_slot_num] = (toast_header->dead_space_available[temp_slot_num]<< 1) & -1 ;
                    int temp_slot_num = pg->slot_num / 8;
                    pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) & ~1;
                }
                else { 
                    page->header->free_size = page->header->free_size + ( 2752 - (size - offset ) ) ; 
                    int temp_slot_num = toast_header->pages_used / 8 ;
                    toast_header->dead_space_available[temp_slot_num] = (toast_header->dead_space_available[temp_slot_num]<< 1) | 1  ;
                    int temp_slot_num = pg->slot_num / 8;
                    pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                }
                size = 0 ; 
                put_the_chunk_in_the_list(toast_header , chunk_id , pg->header->page_num  , 0  ) ; 
            }

        }
    }
}


