typedef struct leaf_node_data{
    int page_id ; 
    int file_offset ; 
    int slot_num  ; 
}leaf_node_data ; 

typedef struct internal_node{
    leaf_node_data data[340] ; 
    int num_of_leaf ; 
    int primary_key[341] ; 
    char root_compare ; 
    struct internal_node *children[340] ; 
    struct internal_node *next ; 
    bool leaf ; 
}internal_node ; 

