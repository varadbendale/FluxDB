page *get_page_from_file( int offset ){
    FILE *file = fopen(/*"name"*/, "rb");
    if (file == NULL ) {
        return NULL  ;
    }
    char buffer[4096];
    fseek(file, offset, SEEK_SET);
    size_t bytes_in_page = fread(buffer, 1, 4096, file);
    if (bytes_in_page == 0 ){
        return NULL ; 
    }
    page * ans = malloc(sizeof(page)); 
    if (ans == NULL ){
        return NULL ; 
    } 
    ans->header = malloc(sizeof(page_header_struct));
    memcpy(ans->header, buffer, sizeof(page_header_struct));
    ans->slot = malloc(sizeof(slots) * ans->header->num);
    memcpy(ans->slot, buffer + sizeof(page_header_struct), sizeof(slots) * ans->header->num);
    int data_offset = sizeof(page_header_struct) + (sizeof(slots) * ans->header->num);
    int data_size = 4096 - data_offset;
    ans->data = malloc(data_size);
    memcpy(ans->data, buffer + data_offset, data_size);
    return ans;
}

int find_the_size_of_stuff(char * data ){
    int size  = 0 ; 
    
}

void FSM(char * data , int page_num , int offset){
    page * pg = get_page_from_file(offset) ; 
    if (pg->header->page_num != page_num  ){
        return  ; 
    }
    find_the_size_of_stuff( data ) ; 
}
