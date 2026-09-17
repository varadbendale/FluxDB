page *get_page_from_file( int offset , char *filename  ){
    FILE *file = fopen(/*"name"*/, "rb");
    if (file == NULL ) {
        return NULL  ;
    }
    char dat[4096];
    fseek(file, offset, SEEK_SET);
    size_t bytes_in_page = fread(dat, 1, 4096, file);
    if (bytes_in_page == 0 ){
        return NULL ; 
    }
    page * ans = malloc(sizeof(page)); 
    if (ans == NULL ){
        return NULL ; 
    } 
    ans->header = malloc(sizeof(page_header_struct));
    memcpy(ans->header, dat, sizeof(page_header_struct));
    ans->slot = malloc(sizeof(slots) * ans->header->num);
    memcpy(ans->slot, dat + sizeof(page_header_struct), sizeof(slots) * ans->header->num);
    int data_offset = sizeof(page_header_struct) + (sizeof(slots) * ans->header->num);
    int data_size = 4096 - data_offset;
    ans->data = malloc(data_size);
    memcpy(ans->data, dat + data_offset, data_size);
    return ans;
}




int find_the_size_of_stuff(char ** data  , table_info * table  , char * ans  ){
    int size  = 0 ; 
    int i = 0 ;
    int j = 0 ; 
    while ( i < table.num_columns){
        if (table.columns[i].col_type == INT || table.columns[i].col_type == BIGINT  || table.columns[i].col_type == SMALLINT  || table.columns[i].col_type == TINYINT    ){
            size = size + strlen(table.columns[i].type_bits_count) ; 
            ans[j] = strdup(integer_num) ; 
            j++ ; 
            ans[j] = strdup(table.columns[i].col_name) ; 
            j++ ; 
        }   
        else if (table.columns[i].col_type == FLOAT || table.columns[i].col_type == DOUBLE   ){
            size = size + strlen(table.columns[i].type_bits_count) ; 
            ans[j] = strdup(real_num) ; 
            j++ ; 
            ans[j] = strdup(table.columns[i].col_name) ; 
            j++ ; 
        }   
        else if (table.columns[i].col_type == CHAR  || table.columns[i].col_type == VARCHAR   ){
            ans[j] = strdup(string_num) ; 
            j++ ; 
            ans[j] = strdup(strlen(table.columns[i].col_name)) ; 
            j++ ; 
            if (strlen(table.columns[i].col_name) > 1024 ){
                //assign the toast table 
                size = size + 32 ; 
            }
            else { 
                ans[j] = strdup(table.columns[i].col_name) ; 
                size = size + strlen(table.columns[i].col_name) ; 
                j++ ; 
            }
        }   
        else if (table.columns[i].col_type == DATE   ){
            size = size + strlen(table.columns[i].type_bits_count) ;
            ans[j] = strdup(date_num) ; 
            j++ ; 
            ans[j] = strdup(table.columns[i].col_name) ; 
            j++ ;  
        }
        else if (table.columns[i].col_type == TIMESTAMP){
            size = size + strlen(table.columns[i].type_bits_count) ; 
            ans[j] = strdup(timestamp_num) ; 
            j++ ; 
            ans[j] = strdup(table.columns[i].col_name) ; 
            j++ ;  
        }
        i++ ; 
    }
    return size ; 
    
}

pagezero *get_pagezero(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;
    }
    char dat[4096];
    size_t bytes_read = fread(dat, 1, 4096, file);
    fclose(file);
    if (bytes_read == 0) {
        return NULL;
    }
    pagezero *ans = malloc(sizeof(pagezero));
    if (ans == NULL) {
        return NULL;
    }
    memcpy(ans, dat, sizeof(pagezero));
    return ans;
}

void FSM(char * data , int page_num , int offset , table_info * table ){
    page * pg  ; 
    char * ans ; 
    int size = 0 ; 
    if (offset > 0 ){
        pg = get_page_from_file(offset) ; 
        if (pg->header->page_num != page_num  ){
            return  ; 
        }
        size = find_the_size_of_stuff( data  , table  , &ans ) ; 
        if (size < pg->header->free_size ){
            if (size < pg->header->normal_free_size){
                slots * temp_slot = malloc(sizeof(slots)) ; 
            }
            else { 
                //rearrange ; 
            }
        }
        else { 
            //split 
        }
    }




}