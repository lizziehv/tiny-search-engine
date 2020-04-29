/*
 * hashtable.c -- CS50 'hashtable' module
 * 
 * see hashtable.h for more information
 * 
 * Author: Lizzie Hernandez
 * Date: April, 2020
 * Purpose: To implement a hashtable structure
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "memory.h"
#include "set.h"
#include "jhash.h"

/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
    set_t **htarray;
    int num_slots;
} hashtable_t; 

/**************** global functions ****************/
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** hashtable_new ****************/
hashtable_t *hashtable_new(const int num_slots){
    hashtable_t *newht = (hashtable_t *) count_malloc(sizeof(hashtable_t));
    // allocate as many spaces for set pointer as caller requests
    if(newht != NULL && num_slots>0){
        newht->htarray = count_calloc(num_slots, sizeof(set_t*));
        newht->num_slots = num_slots;
    }
    return newht;
}

/**************** hashtable_insert ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
    if(ht!=NULL && key!=NULL && item!=NULL){
        unsigned long i = JenkinsHash(key, ht->num_slots);
    
        /* if no key, item pair had already hashed into the same position, create
        a new set at that position */
        set_t **htarray = ht->htarray;
        if(htarray[i]==NULL){
            set_t *new = set_new();
            // new set at index i 
            if(new!=NULL){
                htarray[i] = new; 
            }
            else
                return false;
        }

        // insert to slot where it hashed
        bool res = set_insert(htarray[i], key, item);
        // returns false if key already existed in set, true otherwise
        return res;
    }
    else
        return false;
}

/**************** hashtable_find ****************/
void *hashtable_find(hashtable_t *ht, const char *key){
    if(ht!=NULL && key!=NULL){
        // hash into table to find slot
        unsigned long i = JenkinsHash(key, ht->num_slots);
        set_t * slot = ht->htarray[i];

        // search for key in slot
        if(slot!=NULL){
            return set_find(slot, key);
        }
        else
            return NULL;
    }
    else
        return NULL;
}

/**************** hashtable_print ****************/
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)){
    if(fp!=NULL){
        if(ht!=NULL){
            // loop through all slots in hash table
            for(int i=0; i<ht->num_slots;i++){
                set_t *slot = ht->htarray[i];

                // for each, print all key item pairs
                // if itemprint is NULL, set_print won't print anything
                set_print(slot, fp, itemprint);
                fprintf(fp, "\n");
            }
        }
        else{
            fprintf(fp, "(null)");
        }
    }
}

/**************** hashtable_iterate ****************/
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ){
    if(ht!=NULL && itemfunc!=NULL){
        // loop through all slots in hash table
        for(int i=0; i<ht->num_slots;i++){
            set_t *slot = ht->htarray[i];
            if(slot!=NULL){
                // call set iteration on every slot
                set_iterate(slot, arg, itemfunc);
            }
        }
    }
}

/**************** hashtable_delete ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){
    if(ht!=NULL){
        // loop through all slots in hash table
        for(int i=0; i<ht->num_slots;i++){
            set_t *slot = ht->htarray[i];
            if(slot!=NULL)
                set_delete(slot, itemdelete);
        }
        // free allocated memory spaces
        free(ht->htarray);
        free(ht);
        ht=NULL;
    }
}
