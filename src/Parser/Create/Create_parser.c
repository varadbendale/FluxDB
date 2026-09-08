typedef struct Create_tree_def {
    char * comp ; 
    int type ; 
    int type_bits_count ; 
    int first_decimal ; 
    int second_decimal ; 
    int  primary_key_counter  ; 
    char ** primary_key ; 
    bool not_null ; 
    char *  default ; 
    bool unique ; 
    int  foreign_key_counter ; 
    char ** foreign_key ; 
    int references ; 
    int line;                 
    int col;
    struct Create_tree_def** children;   
    int num ; 
    tree * as ; 
}

typedef Create_tree_def ctree ; 

enum type_of_cols {
    INT  = 50 ,    
    BIGINT ,  
    SMALLINT,   
    TINYINT,     
    FLOAT,      
    DOUBLE ,     
    CHAR ,  
    VARCHAR, 
    BOOL ,
    DATE ,
    TIMESTAMP 
} ;



ctree* createNode( char* comp) {
    ctree* node = malloc(sizeof(ctree));
    if (node == NULL) {
        return NULL;   
    }
    if( comp != NULL  ){
        node->comp = strdup(comp) ; 
    }
    else { 
        node->comp = NULL ; 
    }
    node->as = NULL ; 
    node->children = calloc(300, sizeof(ctree*));
    node->references = 0 ; 
    node->direction = NULL ;  
    node->num = 0  ; 
    node->col = 0 ; 
    node->line = 0 ; 
    node->type = 0 ; 
    node->type_bits_count = 0 ; 
    node->first_decimal = 0 ; 
    node->second_decimal = 0 ; 
    node->primary_key = NULL ; 
    node->foreign_key_counter = 0 ; 
    node->primary_key_counter = 0 ; 
    node->foreign_key = NULL  ; 
    node->not_null = false  ; 
    node->default = NULL  ; 
    node->unique = false  ; 
    return node;
}

bool validate_default(char *default_val, int type) {
    switch (type) {
        case INT:
        case BIGINT:
        case SMALLINT:
        case TINYINT: {
            int start = (default_val[0] == '-') ? 1 : 0;
            for (int i = start; i < strlen(default_val); i++) {
                if (!isdigit(default_val[i])){
                    return false;
                } 
            }
            return true;
        }

        case FLOAT:
        case DOUBLE: {
            int dots = 0;
            int start = (default_val[0] == '-') ? 1 : 0;
            for (int i = start; i < strlen(default_val); i++) {
                if (default_val[i] == '.') {
                    dots++;
                    if (dots > 1) {
                        return false;
                    }
                }
                else if (!isdigit(default_val[i])) {
                    return false;
                }
            }
            return true;
        }

        case BOOL: {
            return (strcmp(default_val, "true") == 0  ||strcmp(default_val, "false") == 0 ||strcmp(default_val, "1") == 0     || strcmp(default_val, "0") == 0);
        }

        case CHAR:
        case VARCHAR:
        case DATE:
        case TIMESTAMP:
            return true;

        default:
            return false;
    }
}


ctree* create_parser(int row , int col  , int endrow , int end_col ){
    ctree* treq = NULL  ; 
    char ***buf = proper_data.query ; 
    int i = row ; 
    int j = col ; 
    ctree * start = NULL ; 
    ctree * node = NULL 
    while ( i <= end_row ){
        while ( j <= end_col){
            if (check == 1 ){
                return NULL ; 
            }
            if (i == row && j == col ){
                if ( strcmp(buf[i][j], "CREATE")==0   ){
                    if ( buf[i][j+1] == NULL ){
                            if (i+1 <= end_row){
                                i = i+ 1 ; 
                                j = 0  ; 
                            }
                        }
                        else { 
                            j++ ; 
                    }   
                    if (strcmp(buf[i][j], "TABLE")==0 ){
                        node =  createNode("CREATE TABLE") ; 
                        start = node ; 
                        if (1){
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
                        if (1) {               
                            if (buf[i][j] != NULL ){
                                node->children[node->num++] = createNode(buf[i][j]) ; 
                                node = node->children[node->num - 1 ] ; 
                            }
                            else { 
                                return NULL ; 
                            }
                        }
                        if (1){
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
                    else {
                        return NULL ; 
                    }
                }
            }

            else if ( strcmp(buf[i][j], ";")==0 ){
                    return node   ; 
            }

            if (strcmp(buf[i][j] , "AS ") != 0 ){
                int braces = 0  ; 
                if (strcmp(buf[i][j]  , "(") == 0 ){
                    if (1){
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

                    braces++ ; 
                    while (braces > 0 ){
                        ctree * tree = NULL ; 
                        int present = 0 ; 
                        if (strcmp(buf[i][j] , ")") == 0 ){
                            braces-- ; 
                            if (braces == 0 ){
                                break ; 
                            }
                        }

                        if (strcmp(buf[i][j] , "(")){
                            braces++ ; 
                        }
                        int tems = 0; 
                        while (buf[i][j] != NULL && strcmp(buf[i][j]  , ',') != 0 ){
                            tems = 1 ; 
                            if (strcmp(buf[i][j] , ')') == 0 ){
                                braces-- ; 
                                if (braces == 0 ){
                                    break ; 
                                }
                            }

                            if (buf[i][j] != NULL  ){
                                if (strcmp(buf[i][j] , "INT" ) == 0 ||strcmp(buf[i][j] , "BIGINT" ) == 0 ||strcmp(buf[i][j] , "SMALLINT" ) == 0 ||strcmp(buf[i][j] , "FLOAT" ) == 0 ||strcmp(buf[i][j] , "TINYINT" ) == 0 ||strcmp(buf[i][j] , "DOUBLE" ) == 0 ||strcmp(buf[i][j] , "CHAR" ) == 0 ||strcmp(buf[i][j] , "VARCHAR" ) == 0 ||strcmp(buf[i][j] , "BOOL" ) == 0 || strcmp(buf[i][j] , "TIMESTAMP" ) == 0 || strcmp(buf[i][j] , "DATE" ) == 0 ){

                                    if (strcmp(buf[i][j] , "INT" ) == 0 ){
                                        tree->type = INT ; 
                                        tree->type_bits_count = 4 ; 
                                    }
                                    if (strcmp(buf[i][j] , "BIGINT" ) == 0 ){
                                        tree->type = BIGINT ; 
                                        tree->type_bits_count = 8 ;     
                                    }
                                    if(strcmp(buf[i][j] , "SMALLINT" ) == 0 ){
                                        tree->type = SMALLINT ; 
                                        tree->type_bits_count = 2 ; 
                                    }
                                    if (strcmp(buf[i][j] , "FLOAT" ) == 0 ){
                                        tree->type = FLOAT ; 
                                        tree->type_bits_count = 4 ; 
                                    }
                                    if (strcmp(buf[i][j] , "TINYINT" ) == 0 ){
                                        tree->type = TINYINT ; 
                                        tree->type_bits_count = 1 ; 

                                    }
                                    if (strcmp(buf[i][j] , "DOUBLE" ) == 0 ){
                                        tree->type = DOUBLE ; 
                                        tree->type_bits_count = 8 ; 
                                    }

                                    if (strcmp(buf[i][j] , "CHAR" ) == 0 ){
                                        tree->type = CHAR ; 
                                        if ( buf[i][j+1] == NULL ){
                                            if (i+1 <= end_row){
                                                i = i+ 1 ; 
                                                j = 0  ; 
                                            }
                                        }
                                        else { 
                                            j++ ; 
                                        }
                                        int num = 0 ; 
                                        if (buf[i][j] != NULL ){
                                            num = stoi(string(buf[i][j])) ; 
                                        }
                                        tree->type_bits_count = num  ; 
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
                                    if (strcmp(buf[i][j] , "VARCHAR" ) == 0 ){
                                        tree->type = VARCHAR ; 
                                        if ( buf[i][j+1] == NULL ){
                                            if (i+1 <= end_row){
                                                i = i+ 1 ; 
                                                j = 0  ; 
                                            }
                                        }
                                        else { 
                                            j++ ; 
                                        }
                                        int num = 0 ; 
                                        if (buf[i][j] != NULL ){
                                            num = stoi(string(buf[i][j])) ; 
                                        }
                                        tree->type_bits_count = num  ; 
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
                                    if(strcmp(buf[i][j] , "BOOL" ) == 0 ){
                                        tree->type = BOOL ; 
                                        tree->type_bits_count = 1 ; 
                                    }
                                    if (strcmp(buf[i][j] , "DATE" ) == 0 ){
                                        tree->type = DATE ; 
                                        tree->type_bits_count = 4 ; 
                                    }
                                    if (strcmp(buf[i][j] , "TIMESTAMP" ) == 0 ){
                                        tree->type = TIMESTAMP ; 
                                        tree->type_bits_count = 8 ; 
                                    }
                                }
                                
                                else if (strcmp(buf[i][j] , PRIMARY ) == 0 || strcmp(buf[i][j] , UNIQUE ) == 0  || strcmp(buf[i][j] , NOT ) == 0 || strcmp(buf[i][j] , DEFAULT ) == 0  ||  strcmp(buf[i][j] , FOREIGN ) == 0  ){
                                    if (strcmp(buf[i][j] , PRIMARY ) == 0 ){
                                        if (present == 1 ){
                                            tree->primary_key_counter = 1 ; 
                                        }
                                        else { 
                                            int a = 0 ; 
                                            tree->primary_key_counter = 0 ; 
                                            while ( a < 2 ){
                                                if ( buf[i][j+1] == NULL ){
                                                    if (i+1 <= end_row){
                                                        i = i+ 1 ; 
                                                        j = 0  ; 
                                                    }
                                                }
                                                else { 
                                                    j++ ; 
                                                }   
                                                a++ ;
                                            }
                                            while ( strcmp(buf[i][j] , ')') != 0 ){
                                                tree->primary_key[tree->primary_key_counter++] = strdup(buf[i][j]) ;  
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
                                            if (tree->primary_key_counter > 0 ){
                                                if (tree->primary_key_counter == 1 ){
                                                    if (tree->primary_key[0] != NULL ){
                                                        tree->comp = strdup(tree->primary_key[0] ) ; 
                                                    }
                                                }
                                                else { 
                                                    tree->comp = "PRIMARY KEY" ; 
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

                                        }
                                    }

                                else  if(strcmp(buf[i][j] , UNIQUE ) == 0  ){
                                    if (present == 1 ){
                                        tree->unique = true ; 
                                    }
                                }

                                else if(strcmp(buf[i][j] , NOT )== 0 ){
                                    if ( buf[i][j+1] == NULL ){
                                        if (i+1 <= end_row){
                                            i = i+ 1 ; 
                                            j = 0  ; 
                                        }
                                    }
                                    else { 
                                        j++ ; 
                                    } 
                                    if (strcmp(buf[i][j] , NULL )== 0){
                                        if (present == 1 ){
                                            tree->not_null = true ; 
                                        }

                                    }
                                }

                                else if (strcmp(buf[i][j] , DEFAULT ) == 0 ){
                                    if ( buf[i][j+1] == NULL ){
                                        if (i+1 <= end_row){
                                            i = i+ 1 ; 
                                            j = 0  ; 
                                        }
                                    }
                                    else { 
                                        j++ ; 
                                    }
                                    
                                    if (validate_default(buf[i][j] , tree->type)  == true ){
                                        tree->default = strdup(buf[i][j] ) ; 
                                    }

                                }

                                else if (strcmp(buf[i][j] , FOREIGN) == 0 ){

                                        int a = 0 ; 
                                        tree->foreign_key_counter = 0 ; 
                                        while ( a < 2 ){
                                            if ( buf[i][j+1] == NULL ){
                                                if (i+1 <= end_row){
                                                    i = i+ 1 ; 
                                                    j = 0  ; 
                                                }
                                            }
                                            else { 
                                                j++ ; 
                                            }   
                                            a++ ; 
                                        }
                                        while ( strcmp(buf[i][j] , ')') != 0 ){
                                            tree->foreign_key[tree->foreign_key_counter++] = strdup(buf[i][j]) ;  
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
                                        if (tree->foreign_key_counter > 0 ){
                                            tree->comp = "FOREIGN KEY" ; 
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

                                        if (strcmp(buf[i][j] , "REFERENCES") == 0 ){
                                            if ( buf[i][j+1] == NULL ){
                                                if (i+1 <= end_row){
                                                    i = i+ 1 ; 
                                                    j = 0  ; 
                                                }
                                            }
                                            else { 
                                                j++ ; 
                                            }  
                                            tree->references = 1  ; 
                                            ctree * temp = createNode(buf[i][j]) ; 

                                            if ( buf[i][j+1] == NULL ){
                                                if (i+1 <= end_row){
                                                    i = i+ 1 ; 
                                                    j = 0  ; 
                                                }
                                            }
                                            else { 
                                                j++ ; 
                                            } 

                                            if (strcmp(buf[i][j] , "(") == 0 ){
                                                if ( buf[i][j+1] == NULL ){
                                                    if (i+1 <= end_row){
                                                        i = i+ 1 ; 
                                                        j = 0  ; 
                                                    }
                                                }
                                                else { 
                                                    j++ ; 
                                                } 

                                                while (strcmp(buf[i][j] , ")") != 0 ){
                                                    temp->children[temp->num++]->comp = strdup( buf[i][j] ); 
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

                                                tree->children[tree->num++] = temp ; 

                                            }
                                        }

                                }


                                }

                                else  {
                                    tree->comp = strdup(buf[i][j]) ; 
                                    present = 1 ; 
                                }
                            }



                            if (1){
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
                        if (tems == 1 ){
                            node->children[node->num++] = tree ; 
                        }

                        braces-- ; 
                    }
                }
            }
            else { 
                node->as = select_query( i , j , end_row , end_col , pain ) ; 
            }





        }
    }
}















