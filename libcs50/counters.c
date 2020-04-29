/*
 * set.c -- CS50 'set' module
 * 
 * see counters.h for more information
 * 
 * Author: Lizzie Hernandez
 * Date: April, 2020
 * Purpose: To implement counters structure
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "counters.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode{
    int key;
    int count;
    struct counternode *next; 
} counternode_t;

/**************** global types ****************/
typedef struct counters{
    counternode_t *head;
} counters_t; 

/**************** global functions ****************/
/* see set.h for comments about exported functions */


/**************** local functions ****************/
/* not visible outside this file */
static counternode_t *counternode_new(const int key);

/**************** counters_new() ****************/
counters_t *counters_new(void){
    counters_t *counters = (counters_t *)count_malloc(sizeof(counters_t));
    if(counters != NULL){
        counters->head = NULL;
    }
    return counters;
}

/**************** counters_add() ****************/
int counters_add(counters_t *ctrs, const int key){
    if(ctrs!=NULL && key>=0){
        // check if key already exists. if it does not, create a new counter
        for (counternode_t *node = ctrs->head; node!= NULL; node = node->next){
            if(node->key==key){
                node->count ++;
                return node->count;
            }
        }
        // otherwise, creste new counter with key value
        counternode_t *new = counternode_new(key);
        
        if(new != NULL){
            new->next = ctrs->head;
            ctrs->head = new;
            return new->count;
        }
        else
            return 0;
    }
    else
        return 0;
}


/**************** counternode_new() ****************/
static counternode_t *counternode_new(const int key){
    if(key>=0){
        // allocate space for node
        counternode_t *node = (counternode_t *)count_malloc(sizeof(counternode_t));

        if(node!=NULL){
            // set key 
            node->key = key;
            node->count = 1;
            node->next = NULL;
        }
        return node;
    }
    else
        return NULL;
}

/**************** counters_get() ****************/
int counters_get(counters_t *ctrs, const int key){
    if(ctrs!=NULL && key>=0){
        // check every node to find key
        for (counternode_t *node = ctrs->head; node!= NULL; node = node->next){
            if(node->key==key){
                return node->count;
            }
        }
        return 0;
    }
    else
        return 0;
}

/**************** counters_set() ****************/
bool counters_set(counters_t *ctrs, const int key, const int count){
    if(ctrs!=NULL && key>=0 && count>=0){
        // check every node to find key
        for (counternode_t *node = ctrs->head; node!= NULL; node = node->next){
            if(node->key==key){
                node->count = count;
                return true;
            }
        }
        
        // else if not found, create a new counter
        counternode_t *new = counternode_new(key);
        
        if(new != NULL){
            // assign new value
            new->count = count;

            //update head
            new->next = ctrs->head;
            ctrs->head = new;   
            return true;         
        }
        else
            return false;
    }
    else
        return false;
}

/**************** counters_print() ****************/
void counters_print(counters_t *ctrs, FILE *fp){
    if(fp!=NULL){
        if(ctrs!=NULL){
            // open bracket
            fprintf(fp, "{");
            
            //comma separated list of key=count pairs
            if(ctrs->head!=NULL){
                counternode_t *node;
                for (node = ctrs->head; node->next!= NULL; node = node->next){
                    fprintf(fp, "%d=%d", node->key, node->count);
                    fprintf(fp, ", ");
                }
                //print last one
                fprintf(fp, "%d=%d", node->key, node->count);
            }
            
            // close bracket
            fprintf(fp, "}");
        }
        else
            fprintf(fp, "(null)\n");
    }
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count)){
    if(ctrs!=NULL && itemfunc!=NULL){
        for (counternode_t *node = ctrs->head; node!= NULL; node = node->next){
            (*itemfunc)(arg, node->key, node->count);
        }
    }
}

/**************** set_delete() ****************/
void counters_delete(counters_t *ctrs){
    if(ctrs!=NULL){
        for (counternode_t *node = ctrs->head; node!= NULL; ){
            counternode_t *next = node->next;	    // remember what comes next
            count_free(node);			            // free the node
            
            node = next;			                // and move on to next
        }

        count_free(ctrs);
        ctrs = NULL;
    }
}
