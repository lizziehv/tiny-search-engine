/*
 * set.c -- CS50 'set' module
 * 
 * see set.h for more information
 * 
 * Author: Lizzie Hernandez
 * Date: April, 2020
 * Purpose: To implement a set structure
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode{
    char *key;
    void *item;
    struct setnode *next; 
} setnode_t;

/**************** global types ****************/
typedef struct set{
    setnode_t *head;
} set_t; 

/**************** global functions ****************/
/* see set.h for comments about exported functions */


/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/**************** set_new() ****************/
set_t *set_new(void){
    set_t *set = (set_t *)count_malloc(sizeof(set_t));
    if(set != NULL){
        set->head = NULL;
    }
    return set;
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item){
    if(set!=NULL && key!=NULL && item!=NULL){

        // check if key already exists. if it does, return false
        for (setnode_t *node = set->head; node!= NULL; node = node->next){
            if(strcmp(node->key, key)==0){
                return false;
            }
        }
        
        // create new node if key does not exist
        setnode_t *new = setnode_new(key, item);

        if(new != NULL){
            new->next = set->head;
            set->head = new;

            #ifdef MEMTEST
                count_report(stdout, "After set_insert");
            #endif

            return true;
        }
        else
            return false;
        
    }
    else
        return false;
}

/**************** setnode_new() ****************/
static setnode_t *setnode_new(const char *key, void *item){
    if(key!=NULL && item!=NULL){
        // allocate space for node
        setnode_t *node = (setnode_t *)count_malloc(sizeof(setnode_t));

        if(node!=NULL){
            // allocate space for key
            node->key = (char *)count_malloc((strlen(key)+1)*sizeof(char));
            // copy key
            strcpy(node->key, key);

            // set item and next
            node->item = item;
            node->next = NULL;
        }
        return node;
    }
    else
        return NULL;
}

/**************** set_find() ****************/
void *set_find(set_t *set, const char *key){
    if(set != NULL && key != NULL){

        // iterate through set until key is found
        for (setnode_t *node = set->head; node!= NULL; node = node->next){
            if(strcmp(node->key, key)==0){
                return node->item;
            }
        }
        // return NULL if key was not found
        return NULL;
    }
    else
        return NULL;
}


/**************** set_print() ****************/
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)){
    if(fp != NULL){
        if(set != NULL){
            // open bracket
            fputc('{', fp);
            
            // iterate through and itemprint each item
            if(itemprint != NULL){
                setnode_t *node;
                if(set->head!=NULL){
                    // stop at second to last node
                    for (node = set->head; node->next != NULL; node = node->next){
                        (*itemprint)(fp, node->key, node->item); 
                        fputc(',', fp);
                    } 
                    // print last node
                    (*itemprint)(fp, node->key, node->item);
                }
            }
            else{
                fputc(' ', fp);
            }

            // closing bracket
            fputc('}', fp);
        }
        else
            fputs("(null)", fp);
    }
}

/**************** set_iterate() ****************/
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ){
    if(set != NULL && itemfunc != NULL){
        // iterate through set and call func on each key, item pair
        for (setnode_t *node = set->head; node!= NULL; node = node->next){
            (*itemfunc)(arg, node->key, node->item);
        }
    }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) ){
    if(set!= NULL){
        for (setnode_t *node = set->head; node!= NULL; ){
            if (itemdelete != NULL) {		    // if possible...
                (*itemdelete)(node->item);	    // delete node's item
            }
            setnode_t *next = node->next;	    // remember what comes next
            count_free(node->key);
            count_free(node);			        // free the node
            
            node = next;			            // and move on to next
        }

        // free and null set
        count_free(set);
        set = NULL;
    }

    #ifdef MEMTEST
    count_report(stdout, "End of bag_delete");
    #endif
}