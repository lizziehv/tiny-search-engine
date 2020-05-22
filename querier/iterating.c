/*
 * Author: Lizzie Hernandez Videa
 * May 17, 2020
 * Purpose: To define item functions for iterating through counters
 * 
 * See iterating.h for details
 */

#include "counters.h"
#include "iterating.h"
#include "memory.h"
#include "tuple.h"

/*########### counters_add_all ##########*/
/*##### See iterating.h for details #####*/
void counters_add_all(void *arg, const int key, const int count){
    counters_t* other_counter = (counters_t*)arg;
    if(count > 0){
        counters_set(other_counter, key, count);
    }
}

/*########### counters_merge ############*/
/*##### See iterating.h for details #####*/
void counters_merge(void *arg, const int key, const int count){
    counters_set((counters_t*)arg, key, count + counters_get(arg, key));
}

/*######### counters_intersect ##########*/
/*##### See iterating.h for details #####*/
void counters_intersect(void *arg, const int key, const int count){
    /* Get value from the other counter */
    counters_t* other_counter = (counters_t*)arg;
    int other_count = counters_get(other_counter, key);

    /* Keep the smallest value */
    int lesser_value;
    if(other_count < count)
        lesser_value = other_count;
    else
        lesser_value = count;
    
    counters_set(other_counter, key, lesser_value);
}

/*############ counters_count ###########*/
/*##### See iterating.h for details #####*/
void counters_count(void *arg, const int key, const int count){
    int* size = (int *)arg;
    *size = *size + 1;
}

/*############ counters_count ###########*/
/*##### See iterating.h for details #####*/
void counters_to_array(void *arg, const int key, const int count){
    tuple_t** array = (tuple_t**)arg;
    
    /* create a new pair to be inserted */
    tuple_t* new = tuple_new(key, count);
    if(new != NULL){
        /* keep index at which it shoud be located */
        int i = tuple_get_score(array[0]);
        tuple_update_score(array[0]);

        /* move to right position using insertion sort */
        int j = i-1;
        while(j > 0 && count > tuple_get_score(array[j])){
            /* Move j to the next index */
            array[j + 1] = array[j];
            j--;
        }

        array[j+1] = new;
    }
}