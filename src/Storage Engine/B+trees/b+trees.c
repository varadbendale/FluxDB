void tree_func_for_insert(internal_node * node  , int pk , char * data , int page_id , int offset , int slot_num ){
    internal_node * stack[32] ; 
    if (node == NULL ){
        return  ; 
    }
    int stack_index[32] ; 
    int k = 0  ; 
    int which_pos = -1 ; 
    internal_node * use_temp = node ; 

    stack[k] = use_temp ;
    stack_index[k] = 0 ;
    while (use_temp != NULL && use_temp->leaf != true ){
        int low = 0 ; 
        int high = use_temp->num_of_leaf - 1 ;
        while ( low <= high ){
            int mid = (low + high ) / 2 ; 
            if (pk > use_temp->primary_key[mid] ){
                low = mid + 1 ; 
            }
            else if (pk ==  use_temp->primary_key[mid]  ){
                return // error ; 
            }
            else {
                high = mid - 1 ; 
            }
        }
        k++ ; 
        stack[k] = use_temp->children[low] ; 
        stack_index[k] = low ; 
        use_temp = use_temp->children[low] ; 
        which_pos = low ; 
    }

    if (stack[k]->num_of_leaf < no_of_data ){
        for (int i = stack[k]->num_of_leaf ; i > which_pos ; i--) {
            stack[k]->primary_key[i] = stack[k]->primary_key[i-1];
            stack[k]->data[i] = stack[k]->data[i-1];
        }
        node->primary_key[low] = pk ;  
        node->data[low].page_id = page_id ; 
        node->data[low].file_offset = offset ; 
        node->data[low].slot_num = slot_num ; 
        node->leaf = true ; 
        node->num_of_leaf++ ; 
    }
    else { 
        internal_node * temp_node ; 
        int catch_which_to_put_as_parent = -1 ; 
        while (k >= 0 ){
            if (stack[k] != NULL && stack[k]->leaf == true ){
                int tem = stack[k]->num_of_leaf /2 ; 
                for ( int i = 0 ; i < tem ; i++ ){
                    temp_node->primary_key[temp_node->num_of_leaf] = stack[k]->primary_key[tem + i ] ; 
                    temp_node->data[temp_node->num_of_leaf] = stack[k]->data[tem + i ] ; 
                    temp_node->num_of_leaf++ ; 
                }
                stack[k]->num_of_leaf = tem ; 
                if (which_pos > tem ){
                    which_pos = which_pos - tem ; 
                    for (int i = temp_node->num_of_leaf + 1 ; i > which_pos ; i--) {
                        temp_node->primary_key[i] = temp_node->primary_key[i-1];
                        temp_node->data[i] = temp_node->data[i-1];
                    }
                    temp_node->primary_key[which_pos] = pk ;  
                    temp_node->data[which_pos].page_id = page_id ; 
                    temp_node->data[which_pos].file_offset = offset ; 
                    temp_node->data[which_pos].slot_num = slot_num ; 
                    temp_node->num_of_leaf++ ;
                }
                else {
                    for (int i = stack[k]->num_of_leaf + 1 ; i > which_pos ; i--) {
                        stack[k]->primary_key[i] = stack[k]->primary_key[i-1];
                        stack[k]->data[i] = stack[k]->data[i-1];
                    }
                    stack[k]->primary_key[which_pos] = pk ;  
                    stack[k]->data[which_pos].page_id = page_id ; 
                    stack[k]->data[which_pos].file_offset = offset ; 
                    stack[k]->data[which_pos].slot_num = slot_num ; 
                    stack[k]->num_of_leaf++ ; 
                }
                stack[k]->next = temp_node ; 
                catch_which_to_put_as_parent = stack[k]->primary_key[stack[k]->num_of_leaf].pk  ; 
            }

            else if (stack[k] != NULL && stack[k]->leaf == false && temp_node != NULL && stack[k]->num_of_children < no_of_data   ){
                int low = 0 ; 
                int high = no_of_data + 1  ; 
                while ( low <= high ){
                    int mid = (low + high ) / 2 ; 
                    if (catch_which_to_put_as_parent > stack[k]->primary_key[mid] ){
                        low = mid + 1 ; 
                    }
                    else if ( catch_which_to_put_as_parent == stack[k]->primary_key[mid] ){
                        return  ; //error ; 
                    }
                    else {
                        high = mid - 1 ; 
                    }
                }
                for (int i = stack[k]->num_of_leaf + 1 ; i > low ; i--) {
                    stack[k]->primary_key[i] = stack[k]->primary_key[i-1];
                    stack[k]->children[i] = stack[k]->children[i-1];
                }
                stack[k]->primary_key[low] = catch_which_to_put_as_parent ; 
                stack[k]->children[low] = temp_node ; 
                stack[k]->num_of_leaf++ ; 
                stack[k]->num_of_children++ ; 
                return  ; 
            }

            else if (stack[k] != NULL && stack[k]->leaf == false && stack[k]->num_of_children == no_of_data  ){
                int lame = k ; 
                while (lame >= 0 && stack[lame]->num_of_leaf == no_of_data ){
                    int low = 0 ;
                    int high = stack[lame]->num_of_leaf - 1  ; 
                    while ( low <= high ){
                        if (catch_which_to_put_as_parent < stack[lame]->primary_key[low]){
                            high = mid - 1 ; 
                        }
                        else if (catch_which_to_put_as_parent == stack[lame]->primary_key[low]){
                            return ; //error 
                        }
                        else { 
                            low = mid + 1 ; 
                        }
                    }

                    if (low != ( stack[lame]->num_of_leaf ) / 2  ){
                        int temp_catch_which_to_put_as_parent = stack[lame]->primary_key[low] ; 
                        for (int i = stack[lame]->num_of_leaf ; i > low ; i--) {
                            stack[lame]->primary_key[i] = stack[lame]->primary_key[i-1] ;
                        }
                        stack[lame]->primary_key[low] = catch_which_to_put_as_parent  ; 
                        catch_which_to_put_as_parent = temp_catch_which_to_put_as_parent ; 
                    }

                    lame--  ; 
                }
                int kalos = lame ; 
                int turner = 0 ; 
                while ( kalos <= k ){ 
                    internal_node * nadda  ; 
                    if (kalos == 0  && lame == -1 ){
                        nadda = (internal_node *)malloc(sizeof(internal_node)) ;
                        memcpy(nadda , stack[0] , sizeof(internal_node)) ;
                        memset(stack[0] , 0 , sizeof(internal_node)) ;
                        stack[0]->leaf = false ;
                        stack[0]->children[0] = nadda ;
                        stack[0]->num_of_children = 1 ;
                        stack[0]->num_of_leaf = 0 ;
                        nadda = stack[0]->children[0] ;
                    }
                    else{
                        nadda = stack[kalos] ; 
                    }
                    
                    if (turner == 0 ){
                        for (int i = nadda->num_of_leaf ; i > stack_index[kalos + 1] ; i--) {
                            nadda->primary_key[i] = nadda->primary_key[i-1] ;
                        }
                        nadda->primary_key[stack_index[kalos + 1]] = catch_which_to_put_as_parent ; 
                        nadda->num_of_leaf++ ; 
                        turner = 1 ; 
                    }
                    
                    internal_node *tempo = (internal_node *)malloc(sizeof(internal_node)) ;
                    memset(tempo , 0 , sizeof(internal_node)) ;

                    int for_the_temp = 0 ; 
                    int tempacola = nadda->num_of_leaf / 2 ; 
                    
                    for_the_temp = 0 ; 
                    for (int o = tempacola  ; o < nadda->num_of_leaf  ; o++ ){
                        tempo->primary_key[for_the_temp] = nadda->primary_key[o] ; 
                        tempo->num_of_leaf++ ; 
                        for_the_temp++ ; 
                    }
                    nadda->num_of_leaf = tempacola ;
                    tempo->leaf = false ; 

                    if ( nadda->leaf == false ){
                        tempacola = nadda->num_of_children / 2 ; 
                        for_the_temp = 0 ; 
                        for (int o = tempacola  ; o < nadda->num_of_children ; o++ ){
                            tempo->children[for_the_temp] = nadda->children[o] ; 
                            nadda->children[o] = NULL ;
                            tempo->num_of_children++ ; 
                            for_the_temp++ ; 
                        }
                        nadda->num_of_children = tempacola ; 
                        kalos++  ; 
                        if ( stack_index[kalos] < tempacola  ){
                            stack[kalos] = nadda->children[stack_index[kalos ] - 1 ]  ; 
                        }
                        else { 
                            stack[kalos] = tempo  ; 
                        }

                        internal_node * where_to_add  ; 
                        if (kalos - 2 >= 0 ){
                            where_to_add = stack[kalos - 2 ] ; 
                        }
                        else { 
                            where_to_add = stack[0 ] ; 
                        }
                        for (int i = where_to_add->num_of_children + 1; i > stack_index[kalos - 1] + 1; i--) {
                            where_to_add->children[i] = where_to_add->children[i-1];
                        }
                        where_to_add->children[stack_index[kalos - 1] + 1] = tempo;
                        where_to_add->num_of_children++;
                    }
                    else {
                        internal_node * the_parent_for_the_stuff = stack[kalos - 1 ] ; 
                        int parent_num = stack_index[kalos -1 ] ; 
                        tempacola = nadda->num_of_data_pushed / 2 ; 
                        for_the_temp = 0 ;
                        for (int o = tempacola  ; o < nadda->num_of_data_pushed ; o++ ){
                            tempo->data[for_the_temp] = nadda->data[o] ; 
                            tempo->num_of_data_pushed++ ; 
                            for_the_temp++ ;   
                        }
                        nadda->num_of_data_pushed = tempacola ; 
                        nadda->num_of_leaf = tempacola ;          
                        tempo->num_of_leaf = tempo->num_of_data_pushed ;
                        tempo->leaf = true ;

                        for (int i = the_parent_for_the_stuff->num_of_data_pushed + 1 ; i > parent_num + 1  ; i--) {
                            the_parent_for_the_stuff->children[i] = the_parent_for_the_stuff->children[i-1] ;
                        }
                        the_parent_for_the_stuff->children[parent_num + 1 ] = tempo   ; 
                        the_parent_for_the_stuff->children[parent_num]->next = tempo ; 
                        the_parent_for_the_stuff->num_of_children++ ; 

                        if ( stack_index[kalos] < nadda->num_of_data_pushed ){
                            for (int i = nadda->num_of_data_pushed ; i > stack_index[kalos] ; i--) {
                                nadda->data[i] = nadda->data[i-1] ;
                            }
                            nadda->num_of_data_pushed++ ; 
                            nadda->num_of_leaf++ ;
                            nadda->data[stack_index[kalos]].page_id = page_id ; 
                            nadda->data[stack_index[kalos]].file_offset = offset ; 
                            nadda->data[stack_index[kalos]].slot_num = slot_num ; 
                        }
                        else { 
                            stack_index[kalos]  = stack_index[kalos] - nadda->num_of_data_pushed  ; 
                            the_parent_for_the_stuff->children[parent_num + 1 ]->data[stack_index[kalos]].page_id = page_id ;
                            the_parent_for_the_stuff->children[parent_num + 1 ]->data[stack_index[kalos]].file_offset = offset ;
                            the_parent_for_the_stuff->children[parent_num + 1 ]->data[stack_index[kalos]].slot_num = slot_num ;
                        }
                        return ; 
                    }
                    
                }

            }
            k-- ; 
        }
    }
     
}
