void tree_func_for_insert(internal_node * node  , int pk , char * data , int page_id , int offset , int slot_num ){
    internal_node * stack[32] ; 
    int k = 0  ; 
    if (node != NULL ){
        stack[k] = node ; 
        k++ ; 
        while (node != NULL && node->leaf != true ){
            int low = 0 ; 
            int high = no_of_data + 1  ; 
            while ( low <= high ){
                int mid = (low + high ) / 2 ; 
                if (pk > node->primary_key[mid] ){
                    low = mid + 1 ; 
                }
                else {
                    high = mid - 1 ; 
                }
            }
            stack[k] = node->children[low] ; 
            k++ ; 
            node = node->children[low] ; 
        }

        stack[k] = node->children[low] ; 
    }

    if (node->num_of_leaf < no_of_data ){
        if (node->num_of_leaf > 0 ){
            int low = 0 ; 
            int high  = node->num_of_leaf ; 
            while ( low <= high ){
                int mid = (low + high ) / 2 ; 
                if (pk == node->primary_key[mid] ){
                    return ; // error 
                }
                else if (pk > node->primary_key[mid] ){
                    low = mid + 1 ; 
                }
                else {
                    high = mid - 1 ; 
                }
            }

            for (int i = node->num_of_leaf + 1 ; i > low ; i--) {
                node->primary_key[i] = node->primary_key[i-1];
                node->data[i] = node->data[i-1];
            }
            node->primary_key[low] = pk ;  
            node->data[low].page_id = page_id ; 
            node->data[low].file_offset = offset ; 
            node->data[low].slot_num = slot_num ; 
        }
        else { 
            node->primary_key[node->num_of_leaf] = pk ;
            node->data[node->num_of_leaf].page_id = page_id ; 
            node->data[node->num_of_leaf].file_offset = offset ; 
            node->data[node->num_of_leaf].slot_num = slot_num ; 
            node->leaf = true ; 
        }
        node->num_of_leaf++ ; 
    }
    else { 
        
    }
     
}
