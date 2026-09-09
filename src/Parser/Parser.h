#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
 
typedef struct Create_tree_def {
    char * comp ; 
    int type ; 
    int type_bits_count ; 
    int first_decimal ; 
    int second_decimal ; 
    int  primary_key_counter  ; 
    char ** primary_key ; 
    bool not_null ; 
    char *  default_val ; 
    bool unique ; 
    int  foreign_key_counter ; 
    char ** foreign_key ; 
    int references ; 
    int line;                 
    int col;
    struct Create_tree_def** children;   
    int num ; 
    tree * as ; 
}ctree



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

typedef struct Tree_def {
    char* comp; 
    struct Tree_def** children;   
    char* as  ; 
    char **direction  ; 
    int line;                 
    int col;
    int num  ; 
} Tree_def;

typedef Tree_def tree  ; 

tree* createNode( char* comp) {
    tree* node = malloc(sizeof(tree));
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
    node->children = calloc(300, sizeof(tree*));
    
    node->direction = NULL ;  
    node->num = 0  ; 
    node->col = 0 ; 
    node->line = 0 ; 
    return node;
}

Tree_def* make_leaf(char* value , int row , int col ) {
    Tree_def* n = malloc(sizeof(Tree_def));
    if (n == NULL) {
        return NULL;   
    }
    if ( value != NULL ){
      n->comp = strdup(value);
    }
    n->as = NULL;
    n->children[300] = NULL;
    n->direction = NULL ; 
    n->line = row ; 
    n->col = col;
    return n;
}

bool if_function( char* word ) ; 
bool if_sql_syntax( char* word ) ; 
int priority(char *string) ; 
tree* expression(char ***buf , tree * node , int check , int i , int j  , int end_row , int end_col  ) ; 
tree *function( char ***buf , tree * node ,int check  ,  int i , int j  , int end_row , int end_col ) ;
tree *case_expr(char ***buf , int check ,  int i , int j , int end_row , int end_col ) ; 
tree *comp_1(char*** buf , tree * node  ,  int i , int j , int end_row , int end_col ) ; 
tree *comp_2(char*** buf , tree * node  ,  int i , int j , int end_row , int end_col ) ; 
tree *comp_3(char*** buf , tree * node  ,  int i , int j , int end_row , int end_col  , int pain  , int having ) ; 
tree * select_query( int row , int col , int check , int end_row , int end_col , int pain) ; 


typedef struct {
    uint32_t db_magic ; 
    uint16_t version ; 
    uint16_t page_size ; 
    uint32_t num_tables ; 
    table_info tables[300] ; 
}pagezero ; 

typedef struct{
    char *table_name;     
    uint32_t num_columns;        
    uint64_t root_page;        
    ColumnDef columns[16];       
}table_info ; 

typedef struct{
    char *col_name ;   
    uint8_t col_type;       
    int type_bits_count ; 
    int first_decimal ; 
    int second_decimal ;   
    bool primary_key ; 
    bool not_null ; 
    char  *default_val ;     
    char foreign_key_table ;   
    char foreign_key_column ;  
}column_info ; 


ctree* createNode( char* comp)  ; 
bool validate_default(char *default_val, int type)  ; 
ctree* create_parser() ; 

