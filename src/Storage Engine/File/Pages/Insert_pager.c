struct_for_insert(itree * tree ){
    int error ; 
    insert_page * ins ; 
    if (strcmp(tree->comp , "INSERT INTO" )== 0 ){
        if (tree->ignore == 1 ){
            error = 1 ; 
        }
        else { 
            error = 0 ; 
        }
    }
    else if (strcmp(tree->comp , "REPLACE INTO" ) == 0 ){
        error = 2 ; 
    }
    info_detail * temp_info; 
    pagezero * header = // fucntion to read the page header 
    table_info * temp_table = NULL ; 
    if (tree->table != NULL ){
        if (ins->table_num > 0 ){
            int temp = 0 ; 
            while (temp < tree->table_num){
                if (strcmp(tree->table , ins->info[temp]->table ) == 0 ){
                    temp_info = ins->info[temp] ; 
                }
                else { 
                    ins->table_num++ ; 
                    ins->info[ins->table_num]->table = strdup(tree->table) ; 
                    ins->info[ins->table_num]->row_num = 0 ; 
                }
                temp++ ; 
            }
            temp = 0 ; 
            while (temp < header.num_tables){
                if (strcmp(header.tables[temp].table_name , tree->table  ) == 0 ){
                    temp_table = header.tables[temp] ; 
                    break ; 
                }
                temp++ ; 
            }
        }
    }
    char ** temp_columns = calloc(128, sizeof(char *));
    int heck = 0 ; 
    int hash_1[307] ; 
    int hash_2[307] ; 
    int position_org[307] ; 
    int position_written[307] ; 
    if (table->table_col_num > 0 ){
        int temp_1 = 0 ; 
        int temp_2 = 0 ; 
        while ( temp_1 < temp_table.num_columns ){
            int num = hash_num(temp_table.columns[temp_1].col_name) ; 
            hash_1[num] = num ; 
            position_org[num] = temp_1 ; 
            temp_1++ ;    
        }
        while ( temp_2 < table->table_col_num ){
            int num = hash_num( table->table_cols[temp_2]) ; 
            position[num] = temp_2 ; 
            position_written[temp_2] = num ; 
            temp_2++ ;    
        }
        temp_1 = 0 ; 
        while (temp_1 < temp_table.num_columns  ){
            int num = hash_num(temp_table.columns[temp_1].col_name) ; 
            if (hash_2[num] != num ){
                if (temp_table.columns[temp_1].default_stuff == true ){
                    char cleaned[32];
                    sscanf(temp_table.default_val, " %31[^\t\n ]", cleaned);
                    temp_column[heck++] = strdup(cleaned);
                }
                else { 
                    if (temp_table.columns[temp_1].not_null == true ){
                        return NULL ; 
                    }
                    else { 
                        heck++ ; 
                    }
                }
            }
        }

    }
    char ** col_temp = calloc(128, sizeof(char *)); 
    int d = 0  ; 
    int counter = 0 ; 
    while ( d < tree->num ){
        if (strcmp(tree->children[d]->comp ,"VALUE_ROW" ) == 0 ){
            itree * temp_tree = tree->children[d] ; 
            int e = 0 ; 
            int col_num = 0 ; 
            while (e < temp_tree->num){
                col_temp[hash_1[hash_2[hash_num(temp_tree->children[e]->comp)]]] = strdup(temp_tree->children[e]->comp) ; 
                e++ ; 
            }
            int f = 0 ; 
            while ( f < temp_table.num_columns){
                if (col_temp[f] == NULL ){
                   col_temp[f] = temp_column[counter++] ; 
                }
                f++ ; 
            }
        }
        d++ ; 
    }





}