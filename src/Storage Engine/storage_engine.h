#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PAGE_SIZE 4096 

typedef struct {
    char col_name[64];
    uint8_t col_type;
    int32_t type_bits_count;
    int32_t first_decimal;
    int32_t second_decimal;
    bool primary_key;
    bool foreign_key;
    bool default_stuff;
    bool not_null;
    bool unique;
    char default_val[32];
    char foreign_key_table[64];
    char foreign_key_column[64];
} column_info;

typedef struct {
    char table_name[64];
    uint32_t num_columns;
    uint64_t root_page;
    column_info columns[16];
} table_info;

typedef struct {
    uint32_t db_magic;
    uint16_t version;
    uint16_t page_size;
    uint32_t num_tables;
    table_info tables[300];
} pagezero;

typedef struct {
    int hash[307] ; 
    int num ; 
    int primary_key[307] ; 
    int pri_num ; 
}info  ; 



typedef struct acutal_values{
    int error_handling  ; 
    char ** info ; 
    char ** columns ; 
    int col_num ;  
    itree * condition ; 
}acutal_values 


typedef struct info_detail{
    char * table ; 
    int row_num ; 
    acutal_values **info  ; 
}info_detail 


typedef struct insert{
    int table_num ; 
    info_detail ** info ; 
}insert_page 



void default_column_info(column_info * col) ; 
void default_table_info(table_info * ti);
void default_pagezero(pagezero * pg);
int hash_num( char *str) ; 
void prepared_the_intel_hash(info * intel);
void take_care_of_stuff(ctree * tree , info * intel) ; 
void do_the_foreign_key(ctree * tree  , table_info * ti ) ; 
void main_create_parser(pagezero * pg) ; 
int db_open(const char *filename)  ; 
void write(int fd, int page_num, uint8_t *buf)  ; 
void read(int fd, int page_num, uint8_t *buf)  ;
void write_pagezero(pagezero * pg, const char * filename) ; 
void read_pagezero(pagezero * pg, const char * filename) ; 
void close(int fd)  ; 