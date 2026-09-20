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
    while ( size > 0 ){
        if (toast_header->dead_space > 0 ){
            while ( a < toast_header->pages_used ){
                int page_num = -1  ; 
                uint8_t temp = toast_header->dead_slots[a] ;
                for (int j = 7; j >= 0; j--) {
                    int bit = (temp >> j) & 1;
                    if (bit == 0) {
                        
                        break;
                    }
                }
                
                page * temp = get_page_from_file()
                a++ ; 
                size = size - 
            }
        }

    }



}
