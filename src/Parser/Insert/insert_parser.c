
insert_parser_tree *createNodeInsert(char *comp) {
    insert_parser_tree *node = malloc(sizeof(insert_parser_tree));  
    if (node == NULL) {
        return NULL;
    }
    if (comp != NULL ){
        node->comp = strdup(comp) ; 
    }
    else if (comp == NULL ){
        node->comp = NULL;   
    }
    node->table = NULL;
    node->table_cols = NULL;
    node->insert_values = NULL;
    node->children = calloc(300, sizeof(insert_parser_tree *)); 
    if (node->children == NULL) {         
        free(node);
        return NULL;
    }
    node->num = 0;
    node->table_col_num = 0;
    node->insert_val_num = 0;
    node->ignore = 0;
    return node;
}

itree * insert_parser(){
    char ***buf = proper_data.query ; 
    int row = 0  ; 
    int col = 0 ; 
    int i = 0 ; 
    int j = 0  ; 
    int temp = 0 ; 
    while (buf[i][0] != NULL ){
        j = 0 ; 
        while (buf[i][j] != NULL ){
            j++ ; 
        }
        if (j > max ){
            max = j ; 
        }
        i++ ; 
    }
    end_row  = i ; 
    end_col = j ; 
    i = row ; 
    j = col ; 
    itree * start = NULL ; 
    itree * node = NULL ;
    while ( i <= end_row ){
        while ( j <= end_col){
            if (i == row && j == col ){
                if ( strcmp(buf[i][j], "INSERT")==0   ){
                    int check = 0 ; 
                    if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                    }   
                    if ( strcmp(buf[i][j], "IGNORE")==0   ){ 
                        temp = 1  ; 
                    }
                    if (strcmp(buf[i][j], "INTO")==0  ){
                        node = createNodeInsert("INSERT INTO") ; 
                        start = node ; 
                    }
                    if (check == 1 && temp == 1 ){
                        node->ignore = 1 ; 
                    }

              }	
              else if (strcmp(buf[i][j], "REPLACE")==0  ){
                    if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                    }   
                    if (strcmp(buf[i][j], "INTO")==0  ){
                        node = createNodeInsert("REPLACE INTO") ; 
                        node->replace = 1 ; 
                        start = node ; 
                    }
              }
              else { 
                //error ;   
              }
            }
            if ( buf[i][j+1] == NULL ){
                    if (i+1 <= end_row){
                        i = i+ 1 ; 
                        j = 0  ; 
                    }
                }
            else { 
                j++ ; 
            }  
            if (buf[i][j] != NULL ){
                node->table = strdup(buf[i][j] ) ; 
            }

            if ( buf[i][j+1] == NULL ){
                if (i+1 <= end_row){
                    i = i+ 1 ; 
                    j = 0  ; 
                }
            }
            else { 
                j++ ; 
            }  

            if (strcmp(buf[i][j] , "(" ) == 0  ){
                if ( buf[i][j+1] == NULL ){
                    if (i+1 <= end_row){
                        i = i+ 1 ; 
                        j = 0  ; 
                    }
                }
                else { 
                    j++ ; 
                }  
                while (strcmp(buf[i][j] , ")" ) == 0 ){
                    node->table_cols[node->table_col_num++] = strdup(buf[i][j]) ; 
                    if ( buf[i][j+1] == NULL ){
                        if (i+1 <= end_row){
                            i = i+ 1 ; 
                            j = 0  ; 
                        }
                    }
                    else { 
                        j++ ; 
                    }  

                }
            }
            else if ( strcmp(buf[i][j] , "SELECT" ) == 0 ){
                // select work needs to be done  ; 
            }

            else if (strcmp(buf[i][j] , "VALUES" ) == 0  ){
                if ( buf[i][j+1] == NULL ){
                    if (i+1 <= end_row){
                        i = i+ 1 ; 
                        j = 0  ; 
                    }
                }
                else { 
                    j++ ; 
                }  
                if (strcmp(buf[i][j] , "SELECT" ) == 0  ){
                    
                }

                while (strcmp(buf[i][j] , "(" ) == 0  ){
                    if ( buf[i][j+1] == NULL ){
                        if (i+1 <= end_row){
                            i = i+ 1 ; 
                            j = 0  ; 
                        }
                    }
                    else { 
                        j++ ; 
                    }  
                    int kim = 0 ; 
                    while (kim < node->table_col_num ){
                        if (1){
                            if (strcmp(buf[i][j] , "(" ) == 0  ){
                                if (strcmp(buf[i][j] , "SELECT" ) == 0  ){
                                    //work on it okay 
                                    continue ; 
                                }
                                else { 
                                    //error
                                }
                            }
                            else { 
                                node->children[node->num++]->insert_values[kim] = strdup(buf[i][j]) ; 
                            }
                        }
                        if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                        }  
                        if (strcmp(buf[i][j] , ",") == 0 ){
                            if ( buf[i][j+1] == NULL ){
                                if (i+1 <= end_row){
                                    i = i+ 1 ; 
                                    j = 0  ; 
                                }
                            }
                            else { 
                                j++ ; 
                            }  
                        }
                        kim++ ; 
                    }
                    if (strcmp(buf[i][j] , ")") == 0 ){
                        if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                        }  
                    }
                    if (strcmp(buf[i][j] , ",") == 0 ){
                        if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                        }  
                    }
                }
            }
            else if (strcmp(buf[i][j] , "ON") == 0 ){
                if ( buf[i][j+1] == NULL ){
                    if (i+1 <= end_row){
                        i = i+ 1 ; 
                        j = 0  ; 
                    }
                }
                else { 
                    j++ ; 
                }  
                if (strcmp(buf[i][j] , "DUPLICATE") == 0 ){
                    if ( buf[i][j+1] == NULL ){
                        if (i+1 <= end_row){
                            i = i+ 1 ; 
                            j = 0  ; 
                        }
                    }
                    else { 
                        j++ ; 
                    }   
                    if (strcmp(buf[i][j] , "KEY") == 0 ){
                        if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                        }  
                        if (strcmp(buf[i][j] , "UPDATE") == 0 ){
                            if ( buf[i][j+1] == NULL ){
                                if (i+1 <= end_row){
                                    i = i+ 1 ; 
                                    j = 0  ; 
                                }
                            }
                            else { 
                                j++ ; 
                            }  
                            node->children[node->num++] = createNodeInsert("ON DUPLICATE KEY UPDATE") ; 
                            
                        }
                    }
                    else { 
                        //error 
                    }
                }
                else { 
                    //error ; 
                }
            }
            else if (strcmp(buf[i][j] , "RETURNING") == 0 ){
                node->children[node->num++] = strdup("RETURNING") ; 
                node = node->children[node->num -1 ] ; 
                if ( buf[i][j+1] == NULL ){
                    if (i+1 <= end_row){
                        i = i+ 1 ; 
                        j = 0  ; 
                    }
                }
                else { 
                    j++ ; 
                }  
                while (strcmp(buf[i][j] , ";") == 0 ){
                    node->children[node->num++] = createNodeInsert(buf[i][j]) ; 
                    if ( buf[i][j+1] == NULL ){
                        if (i+1 <= end_row){
                            i = i+ 1 ; 
                            j = 0  ; 
                        }
                    }
                    else { 
                        j++ ; 
                    }  
                }

            }

        }
    }
}
