/*
 * Author: Lizzie Hernandez Videa
 * Date: Wednesday April 29th, 2020
 * 
 * Purpose: to create a program that crawls the web 
 * and retrieves webpages starting from a seed URL
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"

/* ######### local methods ######### */
int crawler(char *seed_url, char *page_dir, int max_depth, bag_t *bag, hashtable_t *htable);

int main(int argc, char *argv[]){
    int MAX_ALLOWABLE = 200;

    /* Exit on incorrect usage */
    if(argc != 4) {
        fprintf(stderr, "Error: Incorrect argument input.\n");
        return 1;
    }

    /* Get arguments */
    char *seed_url = argv[1];                   // root to start from

    char *page_dir = argv[2];                   // to write downloaded pages
    
    int max_depth;                              // maximum crawl depth
    int ret = sscanf(argv[3], "%d",&max_depth); 
    
    /* Check validity of arguments */
    if(!IsInternalURL(seed_url) || !NormalizeURL(seed_url)){ // check seed
        fprintf(stderr, "Error: Incorrect 1st argument.\n");
        return 1;  
    }
    else if(ret != 1 || max_depth<0){                         // check depth
        fprintf(stderr, "Error: Incorrect 3rd argument.\n");
        return 1;
    }
    else if(max_depth > MAX_ALLOWABLE){
        fprintf(stderr, "Please choose a depth less than or equal to %d.\n", MAX_ALLOWABLE);
        return 1;
    }
    else if(!directory_exists(page_dir)){
        fprintf(stderr, "Error: Incorrect 2nd argument.\n");
        return 1;
    }
  
    /* create a bag, return on error */
    bag_t *bag = bag_new();
    if(bag == NULL){
        fprintf(stderr, "Error creating bag.\n");
        return 2;
    }

    /* create a hashtable, return on error */
    hashtable_t *htable = hashtable_new(MAX_ALLOWABLE);
    if(htable == NULL){
        fprintf(stderr, "Error creating hashtable.\n");
        return 2;
    }

    ret = crawler(seed_url, page_dir, max_depth, bag, htable);

    /* Clean up modules */
    bag_delete(bag, NULL);
    hashtable_delete(htable, NULL);

    if(ret!=0) return 3;

    return 0;
}


/* @param seed_url - url to start fetching from
 * @param page_dir - directory to write files into
 * @param max_depth - maximum recursion depth of pages to visit
 * @param bag - initialized bag module to keep track of which pages to visit
 * @param htable - initialized hashtable to keep track of which pages have been visited
 * 
 * Use to crawl a website and retrieve webpages starting with a specified URL
 */
int crawler(char *seed_url, char *page_dir, int max_depth, bag_t *bag, hashtable_t *htable){
    /* Make a webpage for the seed url, return on error */
    char *url = assertp(count_malloc(strlen(seed_url)+1), "Error: Could not copy URL.");
    strcpy(url, seed_url);
    webpage_t *first_page = webpage_new(url, 0, NULL);
    if(first_page == NULL){
        fprintf(stderr, "Error creating page.\n");
        return 1;
    }

    /* keep track of id of inserted items */
    int current_id = 1;

    bag_insert(bag, first_page);
    hashtable_insert(htable, url, ""); 

    /* Keep popping pages from bag */
    webpage_t *page;
    while((page = bag_extract(bag)) != NULL){
        /* retrieve a webpage for that url or log error */
        if(!webpage_fetch(page)){
            fprintf(stderr, "Could not fetch page %s.\n", webpage_getURL(page)); 
        }
        else{
            /* For testing purposes */
            fprintf(stdout, "Exploring page with url %s...\n", webpage_getURL(page));
            /* Save page */
            pagesaver(page_dir, page, current_id);
            current_id += 1;

            int curr_depth = webpage_getDepth(page);

            /* Visit all 'neighbor' URLs */
            if(curr_depth < max_depth){
                int pos = 0;
                char *next_url;

                while((next_url=webpage_getNextURL(page, &pos))!= NULL){                
                    if(!NormalizeURL(next_url)){
                        free(next_url);
                        fprintf(stderr, "Page '%s' could not be normalized.\n", next_url);
                    }
                    else if(IsInternalURL(next_url) && hashtable_insert(htable, next_url, "")){
                        /* If not in hashtable, insert it */
                        webpage_t *new_page = webpage_new(next_url, curr_depth + 1, NULL);
                        if(new_page != NULL){
                            bag_insert(bag, new_page);
                        }
                        else
                            fprintf(stderr, "Page '%s' could not be created.\n", next_url);
                    }
                    else{
                        /* For testing */
                        if(!IsInternalURL(next_url)){
                            fprintf(stdout, "Found external url: %s.\n", next_url);
                        }

                        free(next_url);
                    }
                }
            }
        }
        /* Clean up */
        webpage_delete(page);
    }
    return 0;
}
