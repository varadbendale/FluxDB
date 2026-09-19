page *get_page_from_file(int offset, char *filename) {
    FILE *file = fopen(/*"name"*/, "rb");
    if (file == NULL) {
        return NULL;
    }
    char dat[4096];
    fseek(file, offset, SEEK_SET);
    size_t bytes_in_page = fread(dat, 1, 4096, file);
    if (bytes_in_page == 0) {
        return NULL;
    }
    page *ans = malloc(sizeof(page));
    if (ans == NULL) {
        return NULL;
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


int find_the_size_of_stuff_get_the_data_and_get_the_primary_key_num(char **data, table_info *table, char *ans, float *pk) {
    int size = 0;
    int i = 0;
    int j = 0;

    while (i < table.num_columns) {
        if (table.columns[i].primary_key == true) {
            pk = &(float)atof(table.columns[i].col_name);
        }
        if (table.columns[i].col_type == INT || table.columns[i].col_type == BIGINT || table.columns[i].col_type == SMALLINT || table.columns[i].col_type == TINYINT) {
            size = size + strlen(table.columns[i].type_bits_count);
            ans[j] = strdup(integer_num);
            j++;
            ans[j] = strdup(table.columns[i].col_name);
            j++;
        }
        else if (table.columns[i].col_type == FLOAT || table.columns[i].col_type == DOUBLE) {
            size = size + strlen(table.columns[i].type_bits_count);
            ans[j] = strdup(real_num);
            j++;
            ans[j] = strdup(table.columns[i].col_name);
            j++;
        }
        else if (table.columns[i].col_type == CHAR || table.columns[i].col_type == VARCHAR) {
            ans[j] = strdup(string_num);
            j++;
            ans[j] = strdup(strlen(table.columns[i].col_name));
            j++;
            if (strlen(table.columns[i].col_name) > 1024) {
                //assign the toast table
                size = size + 32;
            }
            else {
                ans[j] = strdup(table.columns[i].col_name);
                size = size + strlen(table.columns[i].col_name);
                j++;
            }
        }
        else if (table.columns[i].col_type == DATE) {
            size = size + strlen(table.columns[i].type_bits_count);
            ans[j] = strdup(date_num);
            j++;
            ans[j] = strdup(table.columns[i].col_name);
            j++;
        }
        else if (table.columns[i].col_type == TIMESTAMP) {
            size = size + strlen(table.columns[i].type_bits_count);
            ans[j] = strdup(timestamp_num);
            j++;
            ans[j] = strdup(table.columns[i].col_name);
            j++;
        }
        i++;
    }
    return size;
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


/*
    int slots_figured_out(page *pg, float pk) {
        int low = 0;
        int high = pg->slot_num;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (pk < pg->slot[mid].pk) {
                high = mid - 1;
            }
            else if (pk == pg->slot[mid].pk) {
                return NULL;
            }
            else {
                low = mid + 1;
            }
        }
        return low;
    }
*/


for (int i = 7; i >= 0; i--) {
    int bit = (val >> i) & 1;
    printf("%d", bit);
}


void FSM(char *insert_data, int page_num, int offset, table_info *table) {
    page *pg;
    char *ans;
    int size = 0;
    float *primary_key = 0;
    int first = 0;

    if (offset > 0) {
        pg = get_page_from_file(offset);
        if (pg->header->page_num != page_num) {
            return;
        }
        size = find_the_size_of_stuff_get_the_data_and_get_the_primary_key_num(data, table, &ans, &primary_key);
        size = size + sizeof(slots);
        if (size < pg->header->free_size) {
            if (size < pg->header->normal_free_size) {
                int current_offset = pg->header->current_offset + pg->header->current_size;
                memcpy(pg->data + insert_offset, ans, size - sizeof(slots));
                pg->header->free_size = pg->header->free_size - size;
                pg->header->normal_free_size = pg->header->normal_free_size - size;
                pg->slots[pg->slot_num].size = size - sizeof(slots);
                pg->slots[pg->slot_num].offset = pg->current_offset + pg->slots[pg->slot_num].size;
                pg->slots[pg->slot_num].pk = *primary_key;
                pg->header->current_offset = current_offset;
                pg->header->current_size = size - sizeof(slots);
                int temp_slot_num = pg->slot_num / 8;
                pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                pg->slot_num++;
            }

            else {
                int i = 0;
                int j = 0;
                int k = 0;
                int where_to_put = -1;
                int temp_slot_num = pg->slot_num / 8;
                int temp_mod = pg->slot_num % 8;
                uint8_t temp;
                while (i < temp_slot_num) {
                    temp = pg->header->dead_slots[i];
                    for (int j = 7; j >= 0; j--) {
                        int bit = (temp >> j) & 1;
                        if (bit == 0) {
                            where_to_put = (i * 8) + j;
                            k = i;
                            if (size - sizeof(slots) > pg->slots[where_to_put].size) {
                                where_to_put = -1;
                            }
                            break;
                        }
                    }
                    if (where_to_put != -1) {
                        break;
                    }
                }

                if (where_to_put != -1) {
                    pg->header->free_size = pg->header->free_size + pg->slots[where_to_put].size - (size - sizeof(slots));
                    pg->slots[where_to_put].size = size - sizeof(slots);
                    pg->slots[where_to_put].pk = primary_key;
                    memcpy(pg->data + pg->slots[where_to_put].offset, ans, size);
                    int index = temp_mod;
                    int bit_pos = 7 - index;
                    temp |= (1 << bit_pos);
                    pg->header->dead_slots[k] = temp;
                }
                else {
                    int a = 0 ; 
                    int temp_pointer = 0 ; 
                    while ( a < pg->slot_num ){
                        int first = a / 8 ; 
                        int second  = 7 - (a % 8);
                        if( (pg->header->dead_slots[first] >> second) & 1  == 1 ){
                            if (pg->slot[a].offset != temp_pointer ){
                                memmove(  pg->data + temp_pointer , pg->data + pg->slot[a].offset  , pg->slot[a].size)
                            }
                            temp_pointer  = temp_pointer + pg->slot[a].size ; 
                            a++ ; 
                        }
                        else { 
                            a++
                            continue ; 
                        }
                    }
                    pg->current_size = temp_pointer ; 
                    pg->header->normal_free_size = pg->header->free_size ;
                    int current_offset = pg->header->current_offset + pg->header->current_size;
                    memcpy(pg->data + insert_offset, ans, size - sizeof(slots));
                    pg->header->free_size = pg->header->free_size - size;
                    pg->header->normal_free_size = pg->header->normal_free_size - size;
                    pg->slots[pg->slot_num].size = size - sizeof(slots);
                    pg->slots[pg->slot_num].offset = pg->current_offset + pg->slots[pg->slot_num].size;
                    pg->slots[pg->slot_num].pk = *primary_key;
                    pg->header->current_offset = current_offset;
                    pg->header->current_size = size - sizeof(slots);
                    int temp_slot_num = pg->slot_num / 8;
                    pg->header->dead_slots[temp_slot_num] = (pg->header->dead_slots[temp_slot_num] << 1) | 1;
                    pg->slot_num++;
                }
            }
        }
        else {

        }
    }
}

// need to take all the cases for the toast tables so yeah boi rest is almost done 