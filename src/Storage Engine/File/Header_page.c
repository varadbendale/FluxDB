int hash_num( char *str){
    int hash = 5381;
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
        else { 
            num =  hash_num(tree->children[i]->comp ) ; 
            if (intel->hash[num] != num ){
                 intel->hash[num] = num ; 
            }
            else { 
                //error ; 
            }

            intel->num++ ; 
        }
    }
}

void do_the_foreign_key(ctree * tree  , table_info * ti ){
    int i = 0 ; 
    while ( i < tree->foreign_key_counter ){
        column_info * temp ; 
        temp->col_name = strdup(tree->foreign_key[i]) ; 
        temp->foreign_key_table  = strdup(tree->children[tree->num - 1 ]) ; 
        temp->foreign_key_column = strdup(tree->children[tree->num - 1 ]->children[i]->comp)  ; 
        ti->columns[ti->num_columns++] = temp ; 
    }
} 


void main_create_parser( pagezero * pg ; ){
    ctree * tree = create_parser() ; 
    tree = tree->children[tree->num -1 ] ; 
    pg->db_magic = 0xF1ACDB ; 
    pg->version = 1 ; 
    pg->page_size = 4096 ; 
    table_info * ti  ; 
    ti->table_name = strdup(tree->comp) ; 
    int i = 0 ; 
    info * intel ; 
    take_care_of_stuff(tree , intel ) ; 
    ti->num_columns = intel->num ; 
    int stuff  = 0 ; 
    while ( i < tree->num ){
        column_info * col ; 
        int num = hash_num(tree->children[tree->num -1 ]->comp) ; 
        if ( intel->hash[num] == num ){
            col->col_name = strdup(tree->comp) ;   
        }
        if (tree->primary_key_counter == 1 ){
            col->primary_key = true ; 
        }
        if (intel->primary_key[num] == num ){
            col->primary_key = true ; 
        }
        if (tree->not_null == true ){
            col->not_null = true ; 
        }
        if (tree->unique ){
            col->unique = true ; 
        }
        if (1){
            if (tree->type != -1 ){
                col->col_type = tree->type ; 
                col->type_bits_count = tree->type_bits_count ; 
                if (tree->type == DECIMAL ){
                    col->first_decimal  = tree->first_decimal ; 
                    col->second_decimal = tree->second_decimal ; 
                }
            }
            else { 
                //error ; 
            }
        }
        if (tree->default_val != NULL ){
            col->default_stuff = true ; 
            col->default_val = tree->default_val ; 
        }

        if (strcmp(tree->comp , "FOREIGN KEY") == 0 ){
            do_the_foreign_key(tree , &ti) ; 
            stuff = 1 ; 
        }
        if (stuff == 0 ){
            ti->columns[ti->num_columns++] = col ; 
        }

        i++ ; 
    }
    pg->tables[pg->num_tables++] = ti ; 
}