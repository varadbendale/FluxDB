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



void toast_tables_insert(char * data ){
    get_toast_table_file_header * toast_header = get_toast_table_file_header() ; 
    int a = 0 ; 
    int size = strlen(data) ; 
    toast * tst ; 
    int offset ; 
    while ( size > 0 ){
        if (toast_header->dead_space > 0 ){
            int page_num = -1  ; 
            while ( a < 128 ){
                uint8_t temp = toast_header->dead_slots[a] ;
                for (int j = 7; j >= 0; j--) {
                    int bit = (temp >> j) & 1;
                    if (bit == 0) {
                        page_num = a * 8 + j ; 
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
                int where_to_put = -1;
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
                pg->free_size = pg->normal_free_size ; 
                pg->slot[pg->slot_num].offset = wonder ; 
                if (size + sizeof(slot) > pg->free_size ){
                    if (size + sizeof(slot) - pg->free_size < 48 ){
                        memcpy(pg->data + wonder , data + offset  , size ) ; 
                        pg->slot[pg->slot_num].size = size ; 
                        pg->free_size = pg->free_size  - size  ; 
                        size  = 0 ; 
                    }
                    else { 
                        memcpy(pg->data + wonder , data + offset  , pg->free_size  ) ; 
                        size = size - pg->free_size ; 
                        pg->slot[pg->slot_num].size = pg->free_size ; 
                        offset = offset + pg->free_size ; 
                        pg->free_size = 0  ; 
                        pg->slot_num++ ; 
                    }
                }
                else{
                    memcpy(pg->data + wonder , data + offset  , size ) ; 
                    pg->slot[pg->slot_num].size = size ; 
                    pg->free_size = pg->free_size  - size  ; 
                    size  = 0 ; 
                }
                

            }
        }

    }



}