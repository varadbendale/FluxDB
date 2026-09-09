typedef struct info{
    int hash[307] ; 
    int num ; 
    int primary_key[307] ; 
    int pri_num ; 
}


int  hash_num (const char *str) {
    int  hash = 5381;
    int c;

    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;  
    }

    return hash % 307 ;
}

void prepared_the_intel_hash(info * intel){
    for (int i = 0 ; i < 307 ; i++ ){
        intel[i] = 0 ; 
        primary_key[i] = 0 ; 
    }
}

void take_care_of_stuff(ctree * tree , info * intel){
    prepared_the_intel_hash(intel) ; 
    int num ; 
    while (i < tree->num ){
        if (strcmp(tree->children[i]->comp , "PRIMARY KEY") == 0 ){
            int j = 0 ; 
            while (j < tree->children[i]->num ){
                num =  hash_num(tree->children[i]->children[j]->comp ) ;  
                intel->primary_key[num] = num ; 
                intel->pri_num++ ; 
            }
        }
        else if (strcmp(tree->children[i]->comp , "FOREIGN KEY") == 0 ){

        }
        else { 
            num =  hash_num(tree->children[i]->comp ) ; 
            intel->hash[num] = num ; 
            intel->num++ ; 
        }
    }
}



void main_create_parser(){
    ctree * tree = create_parser() ; 
    take_care_of_stuff(tree) ; 
    tree = tree->children[tree->num -1 ] ; 
    pagezero * pg ; 
    table_info * ti  ; 
    strcpy(ti->table_name , tree->comp , sizeof(tree->comp )) ; 
    int i = 0 ; 
    while ( i < tree->num ){

    }
}